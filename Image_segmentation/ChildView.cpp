
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "Image_segmentation.h"
#include "ChildView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
	:Image_population(nullptr)
{
}

CChildView::~CChildView()
{
	if (Image_population != nullptr) {
		delete Image_population;
	}
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_COMMAND(ID_IMAGE_NEW, &CChildView::OnImageNew)
	ON_COMMAND(ID_IMAGE_DRAW, &CChildView::OnImageDraw)
END_MESSAGE_MAP()



// CChildView 메시지 처리기

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	//dc.BitBlt(0, 0, bmpinfo.bmWidth*2+30, bmpinfo.bmHeight+20, &mainDC, 0, 0, SRCCOPY);
	int scale = 2;
	dc.StretchBlt(0, 0, (bmpinfo.bmWidth * 2 + 30) * scale, (bmpinfo.bmHeight + 20) * scale, &mainDC, 0, 0, bmpinfo.bmWidth * 2 + 30, bmpinfo.bmHeight + 20, SRCCOPY);

	CString str;
	

	if (RI) {
		if (LTS) {
			str.Format(L"generation: %d  |  RI/LTS  |  label_size : %d, featrue_vector_size : %d*%d, neighborhood_size : %d*%d, mating_attmpt : %d ", 
				this->generation, label_size, 1+2*feature_matrix_size, 1 + 2 * feature_matrix_size, 1 + 2 * neighborhood_size, 1 + 2 * neighborhood_size, attempt_mating);
		}
		else {
			str.Format(L"generation: %d  |  RI/RWS  |  label_size : %d, featrue_vector_size : %d*%d, neighborhood_size : %d*%d, mating_attmpt : %d ",
				this->generation, label_size, 1 + 2 * feature_matrix_size, 1 + 2 * feature_matrix_size, 1 + 2 * neighborhood_size, 1 + 2 * neighborhood_size, attempt_mating);
		}
	}
	else {
		if (LTS) {
			str.Format(L"generation: %d  |  GI/LTS  |  label_size : %d, featrue_vector_size : %d*%d, neighborhood_size : %d*%d, mating_attmpt : %d ",
				this->generation, label_size, 1 + 2 * feature_matrix_size, 1 + 2 * feature_matrix_size, 1 + 2 * neighborhood_size, 1 + 2 * neighborhood_size, attempt_mating);
		}
		else {
			str.Format(L"generation: %d  |  GI/RWS  |  label_size : %d, featrue_vector_size : %d*%d, neighborhood_size : %d*%d, mating_attmpt : %d ",
				this->generation, label_size, 1 + 2 * feature_matrix_size, 1 + 2 * feature_matrix_size, 1 + 2 * neighborhood_size, 1 + 2 * neighborhood_size, attempt_mating);
		}
	}

	dc.TextOutW(20, (bmpinfo.bmHeight + 20) * scale + 10, str);

	

	// 그리기 메시지에 대해서는 CWnd::OnPaint()를 호출하지 마십시오.
}



