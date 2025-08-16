// datalayer.h
#ifndef DATALAYER_H
#define DATALAYER_H

#include <vector>
#include <memory>

class Command;

// ͨ�����ݲ㣬���ǵ� Undo/Redo ������
class DataLayer {
private:
	std::vector<std::shared_ptr<Command>> m_history;
	int m_currentVersion; // �α�

public:
	DataLayer(); // ���캯��

	void addCommand(std::shared_ptr<Command> command);
	void undo();
	void redo();
};

#endif // DATALAYER_H