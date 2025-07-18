/*
 * @author�� Karl
 * @date��   2025��7��18��
 *
 * ������demo
 */

#include <iostream>
#include <string>
#include <vector> // ��Ҫ vector ���洢��ʷ��¼

 // Originator: ��Ҫ������״̬�Ķ��� (��֮ǰһ����û�б仯)
class Document {
public:
    std::string content;

    // ����һ������¼ (Memento)��������ǰ��Ҫ�����״̬
    std::string createMemento() const {
        return content;
    }

    // �ӱ���¼�лָ�״̬
    void restoreFromMemento(const std::string& memento) {
        content = memento;
    }

    void print() const {
        std::cout << "Document content: \"" << content << "\"" << std::endl;
    }
};

// Caretaker: �����ˣ�������һ��ר�ŵ���
class HistoryManager {
private:
    Document& m_doc; // ��������Ҫ֪������Ϊ�ĸ��ĵ�����
    std::vector<std::string> m_history; // ��һ�� vector ���洢����¼�б�

public:
    // ���캯������������Ҫ������ĵ�
    HistoryManager(Document& doc) : m_doc(doc) {}

    // ���ݵ�ǰ״̬
    void backup() {
        std::cout << "HistoryManager: Backing up current state..." << std::endl;
        m_history.push_back(m_doc.createMemento());
    }

    // ��������
    void undo() {
        if (m_history.empty()) {
            std::cout << "HistoryManager: No history to undo." << std::endl;
            return;
        }

        // 1. ����ʷ��¼��ȡ�����һ������¼
        std::string lastMemento = m_history.back();
        m_history.pop_back(); // ����ʷ��¼���Ƴ�

        // 2. ���������¼�ָ��ĵ�
        std::cout << "HistoryManager: Undoing..." << std::endl;
        m_doc.restoreFromMemento(lastMemento);
    }
};


int main() {
    // 1. ���� Originator �� Caretaker
    Document doc;
    HistoryManager history(doc);

    // --- ���̿�ʼ ---
    // ���� 1: ���ó�ʼ����
    doc.content = "Version 1";
    history.backup(); // ���� V1
    doc.print();

    // ���� 2: �޸�����
    doc.content = "Version 2";
    history.backup(); // ���� V2
    doc.print();

    // ���� 3: �ٴ��޸�����
    doc.content = "Version 3";
    // ע�⣺�������ǲ����� V3����Ϊ���Ǽ����޸���
    // ��ʷ��¼�������� [V1, V2]
    std::cout << "\n--- Current State ---" << std::endl;
    doc.print();


    // --- ��ʼ���� ---
    std::cout << "\n--- Start Undoing ---" << std::endl;

    // ��һ�γ�����Ӧ�ûָ��� V2
    history.undo();
    doc.print(); // ������� "Version 2"

    // �ڶ��γ�����Ӧ�ûָ��� V1
    history.undo();
    doc.print(); // ������� "Version 1"

    // �����γ�������ʷ��¼�ѿ�
    history.undo();
    doc.print(); // ������� "Version 1"������ʾû����ʷ��¼

    return 0;
}