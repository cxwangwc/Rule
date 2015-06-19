// #include "StdAfx.h"
// #include "cxflowparse.h"
// 
// #include "cxscriptgenerator.h"
// 
// CxKnowlegdeDesc::CxKnowlegdeDesc(const string &ID, const string &name, const string &description
// 	, const string &version, const string &pixFilename, const string &fileVersion)
// 	: m_strID(ID)
// 	, m_strName(name)
// 	, m_strDescription(description)
// 	, m_strVersion(version)
// 	, m_strPixFilename(pixFilename)
// 	, m_strFileVersion(fileVersion)
// {
// 
// }
// 
// CxKnowlegdeDesc::CxKnowlegdeDesc()
// {
// 
// }
// 
// /*
// 		createCDATASection : 添加CDATA标签
// 
//  */
// 
// CxFlowParse::CxFlowParse(const string &strFileName)
// {
// 	if (strFileName.size() != 0)
// 	{
// 		setFileName(strFileName);
// 	}
// }
// 
// 
// CxFlowParse::~CxFlowParse(void)
// {
// 
// }
// 
// bool CxFlowParse::setFileName(const string &strFileName)
// {
// 	m_strFileName = strFileName;
// 	m_xMainNode = XMLNode::openFileHelper(m_strFileName.c_str(), "Knowledge");
// 	if (m_xMainNode.isEmpty())
// 		return false;
// 	m_knowlegdeDescription.m_strFileName = strFileName;
// 	m_knowlegdeDescription.m_strName = m_xMainNode.getAttribute("name");
// 	m_knowlegdeDescription.m_strID = m_xMainNode.getAttribute("id");
// 	m_knowlegdeDescription.m_strDescription = m_xMainNode.getAttribute("description");
// 	m_knowlegdeDescription.m_strVersion = m_xMainNode.getAttribute("version");
// 	m_knowlegdeDescription.m_strPixFilename = m_xMainNode.getAttribute("pixfilename");
// 	readFileVersion(m_knowlegdeDescription.m_strFileVersion);
// 	
// 	return true;
// }
// 
// void CxFlowParse::setUUID(const string &uuid)
// {
// 	m_knowlegdeDescription.m_strID = uuid;
// }
// 
// const string &CxFlowParse::fileName() const
// {
// 	return m_strFileName;
// }
// 
// const CxKnowlegdeDesc &CxFlowParse::knowledgetDescription()
// {
// 	return m_knowlegdeDescription;
// }
// 
// void CxFlowParse::setKnowledgeDescription(const CxKnowlegdeDesc &knowledge)
// {
// 	m_knowlegdeDescription = knowledge;
// }
// 
// bool CxFlowParse::readParam(std::vector<CxInputParam *> &outParams)
// {
// 	XMLNode xNode = m_xMainNode.getChildNode("InputParams");
// 	int paramCount = xNode.nChildNode();
// 
// 	for (int i = 0;i < paramCount;i++)
// 	{
// 		XMLNode paramNode = xNode.getChildNode(i);
// 		string proName = paramNode.getAttribute("name");
// 		string nullAllowed = paramNode.getAttribute("nullallowed");
// 		int propertyCount = paramNode.nChildNode();
// 		CxInputParam *paramTest = new CxInputParam(proName);
// 		paramTest->m_nullAllowed = nullAllowed;
// 		paramTest->m_min = paramNode.getChildNode("min").getText() == 0 ? "" : paramNode.getChildNode("min").getText();
// 		paramTest->m_max = paramNode.getChildNode("max").getText() == 0 ? "" : paramNode.getChildNode("max").getText();
// 		paramTest->m_value = paramNode.getChildNode("value").getText() == 0 ? "" : paramNode.getChildNode("value").getText();
// 		paramTest->m_description = paramNode.getChildNode("description").getText() == 0 ? "" : paramNode.getChildNode("description").getText();
// 		paramTest->m_type = paramNode.getChildNode("type").getText() == 0 ? "" : paramNode.getChildNode("type").getText();
// 		outParams.push_back(paramTest);
// 	}
// 
// 	return true;
// }
// 
// bool CxFlowParse::writeParam(const std::vector<CxInputParam *> &params)
// {
// 	m_xMainNode = m_xMainNode.createXMLTopNode("Knowledge");
// 	m_xMainNode.addAttribute("name", CxCommonFunc::U2G(m_knowlegdeDescription.m_strName.c_str()));
// 	m_xMainNode.addAttribute("id", CxCommonFunc::U2G(m_knowlegdeDescription.m_strID.c_str()));
// 	m_xMainNode.addAttribute("description", CxCommonFunc::U2G(m_knowlegdeDescription.m_strDescription.c_str()));
// 	m_xMainNode.addAttribute("version", CxCommonFunc::U2G(m_knowlegdeDescription.m_strVersion.c_str()));
// 	m_xMainNode.addAttribute("pixfilename", CxCommonFunc::U2G(m_knowlegdeDescription.m_strPixFilename.c_str()));
// 	XMLNode versionNode = m_xMainNode.addChild("FileVersion");
// 	versionNode.addText(m_knowlegdeDescription.m_strFileVersion.c_str());
// 	XMLNode inputNode = m_xMainNode.addChild("InputParams");
// 	for (int i = 0;i < params.size();i++)
// 	{
// 		XMLNode paramNode = inputNode.addChild("Param");
// 		paramNode.addAttribute("name", CxCommonFunc::U2G(params[i]->m_name.c_str()));
// 		paramNode.addAttribute("nullallowed", CxCommonFunc::U2G(params[i]->m_nullAllowed.c_str()));
// 		XMLNode propertyNode = paramNode.addChild("min");
// 		propertyNode.addText(params[i]->m_min.c_str());
// 		propertyNode = paramNode.addChild("max");
// 		propertyNode.addText(params[i]->m_max.c_str());
// 		propertyNode = paramNode.addChild("value");
// 		propertyNode.addText(params[i]->m_value.c_str());
// 		propertyNode = paramNode.addChild("description");
// 		propertyNode.addText(params[i]->m_description.c_str());
// 		propertyNode = paramNode.addChild("type");
// 		propertyNode.addText(params[i]->m_type.c_str());
// 	}
// 	return true;
// }
// 
// bool CxFlowParse::readFileVersion(string &version)
// {
// 	XMLNode xNode = m_xMainNode.getChildNode("FileVersion");
// 	if (xNode.isEmpty())
// 		return false;
// 	version = xNode.getText() ? xNode.getText() : "";
// 	return true;
// }
// 
// bool CxFlowParse::writeFileVersion(const string &version)
// {
// 	XMLNode inputNode = m_xMainNode.addChild("FileVersion");
// 	inputNode.addText(version.c_str());
// 	return true;
// }
// 
// bool CxFlowParse::readCalculateRules(CxCalculateRule &calParam)
// {
// 	XMLNode xNode = m_xMainNode.getChildNode("Rules");
// 	int paramCount = xNode.nChildNode();
// 
// 	for (int i = 0;i < paramCount;i++)
// 	{
// 		XMLNode paramNode = xNode.getChildNode(i);
// 		string proName = paramNode.getAttribute("name");
// 		std::vector<string> vecParam;
// 		XMLNode descNode = paramNode.getChildNode("description");
// 		XMLNode stateNode = paramNode.getChildNode("Statement");
// 		XMLClear stateClear = stateNode.getClear();
// 		vecParam.push_back(proName);
// 		vecParam.push_back(descNode.getText() == 0 ? "" : descNode.getText());
// 		vecParam.push_back(stateClear.lpszValue);
// 		calParam.addParam(vecParam);
// 	}
// 	return true;
// }
// 
// bool CxFlowParse::writeCalculateRules(const CxCalculateRule &calRule)
// {
// 	XMLNode cpNode = m_xMainNode.addChild("Rules");
// 	char tmp[10] = {0};
// 	const std::vector<CxCalculateRulePrivate> &rules = calRule.calculateParam();
// 	for (int i = 0;i < rules.size();i++)
// 	{
// 		XMLNode paramNode = cpNode.addChild("Rule");
// 		paramNode.addAttribute("name", CxCommonFunc::U2G(rules[i].m_strName.c_str()));
// 		XMLNode descNode = paramNode.addChild("description");
// 		XMLNode statementNode = paramNode.addChild("Statement");
// 		descNode.addText(rules[i].m_strDescription.c_str());
// 		statementNode.addClear(rules[i].m_strStatement.c_str());
// 	}
// 
// 	return true;
// }
// 
// 
// bool CxFlowParse::readOutput(std::vector<CxExternalObject *> &objTree, std::vector<CxOutputParam> &outParam)
// {
// 	XMLNode xNode = m_xMainNode.getChildNode("Output");
// 	XMLNode objectsNode = xNode.getChildNode("OutputObjects");
// 	XMLNode paramsNode = xNode.getChildNode("OutputParams");
// 	int objectCount = objectsNode.nChildNode();
// 	int paramsCount = paramsNode.nChildNode();
// 
// 	for (int i = 0;i < objectCount;i++)
// 	{
// 		CxExternalObject *objTmp = new CxExternalObject();
// 		XMLNode objectNode = objectsNode.getChildNode(i);
// 		readOutputObject(objTmp, objectNode);
// 		objTree.push_back(objTmp);
// 	}
// 
// 	for (int i = 0;i < paramsCount;i++)
// 	{
// 		CxOutputParam param;
// 		XMLNode paramNode = paramsNode.getChildNode(i);
// 		readOutputParam(param, paramNode);
// 		outParam.push_back(param);
// 	}
// 
// 	return true;
// }
// 
// bool CxFlowParse::writeOutput(const std::vector<CxExternalObject *> &objTree, const std::vector<CxOutputParam> &outParam)
// {
// 	XMLNode objRootNode = m_xMainNode.addChild("Output");
// 
// 	XMLNode outputParamsNode = objRootNode.addChild("OutputParams");
// 	XMLNode outputObjectNode = objRootNode.addChild("OutputObjects");
// 
// 	for (int i = 0;i < outParam.size();i++)
// 	{
// 		XMLNode paramNode = outputParamsNode.addChild("Param");
// 		writeOutputParam(outParam[i], paramNode);
// 	}
// 
// 	for (int i = 0;i < objTree.size();i++)
// 	{
// 		XMLNode objNode = outputObjectNode.addChild("Object");
// 		writeOutputObject(objTree[i], objNode);
// 	}
// 	return true;
// }
// 
// bool CxFlowParse::readOutputObject(CxExternalObject *objRet, XMLNode &xmlSubNode)
// {
// 	objRet->objName = xmlSubNode.getAttribute("name");
// 	objRet->clsName = xmlSubNode.getAttribute("type");
// 	objRet->attri.clear();
// 
// 	for (int i = 0;i < xmlSubNode.nChildNode();i++)
// 	{
// 		XMLNode node = xmlSubNode.getChildNode(i);
// 		if (strcmp(node.getName(), "child") != 0)
// 		{
// 			objRet->attri.push_back(std::make_pair(node.getName(), std::make_pair(node.getAttribute("type"), node.getText()  == 0 ? "" : node.getText())));
// 		}
// 		else
// 		{
// 			for (int j = 0;j < node.nChildNode();j++)
// 			{
// 				CxExternalObject *objChild = new CxExternalObject();
// 				XMLNode childNode = node.getChildNode(j);
// 				readOutputObject(objChild, childNode);
// 				objRet->children.push_back(objChild);
// 			}
// 		}
// 	}
// 
// 	return true;
// }
// 
// bool CxFlowParse::writeOutputObject(CxExternalObject *objRet, XMLNode &xmlSubNode)
// {
// 	xmlSubNode.addAttribute("name", CxCommonFunc::U2G(objRet->objName.c_str()));
// 	xmlSubNode.addAttribute("type", CxCommonFunc::U2G(objRet->clsName.c_str()));
// 
// 	for (int i = 0;i < objRet->attri.size();i++)
// 	{
// 		XMLNode attrNode = xmlSubNode.addChild(objRet->attri[i].first.c_str());
// 		attrNode.addAttribute("type", CxCommonFunc::U2G(objRet->attri[i].second.first.c_str()));
// 		attrNode.addText(objRet->attri[i].second.second.c_str());
// 	}
// 	objRet->attri.clear();
// 	if (objRet->children.size() != 0)
// 	{
// 		XMLNode childrenNode = xmlSubNode.addChild("child");
// 
// 		for (int i = 0;i < objRet->children.size();i++)
// 		{
// 			XMLNode childrenPropertyNode = childrenNode.addChild("Object");
// 			writeOutputObject(objRet->children[i], childrenPropertyNode);
// 		}
// 	}
// 
// 	return true;
// }
// 
// bool CxFlowParse::readOutputParam(CxOutputParam &outParam, XMLNode &xmlSubNode)
// {
// 	outParam.m_strName = xmlSubNode.getAttribute("name");
// 	XMLNode descNode = xmlSubNode.getChildNode("description");
// 	XMLNode typeNode = xmlSubNode.getChildNode("type");
// 	
// 	outParam.m_strDescription = descNode.getText() == 0 ? "" : descNode.getText();
// 	outParam.m_strType = typeNode.getText() == 0 ? "" : typeNode.getText();
// 
// 	return true;
// }
// 
// bool CxFlowParse::writeOutputParam(const CxOutputParam &outParam, XMLNode &xmlSubNode)
// {
// 	xmlSubNode.addAttribute("name", CxCommonFunc::U2G(outParam.m_strName.c_str()));
// 	xmlSubNode.addChild("description").addText(outParam.m_strDescription.c_str());
// 	xmlSubNode.addChild("type").addText(outParam.m_strType.c_str());
// 
// 	return true;
// }
// 
// bool CxFlowParse::readExtendModName(std::vector<std::pair<string, string> > &modNames)
// {
// 	XMLNode xNode = m_xMainNode.getChildNode("ExtendModules");
// 	int paramCount = xNode.nChildNode();
// 
// 	for (int i = 0;i < paramCount;i++)
// 	{
// 		XMLNode paramNode = xNode.getChildNode(i);
// 		modNames.push_back(std::make_pair(paramNode.getAttribute("name"), paramNode.getAttribute("description")));
// 	}
// 
// 	return true;
// }
// 
// bool CxFlowParse::writeExtendModName(const std::vector<std::pair<string, string> > &modNames)
// {
// 	XMLNode extendRootNode = m_xMainNode.addChild("ExtendModules");
// 
// 	for (int i = 0;i < modNames.size();i++)
// 	{
// 		XMLNode extendNode = extendRootNode.addChild("Module");
// 		extendNode.addAttribute("name", CxCommonFunc::U2G(modNames[i].first.c_str()));
// 		extendNode.addAttribute("description", CxCommonFunc::U2G(modNames[i].second.c_str()));
// 	}
// 
// 	return true;
// }
// 
// const string &CxFlowParse::lastError() const
// {
// 	return m_strLastError;
// }
// 
// void CxFlowParse::save(const string &fileName)
// {
// 	m_xMainNode.writeToFile(fileName.c_str(), "GB2312");
// }
// 
// const string &CxFlowParse::fileVersion() const
// {
// 	return m_knowlegdeDescription.m_strFileVersion;
// }
// 
// void CxFlowParse::setFileVersion(const string &version)
// {
// 	m_knowlegdeDescription.m_strFileVersion = version;
// }
// #ifdef NEED_LIMITERS
// 
// bool CxFlowParse::readLimiters(CxLimiters &calParam)
// {
// 	XMLNode xNode = m_xMainNode.getChildNode("Limiters");
// 	int paramCount = xNode.nChildNode();
// 
// 	for (int i = 0;i < paramCount;i++)
// 	{
// 		XMLNode paramNode = xNode.getChildNode(i);
// 		string proName = paramNode.getAttribute("name");
// 		std::vector<string> vecParam;
// 		XMLNode descNode = paramNode.getChildNode("description");
// 		XMLNode stateNode = paramNode.getChildNode("Statement");
// 		XMLClear stateClear = stateNode.getClear();
// 		vecParam.push_back(proName);
// 		vecParam.push_back(descNode.getText() == 0 ? "" : descNode.getText());
// 		vecParam.push_back(stateClear.lpszValue == 0 ? "" : stateClear.lpszValue);
// 		calParam.addParam(vecParam);
// 	}
// 	return true;
// }
// 
// bool CxFlowParse::writeLimiters(const CxLimiters &calLimits)
// {
// 	XMLNode cpNode = m_xMainNode.addChild("Limiters");
// 	char tmp[10] = {0};
// 	const std::vector<CxCalculateRulePrivate> &rules = calLimits.calculateParam();
// 	for (int i = 0;i < rules.size();i++)
// 	{
// 		XMLNode paramNode = cpNode.addChild("Limiter");
// 		paramNode.addAttribute("name", CxCommonFunc::U2G(rules[i].m_strName.c_str()));
// 		XMLNode descNode = paramNode.addChild("description");
// 		XMLNode statementNode = paramNode.addChild("Statement");
// 		descNode.addText(rules[i].m_strDescription.c_str());
// 		statementNode.addClear(rules[i].m_strStatement.c_str());
// 	}
// 
// 	return true;
// }
// 
// #endif

