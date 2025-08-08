// datalayer.cpp
#include "datalayer.h"
#include "command.h" // 在这里才需要 Command 的完整定义
#include <iostream>

DataLayer::DataLayer() : m_currentVersion(-1) {}

void DataLayer::addCommand(std::shared_ptr<Command> command) {
	if (m_currentVersion + 1 < m_history.size()) {
		m_history.resize(m_currentVersion + 1);
	}
	command->execute();
	m_history.push_back(command);
	m_currentVersion++;
	std::cout << "DataLayer: Command executed and added. History size: "
		<< m_history.size() << ", Version: " << m_currentVersion << std::endl;
}

void DataLayer::undo() {
	if (m_currentVersion < 0) {
		std::cout << "DataLayer: Cannot undo. At initial state." << std::endl;
		return;
	}
	std::cout << "DataLayer: Undoing..." << std::endl;
	m_history[m_currentVersion]->unexecute();
	m_currentVersion--;
}

void DataLayer::redo() {
	if (m_currentVersion + 1 >= m_history.size()) {
		std::cout << "DataLayer: Cannot redo. At latest state." << std::endl;
		return;
	}
	std::cout << "DataLayer: Redoing..." << std::endl;
	m_currentVersion++;
	m_history[m_currentVersion]->execute();
}