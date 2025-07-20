#include <iostream>
#include <string>
#include <vector>
#include <memory> // For std::shared_ptr

// ǰ������
class Command;

// ͨ�����ݲ㣬���µ� HistoryManager�����ǿ�ʼ���� DataLayer
class DataLayer {
private:
    std::vector<std::shared_ptr<Command>> m_history;
    int m_currentVersion = -1; // -1 ��ʾ��������ʷ��¼֮ǰ (��ʼ״̬)

public:
    void addCommand(std::shared_ptr<Command> command);
    void undo();
    void redo();
};

// Command �ӿ�: �����������������ʵ�ֵ���Ϊ
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0; // ִ��/����
    virtual void unexecute() = 0; // ����
};

// Originator: ���ǵ��ĵ���
class Document {
public:
    std::string content;
    void print() const {
        std::cout << "Document content: \"" << content << "\"" << std::endl;
    }
};

// ConcreteCommand: �޸��ĵ����ݵľ�������
class ChangeContentCommand : public Command {
private:
    Document& m_doc;
    std::string m_oldContent;
    std::string m_newContent;

public:
    ChangeContentCommand(Document& doc, const std::string& newContent)
        : m_doc(doc), m_newContent(newContent) {
        // �������ʱ���ͱ���þ�״̬
        m_oldContent = doc.content;
    }

    // ִ��/���������ĵ���������Ϊ������
    void execute() override {
        m_doc.content = m_newContent;
    }

    // ���������ĵ����ݻָ�Ϊ������
    void unexecute() override {
        m_doc.content = m_oldContent;
    }
};

// ������ʵ�� DataLayer �ķ���
void DataLayer::addCommand(std::shared_ptr<Command> command) {
    // ����� undo ֮��ִ����������������δ���� redo ��ʷ
    if (m_currentVersion + 1 < m_history.size()) {
        m_history.resize(m_currentVersion + 1);
    }

    command->execute(); // ����ִ��������
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

    // ����1: �޸�Ϊ Version 1
    std::cout << "\n--- Action 1 ---" << std::endl;
    changeContent(dataLayer, doc, "Version 1");
    doc.print();

    // ����2: �޸�Ϊ Version 2
    std::cout << "\n--- Action 2 ---" << std::endl;
    changeContent(dataLayer, doc, "Version 2");
    doc.print();

    // --- ��ʼ����/���� ---
    std::cout << "\n--- Testing Undo/Redo ---" << std::endl;

    // Undo 1: �ָ��� V1
    dataLayer.undo();
    doc.print(); // ����: Version 1

    // Undo 2: �ָ��� Initial
    dataLayer.undo();
    doc.print(); // ����: Initial Version

    // Redo 1: ǰ���� V1
    dataLayer.redo();
    doc.print(); // ����: Version 1

    // ��ʱ��ʷ״̬�� V1������ִ��һ���²���
    std::cout << "\n--- New Action after Undo ---" << std::endl;
    changeContent(dataLayer, doc, "Version 3 (new branch)");
    doc.print();

    // ���� Redo: Ӧ��ʧ�ܣ���Ϊ V2 ����ʷ�ѱ�����
    std::cout << "\n--- Trying to Redo to V2 ---" << std::endl;
    dataLayer.redo(); // ����: Cannot redo
    doc.print();

    return 0;
}