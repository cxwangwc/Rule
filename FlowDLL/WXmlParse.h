#pragma once
#pragma warning(push)
#pragma warning(disable:4251)

#include <vector>
#include <string>

using namespace std;

/*
XML�ļ�������:MSXML2::IXMLDOMDocumentPtr;  
����Ԫ������:MSXML2::IXMLDOMElementPtr;  
�����ڵ�����:MSXML2::IXMLDOMNodePtr;  
�����ӽڵ�����:MSXML2::IXMLDOMNodeListPtr;  
������������:MSXML2::IXMLDOMNamedNodeMapPtr;  
��ýڵ�����ķ���:get_length();  
��������ӽڵ�ķ���:get_childNodes();  
���ָ���ڵ�ķ���:get_item(������,�����ڵ����ͱ���);  
д�뵥���ڵ���ı����ݷ���:put_text((_bstr_t)"����");  
��õ����ڵ���ı���������:XMLNODE->text;  
��õ������Ե���������:XMLNODEATT->text;  
�ڵ���������Ϊ:XMLNODE->nodeName;  
*/

class WParamDefine;
class WCalcaulateParam;

//#define SCRIPT_TEST

struct _declspec(dllexport) ExternalObject
{
	/*
		objName : ������
		clsName �� ����
		pair<string, pair<string, string> >:�������ơ�ֵ���ͺ�ֵ
		attri �� ���ж�������
		children �� �Ӷ���
	 */
	string objName;
	string clsName; 
	// 
	std::vector<pair<string, pair<string, string> > > attri;
	std::vector<ExternalObject *> children;
	string toString()
	{
		/* 
			ת���ɿ���д��ű������
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
		xml�ĵ���ʽ�Ƿ�����﷨�淶
	 */
	BOOL domcoumentParseRight();

	/*
		�����еĲ�������outParams�� �������ֵ��False�� lastError�᷵�ظô����ַ�������
	 */
	BOOL readParam(std::vector<WParamDefine *> &outParams);
	BOOL writeParam(const std::vector<WParamDefine *> &params);

	/*
		�����еļ���������calParam�� �������ֵ��False�� lastError�᷵�ظô����ַ�������
	 */
	BOOL readCalculateParams(WCalcaulateParam &calParam);
	BOOL writeCalculateParams(const WCalcaulateParam &calParam);

	/*
		�����еļ���������calParam�� �������ֵ��False�� lastError�᷵�ظô����ַ�������
	 */
	BOOL readCalculateRules(WCalcaulateParam &calParam);
	BOOL writeCalculateRules(const WCalcaulateParam &calParam);

	/*
		��ȡ���������	
		��������󶼶���objTree
	 */
	BOOL readObject(std::vector<ExternalObject *> &objTree);
	BOOL writeObject(const std::vector<ExternalObject *> &objTree);

	/*
		��xmlSubNode�ڵ��µĶ���ݹ��ȡ����objRet
	*/
	BOOL readObject(ExternalObject *objRet, MSXML2::IXMLDOMNodePtr &xmlSubNode);
	BOOL writeObject(const ExternalObject *objRet, MSXML2::IXMLDOMNodePtr &xmlSubNode);

	/*
		��ȡ��չģ������
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