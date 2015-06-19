#pragma once
#pragma warning(push)
#pragma warning(disable:4251)

#include <vector>
#include <string>

using namespace std;

/*
XML文件档类型:MSXML2::IXMLDOMDocumentPtr;  
单个元素类型:MSXML2::IXMLDOMElementPtr;  
单个节点类型:MSXML2::IXMLDOMNodePtr;  
所有子节点类型:MSXML2::IXMLDOMNodeListPtr;  
所有属性类型:MSXML2::IXMLDOMNamedNodeMapPtr;  
获得节点个数的方法:get_length();  
获得所有子节点的方法:get_childNodes();  
获得指定节点的方法:get_item(索引号,单个节点类型变量);  
写入单个节点的文本内容方法:put_text((_bstr_t)"内容");  
获得单个节点的文本内容属性:XMLNODE->text;  
获得单个属性的内容属性:XMLNODEATT->text;  
节点名的属性为:XMLNODE->nodeName;  
*/

class WParamDefine;
class WCalcaulateParam;

//#define SCRIPT_TEST

struct _declspec(dllexport) ExternalObject
{
	/*
		objName : 对象名
		clsName ： 类名
		pair<string, pair<string, string> >:参数名称、值类型和值
		attri ： 所有对象属性
		children ： 子对象
	 */
	string objName;
	string clsName; 
	// 
	std::vector<pair<string, pair<string, string> > > attri;
	std::vector<ExternalObject *> children;
	string toString()
	{
		/* 
			转换成可以写入脚本的语句
		 */
		string ret = "\t" + objName + " = " + clsName + ("(");
		for (int i = 0;i < attri.size();i++)
		{
			pair<string, pair<string, string> > p = attri[i];
			ret += p.second.first + "('" + p.second.second+"')";
			if (i != attri.size()-1)
				ret += ",";
		}
		ret += ")\n";
#ifdef SCRIPT_TEST
		ret += "\tprint (" + objName + ")\n";
#endif
		for (int i = 0;i < children.size();i++)
		{
			ret += children[i]->toString();
			ret += "\t" + objName + ".add(" + children[i]->objName + ")\n";
		}
		return ret;
	}
};

class _declspec(dllexport) WXmlParse
{
public:
	WXmlParse(const CString & = _T(""));
	~WXmlParse(void);

	void setFileName(const CString &strFileName);
	const CString &fileName() const;

	//BOOL read();

	/*
		xml文档格式是否符合语法规范
	 */
	BOOL domcoumentParseRight();

	/*
		把所有的参数读入outParams， 如果返回值是False， lastError会返回该错误字符串描述
	 */
	BOOL readParam(std::vector<WParamDefine *> &outParams);
	BOOL writeParam(const std::vector<WParamDefine *> &params);

	/*
		把所有的计算规则读入calParam， 如果返回值是False， lastError会返回该错误字符串描述
	 */
	BOOL readCalculateParams(WCalcaulateParam &calParam);
	BOOL writeCalculateParams(const WCalcaulateParam &calParam);

	/*
		把所有的计算规则读入calParam， 如果返回值是False， lastError会返回该错误字符串描述
	 */
	BOOL readCalculateRules(WCalcaulateParam &calParam);
	BOOL writeCalculateRules(const WCalcaulateParam &calParam);

	/*
		读取输出对象树	
		把输出对象都读入objTree
	 */
	BOOL readObject(std::vector<ExternalObject *> &objTree);
	BOOL writeObject(const std::vector<ExternalObject *> &objTree);

	/*
		把xmlSubNode节点下的对象递归读取存入objRet
	*/
	BOOL readObject(ExternalObject *objRet, MSXML2::IXMLDOMNodePtr &xmlSubNode);
	BOOL writeObject(const ExternalObject *objRet, MSXML2::IXMLDOMNodePtr &xmlSubNode);

	/*
		读取扩展模块名称
	 */
	BOOL readExtendModName(std::vector<std::pair<string, string> > &modNames);
	BOOL writeExtendModName(const std::vector<std::pair<string, string> > &modNames);

	const CString &lastError() const;

	void appendTextNode(MSXML2::IXMLDOMDocumentPtr &pDoc, MSXML2::IXMLDOMElementPtr &element, UINT childLevel = 1);

private:
	CString m_strFileName;
	CString m_strLastError;
};

#pragma warning(pop)