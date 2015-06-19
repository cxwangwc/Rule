#include "StdAfx.h"
#include "WXmlParse.h"
#include "WScriptGenerator.h"

using namespace MSXML2;

/*
		createCDATASection : 添加CDATA标签

 */

WXmlParse::WXmlParse(const CString &strFileName)
	: m_strFileName(strFileName)
{
}


WXmlParse::~WXmlParse(void)
{

}

void WXmlParse::setFileName(const CString &strFileName)
{
	m_strFileName = strFileName;
}

const CString &WXmlParse::fileName() const
{
	return m_strFileName;
}

BOOL WXmlParse::domcoumentParseRight()
{
	return TRUE;
}

// BOOL WXmlParse::read()
// {
// 	if (m_strFileName.IsEmpty())
// 	{
// 		return FALSE;
// 	}
// 	MSXML2::IXMLDOMDocumentPtr doc;
// 	MSXML2::IXMLDOMElementPtr docRoot; 
// 	MSXML2::IXMLDOMNodeListPtr elementNodes; 
// 	MSXML2::IXMLDOMNodePtr xmlNode;  
// 	LONG length, subLength, attrLength;
// 	MSXML2::IXMLDOMNodeListPtr subElementNodes; 
// 
// 	doc.CreateInstance(__uuidof(DOMDocument30)); 
// 	doc->load((_variant_t)m_strFileName);
// 	docRoot = doc->GetdocumentElement();
// 	docRoot->get_childNodes(&elementNodes); 
// 	elementNodes->get_length(&length);
// 	if (length != 6)
// 	{
// 		return FALSE;
// 	}
// 	elementNodes->get_length(&length);
// 	
// 	elementNodes->get_item(0, &xmlNode);
// 	xmlNode->get_childNodes(&subElementNodes);
// 	subElementNodes->get_length(&subLength);
// }

BOOL WXmlParse::readParam(std::vector<WParamDefine *> &outParams)
{
	MSXML2::IXMLDOMDocumentPtr doc;
	MSXML2::IXMLDOMElementPtr docRoot;  
	MSXML2::IXMLDOMElementPtr docElement;  
	MSXML2::IXMLDOMNodeListPtr elementNodes;  
	MSXML2::IXMLDOMNamedNodeMapPtr elementAttributes;
	MSXML2::IXMLDOMNodePtr xmlNode;  
	MSXML2::IXMLDOMNodePtr xmlSubNode; 
	MSXML2::IXMLDOMNodePtr xmlAttrNode; 
	doc.CreateInstance(__uuidof(DOMDocument30));  
	if (m_strFileName.IsEmpty())
	{
		return FALSE;
	}
	doc->load((_variant_t)m_strFileName);

	docRoot = doc->GetdocumentElement();//获得根节点;  
	docRoot->get_childNodes(&elementNodes);  
	LONG length, subLength, attrLength;
	elementNodes->get_length(&length);
	MSXML2::IXMLDOMNodeListPtr subElementNodes; 

	// 0 : 
	elementNodes->get_item(0, &xmlNode);
	xmlNode->get_childNodes(&subElementNodes);
	subElementNodes->get_length(&subLength);
	for (int j = 0; j < subLength;j++)
	{
		subElementNodes->get_item(j, &xmlSubNode);//获得某个属性
		xmlSubNode->get_attributes(&elementAttributes); 
		MSXML2::IXMLDOMNodePtr proNode = elementAttributes->getNamedItem("name");
		string proName = proNode->text;
		WParamDefine *paramTest = new WParamDefine(proName);
		MSXML2::IXMLDOMNodeListPtr subAttrNodes;  
		xmlSubNode->get_childNodes(&subAttrNodes);
		subAttrNodes->get_length(&attrLength);
		for (int k = 0;k < attrLength;k++)
		{
			subAttrNodes->get_item(k, &xmlAttrNode);
			string attrName = xmlAttrNode->nodeName;
			string attrText = xmlAttrNode->text;
			paramTest->addValue(attrName, attrText);
		}
		outParams.push_back(paramTest);
	}

	return TRUE;
}

