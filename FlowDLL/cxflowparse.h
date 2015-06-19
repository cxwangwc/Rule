#pragma once

#include "cxxmlparser.h"
#include "cxparamdefine.h"
#include "cxexternalobject.h"

//��Ҫ�޶�����
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
		�����еĲ�������outParams�� �������ֵ��False�� lastError�᷵�ظô����ַ�������
	 */
	bool readParam(std::vector<CxInputParam *> &outParams);
	bool writeParam(const std::vector<CxInputParam *> &params);

	const CxKnowlegdeDesc &knowledgetDescription();
	void setKnowledgeDescription(const CxKnowlegdeDesc &);
	/*
		��ȡ�汾��
	 */
	bool readFileVersion(string &version);
	bool writeFileVersion(const string &version);

#ifdef NEED_LIMITERS
	bool readLimiters(CxLimiters &calParam);
	bool writeLimiters(const CxLimiters &calParam);
#endif

	/*
		�����еļ���������calParam�� �������ֵ��False�� lastError�᷵�ظô����ַ�������
	 */
// 	bool readCalculateParams(WCalcaulateParam &calParam);
// 	bool writeCalculateParams(const WCalcaulateParam &calParam);

	/*
		�����еļ���������calParam�� �������ֵ��False�� lastError�᷵�ظô����ַ�������
	 */
	bool readCalculateRules(CxCalculateRule &calParam);
	bool writeCalculateRules(const CxCalculateRule &calParam);

	/*
		��ȡ���������	
		��������󶼶���objTree
	 */
	bool readOutput(std::vector<CxExternalObject *> &objTree, std::vector<CxOutputParam> &outParam);
	bool writeOutput(const std::vector<CxExternalObject *> &objTree, const std::vector<CxOutputParam> &outParam);

	/*
		��xmlSubNode�ڵ��µĶ���ݹ��ȡ����objRet
	*/
	bool readOutputObject(CxExternalObject *objRet, XMLNode &xmlSubNode);
	bool writeOutputObject(CxExternalObject *objRet, XMLNode &xmlSubNode);

	bool readOutputParam(CxOutputParam &outParam, XMLNode &xmlSubNode);
	bool writeOutputParam(const CxOutputParam &outParam, XMLNode &xmlSubNode);

	/*
		��ȡ��չģ������
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

