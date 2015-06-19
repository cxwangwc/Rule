// CxRunDialog.cpp : implementation file
//

#include "stdafx.h"
#include "FlowMFC.h"
#include "CxRunDialog.h"
#include "afxdialogex.h"
#include "CxListCtrl.h"

#include "cxscriptgenerator.h"
#include "cxxmlhelper.h"
#include "cxtranstoxml.h"

#include <locale.h>

extern "C" __declspec(dllexport) CxRunDialog* CxShowRunDialog()
{
	// 切记，此处需要加下面这行代码，不然对话框显示不出来。
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CxRunDialog *ph = new CxRunDialog();
	ph->Create(CxRunDialog::IDD);
	ph->ShowWindow(SW_SHOW);
	//ph->DoModal(); //ASSERT error
	return ph;
}

static string GBKToUTF8(const std::string& strGBK)  
{  
	string strOutUTF8 = "";  
	WCHAR * str1;  
	int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);  
	str1 = new WCHAR[n];  
	MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);  
	n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);  
	char * str2 = new char[n];  
	WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);  
	strOutUTF8 = str2;  
	delete[]str1;  
	str1 = NULL;  
	delete[]str2;  
	str2 = NULL;  
	return strOutUTF8;  
}  

static string UTF8ToGBK(const std::string& strUTF8)  
{  
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);  
	WCHAR * wszGBK = new WCHAR[len + 1];  
	memset(wszGBK, 0, len * 2 + 2);  
	MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, wszGBK, len);  

	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);  
	char *szGBK = new char[len + 1];  
	memset(szGBK, 0, len + 1);  
	WideCharToMultiByte(CP_ACP,0, wszGBK, -1, szGBK, len, NULL, NULL);  
	//strUTF8 = szGBK;  
	std::string strTemp(szGBK);  
	delete[]szGBK;  
	delete[]wszGBK;  
	return strTemp;  
}  

// CxRunDialog dialog

IMPLEMENT_DYNAMIC(CxRunDialog, CDialog)

CxRunDialog::CxRunDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CxRunDialog::IDD, pParent)
{
	setlocale(LC_ALL,"chs");
	GetPrivateProfileString(_T("General"), _T("CappPath"), _T("."), m_cappPath.GetBuffer(MAX_PATH), MAX_PATH, _T("./caxa.ini"));
}

CxRunDialog::~CxRunDialog()
{
}

void CxRunDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_KNOWLEDGE_LIST, m_combo);
	DDX_Control(pDX, IDC_DESC_EDIT, m_descEdit);
	DDX_Control(pDX, IDC_FVERSION_EDIT, m_fileversionEdit);
	DDX_Control(pDX, IDC_ID_EDIT, m_idEdit);
	DDX_Control(pDX, IDC_VERSION_EDIT, m_versionEdit);
	DDX_Control(pDX, IDC_PIX_EDIT, m_pixfileEdit);
	DDX_Control(pDX, IDC_RESULT, m_resultEditor);
	DDX_Control(pDX, IDC_PARAM_LIST, m_paramCtrl);
}


BOOL CxRunDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	// TODO: Add extra initialization here

/*
	Label : Width 100 Height 20 Interval 10
*/
// 	CStatic *knowledgeLabel = new CStatic();
// 	knowledgeLabel->Create(_T("知识列表"), WS_CHILD | WS_VISIBLE, CRect(10, 10, 110,30), this, 0);  
// 	m_combo = new CComboBox();
// 	m_combo->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, CRect(140, 10, 240,30), this, 0);  
// 
//  
//  	knowledgeLabel = new CStatic();
// 	knowledgeLabel->Create(_T("知识描述"), WS_CHILD | WS_VISIBLE, CRect(10, 40, 110,30), this, 0);  
//  	m_descEdit = new CEdit();
//  	m_descEdit->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, CRect(140, 40, 240,30), this, 0);   
	m_scriptGe = new CxScriptGenerator("");
	//m_paramCtrl = new CEditListCtrl();
	//m_paramCtrl.Create( LVS_REPORT | WS_CHILD | WS_VISIBLE, CRect(600, 10, 1220, 400), this, 90);
	m_paramCtrl.InsertColumn (0,_T("参数名称")); //添加列名
	m_paramCtrl.InsertColumn (1,_T("参数值")); 
	m_paramCtrl.InsertColumn (2,_T("说明"));
	m_paramCtrl.InsertColumn (3,_T("最大值"));
	m_paramCtrl.InsertColumn (4,_T("最小值"));
	for (int i = 0;i < 5;i++)
		m_paramCtrl.SetColumnWidth (i,80); //设置列宽
	return TRUE;  // return TRUE  unless you set the focus to a control
}