BOOL WXmlParse::writeParam(const std::vector<WParamDefine *> &params)
{
	/*MSXML2::IXMLDOMDocumentPtr doc;
	MSXML2::IXMLDOMElementPtr docRoot;  
	MSXML2::IXMLDOMElementPtr docElement;  
	MSXML2::IXMLDOMNodeListPtr elementNodes;  
	MSXML2::IXMLDOMNamedNodeMapPtr elementAttributes;
	MSXML2::IXMLDOMNodePtr xmlNode;  
	MSXML2::IXMLDOMNodePtr xmlSubNode; 
	MSXML2::IXMLDOMNodePtr xmlAttrNode; 
	doc.CreateInstance(__uuidof(DOMDocument30));  
	if (m_strFileName.IsEmpty())
	{
	return FALSE;
	}

	docRoot = doc->createElement("ModuleName");
	//docRoot->setAttribute(_T("name"), (_variant_t) _T("CategoryProduct"));

	MSXML2::IXMLDOMElementPtr inputElement = doc->createElement("InputParam");
	docRoot->appendChild(inputElement);
	for (int i = 0;i < params.size();i++)
	{

	}*/

	MSXML2::IXMLDOMDocumentPtr pDoc = NULL; 
	MSXML2::IXMLDOMElementPtr xmlRoot = NULL; 
	MSXML2::IXMLDOMElementPtr xmlInput = NULL; 
	MSXML2::IXMLDOMProcessingInstructionPtr pPI = NULL; 
	//创建DOMDocument对象 
	pDoc.CreateInstance(__uuidof(DOMDocument30)); 
	pPI = pDoc->createProcessingInstruction("xml","version='1.0' encoding='gb2312'"); 
	_variant_t vNullVal; 
	vNullVal.vt = VT_NULL; 
	pDoc->insertBefore(pPI, vNullVal);
	//创建元素并添加到文档中 
	xmlRoot = pDoc->createElement((_bstr_t)"ModuleName"); 
	//设置属性 
	
	pDoc->appendChild(xmlRoot);
	appendTextNode(pDoc,xmlRoot,1);

	xmlInput = pDoc->createElement((_bstr_t)"InputParams");
	xmlInput->setAttribute(_T("name"), (_variant_t) _T("CategoryProduct"));
	xmlRoot->appendChild(xmlInput);
	appendTextNode(pDoc, xmlInput, 2);

	for (int i = 0;i < params.size();i++)
	{
		MSXML2::IXMLDOMElementPtr pProperty = pDoc->createElement((_bstr_t)"Property");
		pProperty->setAttribute((_bstr_t)"name", (_bstr_t)(params[i]->m_name.c_str()));
		xmlInput->appendChild(pProperty);
		appendTextNode(pDoc, pProperty, 3);
		WExtraMap::iterator it = params[i]->m_extraParams.begin();
		while (it != params[i]->m_extraParams.end())
		{
			MSXML2::IXMLDOMElementPtr pText = pDoc->createElement((_bstr_t)it->first.c_str());
			pText->put_text((_bstr_t)it->second.c_str());
			pProperty->appendChild(pText);
			appendTextNode(pDoc, pProperty, 3);
			it++;
		}
	}

	//添加“author”元素 
// 	MSXML2::IXMLDOMElementPtr pNode; 
// 	pNode=pDoc->createElement((_bstr_t)"Bank"); 
// 	xmlRoot->appendChild(pNode);
// 
// 	appendTextNode(pDoc,pNode,2);
// 
// 	MSXML2::IXMLDOMElementPtr pNode2; 
// 	pNode2=pDoc->createElement((_bstr_t)"Head"); 
// 	pNode->appendChild(pNode2);
// 
// 	appendTextNode(pDoc,pNode2,3);
// 
// 	MSXML2::IXMLDOMElementPtr pNode3; 
// 	pNode3=pDoc->createElement((_bstr_t)"PBCCode"); 
// 	pNode3->Puttext("局势"); 
// 	pNode2->appendChild(pNode3);
// 
// 	appendTextNode(pDoc,pNode2,2);
// 
// 	pNode3=pDoc->createElement((_bstr_t)"Code"); 
// 	pNode3->Puttext("局势"); 
// 	pNode2->appendChild(pNode3);
// 
// 	///////////////////////////////////////////////////
// 	appendTextNode(pDoc,pNode2,2);
// 	//////////////////////////////////////////////////////
// 	appendTextNode(pDoc,pNode,2);
// 
// 	pNode2=pDoc->createElement((_bstr_t)"Table1"); 
// 	pNode->appendChild(pNode2);
// 
// 	appendTextNode(pDoc,pNode2,3);
// 
// 	pNode3=pDoc->createElement((_bstr_t)"PBCCode"); 
// 	pNode3->Puttext("局势"); 
// 	pNode2->appendChild(pNode3);
// 
// 	appendTextNode(pDoc,pNode2,2);
// 
// 	pNode3=pDoc->createElement((_bstr_t)"Code"); 
// 	pNode3->Puttext("局势"); 
// 	pNode2->appendChild(pNode3);
// 
// 	///////////////////////////////////////////////////
// 	appendTextNode(pDoc,pNode2,2);
// 	//////////////////////////////////////////////////////
// 
// 	appendTextNode(pDoc,pNode,1);
// 	appendTextNode(pDoc,xmlRoot,0);
// 
 	pDoc->save("D:\\he.xml");

	return TRUE;
}

