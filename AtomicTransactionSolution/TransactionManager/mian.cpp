// main.cpp
#include "datalayer.h"
#include "document.h"
#include <iostream>

int main() {
	DataLayer dataLayer;

	// 1. ͨ�� DataLayer ����һ���������� Document ԭ��
	auto doc = dataLayer.createAtom<Document>();
	doc->setContent("��ʼ�汾");
	std::cout << "--- ��ʼ״̬ ---" << std::endl;
	doc->print();

	// --- ���� 1: �޸����� ---
	std::cout << "\n--- ���� 1 ---" << std::endl;
	dataLayer.startTrans();
	doc->setContent("�汾 1"); // ֱ���޸ģ�����ڱ�����
	dataLayer.commit();
	doc->print();

	// --- ���� 2: �ٴ��޸� ---
	std::cout << "\n--- ���� 2 ---" << std::endl;
	dataLayer.startTrans();
	doc->setContent("�汾 2");
	dataLayer.commit();
	doc->print();

	// --- ��ʼ����/���� ---
	std::cout << "\n--- ���Գ��������� ---" << std::endl;

	// ���� 1: �ָ��� "�汾 1"
	dataLayer.undo();
	doc->print(); // ����: �汾 1

	// ���� 2: �ָ��� "��ʼ�汾"
	dataLayer.undo();
	doc->print(); // ����: ��ʼ�汾

	// ���� 1: ǰ���� "�汾 1"
	dataLayer.redo();
	doc->print(); // ����: �汾 1

	// ���� 2: ǰ���� "�汾 2"
	dataLayer.redo();
	doc->print(); // ����: �汾 2

	// --- ��������ع� ---
	std::cout << "\n--- ��������ع� ---" << std::endl;
	dataLayer.startTrans();
	doc->setContent("һ�����ᱻ�ύ�İ汾");
	std::cout << "�޸ĺ��ύǰ:" << std::endl;
	doc->print();
	dataLayer.rollback();
	std::cout << "�ع���:" << std::endl;
	doc->print(); // ����: �汾 2

	return 0;
}