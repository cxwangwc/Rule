#pragma once

class CxScriptGenerator;

#include "CxListCtrl.h"
#include "cxflowparse.h"

// CxRunDialog dialog

class CxRunDialog : public CDialog
{
	DECLARE_DYNAMIC(CxRunDialog)

public:
	CxRunDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CxRunDialog();

	void OnEndlabeleditList1(NMHDR* pNMHDR, LRESULT* pResult);
	void setParam(const std::vector<CxInputParam *> &);

	vector<CxExternalObject *> ResultObjects() const;
	vector<pair<string, string>> ResultParams();

	__declspec(dllexport) void GetResult(vector<CxExternalObject *> &,  vector<pair<string, string> > &);


// Dialog Data
	enum { IDD = 3000 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()

	HICON m_hIcon;

public:
	void setKnowledge(const CxKnowlegdeDesc &);
	string getFileName(const CString &name = _T("Category Files (*.*)|*.rxl||"));

	CxScriptGenerator *m_scriptGe;
	std::map<std::string, CxKnowlegdeDesc> m_knowledges;

	CString m_cappPath;

	CComboBox m_combo;
	CEdit m_descEdit;
	CEdit m_fileversionEdit;
	CEdit m_idEdit;
	CEdit m_versionEdit;
	CEdit m_pixfileEdit;
	CEdit m_resultEditor;
	CEditListCtrl m_paramCtrl;

	std::vector<CxInputParam *> m_inputParams;

	// 执行脚本获取得到的结果
	vector<CxExternalObject *> m_objList;
	vector<pair<string, string> > m_retParams;

public:
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnBnClickedButtonFile();
	afx_msg void OnCbnSelchangeKnowledgeList();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedCancel();
	afx_msg __declspec(dllexport) void OnBnClickedDisplay();
};