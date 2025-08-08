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
	doc.print();

	// Undo 2: 恢复到 Initial
	dataLayer.undo();
	doc.print();

	// Redo 1: 前进到 V1
	dataLayer.redo();
	doc.print();

	// 此时历史状态是 V1，我们执行一个新操作
	std::cout << "\n--- New Action after Undo ---" << std::endl;
	changeContent(dataLayer, doc, "Version 3 (new branch)");
	doc.print();

	// 尝试 Redo: 应该失败，因为 V2 的历史已被覆盖
	std::cout << "\n--- Trying to Redo to V2 ---" << std::endl;
	dataLayer.redo();
	doc.print();

	return 0;
}