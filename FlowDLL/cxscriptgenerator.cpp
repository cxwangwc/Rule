#include "StdAfx.h"
#include "cxscriptgenerator.h"

#include <iostream>
#include <fstream>
#include <Python.h>
#include <frameobject.h>

//#ifdef __EXTERNAL_USING__

#define PY_CODE "#-*- coding: gb2312 -*-"
#define USE_SYSTEM_DECODE "\nimport sys\ndefault_encoding = 'utf-8'\nif sys.getdefaultencoding() != default_encoding:\n\treload(sys)\n\tsys.setdefaultencoding(default_encoding)\n"

using namespace std;

CxScriptGenerator::CxScriptGenerator(string xmlFile, string scriptDefault)
	: m_cp(new CxCalcaulateParam)
	, m_cpRule(new CxCalculateRule)
	, m_file(xmlFile)
	, m_defaultScriptName(scriptDefault)
#ifdef NEED_LIMITERS
	, m_cpLimiter(new CxLimiters)
#endif
{
	 m_xmlParser = new CxFlowParse(m_file);
#ifdef __EXTERNAL_USING__
	 Py_SetPythonHome(_T("."));
#endif
}


CxScriptGenerator::~CxScriptGenerator(void)
{

}

void CxScriptGenerator::setScriptFileName(const string &name)
{
	m_defaultScriptName = name;
}

string CxScriptGenerator::scriptFileName()
{
	return m_defaultScriptName;
}

std::vector<std::string> CxScriptGenerator::objectNames()
{
	std::vector<string> definedNames;
	for (int i = 0;i < m_params.size();i++)
	{
		definedNames.push_back(m_params[i]->m_name);
	}
	for (int i = 0; i < m_objList.size();i++)
	{
		cxObjectNames(m_objList[i], definedNames);
	}
// 
// 	for (int i = 0;i < m_outparamList.size();i++)
// 	{
// 		definedNames.push_back(m_outparamList[i].m_strName);
// 	}

	for (int i = 0;i < m_cpRule->calculateParam().size();i++)
	{
		definedNames.push_back(m_cpRule->calculateParam()[i].m_strName);
	}
#ifdef NEED_LIMITERS
	for (int i = 0;i < m_cpLimiter->calculateParam().size();i++)
	{
		definedNames.push_back(m_cpLimiter->calculateParam()[i].m_strName);
	}
#endif
	return definedNames;
}

void CxScriptGenerator::cxObjectNames(CxExternalObject *cxObj, std::vector<std::string> &definedNames)
{
	if (!cxObj) return;
	definedNames.push_back(cxObj->objName);
	for (int i = 0;i < cxObj->children.size();i++)
	{
		cxObjectNames(cxObj->children[i], definedNames);
	}
}

void CxScriptGenerator::setFile(const string &xmlFile)
{
	m_file = xmlFile;
}

void CxScriptGenerator::writeGlobalParamDeclaration(fstream &mFile)
{
	// 输入参数声明
	string strTemp;
	if (m_params.size() != 0)
	{
		strTemp = "\tglobal ";
		for (int i = 0;i < m_params.size();i++)
		{
			strTemp = strTemp + m_params[i]->m_name;
			if (i < m_params.size()-1)
				strTemp = strTemp + ", ";

		}
		strTemp += "\n";
		mFile << strTemp;
	}
	if (m_cp->calculateParam().size() != 0)
	{
		// 参与计算的参数声明
		strTemp = "\tglobal ";
		const std::vector<CxCalculateRulePrivate> &rules = m_cp->calculateParam();
		for (int i = 0;i < rules.size();i++)
		{
			strTemp = strTemp + rules[i].m_strName;
			if (i < rules.size()-1)
				strTemp = strTemp + ", ";

		}
		strTemp += "\n";
		mFile << strTemp;
	}
}

void CxScriptGenerator::freeMemory()
{
	m_paramValue.clear();
	m_extends.clear();
	CxInputParam::deleteAll(m_params);
	m_cp->clearParams();
	m_cpRule->clearParams();
#ifdef NEED_LIMITERS
	m_cpLimiter->clearParams();
#endif
	m_outparamList.clear();
	for (int i = 0;i <m_objList.size();i++)
	{
		DEL_OBJ(m_objList[i]);
	}
	m_objList.clear();
}

