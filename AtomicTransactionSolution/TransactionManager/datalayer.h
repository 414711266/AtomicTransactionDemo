// datalayer.h
#ifndef DATALAYER_H
#define DATALAYER_H

#include <vector>
#include <memory>

class Command;
class IKAtomData; // ǰ������

// �꣬������ԭ�Ӷ����д�������Զ�����
// ���������Զ���ħ���ĺ��ģ�
#define KSO_WRITE_LOCK_ATOM(pAtom) \
    (pAtom)->get_dataLayer()->writeLockAtom(pAtom)

// ͨ�����ݲ㣬���ǵ� Undo/Redo ������
class DataLayer {
private:
	std::vector<std::shared_ptr<Command>> m_history;
	size_t m_currentVersion; // �α�
	bool m_inTransaction = false; // �Ƿ���������
	std::vector<std::shared_ptr<Command>> m_transCommands; // ��ʱ��������е�����

public:
	DataLayer(); // ���캯��

	// �������
	void startTrans();
	void commit();
	void rollback(); // �ع�/ȡ������

	void addCommand(const std::shared_ptr<Command>& command);

	void undo();
	void redo();

	// ���ģ���ԭ�Ӷ�����Ҫ���޸�ʱ��������������
	void writeLockAtom(IKAtomData* pAtom);

	// ģ�庯�������ڴ����뱾 DataLayer �󶨵�ԭ�Ӷ���
	template<typename T, typename... Args>
	std::shared_ptr<T> createAtom(Args&&... args) {
		auto pAtom = std::make_shared<T>(std::forward<Args>(args)...);
		pAtom->init(this); // ��ԭ���� DataLayer ����
		return pAtom;
	}
};

#endif // DATALAYER_H