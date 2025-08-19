// datalayer.h
#ifndef DATALAYER_H
#define DATALAYER_H

#include <vector>
#include <memory>

class Command;
class IKAtomData; // 前向声明

// 宏，用于在原子对象的写方法中自动加锁
// 这是整个自动化魔法的核心！
#define KSO_WRITE_LOCK_ATOM(pAtom) \
    (pAtom)->get_dataLayer()->writeLockAtom(pAtom)

// 通用数据层，我们的 Undo/Redo 管理器
class DataLayer {
private:
	std::vector<std::shared_ptr<Command>> m_history;
	size_t m_currentVersion; // 游标
	bool m_inTransaction = false; // 是否在事务中
	std::vector<std::shared_ptr<Command>> m_transCommands; // 临时存放事务中的命令

public:
	DataLayer(); // 构造函数

	// 事务控制
	void startTrans();
	void commit();
	void rollback(); // 回滚/取消事务

	void addCommand(const std::shared_ptr<Command>& command);

	void undo();
	void redo();

	// 核心：当原子对象需要被修改时，会调用这个函数
	void writeLockAtom(IKAtomData* pAtom);

	// 模板函数：用于创建与本 DataLayer 绑定的原子对象
	template<typename T, typename... Args>
	std::shared_ptr<T> createAtom(Args&&... args) {
		auto pAtom = std::make_shared<T>(std::forward<Args>(args)...);
		pAtom->init(this); // 将原子与 DataLayer 关联
		return pAtom;
	}
};

#endif // DATALAYER_H