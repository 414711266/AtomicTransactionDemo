// command.cpp
#include "command.h"

ChangeContentCommand::ChangeContentCommand(Document& doc, const std::string& newContent)
	: m_doc(doc), m_newContent(newContent) {
	// �������ʱ���ͱ���þ�״̬
	m_oldContent = doc.content;
}

// ִ��/���������ĵ���������Ϊ������
void ChangeContentCommand::execute() {
	m_doc.content = m_newContent;
}

// ���������ĵ����ݻָ�Ϊ������
void ChangeContentCommand::unexecute() {
	m_doc.content = m_oldContent;
}