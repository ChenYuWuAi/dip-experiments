
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

	return TRUE;
}

void GenerateColoredBMP(LPCTSTR lpszPathName, int rgbRed = 0, int rgbGreen = 0, int rgbBlue = 0) {
	// 通过调色板生成单色图像
	CImage img;
	img.Create(512, 512, 8);  // 宽度 512，高度 512, 8 位色深

	// 检查图像是否创建成功
	if (img.IsNull())
	{
		// 处理错误
		return;
	}

	RGBQUAD palette = { 0 };
	palette.rgbRed = rgbRed;
	palette.rgbGreen = rgbGreen;
	palette.rgbBlue = rgbBlue;


	img.SetColorTable(0, 1, &palette);  // 设置完整调色板‌:ml-citation{ref="1,4" data="citationList"}

	// 填充像素数据
	BYTE* pBits = (BYTE*)img.GetBits();
	int pitch = img.GetPitch();
	// 填充像素数据
	for (int y = 0; y < 512; y++)
	{
		for (int x = 0; x < 512; x++)
		{
			pBits[y * pitch + x] = 0;  // 0 代表调色板的第一个颜色
		}
	}
	

	img.Save(lpszPathName, Gdiplus::ImageFormatBMP);  // 保存文件‌:ml-citation{ref="2,3" data="citationList"}
}

void CDIPExperimentApplicationDoc::OnProcessInvert()
{
	if (m_pDib != nullptr)

		m_pDib->Invert();
	UpdateAllViews(NULL);
	// TODO: 在此添加命令处理程序代码
}

void CDIPExperimentApplicationDoc::OnGenerateBlue() {
	CFileDialog dlg(FALSE, _T("bmp"), NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("BMP Files (*.bmp)|*.bmp|All Files (*.*)|*.*||"));

	if (dlg.DoModal() == IDOK) {  // 用户确认保存路径
		CString path = dlg.GetPathName();
		GenerateColoredBMP(path, 0, 0, 255);  // 传递 color_code=1 生成蓝色图像‌:ml-citation{ref="1,4" data="citationList"}
		OnOpenDocument(path);
		// 刷新视图
		UpdateAllViews(NULL);
	}
}


void CDIPExperimentApplicationDoc::OnGenerateGreen()
{
	CFileDialog dlg(FALSE, _T("bmp"), NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("BMP Files (*.bmp)|*.bmp|All Files (*.*)|*.*||"));

	if (dlg.DoModal() == IDOK) {  // 用户确认保存路径
		CString path = dlg.GetPathName();
		GenerateColoredBMP(path, 0, 255, 0);  // 传递 color_code=1 生成蓝色图像‌:ml-citation{ref="1,4" data="citationList"}
		OnOpenDocument(path);
		// 刷新视图
		UpdateAllViews(NULL);
	}
}

void CDIPExperimentApplicationDoc::OnGenerateRed()
{
	CFileDialog dlg(FALSE, _T("bmp"), NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("BMP Files (*.bmp)|*.bmp|All Files (*.*)|*.*||"));

	if (dlg.DoModal() == IDOK) {  // 用户确认保存路径
		CString path = dlg.GetPathName();
		GenerateColoredBMP(path, 255, 0, 0);  // 传递 color_code=1 生成蓝色图像‌:ml-citation{ref="1,4" data="citationList"}
		OnOpenDocument(path);
		// 刷新视图
		UpdateAllViews(NULL);
	}
}
