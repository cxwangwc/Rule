#include "StdAfx.h"
#include "cxparamdefine.h"

#include <list>

const char* CxCommonFunc::ws = " \t\n\r\f\v";

char* CxCommonFunc::U2G(const char* utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len+1];
	memset(wstr, 0, len+1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len+1];
	memset(str, 0, len+1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if(wstr) delete[] wstr;
	return str;
}

//GB2312到UTF-8的转换
char* CxCommonFunc::G2U(const char* gb2312)
{
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len+1];
	memset(wstr, 0, len+1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len+1];
	memset(str, 0, len+1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if(wstr) delete[] wstr;
	return str;
}



string CxInputParam::value(const string &key)
{
	CxStringMap::iterator it = m_extraParams.begin();  
	while(it != m_extraParams.end())  
	{ 
		if (CxCommonFunc::compareNoCase(it->first, key))
			return it->second;

		it++;
	}

	return string("");
}

bool CxInputParam::contains(const string &key)
{
	CxStringMap::iterator it = m_extraParams.begin();  
	while(it != m_extraParams.end())  
	{ 
		if (CxCommonFunc::compareNoCase(it->first, key))
			return true;

		it++;
	}

	return false;
}

void CxInputParam::deleteAll(std::vector<CxInputParam *> &paramS)
{
	for (int i = 0;i < paramS.size();i++)
	{
		DEL_OBJ(paramS[i]);
	}
	paramS.clear();
}

CxInputParam::CxInputParam(string name, string nullAllowed)
	: m_name(name)
	, m_nullAllowed(nullAllowed)
{

}

bool CxInputParam::isStringParam()
{
	CxStringMap::iterator it = m_extraParams.begin();  
	while(it != m_extraParams.end())  
	{ 
		if (it->first.compare("type") == 0 && it->second.compare("str") == 0)
			return TRUE;
		it++;
	}

	return FALSE;
}

bool CxInputParam::isValid(string inputText)
{
	if (isStringParam())
	{
		return true;
	}

	string type = value("type");
	string maxV = value("max");
	string minV = value("min");

	// 没有设定最大最小值
	if (minV.size() == 0 || maxV.size() == 0)
	{
		return true;
	}
	else if (atof(minV.c_str()) <= atof(inputText.c_str()) && atof(maxV.c_str()) >= atof(inputText.c_str()))
	{
		return true;
	}

	return true;
}

bool CxInputParam::addValue(string desc, string value)
{
	if (contains(desc))
	{
		// 如果存在一样的key，则提示用户，写入日志文件
		/***/
	}
	m_extraParams.insert(std::make_pair(desc, value));
	return true;
}

string CxInputParam::paramValue()
{
	string v = value("value");
	if (value("type").compare("str") == 0)
	{
		v = "'" + v + "'";
	}
	return v;
}

string CxInputParam::toString(string sep)
{
	string retString;

	CxStringMap::iterator it = m_extraParams.begin();  
	while(it != m_extraParams.end())  
	{ 
		retString += it->first + ":" +it->second + sep;

		it++;
	}

	return retString;
}

string CxInputParam::description()
{
	return value("description");
}


CxCalculateRulePrivate::CxCalculateRulePrivate(string strName, string description, string statement)
	: m_strName(strName)
	, m_strDescription(description)
	, m_strStatement(statement)
{

}

string CxCalculateRulePrivate::toString() const
{
	vector<string> lineStatements;
	string opStatements = m_strStatement;
	string retString;
	CxCommonFunc::split(opStatements, "\n" , &lineStatements);
	for (int i = 0;i < lineStatements.size();i++)
	{
		lineStatements[i] = "\t" + lineStatements[i];
		retString += lineStatements[i]+"\n";
	}
	return retString;


	/*
	vector<string> lineStatements;
	string opStatements = m_strStatement;
	string retString;
	CxCommonFunc::split(opStatements, "\n" , &lineStatements);

	for (int i = 0;i < lineStatements.size();i++)
	{
		// trim
		CxCommonFunc::trim(lineStatements[i]);
	}
	// 去掉所有空行
	CxCommonFunc::eraseEmptyLine(lineStatements);

	if (lineStatements.size() == 0)
	{
		;
	}
	else if (CxCommonFunc::compareNoCase(lineStatements[0].substr(0, 2), "if"))
	{
		std::list<std::string> judgeStatements; //判断语句栈
		std::list<std::string> statements; //执行语句栈

		string tmpState;
		bool continueReadState = false;

		// 入栈
		for (int i = 0;i < lineStatements.size();i++)
		{
			if (CxCommonFunc::compareNoCase(lineStatements[i].substr(0, 2), "if") || 
				CxCommonFunc::compareNoCase(lineStatements[i].substr(0, 4), "elif") || 
				CxCommonFunc::compareNoCase(lineStatements[i].substr(0, 4), "else"))
			{
				judgeStatements.push_back(lineStatements[i]);
				continueReadState = true;
			}
			else
			{
				for (;i < lineStatements.size();i++)
				{
					if (CxCommonFunc::compareNoCase(lineStatements[i].substr(0, 2), "if") || 
						CxCommonFunc::compareNoCase(lineStatements[i].substr(0, 4), "elif") || 
						CxCommonFunc::compareNoCase(lineStatements[i].substr(0, 4), "else"))
					{
						i -= 1;
						break;
					}
					else
					{
						tmpState += lineStatements[i]+";;";
					}
				}
				statements.push_back(tmpState);
			}
		}
	}
	else
	{
		for (int i = 0;i < lineStatements.size();i++)
		{
			retString += "\n\t" + lineStatements[i];
		}
	}

	return retString;
	*/
}
// 
// string CxCalculateRulePrivate::toString() const
// {
// 	vector<string> lineStatements;
// 	string opStatements = m_strStatement;
// 	string retString;
// 	CxCommonFunc::split(opStatements, "\n" , &lineStatements);
// 
// 	for (int i = 0;i < lineStatements.size();i++)
// 	{
// 		// trim
// 		CxCommonFunc::trim(lineStatements[i]);
// 	}
// 	// 去掉所有空行
// 	CxCommonFunc::eraseEmptyLine(lineStatements);
// 
// 	if (lineStatements.size() == 0)
// 	{
// 		;
// 	}
// 	else if (CxCommonFunc::compareNoCase(lineStatements[0].substr(0, 2), "if"))
// 	{
// 		for (int i = 0;i < lineStatements.size();i++)
// 		{
// 			if (CxCommonFunc::compareNoCase(lineStatements[i].substr(0, 2), "if") || 
// 				CxCommonFunc::compareNoCase(lineStatements[i].substr(0, 4), "elif") || 
// 				CxCommonFunc::compareNoCase(lineStatements[i].substr(0, 4), "else"))
// 			{
// 				// 以if elif else开头的语句
// 				retString += "\n\t" + lineStatements[i];
// 			}
// 			else
// 			{
// 				retString += "\n\t\t" + lineStatements[i];
// 			}
// 		}
// 	}
// 	else
// 	{
// 		for (int i = 0;i < lineStatements.size();i++)
// 		{
// 			retString += "\n\t" + lineStatements[i];
// 		}
// 	}
// 
// 	return retString;
// }

