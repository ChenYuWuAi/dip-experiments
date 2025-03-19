#include "pch.h"
#include "Dib.h"
#include "math.h"
#include <algorithm>
#include <thread>
#include <future>
#include <chrono>
#include <vector>
#include <iostream>
#include <windows.h>


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

void CDib::BitPlane(std::vector<CDib>& planes) {
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
#define LIMIT_MIN_MAX(value) (value < 0 ? 0 : (value > 255 ? 255 : value))

void CDib::Kerneling(const int* kernel, int sum_divide, bool normalize)
{
	unsigned char* pDibBits = new unsigned char[m_nHeight * m_nWidthBytes];
	memcpy(pDibBits, m_pDibBits, m_nHeight * m_nWidthBytes);
	int* pDibBits_minmax = new int[m_nHeight * m_nWidthBytes];
	for (int i = 1; i < m_nHeight - 1; i++)
	{
		for (int j = 1; j < m_nWidth - 1; j++)
		{
			pDibBits_minmax[i * m_nWidthBytes + j] = pDibBits[i * m_nWidthBytes + j];
		}
	}

	int minVal = INT_MAX, maxVal = INT_MIN;

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
			sum /= sum_divide;
			minVal = min(minVal, sum);
			maxVal = max(maxVal, sum);
			*(pDibBits_minmax + i * m_nWidthBytes + j) = sum;
		}
	}

	for (int i = 1; i < m_nHeight - 1; i++)
	{
		for (int j = 1; j < m_nWidth - 1; j++)
		{
			int value = *(pDibBits_minmax + i * m_nWidthBytes + j);
			if (normalize)
				value = 255 * (value - minVal) / (maxVal - minVal);
			*(m_pDibBits + i * m_nWidthBytes + j) = LIMIT_MIN_MAX(value);
		}
	}

	delete[] pDibBits;
	delete[] pDibBits_minmax;
}

// 生成分离的高斯核，size表示核的大小，sigma表示标准差，生成size * 2的核，分别表示x和y方向的核
std::vector<float> CDib::GenerateGaussianKernel(int size, float sigma) {
	std::vector<float> kernel(size * 2);
	float sum = 0.0f;
	int halfSize = size / 2;
	float sigma2 = 2 * sigma * sigma;

	// x方向的核
	for (int i = 0; i < size; i++) {
		kernel[i] = exp(-pow(i - halfSize, 2) / sigma2);
		sum += kernel[i];
	}
	// y方向的核
	for (int i = 0; i < size; i++) {
		kernel[i + size] = exp(-pow(i - halfSize, 2) / sigma2);
		sum += kernel[i + size];
	}

	// 归一化
	for (int i = 0; i < size; i++) {
		kernel[i] /= sum;
	}

	return kernel;
}

void CDib::SeparableConvolution(const std::vector<float>& kernel)
{
	// 指针
	unsigned char* pDibBits = m_pDibBits;
	if (!pDibBits)
		return;

	auto start_time = std::chrono::high_resolution_clock::now();
	int width = GetWidth();
	int height = GetHeight();
	int kernelSize = kernel.size() / 2;
	int halfKernelSize = kernelSize / 2;
	int minVal = INT_MAX;
	int maxVal = INT_MIN;

	// 临时存储
	std::vector<int> temp(GetHeight() * GetWidth());
	int* pDibBits_minmax = new int[height * width];

	// x方向卷积
	int startk, endk;
	for (int i = 0; i < height; i++) {
		float sum = 0.0f;
		for (int j = 0; j < width; j++) {
			sum = 0.0f;
			for (int k = -halfKernelSize; k <= halfKernelSize; k++) {
				int x = j + k;
				if (x < 0 || x >= width) continue;
				sum += pDibBits[i * width + x] * kernel[k + halfKernelSize];
			}
			temp[i * width + j] = static_cast<int>(sum);
		}
	}
	// y方向卷积
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			float sum = 0.0f;
			for (int k = -halfKernelSize; k <= halfKernelSize; k++) {
				int y = i + k;
				if (y < 0 || y >= width) continue;
				sum += temp[y * width + j] * kernel[k + halfKernelSize];
			}
			pDibBits_minmax[i * width + j] = static_cast<int>(sum);
			minVal = min(sum, minVal);
			maxVal = max(sum, maxVal);
		}
	}

	// 最值归一化
	for (int i = 1; i < height - 1; i++)
	{
		for (int j = 1; j < width - 1; j++)
		{
			int value = *(pDibBits_minmax + i * width + j);
			value = 255 * (value - minVal) / (maxVal - minVal);
			*(pDibBits + i * width + j) = LIMIT_MIN_MAX(value);
		}
	}

	delete[] pDibBits_minmax;


	auto end_time = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

	char string_debug[30];
	sprintf_s(string_debug, "单线程: %d ms", duration);
	OutputDebugStringA(string_debug);
}

