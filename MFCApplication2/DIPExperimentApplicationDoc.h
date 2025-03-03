
// DIPExperimentApplicationDoc.h: CDIPExperimentApplicationDoc 类的接口
//


#pragma once
#include "Dib.h"

class CDIPExperimentApplicationDoc : public CDocument
{
protected: // 仅从序列化创建
	CDIPExperimentApplicationDoc() noexcept;
	DECLARE_DYNCREATE(CDIPExperimentApplicationDoc)

// 特性
public:
	CDib* m_pDib = nullptr;

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CDIPExperimentApplicationDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	afx_msg void OnProcessInvert();
};
