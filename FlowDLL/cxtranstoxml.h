#ifndef CXTRANSTOXML_H
#define CXTRANSTOXML_H

#include "cxscriptgenerator.h" 

class _declspec(dllexport) CxTransToXml
{

public:

	CxTransToXml();
	~CxTransToXml();

	bool readTemplate(const string &);

// 	// 保存到文件
	bool saveToFile(const vector<CxExternalObject *> &objList, 
		const vector<pair<string, string> > &retParams
		, const string &);

private:
	// 表区列名称列表
	vector<string> m_tableAreaColumnTitles;

	// 表头列表名称
	vector<string> m_headerTitles;

	// 公共信息列表名称
	vector<string> m_commonTitles;

	// xml根节点
	XMLNode m_xMainNode;

	// 公共信息节点
	XMLNode m_commonHeadNode;
	
	XMLNode m_cardNode;
	XMLNode m_cardHeadNode;
	XMLNode m_cardTableNode;

	// 卡片节点
	vector<XMLNode> m_cardNodes;
	vector<XMLNode> m_cardHeadNodes;
	vector<XMLNode> m_cardTableNodes;
};



#endif