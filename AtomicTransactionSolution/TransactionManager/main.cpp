// main.cpp
#include "datalayer.h"
#include "document.h"
#include <iostream>

int main() {
	DataLayer dataLayer;

	// 1. 通过 DataLayer 创建一个受其管理的 Document 原子
	auto doc = dataLayer.createAtom<Document>();
	doc->setContent("初始版本");
	std::cout << "--- 初始状态 ---" << '\n';
	doc->print();

	// --- 事务 1: 修改内容 ---
	std::cout << "\n--- 事务 1 ---" << '\n';
	dataLayer.startTrans();
	doc->setContent("版本 1"); // 直接修改，框架在背后工作
	dataLayer.commit();
	doc->print();

	// --- 事务 2: 再次修改 ---
	std::cout << "\n--- 事务 2 ---" << '\n';
	dataLayer.startTrans();
	doc->setContent("版本 2");
	dataLayer.commit();
	doc->print();

	// --- 开始撤销/重做 ---
 	std::cout << "\n--- 测试撤销与重做 ---" << '\n';

	// 撤销 1: 恢复到 "版本 1"
	dataLayer.undo();
	doc->print(); // 期望: 版本 1

	// 撤销 2: 恢复到 "初始版本"
	dataLayer.undo();
	doc->print(); // 期望: 初始版本

	// 重做 1: 前进到 "版本 1"
	dataLayer.redo();
	doc->print(); // 期望: 版本 1

	// 重做 2: 前进到 "版本 2"
	dataLayer.redo();
	doc->print(); // 期望: 版本 2

	// --- 测试事务回滚 ---
	std::cout << "\n--- 测试事务回滚 ---" << '\n';
	dataLayer.startTrans();
	doc->setContent("一个不会被提交的版本");
	std::cout << "修改后，提交前:" << '\n';
	doc->print();
	dataLayer.rollback();
	std::cout << "回滚后:" << '\n';
	doc->print(); // 期望: 版本 2

	return 0;
}