/**
 * @author�� Karl
 * @date��   2025��7��18��
 *
 * ������demo
 */

#include <iostream>
#include <string>

 // Originator: ��Ҫ������״̬�Ķ���
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

int main() {
	// Caretaker (������) �Ľ�ɫ�� main ��������

	// 1. ���� Originator
	Document doc;
	doc.content = "Version 1";
	std::cout << "--- Initial State ---" << std::endl;
	doc.print();

	// 2. Caretaker ����Ҫ����һ��Σ�ղ���(�޸�)�������ڲ���ǰ���� Originator ��Ҫһ�� Memento
	std::cout << "\nCaretaker: 'I'm about to change you, Document! Give me a memento first.'" << std::endl;
	std::string memento = doc.createMemento(); // �õ�����¼

	// 3. Originator ��״̬���ı�
	doc.content = "Version 2";
	std::cout << "\n--- State Modified ---" << std::endl;
	doc.print();

	// 4. �û���������Caretaker ��֮ǰ���ܵ� Memento ���� Originator
	std::cout << "\nCaretaker: 'Undo! Document, restore yourself using this memento.'" << std::endl;
	doc.restoreFromMemento(memento);
	std::cout << "\n--- State Restored ---" << std::endl;
	doc.print();

	return 0;
}