#ifndef COMMAND_H
#define COMMAND_H

// #include "document.h"
#include <string>
#include <iostream>
class DataLayer;
class Document;

// Command �ӿ�: �����������������ʵ�ֵ���Ϊ
class Command {
public:
	virtual ~Command() = default;
	virtual void execute() = 0; // ִ��/����
	virtual void unexecute() = 0; // ����
};

// ԭ�Ӷ���Ļ���ӿ�
class IKAtomData {
protected:
	DataLayer* m_pDataLayer = nullptr;

public:
	virtual ~IKAtomData() = default;
	void init(DataLayer* pLayer) { m_pDataLayer = pLayer; }
	DataLayer* get_dataLayer() const { return m_pDataLayer; }

	// ÿ��ԭ�Ӷ������ܿ�¡�Լ������Ǵ�������¼�Ĺؼ�
	virtual std::shared_ptr<IKAtomData> clone() const = 0;
	// ÿ��ԭ�Ӷ������ܴ���һ����¡���лָ�״̬
	virtual void restore(const IKAtomData* pOther) = 0;
};

// ͨ�������������� Document ��
// ʡȥ�� ChangeContentCommand��ȡ����֮����һ��ͨ�õ� AtomCommand
class AtomCommand : public Command {
private:
	IKAtomData* m_pAtom; // ָ���޸ĵ�ԭ�ӵ�ָ�� (��ָ��)
	std::shared_ptr<IKAtomData> m_oldState; // �޸�ǰ��״̬ (����¼)
	std::shared_ptr<IKAtomData> m_newState; // �޸ĺ��״̬

public:
	AtomCommand(IKAtomData* pAtom, std::shared_ptr<IKAtomData> oldState);
	void setNewState(std::shared_ptr<IKAtomData> newState);

	void execute() override;
	void unexecute() override;
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