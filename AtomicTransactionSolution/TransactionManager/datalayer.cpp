// datalayer.cpp
#include "datalayer.h"
#include "command.h"
#include <iostream>

DataLayer::DataLayer() : m_currentVersion(-1) {}

void DataLayer::addCommand(std::shared_ptr<Command> command) {
	if (m_currentVersion + 1 < m_history.size()) {
		m_history.resize(m_currentVersion + 1);
	}
	command->execute();
	m_history.push_back(command);
	m_currentVersion++;
	std::cout << "DataLayer: ������ִ�в�������ʷ��¼����ʷ��С��"
		<< m_history.size() << "���汾�ţ�" << m_currentVersion << std::endl;
}

void DataLayer::undo() {
	if (m_currentVersion < 0) {
		std::cout << "DataLayer: �޷��������Ѵ��ڳ�ʼ״̬��" << std::endl;
		return;
	}
	std::cout << "DataLayer: ���ڳ���..." << std::endl;
	m_history[m_currentVersion]->unexecute();
	m_currentVersion--;
}

void DataLayer::redo() {
	if (m_currentVersion + 1 >= m_history.size()) {
		std::cout << "DataLayer: �޷��������Ѵ�������״̬��" << std::endl;
		return;
	}
	std::cout << "DataLayer: ��������..." << std::endl;
	m_currentVersion++;
	m_history[m_currentVersion]->execute();
}