
// DIPExperimentApplicationDoc.cpp: CDIPExperimentApplicationDoc 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "DIPExperimentApplication.h"
#endif

#include "DIPExperimentApplicationDoc.h"
#include "CBitPlaneDlg.h"
#include "DialogHistogram.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDIPExperimentApplicationDoc

IMPLEMENT_DYNCREATE(CDIPExperimentApplicationDoc, CDocument)

BEGIN_MESSAGE_MAP(CDIPExperimentApplicationDoc, CDocument)
	ON_COMMAND(ID_PROCESS_INVERT, &CDIPExperimentApplicationDoc::OnProcessInvert)
	ON_COMMAND(ID_GENERATE_BLUE, &CDIPExperimentApplicationDoc::OnGenerateBlue)
	ON_COMMAND(ID_GENERATE_GREEN, &CDIPExperimentApplicationDoc::OnGenerateGreen)
	ON_COMMAND(ID_GENERATE_RED, &CDIPExperimentApplicationDoc::OnGenerateRed)
	ON_COMMAND(ID_GENERATE_GREY, &CDIPExperimentApplicationDoc::OnGenerateGrey)
	ON_COMMAND(ID_LINEAR_TRANSFORM, &CDIPExperimentApplicationDoc::OnLinearTransform)
	ON_COMMAND(ID_BITPLANE, &CDIPExperimentApplicationDoc::OnBitplane)
	ON_COMMAND(ID_HISTOGRAM, &CDIPExperimentApplicationDoc::OnHistogram)
	ON_COMMAND(ID_HISTOGRAM_BALANCE, &CDIPExperimentApplicationDoc::OnHistogramBalance)
	ON_COMMAND(ID_RESTORE, &CDIPExperimentApplicationDoc::OnRestore)
	ON_COMMAND(ID_SMOOTH, &CDIPExperimentApplicationDoc::OnSmooth)
	ON_COMMAND(ID_LAPLACE, &CDIPExperimentApplicationDoc::OnLaplace)
	ON_COMMAND(ID_SOBER_X, &CDIPExperimentApplicationDoc::OnSoberX)
	ON_COMMAND(ID_SOBER_Y, &CDIPExperimentApplicationDoc::OnSoberY)
	ON_COMMAND(ID_SHARPEN, &CDIPExperimentApplicationDoc::OnSharpen)
	ON_COMMAND(ID_SIGMA1, &CDIPExperimentApplicationDoc::OnSigma1)
	ON_COMMAND(ID_SIGMA5, &CDIPExperimentApplicationDoc::OnSigma5)
	ON_COMMAND(ID_SIGMA9, &CDIPExperimentApplicationDoc::OnSigma9)
	ON_COMMAND(ID_SIGMA13, &CDIPExperimentApplicationDoc::OnSigma13)
	ON_COMMAND(ID_SOBER, &CDIPExperimentApplicationDoc::OnSober)
	ON_COMMAND(ID_CLAHE, &CDIPExperimentApplicationDoc::OnClahe)
END_MESSAGE_MAP()


// CDIPExperimentApplicationDoc 构造/析构

CDIPExperimentApplicationDoc::CDIPExperimentApplicationDoc() noexcept
{
	// TODO: 在此添加一次性构造代码

}

CDIPExperimentApplicationDoc::~CDIPExperimentApplicationDoc()
{
	if (m_pDib != nullptr)
	{
		delete m_pDib;
		m_pDib = nullptr;
	}
}

BOOL CDIPExperimentApplicationDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CDIPExperimentApplicationDoc 序列化

void CDIPExperimentApplicationDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CDIPExperimentApplicationDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CDIPExperimentApplicationDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CDIPExperimentApplicationDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl* pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CDIPExperimentApplicationDoc 诊断

#ifdef _DEBUG
void CDIPExperimentApplicationDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDIPExperimentApplicationDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDIPExperimentApplicationDoc 命令
BOOL CDIPExperimentApplicationDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// 先释放旧的 m_pDib
	if (m_pDib != nullptr)
	{
		delete m_pDib;
		m_pDib = nullptr;
	}
	// 使用Dlib加载BMP文件
	m_pDib = new CDib();

	if (m_pDib == NULL)
	{
		delete m_pDib;
		m_pDib = nullptr;
		return FALSE;
	}

	m_pDib->LoadFile(lpszPathName);
	m_strPathName = lpszPathName;

	return TRUE;
}

void CDIPExperimentApplicationDoc::OnProcessInvert()
{
	if (m_pDib != nullptr)
		m_pDib->Invert();
	UpdateAllViews(NULL);
	// TODO: 在此添加命令处理程序代码
}

void CDIPExperimentApplicationDoc::OnGenerateBlue() {

	if (m_pDib != nullptr)
		m_pDib->GenerateColoredBMP(3);

	UpdateAllViews(NULL);
}


