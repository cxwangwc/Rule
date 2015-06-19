
// FlowMFCCallDlg.h : ͷ�ļ�
//

#pragma once

#include "CxRunDialog.h"

// CFlowMFCCallDlg �Ի���
class CFlowMFCCallDlg : public CDialogEx
{
// ����
public:
	CFlowMFCCallDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FLOWMFCCALL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	vector<CxExternalObject *> m_objs;
	vector<pair<string, string> > m_params;

	CxRunDialog *m_cRun;
	CEdit m_resultEditor;
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedGetResult();
	afx_msg void OnBnClickedOk();
};
