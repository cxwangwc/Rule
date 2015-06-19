#include "StdAfx.h"
#include "cxtranstoxml.h"
CxTransToXml::CxTransToXml()
{

}

CxTransToXml::~CxTransToXml()
{

}

bool CxTransToXml::readTemplate(const string &fileName)
{
	m_xMainNode = XMLNode::openFileHelper(fileName.c_str(), "REPORT");

	if (m_xMainNode.isEmpty())
		return false;

	m_commonTitles.clear();
	m_headerTitles.clear();
	m_tableAreaColumnTitles.clear();

	m_commonHeadNode = m_xMainNode.getChildNode("HEAD");
	for (int i = 0;i < m_commonHeadNode.nChildNode();i++)
	{
		XMLNode commondInfo = m_commonHeadNode.getChildNode(i);
		m_commonTitles.push_back(commondInfo.getAttribute("name"));
	}

	if (m_xMainNode.nChildNode() > 1)
	{
		m_cardNode = m_xMainNode.getChildNode("CARD");
		m_cardHeadNode = m_cardNode.getChildNode("HEAD");

		for (int i = 0;i < m_cardHeadNode.nChildNode();i++)
		{
			XMLNode cardHeadInfo = m_cardHeadNode.getChildNode(i);
			m_headerTitles.push_back(cardHeadInfo.getAttribute("name"));
		}

		m_cardTableNode = m_cardNode.getChildNode("TABLE");
		if (m_cardTableNode.nChildNode() != 0)
		{
			XMLNode lineNode = m_cardTableNode.getChildNode("LINERECORD");
			for (int i = 0;i < lineNode.nChildNode();i++)
			{
				XMLNode lineHeadInfo = lineNode.getChildNode(i);
				m_tableAreaColumnTitles.push_back(lineHeadInfo.getAttribute("name"));
			}
		}
	}

	return true;
}

bool CxTransToXml::saveToFile(const vector<CxExternalObject *> &objList, 
	const vector<pair<string, string> > &retParams
	, const string &fileName)
{
	XMLNode dNode;
	while (m_commonHeadNode.nChildNode() != 0)
	{
		dNode = m_commonHeadNode.getChildNode(0);
		dNode.deleteNodeContent();
	}

	while (m_cardHeadNode.nChildNode() != 0)
	{
		dNode = m_cardHeadNode.getChildNode(0);
		dNode.deleteNodeContent();
	}
	while (m_cardTableNode.nChildNode() != 0)
	{
		dNode = m_cardTableNode.getChildNode(0);
		dNode.deleteNodeContent();
	}

	for (int i = 0;i < retParams.size();i++)
	{
		// 在通用信息里
		if (CxCommonFunc::contains(m_commonTitles, CxCommonFunc::U2G(retParams[i].first.c_str())))
		{
			XMLNode titleNode = m_commonHeadNode.addChild("CommonNode");
			titleNode.addAttribute("name", CxCommonFunc::U2G(retParams[i].first.c_str()));
			titleNode.addText(CxCommonFunc::U2G(retParams[i].second.c_str()));
		}
		// 在卡片头信息里面
		else if (CxCommonFunc::contains(m_headerTitles, CxCommonFunc::U2G(retParams[i].first.c_str())))
		{
			XMLNode titleNode = m_cardHeadNode.addChild("COLUMN");
			titleNode.addAttribute("name", CxCommonFunc::U2G(retParams[i].first.c_str()));
			XMLNode cellNode = titleNode.addChild("CELL");
			cellNode.addAttribute("Row", "0");
			cellNode.addText(CxCommonFunc::U2G(retParams[i].second.c_str()));
		}
	}

	char recordNum[10] = {0};
	for (int i = 0;i < objList.size();i++)
	{
		XMLNode lineNode = m_cardTableNode.addChild("LINERECORD");
		sprintf(recordNum, "%d", i);
		lineNode.addAttribute("RecordNum", recordNum);
		for (int j = 0;j < objList[i]->attri.size();j++)
		{
			if (j < m_tableAreaColumnTitles.size())
			{
				XMLNode columnNode = lineNode.addChild("COLUMN");
				columnNode.addAttribute("name", m_tableAreaColumnTitles[j].c_str());
				columnNode.addText(CxCommonFunc::U2G(objList[i]->attri[j].second.second.c_str()));
			}
		}
	}

	m_xMainNode.addChild(m_commonHeadNode);
	m_xMainNode.addChild(m_cardNode);

	//m_xMainNode.writeToFile(fileName.c_str(), "utf-8");
	m_xMainNode.writeToFile(fileName.c_str(), "GB2312");

	return true;
}