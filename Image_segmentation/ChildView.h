
// ChildView.h: CChildView 클래스의 인터페이스
//


#pragma once
#include"Img_population.h"
#include<string>

class Img_population;

// CChildView 창

class CChildView : public CWnd
{
// 생성입니다.
public:
	CChildView();

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CChildView();

	// 생성된 메시지 맵 함수
protected:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnImageNew();
	afx_msg void OnImageDraw();
	DECLARE_MESSAGE_MAP()


public:
	CBitmap Image;
	BITMAP bmpinfo;
	CDC ImageDC;
	CDC mainDC;
	CBitmap mainBmp;
	Img_population* Image_population;

	int generation;
	bool RI, LTS;

};

