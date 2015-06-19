#pragma once

#include <vector>
#include <string>

using namespace std;

//#define SCRIPT_TEST

class _declspec(dllexport) CxOutputParam
{
public:
	CxOutputParam(const string &name, const string &desc, const string &type)
		: m_strName(name), m_strDescription(desc), m_strType(type)
	{

	}

	CxOutputParam()
	{

	}

	string m_strName;
	string m_strDescription;
	string m_strType;
};

struct _declspec(dllexport) CxExternalObject
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
	std::vector<CxExternalObject *> children;
// 	string toString()
// 	{
// 		/* 
// 			ת���ɿ���д��ű������
// 		 */
// 		string ret = "\t" + objName + " = " + clsName + ("(");
// 		for (int i = 0;i < attri.size();i++)
// 		{
// 			pair<string, pair<string, string> > p = attri[i];
// 			ret += p.second.first + "('" + p.second.second+"')";
// 			if (i != attri.size()-1)
// 				ret += ",";
// 		}
// 		ret += ")\n";
// 		ret += "\t" + objName +".setObjName('" + objName + "')\n"; 
// 		
// #ifdef SCRIPT_TEST
// 		ret += "\tprint (" + objName + ")\n";
// #endif
// 		for (int i = 0;i < children.size();i++)
// 		{
// 			ret += children[i]->toString();
// 			ret += "\t" + objName + ".add(" + children[i]->objName + ")\n";
// 		}
// 		return ret;
// 	}
	string toString()
	{
		/* 
			ת���ɿ���д��ű������
		 */
		string ret = "\t" + objName + " = " + clsName + ("()");
		for (int i = 0;i < attri.size();i++)
		{
			pair<string, pair<string, string> > p = attri[i];
			ret += "\n\t"+objName+".addMember('"+p.second.first+"', '"+p.first+ "', '"+p.second.second+"')";
		}
		ret += "\n";
		ret += "\t" + objName +".setObjName('" + objName + "')\n"; 
		
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
