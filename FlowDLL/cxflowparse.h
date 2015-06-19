#pragma once

#include "cxxmlparser.h"
#include "cxparamdefine.h"
#include "cxexternalobject.h"

//需要限定条件
#define NEED_LIMITERS

#ifdef NEED_LIMITERS
typedef CxCalculateRule CxLimiters;
#endif

class _declspec(dllexport) CxKnowlegdeDesc
{
public:
	CxKnowlegdeDesc(const string &ID, const string &name, const string &description
		, const string &version, const string &pixFilename, const string &fileVersion);
	CxKnowlegdeDesc();

	string m_strID; // UUID
	string m_strFileName;
	string m_strName;
	string m_strDescription;
	string m_strVersion;
	string m_strPixFilename;
	string m_strFileVersion;
};

class _declspec(dllexport) CxFlowParse
{
public:
	CxFlowParse(const string &fileName = "");
	~CxFlowParse(void);

	bool setFileName(const string &strFileName);
	const string &fileName() const;

	void setUUID(const string &uuid);

	/*
		把所有的参数读入outParams， 如果返回值是False， lastError会返回该错误字符串描述
	 */
	bool readParam(std::vector<CxInputParam *> &outParams);
	bool writeParam(const std::vector<CxInputParam *> &params);

	const CxKnowlegdeDesc &knowledgetDescription();
	void setKnowledgeDescription(const CxKnowlegdeDesc &);
	/*
		读取版本号
	 */
	bool readFileVersion(string &version);
	bool writeFileVersion(const string &version);

#ifdef NEED_LIMITERS
	bool readLimiters(CxLimiters &calParam);
	bool writeLimiters(const CxLimiters &calParam);
#endif

	/*
		把所有的计算规则读入calParam， 如果返回值是False， lastError会返回该错误字符串描述
	 */
// 	bool readCalculateParams(WCalcaulateParam &calParam);
// 	bool writeCalculateParams(const WCalcaulateParam &calParam);

	/*
		把所有的计算规则读入calParam， 如果返回值是False， lastError会返回该错误字符串描述
	 */
	bool readCalculateRules(CxCalculateRule &calParam);
	bool writeCalculateRules(const CxCalculateRule &calParam);

	/*
		读取输出对象树	
		把输出对象都读入objTree
	 */
	bool readOutput(std::vector<CxExternalObject *> &objTree, std::vector<CxOutputParam> &outParam);
	bool writeOutput(const std::vector<CxExternalObject *> &objTree, const std::vector<CxOutputParam> &outParam);

	/*
		把xmlSubNode节点下的对象递归读取存入objRet
	*/
	bool readOutputObject(CxExternalObject *objRet, XMLNode &xmlSubNode);
	bool writeOutputObject(CxExternalObject *objRet, XMLNode &xmlSubNode);

	bool readOutputParam(CxOutputParam &outParam, XMLNode &xmlSubNode);
	bool writeOutputParam(const CxOutputParam &outParam, XMLNode &xmlSubNode);

	/*
		读取扩展模块名称
	 */
	bool readExtendModName(std::vector<std::pair<string, string> > &modNames);
	bool writeExtendModName(const std::vector<std::pair<string, string> > &modNames);

	void save(const string &fileName);
	const string &lastError() const;

	const string &fileVersion() const;
	void setFileVersion(const string &version);

private:
	XMLNode m_xMainNode;

	string m_strFileName;
	string m_strLastError;
	CxKnowlegdeDesc m_knowlegdeDescription;
};

