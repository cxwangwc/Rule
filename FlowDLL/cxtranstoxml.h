#ifndef CXTRANSTOXML_H
#define CXTRANSTOXML_H

#include "cxscriptgenerator.h" 

class _declspec(dllexport) CxTransToXml
{

public:

	CxTransToXml();
	~CxTransToXml();

	bool readTemplate(const string &);

// 	// ���浽�ļ�
	bool saveToFile(const vector<CxExternalObject *> &objList, 
		const vector<pair<string, string> > &retParams
		, const string &);

private:
	// �����������б�
	vector<string> m_tableAreaColumnTitles;

	// ��ͷ�б�����
	vector<string> m_headerTitles;

	// ������Ϣ�б�����
	vector<string> m_commonTitles;

	// xml���ڵ�
	XMLNode m_xMainNode;

	// ������Ϣ�ڵ�
	XMLNode m_commonHeadNode;
	
	XMLNode m_cardNode;
	XMLNode m_cardHeadNode;
	XMLNode m_cardTableNode;

	// ��Ƭ�ڵ�
	vector<XMLNode> m_cardNodes;
	vector<XMLNode> m_cardHeadNodes;
	vector<XMLNode> m_cardTableNodes;
};



#endif