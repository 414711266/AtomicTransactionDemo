// datalayer.h
#ifndef DATALAYER_H
#define DATALAYER_H

#include <vector>
#include <memory>

class Command;

// 通用数据层，我们的 Undo/Redo 管理器
class DataLayer {
private:
	std::vector<std::shared_ptr<Command>> m_history;
	int m_currentVersion; // 游标

public:
	DataLayer(); // 构造函数

	void addCommand(std::shared_ptr<Command> command);
	void undo();
	void redo();
};

#endif // DATALAYER_H