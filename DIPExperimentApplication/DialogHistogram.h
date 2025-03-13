#pragma once
#include "afxdialogex.h"
#include "Dib.h"


// DialogHistogram 对话框

class DialogHistogram : public CDialogEx
{
	DECLARE_DYNAMIC(DialogHistogram)

public:
	DialogHistogram(CWnd* pParent = nullptr, CDib* m_pCdib = nullptr);   // 标准构造函数
	virtual ~DialogHistogram();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_HISTO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	long* m_pHist;
	int m_nMax;
	afx_msg void OnPaint();
};