bool CxScriptGenerator::readParams()
{
	freeMemory();
	if (!m_xmlParser->setFileName(m_file))
		return false;
	if (m_xmlParser->readParam(m_params))
	{
		m_xmlParser->readCalculateRules(*m_cpRule);
#ifdef NEED_LIMITERS
		m_xmlParser->readLimiters(*m_cpLimiter);
#endif
		m_xmlParser->readOutput(m_objList, m_outparamList);
		m_xmlParser->readExtendModName(m_extends);
		return true;
	}
	return false;
}

bool CxScriptGenerator::writeScript(const string &name)
{
	string strTemp;
	char charTmp[100];
	fstream  mFile;
	mFile.open(name, ios::out);
	if (!mFile.is_open())
		return false;
	//mFile<<PY_CODE<<"\n\n";
	//mFile << "#-*- coding: utf-8 -*-\n";
	mFile << USE_SYSTEM_DECODE;
	mFile<<"\nfrom math import *\nfrom OutObject import *\n";

	for (int i = 0;i < m_extends.size();i++)
	{
		mFile << "from " << m_extends[i].first << " import *\n";
	}

	m_paramValue.clear();
	// 输入参数定义
	vector<string> inputParamNames;
	for (int i = 0;i < m_params.size();i++)
	{
		if (m_params[i]->m_type.compare("str") == 0)
			strTemp = m_params[i]->m_name+ "='" + m_params[i]->m_value+"'";
		else
			strTemp = m_params[i]->m_name+ "=" + m_params[i]->m_value;
		m_paramValue.push_back(atof(m_params[i]->m_value.c_str()));
		inputParamNames.push_back(m_params[i]->m_name);
		mFile << string("\n#"+ m_params[i]->m_description);
		mFile << string("\n" + strTemp);
	}

	const std::vector<CxCalculateRulePrivate> &cpParams = m_cp->calculateParam();
	const std::vector<CxCalculateRulePrivate> &ruleParams = m_cpRule->calculateParam();

	for (int i = 0;i < cpParams.size();i++)
	{
		strTemp = cpParams[i].m_strName+ "=0";
		mFile << string("\n#"+ cpParams[i].m_strDescription);
		mFile << string("\n" + strTemp);
	}

	mFile << "\n";

	// setParam函数定义
	mFile<<"#setParam Function \n";
	mFile<<"def setParam(valList):\n";
	sprintf(charTmp, "\tif len(valList) < %d:\n", m_params.size()); 
	mFile << string(charTmp);
	mFile << "\t\tprint ('set Param failed')\n\t\treturn False\n";
	writeGlobalParamDeclaration(mFile);

	for (int i = 0;i < m_params.size();i++)
	{
		// 参数赋值
		sprintf(charTmp, "%s=valList[%d]\n", m_params[i]->m_name.c_str(), i); 
		mFile << string(("\t")+ string(charTmp));
	}

	for (int i = 0;i < cpParams.size();i++)
	{
		mFile << string("\n\t" + cpParams[i].m_strStatement);
	}

	mFile << "\n\treturn True\n";

#ifdef NEED_LIMITERS
	// 检查限定条件函数
	const std::vector<CxCalculateRulePrivate> &cpLimits = m_cpLimiter->calculateParam();
	mFile<<"#Check params limits function\ndef checkLimits():\n";
	writeGlobalParamDeclaration(mFile);
	if (cpLimits.size() == 0)
	{
		mFile<<"\treturn 1";
	}
	else
	{
		string lJudgeState;
		for (int i = 0;i < cpLimits.size();i++)
		{
			string limitState = cpLimits[i].m_strStatement;
			vector<string> lStates;
			CxCommonFunc::split(limitState, "\n", &lStates);
			for (int j = 0;j < lStates.size();j++)
			{
				CxCommonFunc::trim(lStates[j]);
				if (lStates[j].length() <= 0)
					continue;
				lJudgeState += lStates[j];
				if (i == cpLimits.size() - 1 && j == lStates.size()-1)
				{
					;
				}
				else
				{
					lJudgeState += " and ";
				}
			}
		}
		CxCommonFunc::trim(lJudgeState);
		if (lJudgeState.length() <= 0)
		{
			lJudgeState = "\tif True";
		}
		else
		{
			lJudgeState = "\tif " + lJudgeState;
		}
		lJudgeState += ":\n\t\treturn 1\n\telse:\n\t\treturn 0\n\n";
		mFile << lJudgeState;
	}
#endif

	mFile << "#startCalculate Function \n";
	mFile << "def startCalculate():\n";
	writeGlobalParamDeclaration(mFile);

	/*
		把输出的结果和对象都存放在retValue中
			retObject:存放对象树
			retParam：存放输出参数
	*/

	mFile << "\n\tretValue = []\n\tretObject = []\n\tretParam = []\n";
	mFile << "\n\tretValue.append(retObject)\n\tretValue.append(retParam)";

	// 先声明输出参数，若在规则中没有使用，不声明在脚本中会出错
	for (int i = 0;i < m_outparamList.size();i++)
	{
		string oName = m_outparamList[i].m_strName;
		if (CxCommonFunc::contains(inputParamNames, oName))
		{
			continue;
		}
		else
		{
			mFile << "\n\t#" << m_outparamList[i].m_strDescription;
			mFile << "\n\t" << oName << "=0";
		}
	}
	mFile <<  "\n";
	// out obj
	for (int i = 0;i < m_objList.size();i++)
	{
		string tst = m_objList[i]->toString();
		mFile << tst;
		strTemp = "\n\tretObject.append("+ m_objList[i]->objName+ ")\n";
		mFile << strTemp;
	}
	// rules
	for (int i = 0;i < ruleParams.size();i++)
	{
		mFile << ruleParams[i].toString() << "\n";
	}

	for (int i = 0;i < m_outparamList.size();i++)
	{
		mFile << "\n\tretParam.append(('" << m_outparamList[i].m_strName << "',str(" << m_outparamList[i].m_strName << ")))";
	}

#ifdef SCRIPT_TEST
	mFile << "\n\tfor r in retValue:\n\t\tprint (r.out())";
#endif
	mFile<<"\n\treturn retValue";
	mFile.close();
	return true;
}

