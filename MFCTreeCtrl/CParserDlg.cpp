// CParserDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MFCTreeCtrl.h"
#include "CParserDlg.h"
#include "afxdialogex.h"


extern std::string _w2a(const wchar_t *w);
extern std::wstring _a2w(const char *a);

extern void HexToStr(unsigned char *pbDest, unsigned char *pbSrc, int nLen);
extern void ExpandAllNode(HTREEITEM hItem, CTreeCtrl& m_treeShow);
extern void StrToHex(char *pbDest, char *pbSrc, int nLen);
extern std::string UTF8ToGB(const char* str);
extern int IsTextUTF8(char* str, ULONGLONG length);

// CParserDlg 对话框

IMPLEMENT_DYNAMIC(CParserDlg, CDialogEx)

CParserDlg::CParserDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PARSER, pParent)
{

}

CParserDlg::~CParserDlg()
{
}

void CParserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_tree);
}


BEGIN_MESSAGE_MAP(CParserDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CParserDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CParserDlg::OnBnClickedCancel)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CParserDlg::OnTvnSelchangedTree1)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &CParserDlg::OnNMDblclkTree1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TREE1, &CParserDlg::OnNMCustomdrawTree1)
END_MESSAGE_MAP()


// CParserDlg 消息处理程序


void CParserDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CParserDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CParserDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);


	*pResult = 0;
}





void CParserDlg::PrintUnknownFields(
	const UnknownFieldSet& unknown_fields, HTREEITEM parent) {
	for (int i = 0; i < unknown_fields.field_count(); i++) {
		const UnknownField& field = unknown_fields.field(i);
		string field_number = SimpleItoa(field.number());

		switch (field.type()) {
		case UnknownField::TYPE_VARINT: {
			HTREEITEM hCurItem = m_tree.InsertItem(_a2w(std::string("(" + field_number + ")" + "Varint").c_str()).c_str(), parent);
			m_tree.SetItemData(hCurItem, DECSCRIPTION);
			hCurItem = m_tree.InsertItem(_a2w(SimpleItoa(field.varint()).c_str()).c_str(), hCurItem);
			m_tree.SetItemData(hCurItem, OTHER_DATA);
			break;
		}
		case UnknownField::TYPE_FIXED32: {
			HTREEITEM hCurItem = m_tree.InsertItem(_a2w(std::string("(" + field_number + ")" + "Fixed32").c_str()).c_str(), parent);
			m_tree.SetItemData(hCurItem, DECSCRIPTION);

			hCurItem = m_tree.InsertItem(_a2w(SimpleItoa(field.fixed32()).c_str()).c_str(), hCurItem);
			m_tree.SetItemData(hCurItem, OTHER_DATA);
			break;
		}
		case UnknownField::TYPE_FIXED64: {
			HTREEITEM hCurItem = m_tree.InsertItem(_a2w(std::string("(" + field_number + ")" + "Fixed64").c_str()).c_str(), parent);
			m_tree.SetItemData(hCurItem, DECSCRIPTION);

			hCurItem = m_tree.InsertItem(_a2w(SimpleItoa(field.fixed64()).c_str()).c_str(), hCurItem);
			m_tree.SetItemData(hCurItem, OTHER_DATA);
			break;
		}
		case UnknownField::TYPE_LENGTH_DELIMITED: {
			const string& value = field.length_delimited();
			UnknownFieldSet embedded_unknown_fields;

			HTREEITEM hCurItem = m_tree.InsertItem(_a2w(std::string("(" + field_number + ")" + "Length_Delimited" + "(length: " + std::to_string(value.length()) + ")").c_str()).c_str(), parent);
			m_tree.SetItemData(hCurItem, DECSCRIPTION);
			if (!value.empty() && embedded_unknown_fields.ParseFromString(value)) {

				PrintUnknownFields(embedded_unknown_fields, hCurItem);

			}
			else {
				// This field is not parseable as a Message.
				// So it is probably just a plain string.

				int len = value.length() * 3 + 1;
				std::shared_ptr<char> hexstr = std::shared_ptr<char>(new char[len], [](char *p) { delete[]p; });
				memset(hexstr.get(), 0, len);

				HexToStr((unsigned char *)hexstr.get(), (unsigned char *)value.c_str(), value.length());

				std::string &temp = std::string((const char *)hexstr.get());

				if (IsTextUTF8((char *)value.c_str(), value.length())) {
					hCurItem = m_tree.InsertItem(_a2w((temp + "(" + UTF8ToGB(value.c_str()) + ")").c_str()).c_str(), hCurItem);
				}
				else {
					hCurItem = m_tree.InsertItem(_a2w((temp + "(" + value + ")").c_str()).c_str(), hCurItem);
				}
				
				m_tree.SetItemData(hCurItem, DELIMITED_DATA);
			}
			break;
		}
		case UnknownField::TYPE_GROUP: {
			//const string& value = field.group();
			HTREEITEM hCurItem = m_tree.InsertItem(_a2w(std::string("(" + field_number + ")" + "group").c_str()).c_str(), parent);
			m_tree.SetItemData(hCurItem, GROUP_DATA);
			const UnknownFieldSet &embedded_unknown_fields = field.group();
			PrintUnknownFields(embedded_unknown_fields, hCurItem);
			

// 			HTREEITEM hCurItem = m_tree.InsertItem(_a2w(std::string("(" + field_number + ")" + "group").c_str()).c_str(), parent);
// 
// 			if (!value.empty() && embedded_unknown_fields.ParseFromString(value)) {
// 
// 				PrintUnknownFields(embedded_unknown_fields, hCurItem);
// 
// 			}
			break;
		}
		}
	}
}

void CParserDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	int len = GetData().length() / 2;
	std::shared_ptr<char> dest = std::shared_ptr<char>(new char[len], [](char *p) {delete p; });
	memset(dest.get(), 0, len);
	StrToHex(dest.get(), (char *)GetData().c_str(), len);

	HTREEITEM root = m_tree.InsertItem(_T("ROOT"));
	UnknownFieldSet unknown_fields;
	if (unknown_fields.ParseFromString(std::string((const char *)dest.get(), len))) {

		PrintUnknownFields(unknown_fields, root);
	}


	// 	std::string filename = ".\\2.bin";
	// 	std::ifstream infile(filename, std::ios::binary);
	// 	if (infile.is_open()) {
	// 		infile.seekg(0, ios_base::end);
	// 		int len = infile.tellg();
	// 		char *buffer = new char[len];
	// 		infile.seekg(0, ios_base::beg);
	// 		infile.read(buffer, len);
	// 		infile.close();
	// 
	// 		UnknownFieldSet unknown_fields;
	// 		if (unknown_fields.ParseFromString(std::string((const char *)buffer, len))) {
	// 			
	// 			PrintUnknownFields(unknown_fields, root);
	// 		}
	// 	}
	ExpandAllNode(root, m_tree);
}


void CParserDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (m_tree.GetSafeHwnd())
		m_tree.MoveWindow(0, 0, cx, cy, TRUE);
}


BOOL CParserDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_tree.ModifyStyle(NULL, TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES);


	m_ToolTip.CreateEx(this, TTS_ALWAYSTIP);
	CRect rect, rect1;
	m_tree.GetClientRect(&rect);   //获取树控件的客户区矩形
	m_ToolTip.AddTool(GetDlgItem(IDC_TREE1), _T("我的树"), &rect, IDC_TREE1);
	m_ToolTip.SetMaxTipWidth(700);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CParserDlg::OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM selectedItem = m_tree.GetSelectedItem();
	dlg.SetData(m_tree.GetItemText(selectedItem).GetBuffer());
	dlg.DoModal();
	*pResult = 0;
}


void CParserDlg::OnNMCustomdrawTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	LPNMTVCUSTOMDRAW pDraw = (LPNMTVCUSTOMDRAW)pNMHDR;

	switch (pDraw->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		return;
	case CDDS_ITEMPREPAINT:
		// 这里做判断
		DWORD_PTR data = m_tree.GetItemData((HTREEITEM)pDraw->nmcd.dwItemSpec);
		switch (data)
		{
		case DECSCRIPTION:
			pDraw->clrText = RGB(65, 105, 225); // 这样设置文字颜色
			break;
		case DELIMITED_DATA:
			pDraw->clrText = RGB(0, 100, 0); // 这样设置文字颜色
			break;
		case OTHER_DATA:
			pDraw->clrText = RGB(128, 0, 128); // 这样设置文字颜色
			break;
		case GROUP_DATA:
			pDraw->clrText = RGB(255, 0, 0); // 这样设置文字颜色
			break;
		}

		break;
	}

	*pResult = 0;
}


BOOL CParserDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_MOUSEMOVE) {
		CPoint pt = pMsg->pt;
		m_tree.ScreenToClient(&pt);   //转换成控件客户区的坐标
		UINT uFlags;
		HTREEITEM item;
		item = m_tree.HitTest(pt, &uFlags);	//测试这个点 具体用法参看MSDN
		if (item != NULL && (uFlags&TVHT_ONITEMLABEL)) {
			//更新提示信息 内容可自定
			CString text = m_tree.GetItemText(item);
			if (text.GetLength() < 1024) {
				m_ToolTip.UpdateTipText(text, GetDlgItem(IDC_TREE1), IDC_TREE1);
				m_ToolTip.RelayEvent(pMsg);
			}

		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