BOOL WXmlParse::readCalculateParams(WCalcaulateParam &calParam)
{
	MSXML2::IXMLDOMDocumentPtr doc;
	MSXML2::IXMLDOMElementPtr docRoot;  
	MSXML2::IXMLDOMElementPtr docElement;  
	MSXML2::IXMLDOMNodeListPtr elementNodes;  
	MSXML2::IXMLDOMNamedNodeMapPtr elementAttributes;
	MSXML2::IXMLDOMNodePtr xmlNode;  
	MSXML2::IXMLDOMNodePtr xmlSubNode; 
	MSXML2::IXMLDOMNodePtr xmlAttrNode; 
	doc.CreateInstance(__uuidof(DOMDocument30));  
	if (m_strFileName.IsEmpty())
	{
		return FALSE;
	}
	doc->load((_variant_t)m_strFileName);

	docRoot = doc->GetdocumentElement();//获得根节点;  
	docRoot->get_childNodes(&elementNodes);  
	LONG length, subLength, attrLength;
	elementNodes->get_length(&length);
	MSXML2::IXMLDOMNodeListPtr subElementNodes; 

	// 0 : 
	elementNodes->get_item(2, &xmlNode);
	xmlNode->get_childNodes(&subElementNodes);
	subElementNodes->get_length(&subLength);
	for (int j = 0; j < subLength;j++)
	{
		subElementNodes->get_item(j, &xmlSubNode);//获得某个属性
		xmlSubNode->get_attributes(&elementAttributes); 
		MSXML2::IXMLDOMNodePtr proNode = elementAttributes->getNamedItem("name");
		string proName = proNode->text;
		MSXML2::IXMLDOMNodeListPtr subAttrNodes; 
		xmlSubNode->get_childNodes(&subAttrNodes);
		subAttrNodes->get_length(&attrLength);
//		ASSERT(attrLength == 2);

		MSXML2::IXMLDOMNodePtr descNode;
		MSXML2::IXMLDOMNodePtr stateNode;
		subAttrNodes->get_item(0, &descNode);
		subAttrNodes->get_item(1, &stateNode);
		std::vector<string> vecParam;
		string descText = descNode->text;
		string stateText = stateNode->text;
		vecParam.push_back(proName);
		vecParam.push_back(descText);
		vecParam.push_back(stateText);

		calParam.addParam(vecParam);
	}

	return TRUE;
}

BOOL WXmlParse::writeCalculateParams(const WCalcaulateParam &calParam)
{
	return TRUE;
}

