#pragma  once
#pragma warning(push)
#pragma warning(disable:4251)

#include "cxflowparse.h"
#include <map>
#include <string>

class _declspec(dllexport) CxXmlHelper
{
public:
	CxXmlHelper(const std::string &path);
	~CxXmlHelper();

	std::map<std::string, CxKnowlegdeDesc> knowlegdes();

	bool read(const string &path);

private:
	void visit(const string &path, vector<string> &xmlFiles);

	std::map<std::string, CxKnowlegdeDesc> m_knowledges;
};

#pragma warning(pop)