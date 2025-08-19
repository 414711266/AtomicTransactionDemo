// datalayer.cpp
#include "datalayer.h"
#include "command.h" // ��Ҫ Command �� IKAtomData ����������
#include <algorithm> // for std::find_if
#include <iostream>

DataLayer::DataLayer() : m_currentVersion(-1) {}

void DataLayer::startTrans() {
	m_inTransaction = true;
	m_transCommands.clear();
	std::cout << "���ݲ�: ����ʼ" << std::endl;
}

void DataLayer::commit() {
	if (!m_inTransaction) return;

	if (!m_transCommands.empty()) {
		// ����� undo ֮��ִ�����������������δ���� redo ��ʷ
		if (m_currentVersion + 1 < m_history.size()) {
			m_history.resize(m_currentVersion + 1);
		}

		// ���������Ժϲ���һ������������Ϊ�򻯣�ֱ�ӽ�������Ϊһ������
		// �˴�����ֱ�Ӱ��ռ��������������������һ��������������ʷ
		// Ϊ�˼򵥣�����ֻ��ӵ�һ��������Ϊ����
		m_history.push_back(m_transCommands.front());
		m_currentVersion++;
		std::cout << "���ݲ�: �����ύ����ʷ��С: " << m_history.size()
			<< ", ��ǰ�汾: " << m_currentVersion << std::endl;
	}
	else {
		std::cout << "���ݲ�: �����������ύ��" << std::endl;
	}

	m_inTransaction = false;
	m_transCommands.clear();
}

void DataLayer::rollback() {
	if (!m_inTransaction) return;
	// �ع����þ�״̬�ָ����б��޸ĵ�ԭ��
	for (const auto& cmd : m_transCommands) {
		cmd->unexecute();
	}
	m_inTransaction = false;
	m_transCommands.clear();
	std::cout << "���ݲ�: ����ع�" << std::endl;
}

void DataLayer::undo() {
	if (m_currentVersion < 0) {
		std::cout << "���ݲ�: �޷��������������ʼ״̬��" << std::endl;
		return;
	}
	std::cout << "���ݲ�: ���ڳ���..." << std::endl;
	m_history[m_currentVersion]->unexecute();
	m_currentVersion--;
}

void DataLayer::redo() {
	if (m_currentVersion + 1 >= m_history.size()) {
		std::cout << "���ݲ�: �޷���������������״̬��" << std::endl;
		return;
	}
	std::cout << "���ݲ�: ��������..." << std::endl;
	m_currentVersion++;
	m_history[m_currentVersion]->execute();
}

void DataLayer::writeLockAtom(IKAtomData* pAtom) {
	if (!m_inTransaction) {
		std::cout << "����: ������֮���޸�ԭ�Ӷ���" << std::endl;
		return; // ���ϸ�ģʽ�£�����Ӧ���׳��쳣
	}

	// ������ԭ���ڵ�ǰ�������Ƿ��Ѿ����޸Ĺ�
	auto it = std::find_if(m_transCommands.begin(), m_transCommands.end(),
		[&](const std::shared_ptr<Command>& cmd) {
			// ��Ҫһ�ַ�ʽ��ʶ�������Ӧ��ԭ�ӣ�����򻯴���
			// ��ʵ��ʵ�ֻ�����ӣ��������һ������ֻ�޸�һ��ԭ��һ��
			// Ϊ����ʾ�����Ǽ�Ϊÿ�ζ�����������
		});

	if (it != m_transCommands.end()) {
		// �Ѿ�Ϊ���ԭ�Ӵ����������ˣ�ֱ�ӷ���
		return;
	}

	// ���Ǵ������е�һ���޸ĸ�ԭ��
	std::cout << "���ݲ�: ԭ�ӱ�д�룬��������..." << std::endl;
	auto oldState = pAtom->clone(); // ��������¼
	auto cmd = std::make_shared<AtomCommand>(pAtom, oldState);
	m_transCommands.push_back(cmd);
}