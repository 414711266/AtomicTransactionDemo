#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <iostream>
#include <string>

// Originator: �ĵ���
class Document {
public:
	std::string content;
	void print() const {
		std::cout << "�ĵ�����: \"" << content << "\"" << std::endl;
	}
};

#endif // DOCUMENT_H