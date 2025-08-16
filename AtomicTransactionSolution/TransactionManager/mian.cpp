// main.cpp
#include "datalayer.h"
#include "command.h"
#include "document.h"

// Helper function to create and add command
void changeContent(DataLayer& dl, Document& doc, const std::string& newContent) {
	auto cmd = std::make_shared<ChangeContentCommand>(doc, newContent);
	dl.addCommand(cmd);
}

int main() {
    DataLayer dataLayer;
    Document doc;
    doc.content = "��ʼ�汾";
    std::cout << "--- ��ʼ ---" << std::endl;
    doc.print();

    // ����1: �޸�Ϊ �汾1
    std::cout << "\n--- ����1 ---" << std::endl;
    changeContent(dataLayer, doc, "�汾1");
    doc.print();

    // ����2: �޸�Ϊ �汾2
    std::cout << "\n--- ����2 ---" << std::endl;
    changeContent(dataLayer, doc, "�汾2");
    doc.print();

    // --- ��ʼ����/���� ---
    std::cout << "\n--- ���Գ���/���� ---" << std::endl;

    // ����1: �ָ��� �汾1
    dataLayer.undo();
    doc.print();

    // ����2: �ָ��� ��ʼ�汾
    dataLayer.undo();
    doc.print();

    // ����1: ǰ���� �汾1
    dataLayer.redo();
    doc.print();

    // ��ʱ��ʷ״̬�� �汾1��ִ��һ���²���
    std::cout << "\n--- ������ִ���²��� ---" << std::endl;
    changeContent(dataLayer, doc, "�汾3���·�֧��");
    doc.print();

    // ����������Ӧʧ�ܣ���Ϊ �汾2 ����ʷ�ѱ�����
    std::cout << "\n--- ���������ذ汾2 ---" << std::endl;
    dataLayer.redo();
    doc.print();

    return 0;
}