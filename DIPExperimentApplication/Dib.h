#pragma once
#include "atlimage.h"

#include <complex>
#include <vector>
using namespace std;

#define  MAX_SIZE 1000
const int Smoth_Kernel[9] = { 1,1,1,1,1,1,1,1,1 };
const int Sharpen_Kernel[9] = { 0,-1,0,-1,5,-1,0,-1,0 };
const int Sober_X_Kernel[9] = { 1,0,-1,2,0,-2,1,0,-1 };
const int Sober_Y_Kernel[9] = { 1,2,1,0,0,0,-1,-2,-1 };
const int Laplace_Kernel[9] = { 0,-1,0,-1,4,-1,0,-1,0 };

#define Pi 3.1415926535897932354626
#define Ei 2.71828

class CDib :
	public CImage
{
public:
	CDib(void);
	CDib(const CDib& Dib);
	~CDib(void);

public:
	enum chan_color { chan_red = 1, chan_green, chan_blue, chan_black };
	enum HSIChannel { HUE = 0, SATURATION, INTENSITY };

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
	unsigned char* m_pDibBits;

private:
	long* m_pGrayValueCount;
public:
	int GetBitCount();
	void GenerateColoredBMP(int colorCode = 0);
	void LinearTransform();
	void BitPlane(std::vector<CDib>& planes);
	void HistogramBalance();
	void Kerneling(const int* kernel);
};

