// document.h
#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "command.h" // Ϊ�˼̳� IKAtomData
#include <string>
#include "command.h"
class Document : public IKAtomData {
private:
	std::string m_content;

public:
	const std::string& getContent() const { return m_content; }

	// "д"����
	void setContent(const std::string& newContent) {
		// �ؼ�һ�������޸�ǰ�����ú���֪ͨ DataLayer
		// ��ᴥ�����ݲ���
		KSO_WRITE_LOCK_ATOM(this);

		m_content = newContent;
	}

	// ʵ�� IKAtomData �Ľӿ�
	std::shared_ptr<IKAtomData> clone() const override {
		// ����һ���µ� Document ����
		auto newDoc = std::make_shared<Document>();
		newDoc->m_content = this->m_content;
		return newDoc;
	}

	void restore(const IKAtomData* pOther) override {
		// ����һ�� Document ״̬�лָ�
		const Document* pOtherDoc = dynamic_cast<const Document*>(pOther);
		if (pOtherDoc) {
			this->m_content = pOtherDoc->m_content;
		}
	}

	void print() const {
		std::cout << "�ĵ�����: \"" << m_content << "\"" << std::endl;
	}
};

#endif // DOCUMENT_H