#include "StdAfx.h"
#include "cxflowparse.h"

#include "cxscriptgenerator.h"

CxKnowlegdeDesc::CxKnowlegdeDesc(const string &ID, const string &name, const string &description
	, const string &version, const string &pixFilename, const string &fileVersion)
	: m_strID(ID)
	, m_strName(name)
	, m_strDescription(description)
	, m_strVersion(version)
	, m_strPixFilename(pixFilename)
	, m_strFileVersion(fileVersion)
{

}

CxKnowlegdeDesc::CxKnowlegdeDesc()
{

}

/*
		createCDATASection : 添加CDATA标签

 */

CxFlowParse::CxFlowParse(const string &strFileName)
{
	if (strFileName.size() != 0)
	{
		setFileName(strFileName);
	}
}


CxFlowParse::~CxFlowParse(void)
{

}

bool CxFlowParse::setFileName(const string &strFileName)
{
	m_strFileName = strFileName;
	m_xMainNode = XMLNode::openFileHelper(m_strFileName.c_str(), "Knowledge");
	if (m_xMainNode.isEmpty())
		return false;
	m_knowlegdeDescription.m_strFileName = strFileName;
	m_knowlegdeDescription.m_strName = m_xMainNode.getAttribute("name");
	m_knowlegdeDescription.m_strID = m_xMainNode.getAttribute("id");
	m_knowlegdeDescription.m_strDescription = m_xMainNode.getAttribute("description");
	m_knowlegdeDescription.m_strVersion = m_xMainNode.getAttribute("version");
	m_knowlegdeDescription.m_strPixFilename = m_xMainNode.getAttribute("pixfilename");
	readFileVersion(m_knowlegdeDescription.m_strFileVersion);
	
	return true;
}

