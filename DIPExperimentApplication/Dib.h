#pragma once
#include "atlimage.h"

#include <complex>
using namespace std;

#define  MAX_SIZE 1000
const int Smoth_Kernel[9] = {1,1,1,1,1,1,1,1,1};
#define Pi 3.1415926535897932354626
#define Ei 2.71828

class CDib :
	public CImage
{
public:
	CDib(void);
	CDib(CDib &Dib);
	~CDib(void);

public:
	enum chan_color {chan_red=1,chan_green,chan_blue,chan_black};
	enum HSIChannel{HUE=0,SATURATION,INTENSITY};

public:
	void LoadFile(LPCTSTR lpszPathName);

public:
	void Invert();

public:
	long* GrayValueCount();

private:
	long m_nWidth;
	long m_nHeight;
	int m_nWidthBytes;
	int m_nBitCount;
	unsigned char *m_pDibBits;
	
private:
	long *m_pGrayValueCount;
public:
	int GetBitCount();
	void GenerateColoredBMP(int colorCode = 0);
};

