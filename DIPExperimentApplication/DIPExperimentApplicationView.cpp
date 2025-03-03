
// DIPExperimentApplicationView.cpp: CDIPExperimentApplicationView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "DIPExperimentApplication.h"
#endif

#include "MainFrm.h"
#include "DIPExperimentApplicationDoc.h"
#include "DIPExperimentApplicationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDIPExperimentApplicationView

IMPLEMENT_DYNCREATE(CDIPExperimentApplicationView, CView)

BEGIN_MESSAGE_MAP(CDIPExperimentApplicationView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CDIPExperimentApplicationView 构造/析构

CDIPExperimentApplicationView::CDIPExperimentApplicationView() noexcept
{
	// TODO: 在此处添加构造代码

}

CDIPExperimentApplicationView::~CDIPExperimentApplicationView()
{
}

BOOL CDIPExperimentApplicationView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CDIPExperimentApplicationView 绘图

void CDIPExperimentApplicationView::OnDraw(CDC* pDC)
{
	CDIPExperimentApplicationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->m_pDib != nullptr)
	{
		pDoc->m_pDib->Draw(pDC->m_hDC, 0, 0);
		UpdateStatusBar();
	}

}


// CDIPExperimentApplicationView 打印

BOOL CDIPExperimentApplicationView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CDIPExperimentApplicationView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CDIPExperimentApplicationView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CDIPExperimentApplicationView 诊断

#ifdef _DEBUG
void CDIPExperimentApplicationView::AssertValid() const
{
	CView::AssertValid();
}

void CDIPExperimentApplicationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDIPExperimentApplicationDoc* CDIPExperimentApplicationView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDIPExperimentApplicationDoc)));
	return (CDIPExperimentApplicationDoc*)m_pDocument;
}
#endif //_DEBUG


// CDIPExperimentApplicationView 消息处理程序

void CDIPExperimentApplicationView::UpdateStatusBar()
{
	CDIPExperimentApplicationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc || !pDoc->m_pDib)
		return;

	// 获取图像大小和色深位数
	int width = pDoc->m_pDib->GetWidth();
	int height = pDoc->m_pDib->GetHeight();
	int bitCount = pDoc->m_pDib->GetBitCount();

	// 构建状态栏信息字符串
	CString strStatus;
	strStatus.Format(_T("大小: %d x %d, 色深: %d 位"), width, height, bitCount);

	// 获取主框架窗口并更新状态栏
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (pMainFrame)
	{
		pMainFrame->SetMessageText(strStatus);
	}
}