bool CxScriptGenerator::setParam(const vector<double> &params)
{
	return true;
}

CxScriptGenerator::CallResult CxScriptGenerator::callScript(vector<CxExternalObject *> &objList, vector<pair<string, string> > &retParams, string scName)
{
	Py_Initialize();  
	PyObject * pModule = NULL;      
	PyObject * pFunc = NULL;        
	pModule =PyImport_ImportModule(scName.c_str()); // 脚本名称，不带后缀py

	if (!pModule)
	{
		fetchScriptError();
		Py_Finalize();
		return ImportError;
	}

	/*
		不调用setParam函数

	pFunc= PyObject_GetAttrString(pModule, "setParam"); //脚本里的函数

	PyObject *pSetArgs = PyTuple_New(1);
	PyObject *pArgsParams= PyList_New(m_paramValue.size());

	// 参数设置的顺序与界面定义顺序一致，是一个参数列表
	for (int i = 0;i < m_paramValue.size();i++)
	{
		PyList_SetItem(pArgsParams, i, Py_BuildValue("f", m_paramValue[i]));
	}

	// 把pArgsParams list里存放的参数，放入函数参数元组中，0代表元组索引
	PyTuple_SetItem(pSetArgs, 0, pArgsParams);

	PyObject *ret = PyEval_CallObject(pFunc, pSetArgs);
	if (!ret)
	{
		fetchScriptError();
		Py_Finalize();
		return false;
	}
	*/
#ifdef NEED_LIMITERS
	// 检查限定条件函数
	pFunc = PyObject_GetAttrString(pModule, "checkLimits");
	if (!pFunc)
	{
		m_lastError = "No such function : checkLimits, call failed.";
		Py_Finalize();
		return CheckparamError;
	}
	PyObject *limitRet = PyEval_CallObject(pFunc, 0, PyTuple_New(0));
	if (!limitRet)
	{
		fetchScriptError();
		Py_Finalize();
		return CheckparamError;
	}
	//		m_lastError += "\nCheck limits failed, please check.";
	int bRet = -1;
	PyArg_Parse(limitRet, "i", &bRet);
	if (bRet == 0)
	{
		m_lastError = "Check limits failed, please check.";
		Py_Finalize();
		return CheckparamError;
	}
	
#endif

	pFunc = PyObject_GetAttrString(pModule, "startCalculate");
	if (!pFunc)
	{
		m_lastError = "No such function : startCalculate, call failed.";
		return CalculateError;
	}
	// 	返回值
	PyObject *ret = PyEval_CallObject(pFunc, 0, PyTuple_New(0));
	if (!ret)
	{
		fetchScriptError();
		Py_Finalize();
		return CalculateError;
	}

	PyObject *cls = 0;
	char *str = 0;
	int size = PyList_GET_SIZE(ret);
	if (size != 2)
	{
		m_lastError = "The script file's format is not correct";
		Py_Finalize();   
		return CalculateError;
	}

	PyObject *retObject =  PyList_GetItem(ret, 0);
	PyObject *retParam = PyList_GetItem(ret, 1);

	for (Py_ssize_t i = 0;i < PyList_GET_SIZE(retObject);i++)
	{
		PyObject *retChildObject = PyList_GetItem(retObject, i);
		CxExternalObject *cretObject = new CxExternalObject();
		if (!parseParam(retChildObject, cretObject))
		{
			m_lastError = "Read objects from return value failed.";
			return CalculateError;
		}
		objList.push_back(cretObject);
	}

	for (Py_ssize_t i = 0;i < PyList_GET_SIZE(retParam);i++)
	{
		PyObject *retChildParam = PyList_GetItem(retParam, i);
		// retChildParam是一个二项的元组，分别为 名称和值
		PyObject *tupleName = PyTuple_GetItem(retChildParam, 0);
		PyObject *tupleValue = PyTuple_GetItem(retChildParam, 1);
		const char *tName = 0;
		const char *tValue = 0;
		PyArg_Parse(tupleName, "s", &tName);
		PyArg_Parse(tupleValue, "s", &tValue);
		if (tName && tValue)
		{
			retParams.push_back(make_pair(tName, tValue));
		}
		else
		{
			retParams.push_back(make_pair("", ""));
		}
	}

// 	Py_CLEAR(pModule);
// 	Py_CLEAR(pFunc);
// 	Py_CLEAR(ret);
// 	Py_CLEAR(cls);
	Py_Finalize();   
	return CalculateRight;
}

