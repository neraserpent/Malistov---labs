//–еализаци€ всего, что наобещали в объ€влении интерфейсов - ProcessingLib.h
#include <vector>
#include <Windows.h>
#include <wingdi.h>
#include "ProcessingLib.h"
struct HistogrammPixels
{
	unsigned int values[3];
};

class Container: public IContainer
{
public:
	void Load(IContainer* container);
	void Create(const char* header, const int fileSize);
	int GetRGBPoint(const int x, const int y, const int c);
	void SetRGBPoint(const int x, const int y, const int c, const int val);
	char* GetContent();
	int GetSize();
	bool IsError();
	int GetWidth();
	int GetHeight();
	void Release();

	Container();

	char* m_buffer;
	unsigned int width, height, size;
protected:
	virtual ~Container();
};

class ProcLibIO : public IProcLibIO
{
public:
	ProcLibIO();
	IContainer* Open(const char* file);
	bool Save(const char* file, IContainer* container);
	void Release();
protected:
	virtual ~ProcLibIO();
};

class ProcLibProcess : public IProcLibProcess
{
public:
	ProcLibProcess();
	IContainer* ProcessPicture(IContainer* container);
	void CreateHistogram(IContainer* container);
	void FindAlphaBeta(IContainer* container);
	Container* NormalizeHistogram(IContainer* container);
	void Release();
protected:
	virtual ~ProcLibProcess();

	AlphaBeta ab_values;
	std::vector<HistogrammPixels> histo(256);//256
};