void CxFlowParse::setUUID(const string &uuid)
{
	m_knowlegdeDescription.m_strID = uuid;
}

const string &CxFlowParse::fileName() const
{
	return m_strFileName;
}

const CxKnowlegdeDesc &CxFlowParse::knowledgetDescription()
{
	return m_knowlegdeDescription;
}

void CxFlowParse::setKnowledgeDescription(const CxKnowlegdeDesc &knowledge)
{
	m_knowlegdeDescription = knowledge;
}

bool CxFlowParse::readParam(std::vector<CxInputParam *> &outParams)
{
	XMLNode xNode = m_xMainNode.getChildNode("InputParams");
	int paramCount = xNode.nChildNode();

	for (int i = 0;i < paramCount;i++)
	{
		XMLNode paramNode = xNode.getChildNode(i);
		string proName = paramNode.getAttribute("name");
		string nullAllowed = paramNode.getAttribute("nullallowed");
		int propertyCount = paramNode.nChildNode();
		CxInputParam *paramTest = new CxInputParam(proName);
		paramTest->m_nullAllowed = nullAllowed;
		paramTest->m_min = paramNode.getChildNode("min").getText() == 0 ? "" : paramNode.getChildNode("min").getText();
		paramTest->m_max = paramNode.getChildNode("max").getText() == 0 ? "" : paramNode.getChildNode("max").getText();
		paramTest->m_value = paramNode.getChildNode("value").getText() == 0 ? "" : paramNode.getChildNode("value").getText();
		paramTest->m_description = paramNode.getChildNode("description").getText() == 0 ? "" : paramNode.getChildNode("description").getText();
		paramTest->m_type = paramNode.getChildNode("type").getText() == 0 ? "" : paramNode.getChildNode("type").getText();
		outParams.push_back(paramTest);
	}

	return true;
}

