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
    doc.content = "初始版本";
    std::cout << "--- 开始 ---" << std::endl;
    doc.print();

    // 操作1: 修改为 版本1
    std::cout << "\n--- 操作1 ---" << std::endl;
    changeContent(dataLayer, doc, "版本1");
    doc.print();

    // 操作2: 修改为 版本2
    std::cout << "\n--- 操作2 ---" << std::endl;
    changeContent(dataLayer, doc, "版本2");
    doc.print();

    // --- 开始撤销/重做 ---
    std::cout << "\n--- 测试撤销/重做 ---" << std::endl;

    // 撤销1: 恢复到 版本1
    dataLayer.undo();
    doc.print();

    // 撤销2: 恢复到 初始版本
    dataLayer.undo();
    doc.print();

    // 重做1: 前进到 版本1
    dataLayer.redo();
    doc.print();

    // 此时历史状态是 版本1，执行一个新操作
    std::cout << "\n--- 撤销后执行新操作 ---" << std::endl;
    changeContent(dataLayer, doc, "版本3（新分支）");
    doc.print();

    // 尝试重做：应失败，因为 版本2 的历史已被覆盖
    std::cout << "\n--- 尝试重做回版本2 ---" << std::endl;
    dataLayer.redo();
    doc.print();

    return 0;
}