// ProcLib.h

#pragma once
#include <vector>
const int kNumColors = 3;
const int kRed = 2;
const int kGreen = 1;
const int kBlue = 0;

struct AlphaBeta
{
	unsigned int alpha[kNumColors];
	unsigned int beta[kNumColors];
};
//Интерфейсы
class IContainer
{
public:
	virtual void Load(IContainer* container) = 0;
	virtual void Create(const char* header, const int fileSize) = 0;
	virtual int GetRGBPoint(const int x, const int y, const int c) = 0;
	virtual void SetRGBPoint(const int x, const int y, const int c, const int val) = 0;
	virtual char* GetContent() = 0;
	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
	virtual int GetSize() = 0;
	virtual bool IsError() = 0;
	virtual void Release() = 0;
protected:
	virtual ~IContainer() = 0 {};
};
//IO можно было в другой либе, но так вроде тоже приемлемо
class IProcLibIO
{
public:
	virtual IContainer* Open(const char* file) = 0;
	virtual bool Save(const char* file, IContainer* container) = 0;
	virtual void Release() = 0;
protected:
	virtual ~IProcLibIO() = 0 {};
};
//основная часть - обработка изображения
class IProcLibProcess
{
public:
	virtual IContainer* ProcessPicture(IContainer* container) = 0;
	virtual void Release() = 0;
protected:
	virtual ~IProcLibProcess() = 0 {};
};

//возвращение указателя на интерфейс
typedef IProcLibIO* (*CreateProcLibIOFunc)();
typedef IProcLibProcess* (*CreateProcLibProcessFunc)();


/*using namespace System;

namespace ProcessingLib {

	public ref class Class1
	{
		// TODO: здесь следует добавить свои методы для этого класса.
	};
}*/