void CDIPExperimentApplicationDoc::OnGenerateGreen()
{

	if (m_pDib != nullptr)
		m_pDib->GenerateColoredBMP(2);
	UpdateAllViews(NULL);
}

void CDIPExperimentApplicationDoc::OnGenerateRed()
{
	if (m_pDib != nullptr)
		m_pDib->GenerateColoredBMP(1);

	UpdateAllViews(NULL);
}


void CDIPExperimentApplicationDoc::OnGenerateGrey()
{
	if (m_pDib != nullptr)
		m_pDib->GenerateColoredBMP(0);
	UpdateAllViews(NULL);
}

void CDIPExperimentApplicationDoc::OnLinearTransform()
{
	if (m_pDib != nullptr)
		m_pDib->LinearTransform();
	UpdateAllViews(NULL);
}

void CDIPExperimentApplicationDoc::OnBitplane()
{
	if (m_pDib != nullptr) {
		std::vector<CDib> bitPlanes;
		m_pDib->BitPlane(bitPlanes);
		CBitPlaneDlg dlg(bitPlanes);
		dlg.DoModal();
	}
}

void CDIPExperimentApplicationDoc::OnHistogram()
{
	if (!m_pDib)
		return;
	DialogHistogram dlg(nullptr, m_pDib);
	dlg.DoModal();
}

void CDIPExperimentApplicationDoc::OnHistogramBalance()
{
	if (!m_pDib)
		return;
	m_pDib->HistogramBalance();
	UpdateAllViews(NULL);
	DialogHistogram dlg(nullptr, m_pDib);
	dlg.DoModal();
}

void CDIPExperimentApplicationDoc::OnRestore()
{
	// 重新加载原图
	if (m_pDib != nullptr)
	{
		delete m_pDib;
		m_pDib = nullptr;
	}
	else {
		return;
	}
	// 获取文档路径
	LPCTSTR lpszPathName = m_strPathName;
	OnOpenDocument(lpszPathName);
	UpdateAllViews(NULL);
}

void CDIPExperimentApplicationDoc::OnSmooth()
{
	if (m_pDib != nullptr)
		m_pDib->Kerneling(Smoth_Kernel, 9);
	UpdateAllViews(NULL);
}

void CDIPExperimentApplicationDoc::OnLaplace()
{
	if (m_pDib != nullptr)
		m_pDib->Kerneling(Laplace_Kernel);
	UpdateAllViews(NULL);
}

void CDIPExperimentApplicationDoc::OnSoberX()
{
	if (m_pDib != nullptr)
		m_pDib->Kerneling(Sober_X_Kernel);
	UpdateAllViews(NULL);
}

void CDIPExperimentApplicationDoc::OnSoberY()
{
	if (m_pDib != nullptr)
		m_pDib->Kerneling(Sober_Y_Kernel);
	UpdateAllViews(NULL);
}

void CDIPExperimentApplicationDoc::OnSharpen()
{
	if (m_pDib != nullptr)
		m_pDib->Kerneling(Sharpen_Kernel, 1, false);
	UpdateAllViews(NULL);
}

// 图像平滑函数
void CDIPExperimentApplicationDoc::SmoothImage(CDib& image, float sigma) {
	int kernelSize = static_cast<int>(std::ceil(6 * sigma)) | 1; // 确保为奇数
	std::vector<float> kernel = CDib::GenerateGaussianKernel(kernelSize, sigma);
	image.SeparableConvolution(kernel);
	image.MultiThreads_SeparableConvolution(kernel);
}

void CDIPExperimentApplicationDoc::OnSigma1()
{
	if (m_pDib != nullptr)
		SmoothImage(*m_pDib, 1.0f);
	UpdateAllViews(NULL);
}

void CDIPExperimentApplicationDoc::OnSigma5()
{
	if (m_pDib != nullptr)
		SmoothImage(*m_pDib, 5.0f);
	UpdateAllViews(NULL);
}

void CDIPExperimentApplicationDoc::OnSigma9()
{
	if (m_pDib != nullptr)
		SmoothImage(*m_pDib, 9.0f);
	UpdateAllViews(NULL);
}

void CDIPExperimentApplicationDoc::OnSigma13()
{
	if (m_pDib != nullptr)
		SmoothImage(*m_pDib, 13.0f);
	UpdateAllViews(NULL);
}

void CDIPExperimentApplicationDoc::OnSober()
{
	if (m_pDib != nullptr) {
		std::vector<CDib> bitPlanes;
		bitPlanes.resize(2);
		m_pDib->SobelTransform(bitPlanes[0], bitPlanes[1]);
		CBitPlaneDlg dlg(bitPlanes, NULL, 2);
		dlg.DoModal();
	}
}

void CDIPExperimentApplicationDoc::OnClahe()
{
	if (m_pDib != nullptr) {
		m_pDib->CLAHE();
		UpdateAllViews(NULL);
	}
}
