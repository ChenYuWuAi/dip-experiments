#include "pch.h"
#include "CBitPlaneDlg.h"
#include "afxdialogex.h"
#include "resource.h"

IMPLEMENT_DYNCREATE(CBitPlaneDlg, CDialogEx)

CBitPlaneDlg::CBitPlaneDlg(const std::vector<CDib>& bitPlanes, CWnd* pParent /*=nullptr*/, int size /*=8*/)
	: CDialogEx(IDD_BITPLANE_DLG, pParent), m_bitPlanes(bitPlanes), m_currentPlane(0), m_size(size) {
}

void CBitPlaneDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PLANE_INDEX, m_planeIndexLabel);
	DDX_Control(pDX, IDC_BUTTON_PREV, m_prevButton);
	DDX_Control(pDX, IDC_BUTTON_NEXT, m_nextButton);
}

BEGIN_MESSAGE_MAP(CBitPlaneDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_PREV, &CBitPlaneDlg::OnPrev)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CBitPlaneDlg::OnNext)
END_MESSAGE_MAP()

BOOL CBitPlaneDlg::OnInitDialog() {
	CDialogEx::OnInitDialog();
	UpdateBitPlane();
	return TRUE;
}

void CBitPlaneDlg::OnPaint() {
	CPaintDC dc(this);
	int width = m_bitPlanes[0].GetWidth();
	int height = m_bitPlanes[0].GetHeight();
	m_bitPlanes[m_currentPlane].Draw(dc.m_hDC, 0, 0);
}

void CBitPlaneDlg::OnPrev() {
	if (m_currentPlane > 0) {
		--m_currentPlane;
		UpdateBitPlane();
	}
}

void CBitPlaneDlg::OnNext() {
	if (m_currentPlane < m_size - 1) {
		++m_currentPlane;
		UpdateBitPlane();
	}
}

void CBitPlaneDlg::UpdateBitPlane() {
	Invalidate();
	CString str;
	str.Format(_T("当前位平面: %d"), m_currentPlane);
	m_planeIndexLabel.SetWindowText(str);
}