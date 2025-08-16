#ifndef COMMAND_H
#define COMMAND_H

#include "document.h"
#include <string>
#include <memory>

// Command 接口: 定义了所有命令必须实现的行为
class Command {
public:
	virtual ~Command() = default;
	virtual void execute() = 0; // 执行/重做
	virtual void unexecute() = 0; // 撤销
};

// ConcreteCommand: 修改文档内容的具体命令
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