bool CxFlowParse::writeParam(const std::vector<CxInputParam *> &params)
{
	m_xMainNode = m_xMainNode.createXMLTopNode("Knowledge");
	m_xMainNode.addAttribute("name", m_knowlegdeDescription.m_strName.c_str());
	m_xMainNode.addAttribute("id", m_knowlegdeDescription.m_strID.c_str());
	m_xMainNode.addAttribute("description", m_knowlegdeDescription.m_strDescription.c_str());
	m_xMainNode.addAttribute("version", m_knowlegdeDescription.m_strVersion.c_str());
	m_xMainNode.addAttribute("pixfilename", m_knowlegdeDescription.m_strPixFilename.c_str());
	XMLNode versionNode = m_xMainNode.addChild("FileVersion");
	versionNode.addText(m_knowlegdeDescription.m_strFileVersion.c_str());
	XMLNode inputNode = m_xMainNode.addChild("InputParams");
	for (int i = 0;i < params.size();i++)
	{
		XMLNode paramNode = inputNode.addChild("Param");
		paramNode.addAttribute("name", params[i]->m_name.c_str());
		paramNode.addAttribute("nullallowed", params[i]->m_nullAllowed.c_str());
		XMLNode propertyNode = paramNode.addChild("min");
		propertyNode.addText(params[i]->m_min.c_str());
		propertyNode = paramNode.addChild("max");
		propertyNode.addText(params[i]->m_max.c_str());
		propertyNode = paramNode.addChild("value");
		propertyNode.addText(params[i]->m_value.c_str());
		propertyNode = paramNode.addChild("description");
		propertyNode.addText(params[i]->m_description.c_str());
		propertyNode = paramNode.addChild("type");
		propertyNode.addText(params[i]->m_type.c_str());
	}
	return true;
}

