#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <iostream>
#include <string>

// Originator: 文档类
class Document {
public:
	std::string content;
	void print() const {
		std::cout << "文档内容: \"" << content << "\"" << std::endl;
	}
};

#endif // DOCUMENT_H