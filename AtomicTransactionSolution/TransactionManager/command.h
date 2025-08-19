#ifndef COMMAND_H
#define COMMAND_H

// #include "document.h"
#include <string>
#include <iostream>
class DataLayer;
class Document;

// Command 接口: 定义了所有命令必须实现的行为
class Command {
public:
	virtual ~Command() = default;
	virtual void execute() = 0; // 执行/重做
	virtual void unexecute() = 0; // 撤销
};

// 原子对象的基类接口
class IKAtomData {
protected:
	DataLayer* m_pDataLayer = nullptr;

public:
	virtual ~IKAtomData() = default;
	void init(DataLayer* pLayer) { m_pDataLayer = pLayer; }
	DataLayer* get_dataLayer() const { return m_pDataLayer; }

	// 每个原子都必须能克隆自己，这是创建备忘录的关键
	virtual std::shared_ptr<IKAtomData> clone() const = 0;
	// 每个原子都必须能从另一个克隆体中恢复状态
	virtual void restore(const IKAtomData* pOther) = 0;
};

// 通用命令，不再是针对 Document 的
// 省去了 ChangeContentCommand，取而代之的是一个通用的 AtomCommand
class AtomCommand : public Command {
private:
	IKAtomData* m_pAtom; // 指向被修改的原子的指针 (裸指针)
	std::shared_ptr<IKAtomData> m_oldState; // 修改前的状态 (备忘录)
	std::shared_ptr<IKAtomData> m_newState; // 修改后的状态

public:
	AtomCommand(IKAtomData* pAtom, std::shared_ptr<IKAtomData> oldState);
	void setNewState(std::shared_ptr<IKAtomData> newState);

	void execute() override;
	void unexecute() override;
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