bool CxFlowParse::readFileVersion(string &version)
{
	XMLNode xNode = m_xMainNode.getChildNode("FileVersion");
	if (xNode.isEmpty())
		return false;
	version = xNode.getText() ? xNode.getText() : "";
	return true;
}

bool CxFlowParse::writeFileVersion(const string &version)
{
	XMLNode inputNode = m_xMainNode.addChild("FileVersion");
	inputNode.addText(version.c_str());
	return true;
}

bool CxFlowParse::readCalculateRules(CxCalculateRule &calParam)
{
	XMLNode xNode = m_xMainNode.getChildNode("Rules");
	int paramCount = xNode.nChildNode();

	for (int i = 0;i < paramCount;i++)
	{
		XMLNode paramNode = xNode.getChildNode(i);
		string proName = paramNode.getAttribute("name");
		std::vector<string> vecParam;
		XMLNode descNode = paramNode.getChildNode("description");
		XMLNode stateNode = paramNode.getChildNode("Statement");
		XMLClear stateClear = stateNode.getClear();
		vecParam.push_back(proName);
		vecParam.push_back(descNode.getText() == 0 ? "" : descNode.getText());
		vecParam.push_back(stateClear.lpszValue);
		calParam.addParam(vecParam);
	}
	return true;
}

bool CxFlowParse::writeCalculateRules(const CxCalculateRule &calRule)
{
	XMLNode cpNode = m_xMainNode.addChild("Rules");
	char tmp[10] = {0};
	const std::vector<CxCalculateRulePrivate> &rules = calRule.calculateParam();
	for (int i = 0;i < rules.size();i++)
	{
		XMLNode paramNode = cpNode.addChild("Rule");
		paramNode.addAttribute("name", rules[i].m_strName.c_str());
		XMLNode descNode = paramNode.addChild("description");
		XMLNode statementNode = paramNode.addChild("Statement");
		descNode.addText(rules[i].m_strDescription.c_str());
		statementNode.addClear(rules[i].m_strStatement.c_str());
	}

	return true;
}


