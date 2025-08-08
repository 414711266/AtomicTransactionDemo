// datalayer.h
#ifndef DATALAYER_H
#define DATALAYER_H

#include <vector>
#include <memory>

// 前向声明 Command，避免在头文件中引入 command.h
// 这是一个优化，因为 DataLayer 的接口只需要知道 Command 是一个类型，
// 而不需要知道它的具体实现。这可以减少编译依赖。
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