
// FlowMFCCallDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FlowMFCCall.h"
#include "FlowMFCCallDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CFlowMFCCallDlg �Ի���




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


// CFlowMFCCallDlg ��Ϣ�������

BOOL CFlowMFCCallDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFlowMFCCallDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
			
		string retParamTitle("��������б� :\r\n");
		string retParamString;
		string retObjectString;
		string retObjectTitle = string("��������б�:\r\n");

		for (size_t i = 0;i < m_params.size();i++)
		{
			retParamString += "�������ƣ�" + m_params[i].first +" ����ֵ��" + m_params[i].second + "\r\n";
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
			MessageBox( _T("��ȡ����ɹ�"), _T("�����ȡ"), MB_OK | MB_ICONINFORMATION);
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