bool CxFlowParse::readOutput(std::vector<CxExternalObject *> &objTree, std::vector<CxOutputParam> &outParam)
{
	XMLNode xNode = m_xMainNode.getChildNode("Output");
	XMLNode objectsNode = xNode.getChildNode("OutputObjects");
	XMLNode paramsNode = xNode.getChildNode("OutputParams");
	int objectCount = objectsNode.nChildNode();
	int paramsCount = paramsNode.nChildNode();

	for (int i = 0;i < objectCount;i++)
	{
		CxExternalObject *objTmp = new CxExternalObject();
		XMLNode objectNode = objectsNode.getChildNode(i);
		readOutputObject(objTmp, objectNode);
		objTree.push_back(objTmp);
	}

	for (int i = 0;i < paramsCount;i++)
	{
		CxOutputParam param;
		XMLNode paramNode = paramsNode.getChildNode(i);
		readOutputParam(param, paramNode);
		outParam.push_back(param);
	}

	return true;
}

bool CxFlowParse::writeOutput(const std::vector<CxExternalObject *> &objTree, const std::vector<CxOutputParam> &outParam)
{
	XMLNode objRootNode = m_xMainNode.addChild("Output");

	XMLNode outputParamsNode = objRootNode.addChild("OutputParams");
	XMLNode outputObjectNode = objRootNode.addChild("OutputObjects");

	for (int i = 0;i < outParam.size();i++)
	{
		XMLNode paramNode = outputParamsNode.addChild("Param");
		writeOutputParam(outParam[i], paramNode);
	}

	for (int i = 0;i < objTree.size();i++)
	{
		XMLNode objNode = outputObjectNode.addChild("Object");
		writeOutputObject(objTree[i], objNode);
	}
	return true;
}

