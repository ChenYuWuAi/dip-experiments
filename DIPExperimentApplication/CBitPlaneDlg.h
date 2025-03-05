#pragma once
#include "Dib.h"
#include <vector>

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"

class CBitPlaneDlg : public CDialogEx {
public:
    CBitPlaneDlg() = default;
    CBitPlaneDlg(const std::vector<CDib>& bitPlanes, CWnd* pParent = nullptr);

    DECLARE_DYNCREATE(CBitPlaneDlg)

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_BITPLANE_DLG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg void OnPrev();
    afx_msg void OnNext();
    DECLARE_MESSAGE_MAP()

private:
    void UpdateBitPlane();
    std::vector<CDib> m_bitPlanes;
    int m_currentPlane;
    CStatic m_planeIndexLabel;
    CButton m_prevButton;
    CButton m_nextButton;
};