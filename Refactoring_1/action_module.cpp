#include <windows.h>
#include <fstream>
#include <iostream>
#include <vector>
const int kNumColors = 3;
const int kRed = 2;
const int kGreen = 1;
const int kBlue = 0;

struct HistogrammPixels
{
	unsigned int values[3];
};
void process_picture(BITMAPINFO *bmp_info)
{
	const int kWidth = bmp_info->bmiHeader.biWidth;
	const int kHeight = bmp_info->bmiHeader.biHeight;

	std::vector<HistogrammPixels> histo(256); //2^(sizeof(char)*8)


	RGBTRIPLE *color_ptr = reinterpret_cast<RGBTRIPLE *>(bmp_info->bmiColors);

	for (int x = 0; x < kWidth; x++)
	{
		for (int y = 0; y < kHeight; y++)
		{
			RGBTRIPLE *pixel = color_ptr + (kWidth*y + x);

			++histo[pixel->rgbtRed].values[kRed];
			++histo[pixel->rgbtGreen].values[kGreen];
			++histo[pixel->rgbtBlue].values[kBlue];
		}
	}

	const unsigned int kAlphaPixels = kWidth * kHeight * 0.05;
		
	unsigned int alpha[kNumColors];
	for (unsigned int i = 0; i < kNumColors; ++i)
	{
		unsigned int sum = 0;

		int j = 0;
		for (; j < histo.size(); ++j)
		{
			sum += histo[j].values[i];
			
			if (sum >= kAlphaPixels)
				break;
		}
		alpha[i] = j;
	}

	unsigned int beta[kNumColors];
	for (unsigned int i = 0; i < kNumColors; ++i)
	{
		unsigned int sum = 0;

		int j = histo.size() - 1;
		for (; j >= 0 ; --j)
		{
			sum += histo[j].values[i];

			if (sum >= kAlphaPixels)
				break;
		}
		beta[i] = j;
	}


	std::cout << "alpha[R] = " << alpha[kRed] << std::endl;
	std::cout << "beta[R] = " << beta[kRed] << std::endl;
	std::cout << std::endl;

	std::cout << "alpha[G] = " << alpha[kGreen] << std::endl;
	std::cout << "beta[G] = " << beta[kGreen] << std::endl;
	std::cout << std::endl;

	std::cout << "alpha[B] = " << alpha[kBlue] << std::endl;
	std::cout << "beta[B] = " << beta[kBlue] << std::endl;
	std::cout << std::endl;

	//Восстановление. Передается color_ptr и kWidth,kHeight,kNumColors
	for (int x = 0; x < kWidth; x++)
	{
		for (int y = 0; y < kHeight; y++)
		{
			RGBTRIPLE *pixel = color_ptr + (kWidth*y + x);
			
			for (int i = 0; i < kNumColors; ++i)
			{
				BYTE* color = reinterpret_cast<BYTE*>(pixel) + i;
				float val = (float(*color) - float(alpha[i])) / (float(beta[i]) - float(alpha[i]));
				val = val < 0.f ? 0.f : val > 1.f ? 255.f : val * 255.f;
				*color = static_cast<BYTE>(val);
			}

		}
	}
	return ;
}