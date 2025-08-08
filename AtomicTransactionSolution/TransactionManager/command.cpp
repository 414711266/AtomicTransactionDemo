// command.cpp
#include "command.h"

ChangeContentCommand::ChangeContentCommand(Document& doc, const std::string& newContent)
	: m_doc(doc), m_newContent(newContent) {
	// 在命令创建时，就保存好旧状态
	m_oldContent = doc.content;
}

// 执行/重做：将文档内容设置为新内容
void ChangeContentCommand::execute() {
	m_doc.content = m_newContent;
}

// 撤销：将文档内容恢复为旧内容
void ChangeContentCommand::unexecute() {
	m_doc.content = m_oldContent;
}