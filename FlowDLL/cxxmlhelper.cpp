#include "stdafx.h"
#include "cxxmlhelper.h"
#include "cxflowparse.h"

#include <io.h>

CxXmlHelper::CxXmlHelper(const std::string &path)
{

}

CxXmlHelper::~CxXmlHelper()
{

}

std::map<std::string, CxKnowlegdeDesc> CxXmlHelper::knowlegdes()
{
	return m_knowledges;
}

bool CxXmlHelper::read(const std::string &path)
{
	vector<string> xmlFiles;
	visit(path, xmlFiles);

	CxFlowParse parse;
	for (int i = 0;i < xmlFiles.size();i++)
	{
		if (parse.setFileName(xmlFiles[i]))
		{
			CxKnowlegdeDesc desc = parse.knowledgetDescription();
			m_knowledges.insert(make_pair(desc.m_strID, desc));
		}
	}

	return true;
}

void CxXmlHelper::visit(const string &path, vector<string> &xmlFiles)
{
	struct _finddata_t   filefind;     
	string  curr=path+"\\*.*";     
	int   done=0,i,handle;     
	if((handle=_findfirst(curr.c_str(), &filefind)) == -1)
		return;       
	while(!(done=_findnext(handle, &filefind)))     
	{         
		printf("%s\n",filefind.name);      
		if(!strcmp(filefind.name,".."))
		{  
			continue;  
		}  
                  
		if((_A_SUBDIR==filefind.attrib)) //是目录  
		{                
			curr=path+"\\"+filefind.name;     
		}     
		else//不是目录，是文件       
		{     
			if (CxCommonFunc::endsWith(filefind.name, ".rxl"))
				xmlFiles.push_back(path+"\\"+filefind.name);    
		}     
	}             
	_findclose(handle);     
}