void CDib::MultiThreads_SeparableConvolution(const std::vector<float>& kernel)
{
	auto start_time = std::chrono::high_resolution_clock::now();

	// 获取线程数
	int num_threads = std::thread::hardware_concurrency();
	if (num_threads <= 0) num_threads = 4; // 默认4线程

	unsigned char* pDibBits = m_pDibBits;
	if (!pDibBits) return;

	int width = GetWidth();
	int height = GetHeight();
	int kernelSize = kernel.size() / 2;
	int halfKernelSize = kernelSize / 2;
	int minVal = INT_MAX;
	int maxVal = INT_MIN;

	std::vector<int> temp(height * width);
	int* pDibBits_minmax = new int[height * width];

	// X方向卷积（多线程）
	std::vector<std::thread> threads_x;
	int block_size_x = (height + num_threads - 1) / num_threads;
	for (int t = 0; t < num_threads; ++t) {
		int start_row = t * block_size_x;
		int end_row = min((t + 1) * block_size_x, height);
		threads_x.emplace_back([=, &temp]() {
			for (int i = start_row; i < end_row; ++i) {
				for (int j = 0; j < width; ++j) {
					float sum = 0.0f;
					for (int k = -halfKernelSize; k <= halfKernelSize; ++k) {
						int x = j + k;
						if (x < 0 || x >= width) continue;
						sum += pDibBits[i * width + x] * kernel[k + halfKernelSize];
					}
					temp[i * width + j] = static_cast<int>(sum);
				}
			}
			});
	}
	for (auto& t : threads_x) t.join();

	// Y方向卷积（多线程）
	std::vector<std::thread> threads_y;
	int block_size_y = (height + num_threads - 1) / num_threads;
	for (int t = 0; t < num_threads; ++t) {
		int start_row = t * block_size_y;
		int end_row = min((t + 1) * block_size_y, height);
		threads_y.emplace_back([=, &temp]() {
			for (int i = start_row; i < end_row; ++i) {
				for (int j = 0; j < width; ++j) {
					float sum = 0.0f;
					for (int k = -halfKernelSize; k <= halfKernelSize; ++k) {
						int y = i + k;
						if (y < 0 || y >= width) continue;
						sum += temp[y * width + j] * kernel[k + halfKernelSize];
					}
					pDibBits_minmax[i * width + j] = static_cast<int>(sum);
				}
			}
			});
	}
	for (auto& t : threads_y) t.join();

	// 计算min和max值（单线程）
	minVal = INT_MAX;
	maxVal = INT_MIN;
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			int val = pDibBits_minmax[i * width + j];
			if (val < minVal) minVal = val;
			if (val > maxVal) maxVal = val;
		}
	}

	// 归一化（多线程）
	std::vector<std::thread> threads_norm;
	int block_size_norm = (height + num_threads - 1) / num_threads;
	for (int t = 0; t < num_threads; ++t) {
		int start_row = t * block_size_norm + 1;
		int end_row = min((t + 1) * block_size_norm + 1, height - 1);
		threads_norm.emplace_back([=]() {
			for (int i = start_row; i < end_row; ++i) {
				for (int j = 1; j < width - 1; ++j) {
					int value = pDibBits_minmax[i * width + j];
					value = 255 * (value - minVal) / static_cast<float>(maxVal - minVal);
					pDibBits[i * width + j] = LIMIT_MIN_MAX(value);
				}
			}
			});
	}
	for (auto& t : threads_norm) t.join();

	delete[] pDibBits_minmax;

	auto end_time = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

	char string_debug[30];
	sprintf_s(string_debug, "多线程: %d ms", duration);
	OutputDebugStringA(string_debug);
}

void CDib::SobelTransform(CDib& gradientMagnitude, CDib& gradientAngle)
{
	int width = GetWidth();
	int height = GetHeight();

	CDib* pDib[] = { &gradientMagnitude, &gradientAngle };

	for (size_t i = 0; i < 2; i++)
	{
		pDib[i]->Create(width, height, 8, 0);

		pDib[i]->m_nWidth = m_nWidth;
		pDib[i]->m_nHeight = m_nHeight;
		pDib[i]->m_nWidthBytes = abs(pDib[i]->GetPitch());
		pDib[i]->m_nBitCount = 8;
		if (pDib[i]->IsIndexed())
		{
			int nColors = GetMaxColorTableEntries();
			if (nColors > 0)
			{
				RGBQUAD* pal{};
				pal = new RGBQUAD[nColors];
				GetColorTable(0, nColors, pal);
				pDib[i]->SetColorTable(0, nColors, pal);
				delete[] pal;
			}
		}
		pDib[i]->m_pDibBits = (unsigned char*)pDib[i]->GetBits() + (pDib[i]->m_nHeight - 1) * pDib[i]->GetPitch();
	}


	int gx[3][3] = {
		{-1, 0, 1},
		{-2, 0, 2},
		{-1, 0, 1}
	};

	int gy[3][3] = {
		{-1, -2, -1},
		{0, 0, 0},
		{1, 2, 1}
	};

	unsigned char* p_DibBits = m_pDibBits;

	for (int y = 1; y < height - 1; ++y) {
		for (int x = 1; x < width - 1; ++x) {
			int sumX = 0;
			int sumY = 0;

			for (int ky = -1; ky <= 1; ++ky) {
				for (int kx = -1; kx <= 1; ++kx) {
					int pixel = static_cast<int>(*(p_DibBits + (y + ky) * width + x + kx));
					sumX += pixel * gx[ky + 1][kx + 1];
					sumY += pixel * gy[ky + 1][kx + 1];
				}
			}

			int magnitude = static_cast<int>(sqrt(sumX * sumX + sumY * sumY));
			int angle = static_cast<int>(atan2(sumY, sumX) * 180 / M_PI);

			pDib[0]->m_pDibBits[y * width + x] = LIMIT_MIN_MAX(magnitude);
			pDib[1]->m_pDibBits[y * width + x] = LIMIT_MIN_MAX(angle);
		}
	}
}

