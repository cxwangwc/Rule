
// FlowMFCCall.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "FlowMFCCall.h"
#include "FlowMFCCallDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CFlowMFCCallApp

BEGIN_MESSAGE_MAP(CFlowMFCCallApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CFlowMFCCallApp ����

CFlowMFCCallApp::CFlowMFCCallApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CFlowMFCCallApp ����

CFlowMFCCallApp theApp;

//#define COM_TEST
// CFlowMFCCallApp ��ʼ��

BOOL CFlowMFCCallApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

#ifdef COM_TEST
	::CoInitialize(NULL); //��ʼ��com�ӿ�
	CLSID clsid;
	ICAPPXmlInfo *pInterface=NULL;
	HRESULT hRet = ::CLSIDFromProgID(L"CAXAInfo.CAPPInfo",&clsid);
	if(FAILED(hRet))
	{
		return TRUE;
	}
	hRet=CoCreateInstance(clsid,NULL,CLSCTX_INPROC_SERVER,IID_ICAPPXmlInfo,(void **)&pInterface);
	if(FAILED(hRet))
	{
		return TRUE;
	}
	CComBSTR bsPassword = _T("");
	CComBSTR bsFilePath = _T("D:\\TestWC\\Workspace\\����\\���Ź���\\�����ļ�\\����͹̨.cxp");//��ȡ��cxp�ļ�·��
	CComBSTR txtFilePath = _T("D:\\TestWC\\Workspace\\����\\���Ź���\\�����ļ�\\����͹̨.xml");//�����txt�ļ�·��
	VARIANT_BOOL bRet;//����ֵ
	hRet = pInterface->OpenFile(bsFilePath.m_str, bsPassword.m_str);
	hRet = pInterface->InfoToXmlFile(bsFilePath.m_str,0, txtFilePath.m_str);
	hRet =pInterface->CloseFile();
	::CoUninitialize();
#endif

	CFlowMFCCallDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

