
// FlowMFCCallDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FlowMFCCall.h"
#include "FlowMFCCallDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFlowMFCCallDlg 对话框




CFlowMFCCallDlg::CFlowMFCCallDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFlowMFCCallDlg::IDD, pParent)
	, m_cRun(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFlowMFCCallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_RESULT, m_resultEditor);
}

BEGIN_MESSAGE_MAP(CFlowMFCCallDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CFlowMFCCallDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDRESULT, &CFlowMFCCallDlg::OnBnClickedGetResult)
	ON_BN_CLICKED(IDOK, &CFlowMFCCallDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CFlowMFCCallDlg 消息处理程序

BOOL CFlowMFCCallDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFlowMFCCallDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFlowMFCCallDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFlowMFCCallDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFlowMFCCallDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	
	HINSTANCE hDLL = LoadLibrary(_T("FlowMFC.dll"));
	if (hDLL == NULL)
	{
		//MessageBox("Load library failed.");
		return;
	}

	typedef CxRunDialog * (*runFunc)();
	runFunc pDialog = (runFunc) GetProcAddress(hDLL, "CxShowRunDialog");
	if (pDialog == NULL)
	{
		return;
	}

	m_cRun = pDialog();

	//crun->GetResult(objs, params);
	
	//FreeLibrary(hDLL);
	
// 	CxRunDialog *cRun = new CxRunDialog();
// 	cRun->Create(CxRunDialog::IDD);
// 	cRun->DoModal();
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


void CFlowMFCCallDlg::OnBnClickedGetResult()
{
	// TODO: Add your control notification handler code here

	if (m_cRun)
	{
		m_cRun->GetResult(m_objs, m_params);
			
		string retParamTitle("输出参数列表 :\r\n");
		string retParamString;
		string retObjectString;
		string retObjectTitle = string("输出对象列表:\r\n");

		for (size_t i = 0;i < m_params.size();i++)
		{
			retParamString += "参数名称：" + m_params[i].first +" 参数值：" + m_params[i].second + "\r\n";
		}
		
		for (size_t i = 0;i < m_objs.size();i++)
		{
			retObjectString += m_objs[i]->objName + "\r\n";
			for (size_t j = 0;j < m_objs[i]->attri.size();j++)
			{
				retObjectString += "\t" + m_objs[i]->attri[j].first + " : " + m_objs[i]->attri[j].second.second + "\r\n";
			}
		}
		//string gbk = UTF8ToGBK(retString);
		string displayString = retParamTitle + UTF8ToGBK(retParamString) + retObjectTitle + UTF8ToGBK(retObjectString);
		m_resultEditor.SetWindowText(CString(displayString.c_str()));

		if (m_objs.size() != 0)
		{
			MessageBox( _T("获取结果成功"), _T("结果获取"), MB_OK | MB_ICONINFORMATION);
		}
	}
}


void CFlowMFCCallDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	if (m_cRun)
	{
		m_cRun->OnBnClickedDisplay();
	}
}
