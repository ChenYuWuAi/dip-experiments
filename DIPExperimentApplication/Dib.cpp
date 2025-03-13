#include "pch.h"
#include "Dib.h"


CDib::CDib(void) :m_pDibBits(NULL), m_pGrayValueCount(NULL)
{
	// initialized variables
	m_nBitCount = 0;
	m_nWidth = 0;
	m_nHeight = 0;
	m_nWidthBytes = 0;

}


CDib::CDib(const CDib& Dib) :m_pDibBits(NULL), m_pGrayValueCount(NULL)
{
	// initialized variables
	m_nBitCount = 0;
	m_nWidth = 0;
	m_nHeight = 0;
	m_nWidthBytes = 0;

	if (&Dib == NULL)
	{
		return;
	}
	if (!IsNull())
	{
		Destroy();
	}
	Create(Dib.GetWidth(), Dib.GetHeight(), Dib.GetBPP(), 0);
	m_nWidth = Dib.m_nWidth;
	m_nHeight = Dib.m_nHeight;
	if (IsIndexed())
	{
		int nColors = Dib.GetMaxColorTableEntries();
		if (nColors > 0)
		{
			RGBQUAD* pal{};
			pal = new RGBQUAD[nColors];
			Dib.GetColorTable(0, nColors, pal);
			SetColorTable(0, nColors, pal);
			delete[] pal;
		}
	}
	m_nWidthBytes = abs(GetPitch());
	m_nBitCount = GetBPP();
	m_pDibBits = (unsigned char*)GetBits() + (m_nHeight - 1) * GetPitch();
	memcpy(m_pDibBits, Dib.m_pDibBits, m_nHeight * m_nWidthBytes);
}

CDib::~CDib(void)
{
	m_pDibBits = NULL;
	if (m_pGrayValueCount != NULL)
	{
		delete[]m_pGrayValueCount;
		m_pGrayValueCount = NULL;
	}
}

void CDib::LoadFile(LPCTSTR lpszPathName)
{
	Load(lpszPathName);
	m_nWidth = GetWidth();
	m_nHeight = GetHeight();
	m_nWidthBytes = abs(GetPitch());
	m_nBitCount = GetBPP();
	m_pDibBits = (unsigned char*)GetBits() + (m_nHeight - 1) * GetPitch();
}

void CDib::Invert()
{
	for (int i = 0; i < m_nHeight; i++)
	{
		for (int j = 0; j < m_nWidthBytes; j++)
		{
			*(m_pDibBits + i * m_nWidthBytes + j) = 255 - *(m_pDibBits + i * m_nWidthBytes + j);
		}
	}
}

long* CDib::GrayValueCount()
{
	long nColors = GetMaxColorTableEntries();
	if (nColors == 0)
	{
		return NULL;
	}
	long* pGrayValueCount = new long[nColors];
	memset(pGrayValueCount, 0, nColors * sizeof(long));
	for (int i = 0; i < m_nHeight; i++)
	{
		for (int j = 0; j < m_nWidth; j++)
		{
			pGrayValueCount[*(m_pDibBits + i * m_nWidthBytes + j)]++;
		}
	}
	return pGrayValueCount;
}


int CDib::GetBitCount()
{
	return m_nBitCount;
}

void CDib::GenerateColoredBMP(int colorCode) {
	// 通过调色板生成单色图像

	// 检查图像是否创建成功
	if (IsNull() || colorCode > 3 || colorCode < 0)
	{
		// 处理错误
		return;
	}

	RGBQUAD palette[256] = { 0 };
	for (size_t i = 0; i < 256; i++)
	{
		switch (colorCode)
		{
		case 0: // 灰度
			palette[i].rgbRed = i;
			palette[i].rgbGreen = i;
			palette[i].rgbBlue = i;
			break;
		case 1: // 红色
			palette[i].rgbRed = i;
			palette[i].rgbGreen = 0;
			palette[i].rgbBlue = 0;
			break;
		case 2: // 绿色
			palette[i].rgbRed = 0;
			palette[i].rgbGreen = i;
			palette[i].rgbBlue = 0;
			break;
		case 3: // 蓝色
			palette[i].rgbRed = 0;
			palette[i].rgbGreen = 0;
			palette[i].rgbBlue = i;
			break;
		}
	}

	SetColorTable(0, 256, palette);  // 设置完整调色板
}

