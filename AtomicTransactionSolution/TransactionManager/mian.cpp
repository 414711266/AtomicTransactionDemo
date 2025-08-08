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
	doc.print();

	// Undo 2: �ָ��� Initial
	dataLayer.undo();
	doc.print();

	// Redo 1: ǰ���� V1
	dataLayer.redo();
	doc.print();

	// ��ʱ��ʷ״̬�� V1������ִ��һ���²���
	std::cout << "\n--- New Action after Undo ---" << std::endl;
	changeContent(dataLayer, doc, "Version 3 (new branch)");
	doc.print();

	// ���� Redo: Ӧ��ʧ�ܣ���Ϊ V2 ����ʷ�ѱ�����
	std::cout << "\n--- Trying to Redo to V2 ---" << std::endl;
	dataLayer.redo();
	doc.print();

	return 0;
}