BEGIN_MESSAGE_MAP(CxRunDialog, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CxRunDialog::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_FILE,&CxRunDialog::OnBnClickedButtonFile) 
	ON_CBN_SELCHANGE(IDC_KNOWLEDGE_LIST, &CxRunDialog::OnCbnSelchangeKnowledgeList)
	ON_BN_CLICKED(IDOK, &CxRunDialog::OnBnClickedOk)
	ON_NOTIFY(LVN_ENDLABELEDIT, 90, OnEndlabeleditList1)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CxRunDialog::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDCANCEL2, &CxRunDialog::OnBnClickedCancel)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(ID_DISPLAY, &CxRunDialog::OnBnClickedDisplay)
END_MESSAGE_MAP()


// CxRunDialog message handlers

void CxRunDialog::OnBnClickedButtonSearch()
{
	// TODO: Add your control notification handler code here

	WCHAR szPath[MAX_PATH];     //存放选择的目录路径 
	CString str;

	BROWSEINFO bi;  
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;//此参数如为NULL则不能显示对话框
	bi.lpszTitle = _T("请选择知识目录：");   
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.iImage = 0;
	bi.lParam = 0;

	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);  

	if(lp && SHGetPathFromIDList(lp, szPath))
	{
		CxXmlHelper helper("");

		size_t size;                                                                                                                           
		int requiredSize;                                                                                                                   
		requiredSize = wcstombs( NULL, szPath, 0);                                                                   
		char *path = (char *)malloc( requiredSize + 1);                                              
		size = wcstombs( path, szPath, requiredSize + 1);

		std::string sePath(path) ;//= cu.WcharToChar(szPath);
		if (helper.read(sePath))
		{
			m_knowledges = helper.knowlegdes();
			m_combo.ResetContent();

			std::map<std::string, CxKnowlegdeDesc>::iterator it = m_knowledges.begin();
			while (it != m_knowledges.end())
			{  
				string gbkName = UTF8ToGBK(it->second.m_strName);
				it->second.m_strName = gbkName;
				m_combo.AddString(CString(gbkName.c_str()));

				it++;
			}
			m_combo.SetCurSel(m_combo.GetCount()-1);
			OnCbnSelchangeKnowledgeList();
		}

		free(path);
	}

}

void CxRunDialog::OnCbnSelchangeKnowledgeList()
{
	// TODO: Add your control notification handler code here

	if (m_knowledges.size() == 0)
	{
		return;
	}

	int ind = m_combo.GetCurSel();
	CString text;
	m_combo.GetLBText(ind, text);
	int len =WideCharToMultiByte(CP_ACP,0,text,-1,NULL,0,NULL,NULL);  
	char *ptxtTemp =new char[len +1];  
	WideCharToMultiByte(CP_ACP,0,text,-1,ptxtTemp,len,NULL,NULL );  
	string stdText(ptxtTemp);// = cu.WcharToChar(text.GetBuffer(0));

	std::map<std::string, CxKnowlegdeDesc>::iterator it = m_knowledges.begin();
	while (it != m_knowledges.end())
	{
		if (it->second.m_strName.compare(stdText) == 0)
		{
			setKnowledge(it->second);

			m_scriptGe->setFile(it->second.m_strFileName);
			m_scriptGe->readParams();
			setParam(m_scriptGe->inputParams());
			break;
		}
		it++;
	}
}

void CxRunDialog::setKnowledge(const CxKnowlegdeDesc &know)
{
	string gbkID = UTF8ToGBK(know.m_strID);
	string gbkDesc = UTF8ToGBK(know.m_strDescription);
	string gbkFV = UTF8ToGBK(know.m_strFileVersion);
	string gbkV = UTF8ToGBK(know.m_strVersion);
	string gbkPix = UTF8ToGBK(know.m_strPixFilename);
	m_idEdit.SetWindowText(CString(gbkID.c_str()));
	m_descEdit.SetWindowText(CString(gbkDesc.c_str()));
	m_fileversionEdit.SetWindowText(CString(gbkFV.c_str()));
	m_versionEdit.SetWindowText(CString(gbkV.c_str()));
	m_pixfileEdit.SetWindowText(CString(gbkPix.c_str()));
}

