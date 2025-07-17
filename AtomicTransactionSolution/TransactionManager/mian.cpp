/**
 * @author�� Karl
 * @date��   2025��7��18��
 *
 * ������demo
 */

#include <iostream>
#include <string>
#include <vector>

// ����һ���򵥵��ĵ��࣬�����˹��ĵĶ��� (Originator)
class SimpleDocument {
public:
    std::string content;

    void print() const {
        std::cout << "Document content: \"" << content << "\"" << std::endl;
    }
};

int main() {
    // 1. ����һ���ĵ����󣬲����ó�ʼ״̬
    SimpleDocument doc;
    doc.content = "Hello, World!";
    std::cout << "--- Initial State ---" << std::endl;
    doc.print();

    // 2. �޸�ǰ�����ֶ���������״̬
    //    ����� backup_content ������ԭʼ�� "Memento" (����¼)
    std::string backup_content = doc.content;
    std::cout << "\n...State saved manually." << std::endl;

    // 3. �޸��ĵ�����
    doc.content = "Hello, WPS DataLayer!";
    std::cout << "\n--- Modified State ---" << std::endl;
    doc.print();

    // 4. ʵ�� "Undo" �������
    //    �����ֶ��ӱ����лָ�״̬
    doc.content = backup_content;
    std::cout << "\n--- After Manual Undo ---" << std::endl;
    doc.print();

    return 0;
}