bool CxFlowParse::readOutputObject(CxExternalObject *objRet, XMLNode &xmlSubNode)
{
	objRet->objName = xmlSubNode.getAttribute("name");
	objRet->clsName = xmlSubNode.getAttribute("type");
	objRet->attri.clear();

	for (int i = 0;i < xmlSubNode.nChildNode();i++)
	{
		XMLNode node = xmlSubNode.getChildNode(i);
		if (strcmp(node.getName(), "child") != 0)
		{
			objRet->attri.push_back(std::make_pair(node.getName(), std::make_pair(node.getAttribute("type"), node.getText()  == 0 ? "" : node.getText())));
		}
		else
		{
			for (int j = 0;j < node.nChildNode();j++)
			{
				CxExternalObject *objChild = new CxExternalObject();
				XMLNode childNode = node.getChildNode(j);
				readOutputObject(objChild, childNode);
				objRet->children.push_back(objChild);
			}
		}
	}

	return true;
}

bool CxFlowParse::writeOutputObject(CxExternalObject *objRet, XMLNode &xmlSubNode)
{
	xmlSubNode.addAttribute("name", objRet->objName.c_str());
	xmlSubNode.addAttribute("type", objRet->clsName.c_str());

	for (int i = 0;i < objRet->attri.size();i++)
	{
		XMLNode attrNode = xmlSubNode.addChild(objRet->attri[i].first.c_str());
		attrNode.addAttribute("type", objRet->attri[i].second.first.c_str());
		attrNode.addText(objRet->attri[i].second.second.c_str());
	}
	objRet->attri.clear();
	if (objRet->children.size() != 0)
	{
		XMLNode childrenNode = xmlSubNode.addChild("child");

		for (int i = 0;i < objRet->children.size();i++)
		{
			XMLNode childrenPropertyNode = childrenNode.addChild("Object");
			writeOutputObject(objRet->children[i], childrenPropertyNode);
		}
	}

	return true;
}