void CxRunDialog::OnBnClickedOk()
{
	for (size_t i = 0;i < m_inputParams.size();i++)
	{
		CString pName = m_paramCtrl.GetItemText(i, 0);
		CString val = m_paramCtrl.GetItemText(i, 1);
		CString maxValue = m_paramCtrl.GetItemText(i, 3);
		CString minValue = m_paramCtrl.GetItemText(i, 4);

		if (!maxValue.IsEmpty() && !minValue.IsEmpty())
		{
			if (val < minValue && val > maxValue)
			{
				MessageBox( _T("您输入的参数 ")+ pName + _T(" 不在定义区间内"), _T("输入错误"), MB_OK | MB_ICONINFORMATION);
				return;
			}
		}
		else if (maxValue.IsEmpty())
		{
			if (val < minValue)
			{
				MessageBox( _T("您输入的参数 ")+ pName + _T(" 不在定义区间内"), _T("输入错误"), MB_OK | MB_ICONINFORMATION);
				return;
			}
		}
		else if (minValue.IsEmpty())
		{
			if (val > maxValue)
			{
				MessageBox( _T("您输入的参数 ")+ pName + _T(" 不在定义区间内"), _T("输入错误"), MB_OK | MB_ICONINFORMATION);
				return;
			}
		}


		int len =WideCharToMultiByte(CP_ACP,0,val,-1,NULL,0,NULL,NULL);  
		char *ptxtTemp =new char[len +1];  
		WideCharToMultiByte(CP_ACP,0,val,-1,ptxtTemp,len,NULL,NULL );  
		m_inputParams[i]->m_value = ptxtTemp;
		m_inputParams[i]->m_value = GBKToUTF8(m_inputParams[i]->m_value);
		delete[] ptxtTemp;  
	}

	//m_scriptGe->setInputParams(m_inputParams);
	string fileName = m_scriptGe->scriptFileName() + ".py";
	m_scriptGe->writeScript(fileName);

	m_objList.clear();
	m_retParams.clear();
	CxScriptGenerator::CallResult res = m_scriptGe->callScript(m_objList, m_retParams);
	if (res == CxScriptGenerator::CalculateRight)
	{
		MessageBox( _T("执行脚本成功"), _T("调用成功"), MB_OK | MB_ICONINFORMATION);
	}
	else if (res == CxScriptGenerator::ImportError || res == CxScriptGenerator::CalculateError)
	{
		MessageBox( _T("请检查您设置的输入输出参数、规则等是否正确，再重新执行.\n失败原因：")+CString(m_scriptGe->lastError().c_str()), _T("调用脚本失败"), MB_OK | MB_ICONERROR);
		return;

	}
	else if (res == CxScriptGenerator::CheckparamError)
	{
		MessageBox( _T("您输入的参数超出了您设定的限定条件，请检查您设置的输入参数，重新输入之后再执行."), _T("调用脚本失败"), MB_OK | MB_ICONERROR);
		return;
	}

	/*	m_resultEditor.Clear();*/
	string retString = "retParams :\r\n";

	for (int i = 0;i < m_retParams.size();i++)
	{
		string resultDisplay;
		string pDescription;
		string pOutType = "str";
		for (int j = 0;j < m_scriptGe->outparamList().size();j++)
		{
			if (m_scriptGe->outparamList()[j].m_strName.compare(m_retParams[i].first) == 0)
			{
				pDescription = m_scriptGe->outparamList()[j].m_strDescription;
				pOutType = m_scriptGe->outparamList()[j].m_strType;
				break;
			}
		}

		resultDisplay = m_retParams[i].first + " : " + m_retParams[i].second + "  ,Description :" + pDescription;
		retString += resultDisplay + "\r\n";
	}

	retString += "retObject:\r\n";
	for (size_t i = 0;i < m_objList.size();i++)
	{
		retString += m_objList[i]->objName + "\r\n";
		for (size_t j = 0;j < m_objList[i]->attri.size();j++)
		{
			retString += "\t" + m_objList[i]->attri[j].first + " : " + m_objList[i]->attri[j].second.second + "\r\n";
		}
	}
	string gbk = UTF8ToGBK(retString);
	m_resultEditor.SetWindowText(CString(gbk.c_str()));
}