bool CxScriptGenerator::parseParam(_object *paramObj, CxExternalObject *receiveObj)
{
	PyObject *paramValue = PyObject_CallMethod(paramObj, "values", NULL);
	PyObject *keys = PyObject_CallMethod(paramObj, "keys", NULL);
	PyObject *objName = PyObject_CallMethod(paramObj, "objName", NULL);
	PyObject *classname = PyObject_CallMethod(paramObj, "clsName", NULL);

	if (!paramValue || !keys || !objName || !classname)
	{
		return false;
	}
	char *objname = 0;
	char *clsName = 0;
	PyArg_Parse(objName, "s", &objname);
	PyArg_Parse(classname, "s", &clsName);
	receiveObj->objName = objname;
	receiveObj->clsName = clsName;

	for (int  i = 0;i < PyList_GET_SIZE(keys);i++)
	{
		PyObject *iItem = PyList_GetItem(keys, i);
		PyObject *paramName = PyTuple_GetItem(iItem, 0);
		PyObject *paramType = PyTuple_GetItem(iItem, 1);

		char *key = 0;
		char *type = 0;
		char value[100]= {0};
		PyArg_Parse(paramName, "s", &key);
		PyArg_Parse(paramType, "s", &type);

		PyObject *val = PyDict_GetItemString(paramValue, key);
		if (strcmp(type, "str") == 0)
		{
			char *tmpVal = 0;
			PyArg_Parse(val, "s", &tmpVal);
			sprintf(value, "%s", tmpVal);
			if (!value)
			{
				// 如果value为空 则给它赋值为空字符串
				strcpy(value, "");
			}
		}
		else if (strcmp(type, "float") == 0)
		{
			double tmpVal = 0;
			PyArg_Parse(val, "d", &tmpVal);
			sprintf(value, "%f", tmpVal);
		}
		else
		{
			int tmpVal = 0;
			PyArg_Parse(val, "i", &tmpVal);
			sprintf(value, "%d", tmpVal);
		}

		if (key && type && value)
		receiveObj->attri.push_back(make_pair(key, make_pair(type, value)));
	}

	PyObject *children = PyObject_CallMethod(paramObj, "child", NULL);
	if (PyList_GET_SIZE(children) != 0)
	{
		for (int i = 0;i < PyList_GET_SIZE(children);i++)
		{
			PyObject *childObject = PyList_GetItem(children, i);
			CxExternalObject *childExternal = new CxExternalObject();
			if (!parseParam(childObject, childExternal))
			{
				return false;
			}
			receiveObj->children.push_back(childExternal);
			//Py_XDECREF(childObject);
		}
	}
	return true;
	//Py_XDECREF(paramValue);
	//Py_XDECREF(keys);
	//Py_XDECREF(objName);
	//Py_XDECREF(classname);
	//Py_XDECREF(paramValue);
	//Py_XDECREF(children);
}

