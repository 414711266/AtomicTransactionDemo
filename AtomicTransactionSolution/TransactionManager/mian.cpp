#include <iostream>
#include <string>
#include <vector>
#include <memory> // For std::shared_ptr

// 前向声明
class Command;

// 通用数据层，即新的 HistoryManager，我们开始叫它 DataLayer
class DataLayer {
private:
    std::vector<std::shared_ptr<Command>> m_history;
    int m_currentVersion = -1; // -1 表示在所有历史记录之前 (初始状态)

public:
    void addCommand(std::shared_ptr<Command> command);
    void undo();
    void redo();
};

// Command 接口: 定义了所有命令必须实现的行为
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0; // 执行/重做
    virtual void unexecute() = 0; // 撤销
};

// Originator: 我们的文档类
class Document {
public:
    std::string content;
    void print() const {
        std::cout << "Document content: \"" << content << "\"" << std::endl;
    }
};

// ConcreteCommand: 修改文档内容的具体命令
class ChangeContentCommand : public Command {
private:
    Document& m_doc;
    std::string m_oldContent;
    std::string m_newContent;

public:
    ChangeContentCommand(Document& doc, const std::string& newContent)
        : m_doc(doc), m_newContent(newContent) {
        // 在命令创建时，就保存好旧状态
        m_oldContent = doc.content;
    }

    // 执行/重做：将文档内容设置为新内容
    void execute() override {
        m_doc.content = m_newContent;
    }

    // 撤销：将文档内容恢复为旧内容
    void unexecute() override {
        m_doc.content = m_oldContent;
    }
};

// 在这里实现 DataLayer 的方法
void DataLayer::addCommand(std::shared_ptr<Command> command) {
    // 如果在 undo 之后执行了新命令，清除所有未来的 redo 历史
    if (m_currentVersion + 1 < m_history.size()) {
        m_history.resize(m_currentVersion + 1);
    }

    command->execute(); // 立即执行新命令
    m_history.push_back(command);
    m_currentVersion++;
    std::cout << "DataLayer: Command executed and added to history. "
        << "Current version: " << m_currentVersion << std::endl;
}

void DataLayer::undo() {
    if (m_currentVersion < 0) {
        std::cout << "DataLayer: Cannot undo. At initial state." << std::endl;
        return;
    }
    std::cout << "DataLayer: Undoing..." << std::endl;
    m_history[m_currentVersion]->unexecute();
    m_currentVersion--;
}

void DataLayer::redo() {
    if (m_currentVersion + 1 >= m_history.size()) {
        std::cout << "DataLayer: Cannot redo. At latest state." << std::endl;
        return;
    }
    std::cout << "DataLayer: Redoing..." << std::endl;
    m_currentVersion++;
    m_history[m_currentVersion]->execute();
}

// Helper function to create and add command
void changeContent(DataLayer& dl, Document& doc, const std::string& newContent) {
    auto cmd = std::make_shared<ChangeContentCommand>(doc, newContent);
    dl.addCommand(cmd);
}

int main() {
    DataLayer dataLayer;
    Document doc;
    doc.content = "Initial Version";
    std::cout << "--- Start ---" << std::endl;
    doc.print();

    // 操作1: 修改为 Version 1
    std::cout << "\n--- Action 1 ---" << std::endl;
    changeContent(dataLayer, doc, "Version 1");
    doc.print();

    // 操作2: 修改为 Version 2
    std::cout << "\n--- Action 2 ---" << std::endl;
    changeContent(dataLayer, doc, "Version 2");
    doc.print();

    // --- 开始撤销/重做 ---
    std::cout << "\n--- Testing Undo/Redo ---" << std::endl;

    // Undo 1: 恢复到 V1
    dataLayer.undo();
    doc.print(); // 期望: Version 1

    // Undo 2: 恢复到 Initial
    dataLayer.undo();
    doc.print(); // 期望: Initial Version

    // Redo 1: 前进到 V1
    dataLayer.redo();
    doc.print(); // 期望: Version 1

    // 此时历史状态是 V1，我们执行一个新操作
    std::cout << "\n--- New Action after Undo ---" << std::endl;
    changeContent(dataLayer, doc, "Version 3 (new branch)");
    doc.print();

    // 尝试 Redo: 应该失败，因为 V2 的历史已被覆盖
    std::cout << "\n--- Trying to Redo to V2 ---" << std::endl;
    dataLayer.redo(); // 期望: Cannot redo
    doc.print();

    return 0;
}