// DialogHistogram.cpp: 实现文件
//

#include "pch.h"
#include "afxdialogex.h"
#include "DialogHistogram.h"
#include "resource.h"


// DialogHistogram 对话框

IMPLEMENT_DYNAMIC(DialogHistogram, CDialogEx)

DialogHistogram::DialogHistogram(CWnd* pParent /*=nullptr*/, CDib* m_pCdib /*= nullptr*/)
	: CDialogEx(IDD_DLG_HISTO, pParent), m_pHist(nullptr), m_nMax(0)
{
	if (m_pCdib) {
		m_pHist = m_pCdib->GrayValueCount();
		m_nMax = m_pCdib->GetMaxColorTableEntries();
	}
}

DialogHistogram::~DialogHistogram()
{
	if (m_pHist)
	{
		delete[] m_pHist;
	}
}

void DialogHistogram::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DialogHistogram, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// DialogHistogram 消息处理程序

void DialogHistogram::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// 不为绘图消息调用 CDialogEx::OnPaint()
	if (m_pHist == NULL)
	{
		return;
	}
	CRect rect;

	CWnd* pWnd = GetDlgItem(IDC_STATIC);
	this->GetWindowRect(&rect);
	//pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	CDC* pDC = &dc;


	//pWnd->UpdateWindow();
	this->UpdateWindow();
	pDC->Rectangle(rect);


	CPen* pRedPen = new CPen;
	pRedPen->CreatePen(PS_SOLID, 2, RGB(255, 0, 0));//画坐标轴的笔，红色
	CPen* pOldPen = pDC->SelectObject(pRedPen);//选择新画笔，返回并保存原值

	CPen* pBluePen = new CPen;
	pBluePen->CreatePen(PS_SOLID, 2, RGB(0, 0, 255));

	rect.DeflateRect(20, 20);

	/*	pDC->Rectangle(&rect);*/

	pDC->MoveTo(rect.left, rect.bottom);//从当前点移动到参数点
	pDC->LineTo(rect.left, rect.top);//画纵坐标
	pDC->LineTo(rect.left - 5, rect.top + 5);//画箭头
	pDC->MoveTo(rect.left, rect.top);
	pDC->LineTo(rect.left + 5, rect.top + 5);

	pDC->MoveTo(rect.left, rect.bottom);//画横坐标
	pDC->LineTo(rect.right, rect.bottom);
	pDC->LineTo(rect.right - 5, rect.bottom - 5);
	pDC->MoveTo(rect.right, rect.bottom);
	pDC->LineTo(rect.right - 5, rect.bottom + 5);


	float DeltaCount = float(rect.Width() - 10) / m_nMax;

	pDC->SelectObject(pBluePen);//选择蓝色画笔，开始画直方图
	int GreyNumberMax = 0;

	for (int i = 0; i < m_nMax; i++)
	{
		if (GreyNumberMax < m_pHist[i])
		{
			GreyNumberMax = m_pHist[i];//直方图中灰度次数最多的值
		}
	}
	for (int i = 0; i < m_nMax; i++)
	{
		if (GreyNumberMax != 0)
		{
			double Ratio = ((double)m_pHist[i]) / GreyNumberMax;
			pDC->MoveTo(rect.left + (int)(i * DeltaCount) + 2, rect.bottom - 2);
			pDC->LineTo(rect.left + (int)(i * DeltaCount) + 2, rect.bottom - 2 - (int)(Ratio * (rect.Height() - 10)));
		}
	}
	for (int i = 0; i < m_nMax; i++)
	{
		if (i % 10 == 0)
		{
			pDC->MoveTo(rect.left + (int)(i * DeltaCount) + 2, rect.bottom - 2);
			pDC->LineTo(rect.left + (int)(i * DeltaCount) + 2, rect.bottom - 2 + 4);
		}
		if (i % 50 == 0)
		{
			pDC->MoveTo(rect.left + (int)(i * DeltaCount) + 2, rect.bottom - 2);
			pDC->LineTo(rect.left + (int)(i * DeltaCount) + 2, rect.bottom - 2 + 10);
		}
	}

	pDC->SelectObject(pOldPen);

	delete pRedPen;
	delete pBluePen;

}