void CDib::LinearTransform()
{
	for (int i = 0; i < m_nHeight; i++)
	{
		for (int j = 0; j < m_nWidth; j++)
		{
			unsigned char& pixel = *(m_pDibBits + i * m_nWidthBytes + j);
			if (pixel < 100)
			{
				pixel = 0;
			}
			else if (pixel > 150)
			{
				pixel = 255;
			}
			else
			{
				pixel = static_cast<unsigned char>(100 + (pixel - 100) * (200 - 100) / (150 - 100));
			}
		}
	}
}

void CDib::BitPlane(std::vector<CDib> &planes) {
	planes.resize(m_nBitCount);

	for (int i = 0; i < m_nBitCount; ++i) {
		planes[i].Create(m_nWidth, m_nHeight, 8, 0);
		planes[i].m_nWidth = m_nWidth;
		planes[i].m_nHeight = m_nHeight;
		planes[i].m_nWidthBytes = abs(planes[i].GetPitch());
		planes[i].m_nBitCount = 8;
		if (planes[i].IsIndexed())
		{
			int nColors = GetMaxColorTableEntries();
			if (nColors > 0)
			{
				RGBQUAD* pal{};
				pal = new RGBQUAD[nColors];
				GetColorTable(0, nColors, pal);
				planes[i].SetColorTable(0, nColors, pal);
				delete[] pal;
			}
		}
		planes[i].m_pDibBits = (unsigned char*)planes[i].GetBits() + (planes[i].m_nHeight - 1) * planes[i].GetPitch();		
	}

	for (int i = 0; i < m_nHeight; ++i) {
		for (int j = 0; j < m_nWidth; ++j) {
			unsigned char& pixel = *(m_pDibBits + i * m_nWidthBytes + j);
			for (int k = 0; k < 8; ++k) {
				unsigned char bit = (pixel >> k) & 1;
				planes[k].m_pDibBits[i * planes[k].m_nWidthBytes + j] = bit * 255;
			}
		}
	}
}

void CDib::HistogramBalance()
{
	long* pGrayValueCount = GrayValueCount();
	long nColors = GetMaxColorTableEntries();
	if (nColors == 0)
	{
		return;
	}
	long* pGrayValueCountSum = new long[nColors];
	memset(pGrayValueCountSum, 0, nColors * sizeof(long));
	pGrayValueCountSum[0] = pGrayValueCount[0];
	for (int i = 1; i < nColors; i++)
	{
		pGrayValueCountSum[i] = pGrayValueCountSum[i - 1] + pGrayValueCount[i];
	}

	for (int i = 0; i < m_nHeight; i++)
	{
		for (int j = 0; j < m_nWidth; j++)
		{
			*(m_pDibBits + i * m_nWidthBytes + j) = static_cast<unsigned char>(255 * pGrayValueCountSum[*(m_pDibBits + i * m_nWidthBytes + j)] / pGrayValueCountSum[nColors - 1]);
		}
	}
	delete[] pGrayValueCount;
	delete[] pGrayValueCountSum;
}

void CDib::Kerneling(const int * kernel)
{
	// 使用Smoothing_Kernel对图像进行平滑处理
	unsigned char* pDibBits = new unsigned char[m_nHeight * m_nWidthBytes];
	memcpy(pDibBits, m_pDibBits, m_nHeight * m_nWidthBytes);
	for (int i = 1; i < m_nHeight - 1; i++)
	{
		for (int j = 1; j < m_nWidth - 1; j++)
		{
			int sum = 0;
			for (int k = -1; k <= 1; k++)
			{
				for (int l = -1; l <= 1; l++)
				{
					sum += *(pDibBits + (i + k) * m_nWidthBytes + j + l) * kernel[(k + 1) * 3 + l + 1];
				}
			}
			*(m_pDibBits + i * m_nWidthBytes + j) = sum / 9;
		}
	}
}