/*
OPERATIONS
*/
bool CxScriptGenerator::addParam(CxInputParam *newParam)
{
	m_params.push_back(newParam);
	return true;
}

bool CxScriptGenerator::addParam(const string &paramName)
{
	if (paramExist(paramName))
	{
		m_lastError = "WScriptGenerator::addParam failed, the param " + paramName + " is already exist.";
		return false;
	}

	return addParam(new CxInputParam(paramName));
}

bool CxScriptGenerator::delParam(CxInputParam *existParam)
{
	vector<CxInputParam *>::iterator it = std::find(m_params.begin(), m_params.end(), existParam);
	if (it != m_params.end())
	{
		DEL_OBJ(*it);
		m_params.erase(it);
		return true;
	}

	m_lastError = "WScriptGenerator::delParam  failed, param doesn't exist";
	return false;
}

bool CxScriptGenerator::delParam(const string &paramName)
{
	return delParam(findParam(paramName));
}

bool CxScriptGenerator::addCalParam(const CxCalculateRulePrivate &calParam)
{
	std::vector<CxCalculateRulePrivate> ruls = m_cp->calculateParam();
	ruls.push_back(calParam);
	m_cp->setCalculateParam(ruls);
	return true;
}

bool CxScriptGenerator::addCalParam(const std::vector<string> &paramText)
{
	return m_cp->addParam(paramText);
}

bool CxScriptGenerator::delCalPara(const string &paramText)
{
	return m_cp->delParam(paramText);
}

CxExternalObject *CxScriptGenerator::createObject(const string &clsName)
{
	Py_Initialize(); 

	std::vector<string> clsList;
	PyObject *pModule = NULL;      
	PyObject *pClass = NULL;        
	pModule =PyImport_ImportModule("OutObject"); // 脚本名称，不带后缀py
	if (!pModule)
	{
		Py_Finalize();
		return 0;
	}
	string objName = "new" + clsName;
	CxExternalObject *obj = 0;
	pClass = PyObject_GetAttrString(pModule, clsName.c_str());
	pClass = PyObject_CallObject(pClass , PyTuple_New(0));
	if (pClass)
	{
		obj = new CxExternalObject();
		PyObject *clsMembers = PyObject_CallMethod(pClass, "keys", 0);
		for (int i = 0;i < PyList_GET_SIZE(clsMembers);i++)
		{
			PyObject *tupleValue = PyList_GetItem(clsMembers, i);
			if (PyTuple_GET_SIZE(tupleValue) != 2)
			{
				break;
			}
			PyObject *keyValue = PyTuple_GetItem(tupleValue, 0);
			PyObject *keyType = PyTuple_GetItem(tupleValue, 1);

			char *cType = 0;
			char *cValue = 0;
			PyArg_Parse(keyType, "s", &cType);
			PyArg_Parse(keyValue, "s", &cValue);
			obj->attri.push_back(make_pair(cValue, make_pair(cType, getDefaultValue(cType))));
			//Py_XDECREF(tupleValue);
			//Py_XDECREF(keyValue);
			//Py_XDECREF(keyType);
		}
		obj->objName = objName;
		obj->clsName = clsName;
		//Py_XDECREF(clsMembers);
	}
	////Py_XDECREF(pModule);
	////Py_XDECREF(pClass);
	Py_Finalize();
	return obj;
}

