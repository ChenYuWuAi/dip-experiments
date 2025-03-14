﻿
// DIPExperimentApplicationView.h: CDIPExperimentApplicationView 类的接口
//

#pragma once


class CDIPExperimentApplicationView : public CView
{
protected: // 仅从序列化创建
	CDIPExperimentApplicationView() noexcept;
	DECLARE_DYNCREATE(CDIPExperimentApplicationView)

// 特性
public:
	CDIPExperimentApplicationDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CDIPExperimentApplicationView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	void UpdateStatusBar();
};

#ifndef _DEBUG  // DIPExperimentApplicationView.cpp 中的调试版本
inline CDIPExperimentApplicationDoc* CDIPExperimentApplicationView::GetDocument() const
   { return reinterpret_cast<CDIPExperimentApplicationDoc*>(m_pDocument); }
#endif

