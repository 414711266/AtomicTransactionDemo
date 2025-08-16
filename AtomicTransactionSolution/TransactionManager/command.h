#ifndef COMMAND_H
#define COMMAND_H

#include "document.h"
#include <string>
#include <memory>

// Command �ӿ�: �����������������ʵ�ֵ���Ϊ
class Command {
public:
	virtual ~Command() = default;
	virtual void execute() = 0; // ִ��/����
	virtual void unexecute() = 0; // ����
};

// ConcreteCommand: �޸��ĵ����ݵľ�������
class ChangeContentCommand : public Command {
private:
	Document& m_doc;
	std::string m_oldContent;
	std::string m_newContent;

public:
	ChangeContentCommand(Document& doc, const std::string& newContent);

	void execute() override;
	void unexecute() override;
};

#endif // COMMAND_H