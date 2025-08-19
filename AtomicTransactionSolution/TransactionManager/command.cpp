// command.cpp
#include "command.h"
#include <iostream>

AtomCommand::AtomCommand(IKAtomData* pAtom, std::shared_ptr<IKAtomData> oldState)
	: m_pAtom(pAtom), m_oldState(std::move(oldState)) {}

// 这个方法在事务提交时被调用，用来捕获修改后的最终状态
void AtomCommand::setNewState(std::shared_ptr<IKAtomData> newState) {
	m_newState = std::move(newState);
}

void AtomCommand::execute() {
	// 真实的 swap 逻辑会更高效，这里用 restore 模拟
	// 在 commit 时，需要捕获新状态，这里简化处理
	if (!m_newState) {
		// 在redo时，新状态是存在的。第一次执行是在业务代码里完成的
		// 为了让redo能工作，我们需要在commit时捕获新状态
		// 这是一个简化，在下一个迭代中，我们会引入更高效的Swap
		m_newState = m_pAtom->clone();
	}
	std::cout << "命令: 执行/重做" << std::endl;
	m_pAtom->restore(m_newState.get());
}

void AtomCommand::unexecute() {
	std::cout << "命令: 撤销" << std::endl;
	// 在撤销前，捕获当前状态作为未来的“新状态”
	m_newState = m_pAtom->clone();
	m_pAtom->restore(m_oldState.get());
}

// 新增的实现
IKAtomData* AtomCommand::getAtom() const {
	return m_pAtom;
}