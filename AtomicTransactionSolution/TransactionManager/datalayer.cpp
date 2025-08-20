// datalayer.cpp
#include "datalayer.h"
#include "command.h" // 需要 Command 和 IKAtomData 的完整定义
#include <algorithm> // for std::find_if
#include <iostream>

DataLayer::DataLayer()
	: m_currentVersion(-1)
{}

void DataLayer::startTrans()
{
	m_inTransaction = true;
	m_transCommands.clear();
	std::cout << "数据层: 事务开始" << '\n';
}

void DataLayer::commit()
{
	if (!m_inTransaction) return;

	if (!m_transCommands.empty())
	{
		// 如果在 undo 之后执行了新事务，清除所有未来的 redo 历史
		if (m_currentVersion + 1 < m_history.size())
		{
			m_history.resize(m_currentVersion + 1);
		}

		// 多个命令可以合并成一个大的命令，这里为简化，直接将它们视为一个整体
		// 此处我们直接把收集到的命令逐个（或打包成一个组合命令）加入历史
		// 为了简单，我们只添加第一个命令作为代表
		m_history.push_back(m_transCommands.front());
		m_currentVersion++;
		std::cout << "数据层: 事务提交。历史大小: " << m_history.size()
			<< ", 当前版本: " << m_currentVersion << '\n';
	}
	else
	{
		std::cout << "数据层: 空事务，无需提交。" << '\n';
	}

	m_inTransaction = false;
	m_transCommands.clear();
}

void DataLayer::rollback()
{
	if (!m_inTransaction) return;
	// 回滚，用旧状态恢复所有被修改的原子
	for (const auto& cmd : m_transCommands)
	{
		cmd->unexecute();
	}
	m_inTransaction = false;
	m_transCommands.clear();
	std::cout << "数据层: 事务回滚" << '\n';
}

void DataLayer::undo()
{
	if (m_currentVersion < 0)
	{
		std::cout << "数据层: 无法撤销，已在最初始状态。" << '\n';
		return;
	}
	std::cout << "数据层: 正在撤销..." << '\n';
	m_history[m_currentVersion]->unexecute();
	m_currentVersion--;
}

void DataLayer::redo() {
	if (m_currentVersion + 1 >= m_history.size())
	{
		std::cout << "数据层: 无法重做，已在最新状态。" << '\n';
		return;
	}
	std::cout << "数据层: 正在重做..." << '\n';
	m_currentVersion++;
	m_history[m_currentVersion]->execute();
}

void DataLayer::writeLockAtom(IKAtomData* pAtom)
{
	if (!m_inTransaction)
	{
		std::cout << "警告: 在事务之外修改原子对象！" << '\n';
		return; // 在严格模式下，这里应该抛出异常
	}

	// 检查这个原子在当前事务中是否已经被修改过
	// 这里需要把 Command* 向下转型为 AtomCommand* 才能调用 getAtom
	auto it = std::find_if(m_transCommands.begin(), m_transCommands.end(),
		[&](const std::shared_ptr<Command>& cmd) -> bool
		{ // 明确指定返回 bool
			AtomCommand* pAtomCmd = dynamic_cast<AtomCommand*>(cmd.get());
			if (pAtomCmd)
			{
				return pAtomCmd->getAtom() == pAtom; // 正确的比较和返回
			}
			return false;
		});

	if (it != m_transCommands.end())
	{
		// 已经为这个原子创建过命令了，直接返回
		return;
	}

	// 这是此事务中第一次修改该原子
	std::cout << "数据层: 原子被写入，创建备份..." << '\n';
	auto oldState = pAtom->clone(); // 创建备忘录
	auto cmd = std::make_shared<AtomCommand>(pAtom, oldState);
	m_transCommands.push_back(cmd);
}