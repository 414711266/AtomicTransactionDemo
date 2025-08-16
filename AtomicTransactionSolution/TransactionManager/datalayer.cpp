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
	std::cout << "DataLayer: 命令已执行并加入历史记录。历史大小："
		<< m_history.size() << "，版本号：" << m_currentVersion << std::endl;
}

void DataLayer::undo() {
	if (m_currentVersion < 0) {
		std::cout << "DataLayer: 无法撤销，已处于初始状态。" << std::endl;
		return;
	}
	std::cout << "DataLayer: 正在撤销..." << std::endl;
	m_history[m_currentVersion]->unexecute();
	m_currentVersion--;
}

void DataLayer::redo() {
	if (m_currentVersion + 1 >= m_history.size()) {
		std::cout << "DataLayer: 无法重做，已处于最新状态。" << std::endl;
		return;
	}
	std::cout << "DataLayer: 正在重做..." << std::endl;
	m_currentVersion++;
	m_history[m_currentVersion]->execute();
}