#pragma once
#pragma warning(push)
#pragma warning(disable:4251)

#include <map>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#define DEL_OBJ(p) if (p) {delete p; p = 0;}

typedef unsigned int UINT;
typedef unsigned char UCHAR;
typedef std::map<string, string > CxStringMap;

class _declspec(dllexport) CxCommonFunc
{
public:
	// string转小写
	static string strToLower(const string &str)
	{
		string strTmp = str;
		transform(strTmp.begin(),strTmp.end(),strTmp.begin(),tolower);
		return strTmp;
	}

	// string.compareNoCase
	static bool compareNoCase(const string &strA,const string &strB)
	{
		string str1 = strToLower(strA);
		string str2 = strToLower(strB);
		return (str1 == str2);
	}

	static bool endsWith(const string &s, const string &n)
	{
		return s.rfind(n) == (s.length() - n.length());
	}

	static int split(const std::string& s, const std::string& delim,std::vector< std::string >* ret)
	{
		size_t last = 0;  
		size_t index=s.find_first_of(delim,last);  
		while (index!=std::string::npos)  
		{  
			ret->push_back(s.substr(last,index-last));  
			last=index+1;  
			index=s.find_first_of(delim,last);  
		}  
		if (index-last>0)  
		{  
			ret->push_back(s.substr(last,index-last));  
		}  

		return ret->size();
	}

	static const char* ws;

	// trim from end of string (right)
	static std::string& rtrim(std::string& s, const char* t = ws)
	{
		s.erase(s.find_last_not_of(t) + 1);
		return s;
	}

	// trim from beginning of string (left)
	static std::string& ltrim(std::string& s, const char* t = ws)
	{
		s.erase(0, s.find_first_not_of(t));
		return s;
	}

	// trim from both ends of string (left & right)
	static std::string& trim(std::string& s, const char* t = ws)
	{
		return ltrim(rtrim(s, t), t);
	}

	static void eraseEmptyLine(std::vector<string> &lines)
	{
		std::vector<string>::iterator it = lines.begin();
		while (it != lines.end())
		{
			if (it->length() <= 3)
			{
				it = lines.erase(it);
			}
			else
			{
				it++;
			}
		}
	}
	static bool contains(const vector<string> &names, string name)
	{
		for (int i = 0;i < names.size();i++)
		{
			if (name.compare(names[i]) == 0)
			{
				return true;
			}
		}
		return false;
	}

	static char* U2G(const char* utf8);
	//GB2312到UTF-8的转换
	static char* G2U(const char* gb2312);
};

class _declspec(dllexport) CxInputParam
{
public:
	CxInputParam(string name, string nullAllowed = "true");

	// 返回对于key的值
	string value(const string &key);

	// 判断map中是否存在该元素
	bool contains(const string &key);

	// 清空
	static void deleteAll(std::vector<CxInputParam *> &);

	// 输入是否有效
	bool isValid(string);

	// 添加元素
	bool addValue(string desc, string value);

	// string类型的参数
	bool isStringParam();

	// value
	string paramValue();

	// toString, 返回一个以sep分隔的可供用户阅读的字符串
	string toString(string sep = ";");

	// 获取描述性的字符串
	string description();

	string m_name;
	string m_nullAllowed;
	string m_max;
	string m_min;
	string m_value;
	string m_description;
	string m_type;


	CxStringMap m_extraParams;
};

class _declspec(dllexport) CxCalculateRulePrivate
{
public:
	/*
		对应xml里的二个标签
		<description></description>
		<Statement></Statement>
	 */
	CxCalculateRulePrivate(string strName, string description, string statement);
	string toString() const;
	
	string m_strName;
	string m_strDescription;
	string m_strStatement;
};

// 这种类型的变量 应该是用分隔符分开的参数列表，所有的名字都应该唯一
class _declspec(dllexport) CxCalcaulateParam
{
public:
	CxCalcaulateParam(string sep = ";");

	/*
	设置赋值语句
	*/
	void setParamStatement(string statement);

	/*
	添加赋值语句
	*/
	void addParamStatement(string statement);

	/* 
	如果m_strStatement非空，去掉m_strStatement里的换行符、空格等(trim(m_strStatement)), 
	然后调用WParamDefine::splitCString
	返回一个CStringArray赋值列表

	即：形如 a=2;b=3;c=4;
	会返回一个
	a=2
	b=3
	c=4
	这样的CStrigArray，写入脚本中
	*/
	virtual UINT paramStateMent(std::vector<string> &);

	// 添加单个参数
	virtual bool addParam(const std::vector<string> &paramText);
	virtual bool delParam(const string &paramName);

	virtual void clearParams();

	// toString()
	virtual string toString();

	void setCalculateParam(const std::vector<CxCalculateRulePrivate> &);
	const std::vector<CxCalculateRulePrivate> &calculateParam() const;

protected:
	std::vector<string> m_strStatements;
	string m_strClassDesc;

	UINT m_stateCount;

	std::vector<CxCalculateRulePrivate>  m_vecCalculateParams;
	string m_sep; // 分隔符
};

// 用户定义的规则
class _declspec(dllexport) CxCalculateRule : public CxCalcaulateParam
{
public:

	CxCalculateRule();

	string toString();

	/*
		规则解析，把m_strStatements成员转换成脚本可执行的语句
	*/

	UINT paramStateMent(std::vector<string> &);

	// 添加单个参数
	virtual bool addParam(const std::vector<string> &paramText);
	virtual bool delParam(const string &paramName);
	virtual void clearParams();
};

#pragma warning(pop)