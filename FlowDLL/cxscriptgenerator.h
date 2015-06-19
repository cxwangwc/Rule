#pragma once
#pragma warning(push)
#pragma warning(disable:4251)

#include "cxflowparse.h"

struct _object;

class _declspec(dllexport) CxScriptGenerator
{
public:

	enum CallResult
	{
		ImportError,
		SetparamError,
		CheckparamError,
		CalculateError,
		CalculateRight
	};

	CxScriptGenerator(string xmlfile, string scriptDefault = "CXSTD");
	~CxScriptGenerator(void);

// 	static void parseObjectParam(const string &fileName);
// 	static std::map<std::string, std::pair<string, string> > m_objectInitProerty;

	void setUUID(const string &uuid);

	/*
	1. ��ȡ������д��ű�		   �û�����Ĳ���m_params
	2. ������������д��ű�    �û�����ĸ�ֵ���
	3. �������д��ű�			�û�����ļ������
	4. ���ýű�
	*/
	void setFile(const string &xmlFile);
	bool readParams();

	void setScriptFileName(const string &name);
	string scriptFileName();

	/*
		�����������������������Ƶ�
	 */
	std::vector<std::string> objectNames();
	void cxObjectNames(CxExternalObject *, std::vector<std::string> &);

	/*
		�Ѳ���������Ͷ���д��ű�
		�ű������˲�������֮�⣬���γ���������
			setParam �� ���ò���������һ���������������һ����list��
			startCalculate �� ����
	*/
	bool writeScript(const string &name);

	/*
		���ýű����setParam����

		******UNUSED*******

		ÿ�����ýű�ʱ�����Զ����ýű����setParam�������ٵ���startCalculate����
	 */
	bool setParam(const vector<double> &params);

	/*
		���ýű����startCalculate����, �������true����objList�Ƿ��صĶ����б�
	 */
	CallResult callScript(vector<CxExternalObject *> &objList, vector<pair<string, string> > &retParams, string scName = "CXSTD");

	/*
		��Ӳ����� ������в����д��ڲ�������һ�����򷵻�false������true
	 */
	bool addParam(CxInputParam *newParam);
	bool addParam(const string &paramName);

	/*
		ɾ��������ɾ���ɹ�����true
	 */
	bool delParam(CxInputParam *existParam);
	bool delParam(const string &paramName);

	/*
		��Ӽ������
	 */
	bool addCalParam(const CxCalculateRulePrivate &);
	bool addCalParam(const std::vector<string> &paramText);

	/*
		ɾ��������� 
	 */
	bool delCalPara(const string &);


	/*
		���������� �������������ͬһ����������Ӷ����б��У�����Դ���ͬ������

		���parentObject = 0�� ����ӵ����ϲ㣬�����������m_objList�� ��ѯ�Ƿ������parentObjectͬ����
		����������������Ӷ�����û����newObjectͬ���Ķ�����ɹ���ӵ�parentObject���Ӷ����У�
		����ʧ��
	 */
	bool addObject(CxExternalObject *newObject, CxExternalObject *parentObject = 0);

	/*
		�������� ����һ����, �������������ȡ���๹�캯����Ҫ����Ĳ�����ʽ�����ͣ�
		���øù��캯������һ��ExternalObject�� ��д��ű�ʱ�����ڽű��д���һ��
		����ΪclsName����
	 */
	static CxExternalObject *createObject(const string &clsName);
	/*
		��ȡ�����ļ��ϣ���Щ�����Ǵӽű��ж�������
	 */
	static std::vector<string> classNameList();

	/*
		ɾ���������
	 */
	bool delObject(CxExternalObject *newObject, CxExternalObject *parentObject = 0);

	/*
		���ݶ������� ɾ�����󣬵ݹ���ң�ɾ����֮ͬ�ҵ��ĵ�һ��
	 */
	bool delObject(const string &objName, CxExternalObject *parentObject = 0);


	/*
		��ӹ���

		��������һ����ʶ���������ƣ�������˵���͹���������������
		�������Ʊ���Ψһ

		��xml������������������û�õ�
	 */
	bool addRule(const std::vector<string> &newRule);
	bool delRule(string ruleName);

	/*
		���Ϊ��
	 */
	bool saveToFile(const string &otherFile);
	bool openFromFile(const string &otherFile);

	string lastError() const;
	void fetchScriptError();

	/*
		���� ��ȡ��Ա
	 */
	void setObjectList(const std::vector<CxExternalObject *> &);
	const std::vector<CxExternalObject *> & objectList();

	void setOutParamList(const std::vector<CxOutputParam> &);
	const std::vector<CxOutputParam> &outparamList();

	void setInputParams(const std::vector<CxInputParam *> &);
	const std::vector<CxInputParam *> &inputParams();

	void setCalculateParam(CxCalcaulateParam *);
	CxCalcaulateParam *calulateParam();

	void setCalculateRule(CxCalculateRule *);
	CxCalculateRule *calulateRule();

#ifdef NEED_LIMITERS
	void setLimiter(CxCalculateRule *);
	CxLimiters *limiters();
#endif

	void setExtends(const std::vector<std::pair<string, string> > &);
	const std::vector<std::pair<string, string> > &extends();

	CxKnowlegdeDesc currentKnowledge();
	void setKnowledge(const CxKnowlegdeDesc &);

private:
	/*
		��Ϊ���ڽű����γ�����������setParam��startCalculate��
		��������Ĺ��������ű���д�������������Ѷ���Ĳ���������Ϊȫ�ֲ���
		���������������Щ����
	 */
	void writeGlobalParamDeclaration(fstream &);

private:

	bool paramExist(const string &name);
	CxInputParam *findParam(const string &name);
	CxExternalObject *findObject(const string &name, CxExternalObject *parentObject);

	bool parseParam(_object *paramObj, CxExternalObject *receiveObj);
	/*
		�������ͻ�ȡ��Ӧ��Ĭ��ֵ
		���磺 int(float) - '0', str - ''
	*/
	static string getDefaultValue(const string &vType);
	// �ͷ��ڴ�
	void freeMemory();

	std::vector<CxInputParam *> m_params;

	std::vector<CxExternalObject *> m_objList;
	std::vector<CxOutputParam> m_outparamList;

	CxCalcaulateParam *m_cp;
	CxCalculateRule *m_cpRule;
#ifdef NEED_LIMITERS
	CxLimiters *m_cpLimiter;
#endif
	std::vector<std::pair<string, string> > m_extends;

	std::vector<double> m_paramValue;

	CxFlowParse *m_xmlParser;
	string m_file;
	string m_lastError;
	string m_defaultScriptName;
};

#pragma warning(pop)