void CxRunDialog::setParam(const std::vector<CxInputParam *> &inputParams)
{
	m_inputParams = inputParams;
	m_paramCtrl.DeleteAllItems();

 	for (int i = m_inputParams.size()-1;i >= 0;i--)
 	{
		string gbkName = UTF8ToGBK(m_inputParams[i]->m_name);
		string gbkV = UTF8ToGBK(m_inputParams[i]->m_value);
		string gbkDesc = UTF8ToGBK(m_inputParams[i]->m_description);
		string gbkMax = UTF8ToGBK(m_inputParams[i]->m_max);
		string gbkMin = UTF8ToGBK(m_inputParams[i]->m_min);

 		int n=m_paramCtrl.InsertItem (0, CString(gbkName.c_str()));
 		m_paramCtrl.SetItemText (n,1, CString(gbkV.c_str()));
 		m_paramCtrl.SetItemText (n,2,CString(gbkDesc.c_str()));
 		m_paramCtrl.SetItemText (n,3,CString(gbkMax.c_str()));
 		m_paramCtrl.SetItemText (n,4,CString(gbkMin.c_str()));
 	}

}

void CxRunDialog::OnEndlabeleditList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here

	*pResult = TRUE;
}

void CxRunDialog::OnBnClickedButtonFile()
{
	string fileSelected = getFileName();
	if (fileSelected.length() < 3)
		return;
	m_scriptGe->setFile(fileSelected);
	if (m_scriptGe->readParams())
	{
		m_combo.ResetContent();
		CxKnowlegdeDesc desc = m_scriptGe->currentKnowledge();
		string gbkName = UTF8ToGBK(desc.m_strName);
		desc.m_strName = gbkName;
		m_combo.AddString(CString(gbkName.c_str()));
		m_combo.SetCurSel(m_combo.GetCount()-1);
		setKnowledge(desc);
		setParam(m_scriptGe->inputParams());
	}
}

string CxRunDialog::getFileName(const CString &name)
{
	string fileSelected;
	CFileDialog objFileDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY, name, this);
	int nRet = objFileDialog.DoModal();
	if(nRet != IDOK)
		return fileSelected;
	CString strFileName = objFileDialog.GetPathName();
	//文件路径赋值给CEdit类型控件变量
	int len =WideCharToMultiByte(CP_ACP,0,strFileName,-1,NULL,0,NULL,NULL);  
	char *ptxtTemp =new char[len +1];  
	WideCharToMultiByte(CP_ACP,0,strFileName,-1,ptxtTemp,len,NULL,NULL );  
	fileSelected = ptxtTemp;
	return fileSelected;
}

void CxRunDialog::OnBnClickedButtonSave()
{
	// TODO: Add your control notification handler code here

	string fileSelected = getFileName();
	if (fileSelected.length() < 3)
		return;

	string fName = getFileName(_T("图纸模板文件 (*.*)|*.xml||"));
	if (fName.length() != 0)
	{
		CxTransToXml trXml;
		if (!trXml.readTemplate(fName))
			return;
		if( trXml.saveToFile(m_objList, m_retParams, fileSelected))
		{
			MessageBox( _T("保存成功"), _T("结果文件保存成功，结果文件不能作为工艺来运行。"), MB_OK | MB_ICONINFORMATION);
		}
	}
}

vector<CxExternalObject *> CxRunDialog::ResultObjects() const
{
	return m_objList;
}

vector<pair<string, string> > CxRunDialog::ResultParams()
{
	return m_retParams;
}

void CxRunDialog::GetResult(vector<CxExternalObject *> &objs, vector<pair<string, string> > &params)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	objs = m_objList;
	params = m_retParams;
}

void CxRunDialog::OnBnClickedCancel()
{
	OnCancel();
}

HBRUSH CxRunDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CxRunDialog::OnBnClickedDisplay()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	string fName = getFileName();
	if (fName.length() != 0)
	{
		CxTransToXml trXml;
		if (!trXml.readTemplate(fName))
			return;
		string saveName = "C:\\Program Files\\xxcapp.xml";
		trXml.saveToFile(m_objList, m_retParams, saveName);

		PROCESS_INFORMATION pi;
		STARTUPINFO si;
		memset(&si,0,sizeof(si));
		si.cb=sizeof(si);
		si.wShowWindow=SW_SHOW;
		si.dwFlags=STARTF_USESHOWWINDOW;

		bool fRet=CreateProcess(m_cappPath, _T("  \"C:\\Program Files\\xxcapp.xml\"") ,NULL
			,FALSE, NULL, NULL, NULL, NULL, &si, &pi);
		if (!fRet)
		{
			;
		}
	}

}