// 计算变量
CxCalcaulateParam::CxCalcaulateParam(string sep)
	: m_sep(sep)
	, m_stateCount(0)
	, m_strClassDesc("参数定义:")
{

}

// 设置赋值语句
void CxCalcaulateParam::setParamStatement(string statement)
{
	m_strStatements.clear();
	m_stateCount = 0;
	addParamStatement(statement);
}

void CxCalcaulateParam::addParamStatement(string statement)
{
	m_strStatements.push_back(statement);
}

UINT CxCalcaulateParam::paramStateMent(std::vector<string> &retVal)
{
	if (m_strStatements.size() == 0)
	{
		return 0;
	}

	for (int i = 0;i < m_strStatements.size();i++)
	{
		m_stateCount +=  CxCommonFunc::split(m_strStatements[i], m_sep, &retVal);
	}

	return m_stateCount;
}

bool CxCalcaulateParam::addParam(const std::vector<string> &paramText)
{
	if (paramText.size() != 3)
	{
		return false;
	}

	m_vecCalculateParams.push_back(CxCalculateRulePrivate(paramText[0], paramText[1], paramText[2]));
	return true;
}

bool CxCalcaulateParam::delParam(const string &paramName)
{
	std::vector<CxCalculateRulePrivate>::iterator it = m_vecCalculateParams.begin();
	while (it != m_vecCalculateParams.end())
	{
		if (it->m_strName == paramName)
		{
			m_vecCalculateParams.erase(it);
			return true;
		}
		it++;
	}

	return false;
}

void CxCalcaulateParam::clearParams()
{
	m_vecCalculateParams.clear();
}

string CxCalcaulateParam::toString()
{
	string ret = m_strClassDesc;
	for (int i = 0;i < m_strStatements.size();i++)
	{
		ret += m_strStatements[i];
	}
	return ret;
}

void CxCalcaulateParam::setCalculateParam(const std::vector<CxCalculateRulePrivate> &rules)
{
	m_vecCalculateParams = rules;
}

const std::vector<CxCalculateRulePrivate> &CxCalcaulateParam::calculateParam() const
{
	return m_vecCalculateParams;
}

// 规则
CxCalculateRule::CxCalculateRule()
{
	m_strClassDesc = "计算规则:";
}

string CxCalculateRule::toString()
{
	//  暂时调用基类的toString函数
	return CxCalcaulateParam::toString();
}

UINT CxCalculateRule::paramStateMent(std::vector<string> &retVal)
{
	/*
	假设 m_strStatements里的所有string都是一条语句.
	如：
	m_strStatements << "if (a > b and c < d):"
	<< "a = b" << "c = d"
	<< "elif (a < b and c < d)"
	<< "a = c" << "b = d"
	*/

	for (int i = 0;i < m_strStatements.size();i++)
	{
		string element = m_strStatements[i];
		if (!CxCommonFunc::compareNoCase(element.substr(0, 2), "if")  && !CxCommonFunc::compareNoCase(element.substr(0, 4), "elif") && !CxCommonFunc::compareNoCase(element.substr(0, 4), "else"))
		{
			element = "\t" + element;
		}

		retVal.push_back(element);
	}

	return m_stateCount;
}

bool CxCalculateRule::addParam(const std::vector<string> &paramText)
{
	if (paramText.size() != 3)
		return false;

	m_vecCalculateParams.push_back(CxCalculateRulePrivate(paramText[0], paramText[1], paramText[2]));
	return true;
}

bool CxCalculateRule::delParam(const string &paramName)
{
	std::vector<CxCalculateRulePrivate>::iterator it = m_vecCalculateParams.begin();
	while (it != m_vecCalculateParams.end())
	{
		if (it->m_strName == paramName)
		{
			m_vecCalculateParams.erase(it);
			return true;
		}
		it++;
	}

	return false;
}

void CxCalculateRule::clearParams()
{
	m_vecCalculateParams.clear();
}