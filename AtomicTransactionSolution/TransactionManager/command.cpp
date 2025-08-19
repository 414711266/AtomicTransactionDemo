// command.cpp
#include "command.h"
#include <iostream>

AtomCommand::AtomCommand(IKAtomData* pAtom, std::shared_ptr<IKAtomData> oldState)
	: m_pAtom(pAtom), m_oldState(std::move(oldState)) {}

// ��������������ύʱ�����ã����������޸ĺ������״̬
void AtomCommand::setNewState(std::shared_ptr<IKAtomData> newState) {
	m_newState = std::move(newState);
}

void AtomCommand::execute() {
	// ��ʵ�� swap �߼������Ч�������� restore ģ��
	// �� commit ʱ����Ҫ������״̬������򻯴���
	if (!m_newState) {
		// ��redoʱ����״̬�Ǵ��ڵġ���һ��ִ������ҵ���������ɵ�
		// Ϊ����redo�ܹ�����������Ҫ��commitʱ������״̬
		// ����һ���򻯣�����һ�������У����ǻ��������Ч��Swap
		m_newState = m_pAtom->clone();
	}
	std::cout << "����: ִ��/����" << std::endl;
	m_pAtom->restore(m_newState.get());
}

void AtomCommand::unexecute() {
	std::cout << "����: ����" << std::endl;
	// �ڳ���ǰ������ǰ״̬��Ϊδ���ġ���״̬��
	m_newState = m_pAtom->clone();
	m_pAtom->restore(m_oldState.get());
}

// ������ʵ��
IKAtomData* AtomCommand::getAtom() const {
	return m_pAtom;
}