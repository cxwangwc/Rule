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
	1. 读取参数，写入脚本		   用户定义的参数m_params
	2. 处理计算参数，写入脚本    用户定义的赋值语句
	3. 处理规则，写入脚本			用户定义的计算规则
	4. 调用脚本
	*/
	void setFile(const string &xmlFile);
	bool readParams();

	void setScriptFileName(const string &name);
	string scriptFileName();

	/*
		对象、输出输出参数、规则名称等
	 */
	std::vector<std::string> objectNames();
	void cxObjectNames(CxExternalObject *, std::vector<std::string> &);

	/*
		把参数、规则和对象都写入脚本
		脚本里会除了参数定义之外，会形成两个函数
			setParam ： 设置参数（接收一个与输入参数个数一样的list）
			startCalculate ： 处理
	*/
	bool writeScript(const string &name);

	/*
		调用脚本里的setParam函数

		******UNUSED*******

		每当调用脚本时，会自动调用脚本里的setParam函数，再调用startCalculate函数
	 */
	bool setParam(const vector<double> &params);

	/*
		调用脚本里的startCalculate函数, 如果返回true，则objList是返回的对象列表
	 */
	CallResult callScript(vector<CxExternalObject *> &objList, vector<pair<string, string> > &retParams, string scName = "CXSTD");

	/*
		添加参数， 如果现有参数中存在参数名称一样的则返回false，否则true
	 */
	bool addParam(CxInputParam *newParam);
	bool addParam(const string &paramName);

	/*
		删除参数，删除成功返回true
	 */
	bool delParam(CxInputParam *existParam);
	bool delParam(const string &paramName);

	/*
		添加计算参数
	 */
	bool addCalParam(const CxCalculateRulePrivate &);
	bool addCalParam(const std::vector<string> &paramText);

	/*
		删除计算参数 
	 */
	bool delCalPara(const string &);


	/*
		添加输出对象， 输出对象若不在同一个父对象的子对象列表中，则可以存在同名对象

		如果parentObject = 0， 则添加到最上层，否则遍历整个m_objList， 查询是否存在与parentObject同名的
		对象，如果存在且其子对象中没有雨newObject同名的对象，则成功添加到parentObject的子对象中，
		否则失败
	 */
	bool addObject(CxExternalObject *newObject, CxExternalObject *parentObject = 0);

	/*
		根据类名 创建一个类, 会根据类名，读取该类构造函数需要传入的参数格式和类型，
		调用该构造函数创建一个ExternalObject， 在写入脚本时，会在脚本中创建一个
		类名为clsName的类
	 */
	static CxExternalObject *createObject(const string &clsName);
	/*
		获取类名的集合，这些类名是从脚本中读过来的
	 */
	static std::vector<string> classNameList();

	/*
		删除输出对象
	 */
	bool delObject(CxExternalObject *newObject, CxExternalObject *parentObject = 0);

	/*
		根据对象名称 删除对象，递归查找，删除与之同找到的第一个
	 */
	bool delObject(const string &objName, CxExternalObject *parentObject = 0);


	/*
		添加规则

		规则是由一个标识（规则名称）、规则说明和规则语句三部分组成
		规则名称必须唯一

		现xml定义中有两个参数是没用的
	 */
	bool addRule(const std::vector<string> &newRule);
	bool delRule(string ruleName);

	/*
		另存为：
	 */
	bool saveToFile(const string &otherFile);
	bool openFromFile(const string &otherFile);

	string lastError() const;
	void fetchScriptError();

	/*
		设置 获取成员
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
		因为会在脚本里形成两个函数，setParam和startCalculate，
		这个函数的功能是往脚本里写参数声明，即把定义的参数都声明为全局参数
		方便接下来引用这些参数
	 */
	void writeGlobalParamDeclaration(fstream &);

private:

	bool paramExist(const string &name);
	CxInputParam *findParam(const string &name);
	CxExternalObject *findObject(const string &name, CxExternalObject *parentObject);

	bool parseParam(_object *paramObj, CxExternalObject *receiveObj);
	/*
		根据类型获取相应的默认值
		比如： int(float) - '0', str - ''
	*/
	static string getDefaultValue(const string &vType);
	// 释放内存
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