BOOL WXmlParse::readCalculateRules(WCalcaulateParam &calParam)
{
	MSXML2::IXMLDOMDocumentPtr doc;
	MSXML2::IXMLDOMElementPtr docRoot;  
	MSXML2::IXMLDOMElementPtr docElement;  
	MSXML2::IXMLDOMNodeListPtr elementNodes;  
	MSXML2::IXMLDOMNamedNodeMapPtr elementAttributes;
	MSXML2::IXMLDOMNodePtr xmlNode;  
	MSXML2::IXMLDOMNodePtr xmlSubNode; 
	MSXML2::IXMLDOMNodePtr xmlAttrNode; 
	doc.CreateInstance(__uuidof(DOMDocument30));  
	if (m_strFileName.IsEmpty())
	{
		return FALSE;
	}
	doc->load((_variant_t)m_strFileName);

	docRoot = doc->GetdocumentElement();//获得根节点;  
	docRoot->get_childNodes(&elementNodes);  
	LONG length, subLength, attrLength;
	elementNodes->get_length(&length);
	MSXML2::IXMLDOMNodeListPtr subElementNodes; 

	// 0 : 
	elementNodes->get_item(4, &xmlNode);
	xmlNode->get_childNodes(&subElementNodes);
	subElementNodes->get_length(&subLength);
	for (int j = 0; j < subLength;j++)
	{
		subElementNodes->get_item(j, &xmlSubNode);//获得某个属性
		xmlSubNode->get_attributes(&elementAttributes); 
		MSXML2::IXMLDOMNodePtr proNode = elementAttributes->getNamedItem("name");
		string proName = proNode->text;
		MSXML2::IXMLDOMNodeListPtr subAttrNodes; 
		xmlSubNode->get_childNodes(&subAttrNodes);
		subAttrNodes->get_length(&attrLength);

//		ASSERT(attrLength == 4);

		MSXML2::IXMLDOMNodePtr descNode;
		MSXML2::IXMLDOMNodePtr stateNode;
		MSXML2::IXMLDOMNodePtr bResultNode;
		MSXML2::IXMLDOMNodePtr resultTypeNode;
		subAttrNodes->get_item(0, &descNode);
		subAttrNodes->get_item(1, &stateNode);
		subAttrNodes->get_item(2, &bResultNode);
		subAttrNodes->get_item(3, &resultTypeNode);
		std::vector<string> vecParam;
		string descText = descNode->text;
		string stateText = stateNode->text;
		string bResultText = bResultNode->text;
		string resultTypeNodeText = resultTypeNode->text;
		vecParam.push_back(proName);
		vecParam.push_back(descText);
		vecParam.push_back(stateText);
		vecParam.push_back(bResultText);
		vecParam.push_back(resultTypeNodeText);

		calParam.addParam(vecParam);
	}
	return TRUE;
}

BOOL WXmlParse::writeCalculateRules(const WCalcaulateParam &calParam)
{
	return TRUE;
}


BOOL WXmlParse::readObject(std::vector<ExternalObject *> &objTree)
{
	MSXML2::IXMLDOMDocumentPtr doc;
	MSXML2::IXMLDOMElementPtr docRoot;  
	MSXML2::IXMLDOMElementPtr docElement;  
	MSXML2::IXMLDOMNodeListPtr elementNodes;  
	MSXML2::IXMLDOMNamedNodeMapPtr elementAttributes;
	MSXML2::IXMLDOMNodePtr xmlNode;  
	MSXML2::IXMLDOMNodePtr xmlSubNode; 
	MSXML2::IXMLDOMNodePtr xmlAttrNode; 
	doc.CreateInstance(__uuidof(DOMDocument30));  
	doc->load((_variant_t)m_strFileName);

	docRoot = doc->GetdocumentElement();//获得根节点;  
	docRoot->get_childNodes(&elementNodes);  
	LONG length, subLength, attrLength;
	elementNodes->get_length(&length);
	MSXML2::IXMLDOMNodeListPtr subElementNodes; 

	// 3 :  递归获取对象树
	elementNodes->get_item(3, &xmlNode);
	xmlNode->get_childNodes(&subElementNodes);
	subElementNodes->get_length(&subLength);
	for (int j = 0; j < subLength;j++)
	{
		ExternalObject *objTmp = new ExternalObject();
		subElementNodes->get_item(j, &xmlSubNode);//获得某个属性
		xmlSubNode->get_attributes(&elementAttributes);
		readObject(objTmp, xmlSubNode);
		objTree.push_back(objTmp);
	}

	return TRUE;
}

BOOL WXmlParse::writeObject(const std::vector<ExternalObject *> &objTree)
{
	return TRUE;
}

