#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <iostream>
#include <string>

// Originator: 我们的文档类
class Document {
public:
	std::string content;
	void print() const {
		std::cout << "Document content: \"" << content << "\"" << std::endl;
	}
};

#endif // DOCUMENT_H