// document.h
#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "command.h" // 为了继承 IKAtomData
#include <string>
#include "command.h"
class Document : public IKAtomData {
private:
	std::string m_content;

public:
	const std::string& getContent() const { return m_content; }

	// "写"方法
	void setContent(const std::string& newContent) {
		// 关键一步：在修改前，调用宏来通知 DataLayer
		// 这会触发备份操作
		KSO_WRITE_LOCK_ATOM(this);

		m_content = newContent;
	}

	// 实现 IKAtomData 的接口
	std::shared_ptr<IKAtomData> clone() const override {
		// 创建一个新的 Document 副本
		auto newDoc = std::make_shared<Document>();
		newDoc->m_content = this->m_content;
		return newDoc;
	}

	void restore(const IKAtomData* pOther) override {
		// 从另一个 Document 状态中恢复
		const Document* pOtherDoc = dynamic_cast<const Document*>(pOther);
		if (pOtherDoc) {
			this->m_content = pOtherDoc->m_content;
		}
	}

	void print() const {
		std::cout << "文档内容: \"" << m_content << "\"" << std::endl;
	}
};

#endif // DOCUMENT_H