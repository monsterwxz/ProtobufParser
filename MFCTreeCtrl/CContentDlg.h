#pragma once


// CContentDlg 对话框

class CContentDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CContentDlg)

public:
	CContentDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CContentDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CONTENT };
#endif

	CString& GetData() { return m_data; }
	void SetData(const wchar_t *val) { m_data = val; }
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
private:
	CString m_data;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CEdit m_content;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
