//Тела функций имплементации интерфейсов
#include "ProcessingLibImplementation.h"
#include "ProcessingLib.h"
#include <iostream>
#include <fstream>

#define BITMAPHEADERSIZE 54

ProcLibIO::ProcLibIO() {}
ProcLibIO::~ProcLibIO() {}
Container::Container() {}
Container::~Container() {}
ProcLibProcess::ProcLibProcess() {}
ProcLibProcess::~ProcLibProcess() {}

//функции контейнера
void Container::Load(IContainer* container)
{
	m_buffer = new char[container->GetSize()];
	for (int i = 0; i < BITMAPHEADERSIZE; i++)
		m_buffer[i] = container->GetContent()[i];
	width = container->GetWidth();
	height = container->GetHeight();
	size = container->GetSize();
}

void Container::Create(const char* file, const int fileSize)
{
	m_buffer = new char[fileSize];
	for (int i = 0; i < fileSize; i++)
		m_buffer[i] = file[i];
	width = reinterpret_cast<BITMAPINFO*>(m_buffer + sizeof(BITMAPFILEHEADER))->bmiHeader.biWidth;
	height = reinterpret_cast<BITMAPINFO*>(m_buffer + sizeof(BITMAPFILEHEADER))->bmiHeader.biHeight;
	size = fileSize;
}

int Container::GetRGBPoint(const int x, const int y, const int c)
{
	BITMAPINFO * bmph = reinterpret_cast<BITMAPINFO*>(m_buffer + sizeof(BITMAPFILEHEADER));
	unsigned char* rein = reinterpret_cast<unsigned char*>(&(bmph->bmiColors));
	int sizeCon = GetWidth();
	int bpl = bmph->bmiHeader.biSizeImage / bmph->bmiHeader.biHeight;
	int bpp = bmph->bmiHeader.biBitCount / 8;
	return rein[y * bpl + x * bpp + c];
}

void Container::SetRGBPoint(const int x, const int y, const int c, int in)
{
	int bpp = 3;
	int bpl = bpp * GetWidth();
	m_buffer[BITMAPHEADERSIZE + y * bpl + x * bpp + c] = in;
}

int Container::GetWidth()
{
	return width;
}
int Container::GetHeight()
{
	return height;
}
char* Container::GetContent()
{
	return m_buffer;
}
int Container::GetSize()
{
	return size;
}
void Container::Release()
{
	delete[] m_buffer;
	delete this;
}
bool Container::IsError()
{
	if (!m_buffer)
		return true;
	return false;
}

//функции IO-интерфейса
IContainer* ProcLibIO::Open(const char* file)
{
	std::ifstream input(file, std::ios::binary);

	if (!input)
	{
		std::cout << "ProcLibIO::Open() error: Cannot open file " << file << std::endl;
		return NULL;
	}

	std::vector<char> buffer;
	buffer.assign((
		std::istreambuf_iterator<char>(input)),
		(std::istreambuf_iterator<char>()));

	////////////////////////////////////////////////////////
	BITMAPFILEHEADER *file_header = reinterpret_cast<BITMAPFILEHEADER*>(buffer.data());

	if (buffer.size() < sizeof(BITMAPFILEHEADER) || file_header->bfType != 0x4D42)
	{
		std::cout << "Wrong input file" << std::endl;
		system("PAUSE");
		return false;
	}
	BITMAPINFO *bmp_info = reinterpret_cast<BITMAPINFO*>(buffer.data() + sizeof(BITMAPFILEHEADER));

	std::cout << "bmp_info->bmiHeader.biSize				" <<	bmp_info->bmiHeader.biSize			<< std::endl;
	std::cout << "bmp_info->bmiHeader.biWidth				" <<	bmp_info->bmiHeader.biWidth			<< std::endl;
	std::cout << "bmp_info->bmiHeader.biHeight				" <<	bmp_info->bmiHeader.biHeight		<< std::endl;
	std::cout << "bmp_info->bmiHeader.biBitCount			" << bmp_info->bmiHeader.biBitCount << std::endl;
	std::cout << "bmp_info->bmiHeader.biPlanes				" <<	bmp_info->bmiHeader.biPlanes		<< std::endl;	
	std::cout << "bmp_info->bmiHeader.biCompression			" <<	bmp_info->bmiHeader.biCompression	<< std::endl;
	std::cout << "bmp_info->bmiHeader.biSizeImage			" <<	bmp_info->bmiHeader.biSizeImage		<< std::endl;
	std::cout << "bmp_info->bmiHeader.biXPelsPerMeter		" <<	bmp_info->bmiHeader.biXPelsPerMeter	<< std::endl;
	std::cout << "bmp_info->bmiHeader.biYPelsPerMeter		" <<	bmp_info->bmiHeader.biYPelsPerMeter	<< std::endl;
	std::cout << "bmp_info->bmiHeader.biClrUsed				" <<	bmp_info->bmiHeader.biClrUsed		<< std::endl;
	std::cout << "bmp_info->bmiHeader.biClrImportant		" <<	bmp_info->bmiHeader.biClrImportant	<< std::endl;
	///////////////////////////////////////////////////////////

	Container* container = new Container();
	container->Create(buffer.data(), buffer.size());
	return container;
}

