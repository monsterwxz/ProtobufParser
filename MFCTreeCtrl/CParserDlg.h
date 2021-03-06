#pragma once
#include "CContentDlg.h"

// CParserDlg 对话框

class CParserDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CParserDlg)

public:
	CParserDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CParserDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PARSER };
#endif

	std::string GetData() const { return data_; }
	void SetData(std::string val) { data_ = val; }
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
private:
	std::string data_;
public:
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	void PrintUnknownFields(const UnknownFieldSet& unknown_fields, HTREEITEM parent);
private:
	CContentDlg dlg;
	CToolTipCtrl	m_ToolTip;
public:
	CTreeCtrl m_tree;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawTree1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