bool CxFlowParse::readOutputParam(CxOutputParam &outParam, XMLNode &xmlSubNode)
{
	outParam.m_strName = xmlSubNode.getAttribute("name");
	XMLNode descNode = xmlSubNode.getChildNode("description");
	XMLNode typeNode = xmlSubNode.getChildNode("type");
	
	outParam.m_strDescription = descNode.getText() == 0 ? "" : descNode.getText();
	outParam.m_strType = typeNode.getText() == 0 ? "" : typeNode.getText();

	return true;
}

bool CxFlowParse::writeOutputParam(const CxOutputParam &outParam, XMLNode &xmlSubNode)
{
	xmlSubNode.addAttribute("name", outParam.m_strName.c_str());
	xmlSubNode.addChild("description").addText(outParam.m_strDescription.c_str());
	xmlSubNode.addChild("type").addText(outParam.m_strType.c_str());

	return true;
}

bool CxFlowParse::readExtendModName(std::vector<std::pair<string, string> > &modNames)
{
	XMLNode xNode = m_xMainNode.getChildNode("ExtendModules");
	int paramCount = xNode.nChildNode();

	for (int i = 0;i < paramCount;i++)
	{
		XMLNode paramNode = xNode.getChildNode(i);
		modNames.push_back(std::make_pair(paramNode.getAttribute("name"), paramNode.getAttribute("description")));
	}

	return true;
}

bool CxFlowParse::writeExtendModName(const std::vector<std::pair<string, string> > &modNames)
{
	XMLNode extendRootNode = m_xMainNode.addChild("ExtendModules");

	for (int i = 0;i < modNames.size();i++)
	{
		XMLNode extendNode = extendRootNode.addChild("Module");
		extendNode.addAttribute("name", modNames[i].first.c_str());
		extendNode.addAttribute("description", modNames[i].second.c_str());
	}

	return true;
}

const string &CxFlowParse::lastError() const
{
	return m_strLastError;
}

void CxFlowParse::save(const string &fileName)
{
	m_xMainNode.writeToFile(fileName.c_str(), "UTF-8");
}

const string &CxFlowParse::fileVersion() const
{
	return m_knowlegdeDescription.m_strFileVersion;
}

void CxFlowParse::setFileVersion(const string &version)
{
	m_knowlegdeDescription.m_strFileVersion = version;
}
#ifdef NEED_LIMITERS

bool CxFlowParse::readLimiters(CxLimiters &calParam)
{
	XMLNode xNode = m_xMainNode.getChildNode("Limiters");
	int paramCount = xNode.nChildNode();

	for (int i = 0;i < paramCount;i++)
	{
		XMLNode paramNode = xNode.getChildNode(i);
		string proName = paramNode.getAttribute("name");
		std::vector<string> vecParam;
		XMLNode descNode = paramNode.getChildNode("description");
		XMLNode stateNode = paramNode.getChildNode("Statement");
		XMLClear stateClear = stateNode.getClear();
		vecParam.push_back(proName);
		vecParam.push_back(descNode.getText() == 0 ? "" : descNode.getText());
		vecParam.push_back(stateClear.lpszValue == 0 ? "" : stateClear.lpszValue);
		calParam.addParam(vecParam);
	}
	return true;
}

bool CxFlowParse::writeLimiters(const CxLimiters &calLimits)
{
	XMLNode cpNode = m_xMainNode.addChild("Limiters");
	char tmp[10] = {0};
	const std::vector<CxCalculateRulePrivate> &rules = calLimits.calculateParam();
	for (int i = 0;i < rules.size();i++)
	{
		XMLNode paramNode = cpNode.addChild("Limiter");
		paramNode.addAttribute("name", rules[i].m_strName.c_str());
		XMLNode descNode = paramNode.addChild("description");
		XMLNode statementNode = paramNode.addChild("Statement");
		descNode.addText(rules[i].m_strDescription.c_str());
		statementNode.addClear(rules[i].m_strStatement.c_str());
	}

	return true;
}

#endif