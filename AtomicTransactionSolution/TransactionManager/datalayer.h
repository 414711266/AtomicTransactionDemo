// datalayer.h
#ifndef DATALAYER_H
#define DATALAYER_H

#include <vector>
#include <memory>

// ǰ������ Command��������ͷ�ļ������� command.h
// ����һ���Ż�����Ϊ DataLayer �Ľӿ�ֻ��Ҫ֪�� Command ��һ�����ͣ�
// ������Ҫ֪�����ľ���ʵ�֡�����Լ��ٱ���������
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