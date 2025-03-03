
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

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
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
		CMFCFilterChunkValueImpl *pChunk = nullptr;
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


void CDIPExperimentApplicationDoc::OnProcessInvert()
{
	if (m_pDib != nullptr)

		m_pDib->Invert();
	UpdateAllViews(NULL);
	// TODO: 在此添加命令处理程序代码
}