void CDib::CLAHE()
{
	// 仅处理8位灰度图
	if (m_pDibBits == NULL || m_nBitCount != 8)
		return;

	const int tileSize = 128;
	int numTilesX = (m_nWidth + tileSize - 1) / tileSize;
	int numTilesY = (m_nHeight + tileSize - 1) / tileSize;
	const int numBins = 256;
	const int clipLimit = 256;

	// 每个小块对应一个256元素的LUT
	std::vector<std::vector<int>> LUT(numTilesX * numTilesY, std::vector<int>(numBins, 0));

	// lambda：计算某个小块的LUT
	auto computeLUT = [&](int tx, int ty)
		{
			int tileIndex = ty * numTilesX + tx;
			std::vector<long> hist(numBins, 0);
			int xStart = tx * tileSize, yStart = ty * tileSize;
			int xEnd = min(xStart + tileSize, m_nWidth);
			int yEnd = min(yStart + tileSize, m_nHeight);
			int tilePixelCount = (xEnd - xStart) * (yEnd - yStart);

			// 计算直方图
			for (int y = yStart; y < yEnd; ++y)
			{
				for (int x = xStart; x < xEnd; ++x)
				{
					hist[m_pDibBits[y * m_nWidthBytes + x]]++;
				}
			}

			// 剪裁直方图
			int excess = 0;
			for (int i = 0; i < numBins; ++i)
			{
				if (hist[i] > clipLimit)
				{
					excess += hist[i] - clipLimit;
					hist[i] = clipLimit;
				}
			}
			int inc = excess / numBins;
			int rem = excess % numBins;
			for (int i = 0; i < numBins; ++i)
				hist[i] += inc;
			for (int i = 0; i < rem; ++i)
				hist[i]++;

			// 计算累积分布直方图
			std::vector<int> cumHist(numBins, 0);
			cumHist[0] = hist[0];
			for (int i = 1; i < numBins; ++i)
			{
				cumHist[i] = cumHist[i - 1] + hist[i];
			}
			// 归一化映射到[0,255]
			for (int i = 0; i < numBins; ++i)
			{
				LUT[tileIndex][i] = (cumHist[i] * 255) / tilePixelCount;
			}
		};

	// 计算所有小块的LUT
	for (int ty = 0; ty < numTilesY; ++ty)
		for (int tx = 0; tx < numTilesX; ++tx)
			computeLUT(tx, ty);

	// 新缓冲区存储映射后的数据，防止直接覆盖原图数据
	std::vector<unsigned char> newData(m_nHeight * m_nWidthBytes, 0);

	// 对每个像素进行双线性插值映射
	for (int y = 0; y < m_nHeight; ++y)
	{
		// 计算该行所在的垂直小块位置和相对权重
		int ty = y / tileSize;
		double y_ratio = double(y - ty * tileSize) / tileSize;
		// 如果处于最下边缘，直接归入最后一个小块
		if (ty >= numTilesY - 1)
		{
			ty = numTilesY - 1;
			y_ratio = 0.0;
		}
		int ty_next = min(ty + 1, numTilesY - 1);

		for (int x = 0; x < m_nWidth; ++x)
		{
			// 计算该列所在的小块位置和相对权重
			int tx = x / tileSize;
			double x_ratio = double(x - tx * tileSize) / tileSize;
			if (tx >= numTilesX - 1)
			{
				tx = numTilesX - 1;
				x_ratio = 0.0;
			}
			int tx_next = min(tx + 1, numTilesX - 1);

			int pixelValue = m_pDibBits[y * m_nWidthBytes + x];

			// 获取四个邻近小块中的映射值
			int LUT_tl = LUT[ty * numTilesX + tx][pixelValue];
			int LUT_tr = LUT[ty * numTilesX + tx_next][pixelValue];
			int LUT_bl = LUT[ty_next * numTilesX + tx][pixelValue];
			int LUT_br = LUT[ty_next * numTilesX + tx_next][pixelValue];

			// 双线性插值计算新像素值
			double top = LUT_tl * (1.0 - x_ratio) + LUT_tr * x_ratio;
			double bottom = LUT_bl * (1.0 - x_ratio) + LUT_br * x_ratio;
			int newVal = int(top * (1.0 - y_ratio) + bottom * y_ratio + 0.5);
			newData[y * m_nWidthBytes + x] = (unsigned char)newVal;
		}
	}

	memcpy(m_pDibBits, newData.data(), m_nHeight * m_nWidthBytes);
}