int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	this->generation = 0;
	this->RI = true;
	this->LTS = true;

	this->Image.LoadBitmapW(IDB_BITMAP1); //============================================================//
	this->Image.GetBitmap(&bmpinfo);
	
	this->mainDC.CreateCompatibleDC(GetDC());
	this->mainBmp.CreateCompatibleBitmap(GetDC(), bmpinfo.bmWidth*2 + 30, bmpinfo.bmHeight + 20);
	this->mainDC.SelectObject(&mainBmp);

	this->ImageDC.CreateCompatibleDC(&mainDC);
	this->ImageDC.SelectObject(Image);

	int* pData = NULL;
	pData = (int*)malloc(bmpinfo.bmWidthBytes * bmpinfo.bmHeight);
	memset(pData, 0x00, bmpinfo.bmWidthBytes * bmpinfo.bmHeight);
	Image.GetBitmapBits(bmpinfo.bmWidthBytes * bmpinfo.bmHeight, pData);
	RGBQUAD* pRgb = (RGBQUAD*)pData;
	
	int** gray_img = new int* [bmpinfo.bmHeight];
	
	for (int y = 0; y < bmpinfo.bmHeight; y++)
	{
		gray_img[y] = new int[bmpinfo.bmWidth];
		for (int x = 0; x < bmpinfo.bmWidth; x++)
		{
			int nPos = x + (y * bmpinfo.bmWidth);

			int tmp = (pRgb[nPos].rgbRed*30+pRgb[nPos].rgbGreen*59+pRgb[nPos].rgbRed*11)/100;

			gray_img[y][x] = tmp;
		}
	}

	Image_population = new Img_population(bmpinfo,gray_img, this->RI); //===============================================//

	//for (int i = 0; i < 3; i++) {
	//	Image_population->new_population();
	//}

	mainDC.BitBlt(10, 10, bmpinfo.bmWidth, bmpinfo.bmHeight, &ImageDC, 0, 0, SRCCOPY);
	CString A_str;
	A_str = "";
	CString str[8];
	for (int y = 0; y < bmpinfo.bmHeight; y++) {
		for (int x = 0; x < bmpinfo.bmWidth; x++) {

			//int temp = Image_population->org_img[y][x]->label*32  +Image_population->org_img[y][x]->feature_vector;
			//int temp = Image_population->org_img[y][x]->value;

			int temp = Image_population->chr_img[y][x]->get_label()*(256/label_size);
			mainDC.SetPixel(x+bmpinfo.bmWidth+20, y + 10, RGB(temp, temp, temp));
			//TRACE("\n [%d,%d]->%d \n", x, y, Image_population->org_img[y][x]->value);

			//mainDC.SetPixel(x, y + bmpinfo.bmHeight + 10,
			//	RGB(Image_population->chr_img[y][x]->value, Image_population->chr_img[y][x]->value, Image_population->chr_img[y][x]->value));
		}

	}

	delete pRgb;
	for (int i = 0; i < bmpinfo.bmHeight; i++) {
		delete gray_img[i];
	}
	delete[] gray_img;
	return 0;
}


void CChildView::OnImageNew()
{
	for (int i = 0; i < 5; i++) {    //======================================================
		if (LTS)
			Image_population->new_population_LTS();
		else
			Image_population->new_population_RWS();
		Image_population->mating2();
		Image_population->compute_fitness();
		this->generation++;
	}
	for (int y = 0; y < bmpinfo.bmHeight; y++) {
		for (int x = 0; x < bmpinfo.bmWidth; x++) {

			//int temp = Image_population->org_img[y][x]->label*32  +Image_population->org_img[y][x]->feature_vector;
			//int temp = Image_population->org_img[y][x]->value;

			int temp = Image_population->chr_img[y][x]->get_label() * (256 / (label_size+1));
			mainDC.SetPixel(x + bmpinfo.bmWidth + 20, y + 10, RGB(temp, temp, temp));

			//mainDC.SetPixel(x, y + bmpinfo.bmHeight + 10,
			//	RGB(Image_population->chr_img[y][x]->value, Image_population->chr_img[y][x]->value, Image_population->chr_img[y][x]->value));
		}
	}
	

	Invalidate();
}


void CChildView::OnImageDraw()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	this->mainBmp.DeleteObject();
	this->mainBmp.CreateCompatibleBitmap(GetDC(), bmpinfo.bmWidth*2 + 30, bmpinfo.bmHeight + 20);
	this->mainDC.SelectObject(&mainBmp);
	mainDC.BitBlt(10, 10, bmpinfo.bmWidth, bmpinfo.bmHeight, &ImageDC, 0, 0, SRCCOPY);
	Chromosome*** tmp = this->Image_population->chr_img;
	//Chromosome*** tmp = this->Image_population->org_img;
	bool flag;
	for (int y = 0; y < bmpinfo.bmHeight; y++) {
		for (int x = 0; x < bmpinfo.bmWidth; x++) {
			flag = false;
			for (int i = -1; i <= 1; i++) {
				if (((y + i) < 0) || ((y + i) >= bmpinfo.bmHeight)) continue;
				for (int j = -1; j <= -1; j++) {
					if (((x + j) < 0) || ((x + j) >= bmpinfo.bmHeight)) continue;
					if ((i == 0) && (j == 0)) continue;

					if (tmp[y][x]->get_label() != tmp[y + i][x + j]->get_label()) {
						flag=true;
						break;
					}
				}
				if (flag)
					break;
			}

			if (flag)
				//mainDC.SetPixel(x + 10, y + 10, RGB(255, 255, 255));
				mainDC.SetPixel(x + bmpinfo.bmWidth + 20, y + 10, RGB(255, 255, 255));

		}
	}


	Invalidate();
}
