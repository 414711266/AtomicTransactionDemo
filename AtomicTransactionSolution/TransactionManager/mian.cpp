/*
 * @author： Karl
 * @date：   2025年7月18日
 *
 * 事务处理demo
 */

#include <iostream>
#include <string>
#include <vector> // 需要 vector 来存储历史记录

 // Originator: 需要被备份状态的对象 (和之前一样，没有变化)
class Document {
public:
    std::string content;

    // 创建一个备忘录 (Memento)，包含当前需要保存的状态
    std::string createMemento() const {
        return content;
    }

    // 从备忘录中恢复状态
    void restoreFromMemento(const std::string& memento) {
        content = memento;
    }

    void print() const {
        std::cout << "Document content: \"" << content << "\"" << std::endl;
    }
};

// Caretaker: 负责人，现在是一个专门的类
class HistoryManager {
private:
    Document& m_doc; // 负责人需要知道它在为哪个文档服务
    std::vector<std::string> m_history; // 用一个 vector 来存储备忘录列表

public:
    // 构造函数，传入它需要管理的文档
    HistoryManager(Document& doc) : m_doc(doc) {}

    // 备份当前状态
    void backup() {
        std::cout << "HistoryManager: Backing up current state..." << std::endl;
        m_history.push_back(m_doc.createMemento());
    }

    // 撤销操作
    void undo() {
        if (m_history.empty()) {
            std::cout << "HistoryManager: No history to undo." << std::endl;
            return;
        }

        // 1. 从历史记录中取出最后一个备忘录
        std::string lastMemento = m_history.back();
        m_history.pop_back(); // 从历史记录中移除

        // 2. 用这个备忘录恢复文档
        std::cout << "HistoryManager: Undoing..." << std::endl;
        m_doc.restoreFromMemento(lastMemento);
    }
};


int main() {
    // 1. 创建 Originator 和 Caretaker
    Document doc;
    HistoryManager history(doc);

    // --- 流程开始 ---
    // 操作 1: 设置初始内容
    doc.content = "Version 1";
    history.backup(); // 备份 V1
    doc.print();

    // 操作 2: 修改内容
    doc.content = "Version 2";
    history.backup(); // 备份 V2
    doc.print();

    // 操作 3: 再次修改内容
    doc.content = "Version 3";
    // 注意：这里我们不备份 V3，因为我们即将修改它
    // 历史记录里现在有 [V1, V2]
    std::cout << "\n--- Current State ---" << std::endl;
    doc.print();


    // --- 开始撤销 ---
    std::cout << "\n--- Start Undoing ---" << std::endl;

    // 第一次撤销：应该恢复到 V2
    history.undo();
    doc.print(); // 期望输出 "Version 2"

    // 第二次撤销：应该恢复到 V1
    history.undo();
    doc.print(); // 期望输出 "Version 1"

    // 第三次撤销：历史记录已空
    history.undo();
    doc.print(); // 期望输出 "Version 1"，并提示没有历史记录

    return 0;
}