bool ProcLibIO::Save(const char* file, IContainer* container)
{
	std::ofstream out(file, std::ios::binary);
	if (container->IsError() || !out)
	{
		std::cout << "ProcLibIO::Save() error" << std::endl;
		return false;
	}

	out.write(container->GetContent(), container->GetSize());
	out.close();

	return true;
}

void ProcLibIO::Release()
{
	delete this;
}

//бывший action_module.cpp разбит на требуемые отдельные действия:
//построение гистограммы, поиск граничных значений, нормализация
IContainer* ProcLibProcess::ProcessPicture(IContainer* container)
{
	CreateHistogram(container);
	FindAlphaBeta(container);
	return NormalizeHistogram(container);
}

void ProcLibProcess::CreateHistogram(IContainer* container)
{
	//histo.resize(256);

	const int kWidth = container->GetWidth();
	const int kHeight = container->GetHeight();

	for (int x = 0; x < kWidth; x++)
	{
		for (int y = 0; y < kHeight; y++)
		{
			++histo[container->GetRGBPoint(x, y, kRed)].values[kRed];
			++histo[container->GetRGBPoint(x, y, kGreen)].values[kGreen];
			++histo[container->GetRGBPoint(x, y, kBlue)].values[kBlue];
		}
	}
	return;
}

void ProcLibProcess::FindAlphaBeta(IContainer* container)
{
	const unsigned int kAlphaPixels = container->GetHeight() * container->GetWidth() * 0.05;

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
		ab_values.alpha[i] = j;
	}

	for (unsigned int i = 0; i < kNumColors; ++i)
	{
		unsigned int sum = 0;

		int j = histo.size() - 1;
		for (; j >= 0; --j)
		{
			sum += histo[j].values[i];

			if (sum >= kAlphaPixels)
				break;
		}
		ab_values.beta[i] = j;
	}

	return;
}
//восстановление/нормализация
Container* ProcLibProcess::NormalizeHistogram(IContainer* container)
{
	Container* result = new Container();
	result->Load(container);

	for (int x = 0; x < container->GetWidth(); x++)
	{
		for (int y = 0; y < container->GetHeight(); y++)
		{
			for (int cur_color = 0; cur_color < kNumColors; cur_color++)
			{
				int color = container->GetRGBComponent(x, y, cur_color); //BYTE* color = reinterpret_cast<BYTE*>(pixel) + i;
				float val = (color - float(ab_values.alpha[cur_color])) /
					(float(ab_values.beta[cur_color]) - float(ab_values.alpha[cur_color]));
				val = val < 0.f ? 0.f : val > 1.f ? 255.f : val * 255.f;
				result->SetRGBComponent(x, y, cur_color, val);
			}
		}
	}

	return result;
}

void ProcLibProcess::Release()
{
	delete this;
}