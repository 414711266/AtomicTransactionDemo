/**
 * @author： Karl
 * @date：   2025年7月18日
 *
 * 事务处理demo
 */

#include <iostream>
#include <string>

 // Originator: 需要被备份状态的对象
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

int main() {
	// Caretaker (负责人) 的角色由 main 函数扮演

	// 1. 创建 Originator
	Document doc;
	doc.content = "Version 1";
	std::cout << "--- Initial State ---" << std::endl;
	doc.print();

	// 2. Caretaker 决定要进行一个危险操作(修改)，于是在操作前，向 Originator 索要一个 Memento
	std::cout << "\nCaretaker: 'I'm about to change you, Document! Give me a memento first.'" << std::endl;
	std::string memento = doc.createMemento(); // 得到备忘录

	// 3. Originator 的状态被改变
	doc.content = "Version 2";
	std::cout << "\n--- State Modified ---" << std::endl;
	doc.print();

	// 4. 用户请求撤销。Caretaker 将之前保管的 Memento 还给 Originator
	std::cout << "\nCaretaker: 'Undo! Document, restore yourself using this memento.'" << std::endl;
	doc.restoreFromMemento(memento);
	std::cout << "\n--- State Restored ---" << std::endl;
	doc.print();

	return 0;
}