std::vector<string> CxScriptGenerator::classNameList()
{
	Py_Initialize();  
	std::vector<string> clsList;
	PyObject *pModule = NULL;      
	PyObject *pFunc = NULL;        
	pModule =PyImport_ImportModule("OutObject"); // 脚本名称，不带后缀py
	if (!pModule)
	{
		Py_Finalize();
		return clsList;
	}
	pFunc = PyObject_GetAttrString(pModule, "classNameList");

	PyObject *ret = PyEval_CallObject(pFunc, 0, PyTuple_New(0));
	if (ret)
	{
		for (int i = 0;i < PyList_GET_SIZE(ret);i++)
		{
			PyObject *clsName = PyList_GetItem(ret, i);
			char *name = 0;
			PyArg_Parse(clsName, "s", &name);
			clsList.push_back(string(name));
			//Py_XDECREF(clsName);
		}
	}
	////Py_XDECREF(pModule);
	//Py_XDECREF(pFunc);
	//Py_XDECREF(ret);
	Py_Finalize();
	return clsList;
}

bool CxScriptGenerator::addObject(CxExternalObject *newObject, CxExternalObject *parentObject )
{
	if (!parentObject)
	{
		m_objList.push_back(newObject);
	}
	else
	{
		parentObject->children.push_back(newObject);
	}

	return true;
}

bool CxScriptGenerator::delObject(CxExternalObject *newObject, CxExternalObject *parentObject)
{
	std::vector<CxExternalObject *> *removeVec;
	vector<CxExternalObject *>::iterator it;
	if (!parentObject)
	{
		removeVec = &m_objList;
	}
	else
	{
		removeVec = &(parentObject->children);
	}
	it = std::find(m_objList.begin(), m_objList.end(), newObject);
	if (it != removeVec->end())
	{
		DEL_OBJ(*it);
		removeVec->erase(it);
	}

	return false;
}

bool CxScriptGenerator::delObject(const string &objName, CxExternalObject *parentObject)
{
	CxExternalObject *obj = findObject(objName, parentObject);
	if (!obj)
		return false;

	return delObject(obj, parentObject);
}


bool CxScriptGenerator::addRule(const std::vector<string> &newRule)
{
	return m_cpRule->addParam(newRule);
}

bool CxScriptGenerator::delRule(string ruleName)
{
	return m_cpRule->delParam(ruleName);
}

void CxScriptGenerator::setUUID(const string &uuid)
{
	m_xmlParser->setUUID(uuid);
}

bool CxScriptGenerator::saveToFile(const string &otherFile)
{
	m_xmlParser->writeParam(m_params);
	m_xmlParser->writeLimiters(*m_cpLimiter);
	m_xmlParser->writeOutput(m_objList, m_outparamList);
	m_xmlParser->writeCalculateRules(*m_cpRule);
	m_xmlParser->writeExtendModName(m_extends);
	m_xmlParser->save(otherFile);
	return true;
}

bool CxScriptGenerator::openFromFile(const string &otherFile)
{
	setFile(otherFile);
	return readParams();
}

string CxScriptGenerator::lastError() const
{
	return m_lastError;
}