BOOL WXmlParse::readObject(ExternalObject *objRet, MSXML2::IXMLDOMNodePtr &xmlSubNode)
{
	MSXML2::IXMLDOMNamedNodeMapPtr elementAttributes;
	MSXML2::IXMLDOMNodeListPtr subElementNodes; 
	MSXML2::IXMLDOMNodeListPtr subSubElementNodes; 
	MSXML2::IXMLDOMNodePtr xmlNodeTmp; 
	xmlSubNode->get_attributes(&elementAttributes);

	LONG attrLength;
	attrLength = elementAttributes->Getlength();
	MSXML2::IXMLDOMNodePtr nameNode = elementAttributes->getNamedItem("name");
	MSXML2::IXMLDOMNodePtr clsNode = elementAttributes->getNamedItem("type");
	objRet->objName = nameNode->text;
	objRet->clsName = clsNode->text;
	LONG length, subLength;
	xmlSubNode->get_childNodes(&subElementNodes);
	subElementNodes->get_length(&length);
	objRet->attri.clear();

	for (int i = 0;i < length;i++)
	{
		subElementNodes->get_item(i, &xmlNodeTmp);
		string childName = xmlNodeTmp->nodeName;
		if (strcmp(xmlNodeTmp->nodeName,"child") != 0)
		{
			xmlNodeTmp->get_attributes(&elementAttributes);
			if (!elementAttributes)
				return FALSE;
			MSXML2::IXMLDOMNodePtr typeNode = elementAttributes->getNamedItem("type");
			objRet->attri.push_back(std::make_pair(xmlNodeTmp->nodeName, std::make_pair(typeNode->text, xmlNodeTmp->text)));
		}
		else
		{
			xmlNodeTmp->get_childNodes(&subSubElementNodes);
			subSubElementNodes->get_length(&subLength);
			for (int j = 0;j < subLength;j++)
			{
				ExternalObject *objSub = new ExternalObject();
				subSubElementNodes->get_item(j, &xmlNodeTmp);
				readObject(objSub, xmlNodeTmp);
				objRet->children.push_back(objSub);
			}
		}
	}

	return TRUE;
}

BOOL WXmlParse::writeObject(const ExternalObject *objRet, MSXML2::IXMLDOMNodePtr &xmlSubNode)
{
	return TRUE;
}

BOOL WXmlParse::readExtendModName(std::vector<std::pair<string, string> > &modNames)
{
	MSXML2::IXMLDOMDocumentPtr doc;
	MSXML2::IXMLDOMElementPtr docRoot;  
	MSXML2::IXMLDOMElementPtr docElement;  
	MSXML2::IXMLDOMNodeListPtr elementNodes;  
	MSXML2::IXMLDOMNamedNodeMapPtr elementAttributes;
	MSXML2::IXMLDOMNodePtr xmlNode;  
	MSXML2::IXMLDOMNodePtr xmlSubNode; 
	MSXML2::IXMLDOMNodePtr xmlAttrNode; 
	doc.CreateInstance(__uuidof(DOMDocument30));  
	doc->load((_variant_t)m_strFileName);

	docRoot = doc->GetdocumentElement();//获得根节点;  
	docRoot->get_childNodes(&elementNodes);  
	LONG length, subLength, attrLength;
	elementNodes->get_length(&length);
	MSXML2::IXMLDOMNodeListPtr subElementNodes; 

	// 3 :  扩展模块列表
	elementNodes->get_item(1, &xmlNode);
	xmlNode->get_childNodes(&subElementNodes);
	subElementNodes->get_length(&subLength);
	for (int j = 0; j < subLength;j++)
	{
		subElementNodes->get_item(j, &xmlSubNode);//获得某个属性
		xmlSubNode->get_attributes(&elementAttributes);
		MSXML2::IXMLDOMNodePtr nameNode = elementAttributes->getNamedItem("name");
		MSXML2::IXMLDOMNodePtr descNode = elementAttributes->getNamedItem("description");
		modNames.push_back(std::make_pair(nameNode->text, descNode->text));
	}

	return TRUE;
}

BOOL WXmlParse::writeExtendModName(const std::vector<std::pair<string, string> > &modNames)
{
	return TRUE;
}

const CString &WXmlParse::lastError() const
{
	return m_strLastError;
}

// 往xml文件里添加换行、制表符
void WXmlParse::appendTextNode(MSXML2::IXMLDOMDocumentPtr &pDoc, MSXML2::IXMLDOMElementPtr &element, UINT childLevel)
{
	CString text(_T("\r\n"));
	for (UINT i = 0;i < childLevel;i++)
		text += "\t";
	element->appendChild(pDoc->createTextNode((_bstr_t)text));   
}