void CxScriptGenerator::fetchScriptError()
{
	if (!Py_IsInitialized())
	{
		m_lastError = "Python 运行环境没有初始化！";
		return;
	}
	char emsg[200] = {0};
	const char *err = 0;

	if (PyErr_Occurred() != NULL)
	{
		PyObject *type_obj, *value_obj, *traceback_obj;
		PyErr_Fetch(&type_obj, &value_obj, &traceback_obj);
		if (value_obj == NULL)
			return;

		m_lastError.clear();
		PyErr_NormalizeException(&type_obj, &value_obj, 0);
		if (PyObject_Str(value_obj))
		{
			struct _typeobject *typeObj = value_obj->ob_type;
			
			m_lastError += typeObj->tp_name;

			PyObject *pyErrstr = PyObject_Str(value_obj);
			if (pyErrstr)
			{
				PyArg_Parse(pyErrstr, "s", &err);
				if (err)
				{
					m_lastError +=":\n\t";
					m_lastError += err;
				}
			}
		}

		if (traceback_obj != NULL)
		{
			m_lastError += "\nTraceback:\n\t";
			PyTracebackObject *traceback = (PyTracebackObject *)traceback_obj;

			while (traceback) 
			{
				PyCodeObject *codeobj = traceback->tb_frame->f_code;
				PyObject *traText = codeobj->co_name;
				PyObject *traFile = codeobj->co_filename;
				const char *fErr = 0;
				PyArg_Parse(traText, "s", &err);
				PyArg_Parse(traFile, "s", &fErr);
				sprintf(emsg, "When call method %s in file %s, an error occurred in line %d.\n", err, fErr, traceback->tb_lineno);
				traceback = traceback->tb_next;
			}
			m_lastError += emsg;
		}
		//Py_XDECREF(type_obj);
		//Py_XDECREF(value_obj);
		//Py_XDECREF(traceback_obj);
	}    
}

bool CxScriptGenerator::paramExist(const string &name)
{
	for (int i = 0;i < m_params.size();i++)
	{
		if (name.compare(m_params[i]->m_name) == 0)
			return true;
	}

	return false;
}

CxInputParam *CxScriptGenerator::findParam(const string &name)
{
	for (int i = 0;i < m_params.size();i++)
	{
		if (name.compare(m_params[i]->m_name) == 0)
			return m_params[i];
	}

	return 0;
}

CxExternalObject *CxScriptGenerator::findObject(const string &name, CxExternalObject *parentObject)
{
	std::vector<CxExternalObject *> objList = parentObject ? parentObject->children : m_objList;
	for (int i = 0;i < objList.size();i++)
	{
		if (objList[i]->objName == name)
		{
			return objList[i];
		}
	}

	return 0;
}

void CxScriptGenerator::setObjectList(const std::vector<CxExternalObject *> &obj)
{
	m_objList = obj;
}

const std::vector<CxExternalObject *> &CxScriptGenerator::objectList()
{
	return m_objList;
}

void CxScriptGenerator::setOutParamList(const std::vector<CxOutputParam> &paramList)
{
	m_outparamList = paramList;
}

const std::vector<CxOutputParam> &CxScriptGenerator::outparamList()
{
	return m_outparamList;
}

void CxScriptGenerator::setInputParams(const std::vector<CxInputParam *> &pa)
{
	for (int i = 0;i < m_params.size();i++)
	{
		DEL_OBJ(m_params[i]);
	}
	m_params.clear();
	m_params = pa;
}

const std::vector<CxInputParam *> &CxScriptGenerator::inputParams()
{
	return m_params;
}

void CxScriptGenerator::setCalculateParam(CxCalcaulateParam *p)
{
	DEL_OBJ(m_cp);
	m_cp = p;
}

CxCalcaulateParam *CxScriptGenerator::calulateParam()
{
	return m_cp;
}

void CxScriptGenerator::setCalculateRule(CxCalculateRule *r)
{
	DEL_OBJ(m_cpRule);
	m_cpRule = r;
}

CxCalculateRule *CxScriptGenerator::calulateRule()
{
	return m_cpRule;
}

#ifdef NEED_LIMITERS
void CxScriptGenerator::setLimiter(CxLimiters *r)
{
	DEL_OBJ(m_cpLimiter);
	m_cpLimiter = r;
}

CxLimiters *CxScriptGenerator::limiters()
{
	return m_cpLimiter;
}
#endif

void CxScriptGenerator::setExtends(const std::vector<std::pair<string, string> > &ex)
{
	m_extends = ex;
}

const std::vector<std::pair<string, string> > &CxScriptGenerator::extends()
{
	return m_extends;
}

string CxScriptGenerator::getDefaultValue(const string &vType)
{
	if (vType.compare("str") == 0)
	{
		return "";
	}
	else
	{
		return "0";
	}
}

CxKnowlegdeDesc CxScriptGenerator::currentKnowledge()
{
	return m_xmlParser->knowledgetDescription();
}

void CxScriptGenerator::setKnowledge(const CxKnowlegdeDesc &kDesc)
{
	m_xmlParser->setKnowledgeDescription(kDesc);
}