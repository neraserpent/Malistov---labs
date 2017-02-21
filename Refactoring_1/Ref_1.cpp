//#include "stdafx.h"
#include <windows.h>
#include <fstream>
#include <iostream>
#include <vector>
//#include "action_module.h"
#include <memory>

//header с интерфейсом библиотечного класса
#include "../ProcLib/ProcLib.h"

//указатель на функцию типа CreateBitmapFixerFunc
CreateProcLibIOFunc CreateProcLibIO;
CreateProcLibProcessFunc CreateProcLibProcess;

struct ReleasePointer
{
	template<class T>
	void operator()(T *pointer)
	{
		pointer->Release();
	}
};

int main()
{
	//std::ifstream input("H:\\Малистов\\Refactoring_1\\in.bmp", std::ios::binary);
	/*/no smart ptrs
	IProcLibIO* image_io = nullptr; //указатель на интерфейс, получаемый из dll
	IProcLibProcess* image_operate = nullptr;
	HINSTANCE dll_handle = 0; //handle подгружаемой dll

	try
	{
		//Загрузка dll
		dll_handle = LoadLibraryA("H:\\Malistov\\Refactoring_1\\Debug\\ProcLib.dll");

		if (!dll_handle)
		{
			printf("exc 1");
			throw std::exception();
		}

		//получение указателя на функцию, создающую реализацию интерфейса
		CreateProcLibIO = (CreateProcLibIOFunc)GetProcAddress(dll_handle, "CreateProcLibIO");
		CreateProcLibProcess = (CreateProcLibProcessFunc)GetProcAddress(dll_handle, "CreateProcLibProcess");

		if ((!CreateProcLibIO) || (!CreateProcLibProcess))
		{
			printf("exc 2");
			throw std::exception();
		}

		//создание реализации интерфейса через функцию, полученную из dll. Вызов new происходит на стороне dll
		image_io = CreateProcLibIO();
		image_operate = CreateProcLibProcess();

		if ((!image_io) || (!image_operate))
		{
			printf("exc 3");
			throw std::exception();
		}

		IContainer *container1, *container2;

		//далее открытие файла, получение порогов, исправление гистограммы и сохранение результата
		container1 = image_io->Open("H:\\Malistov\\Refactoring_1\\in.bmp");

		container2 = image_operate->ProcessPicture(container1);

		if (!image_io->Save("H:\\Malistov\\Refactoring_1\\out.bmp", container2))
		{
			printf("exc 4");
			throw std::exception();
		}

		//освобождение ресурсов
		image_io->Release(); //вызов delete происходит на стороне dll
		image_operate->Release();
		container1->Release();
		container2->Release();
		FreeLibrary(dll_handle);
		std::cout << "Done" << std::endl;
	}
	catch (const std::exception) 
	{
		if (image_io)
			image_io->Release();

		if (image_operate)
			image_operate->Release();

		if (dll_handle)
			FreeLibrary(dll_handle);

		std::cout << "exception have been thrown" << std::endl;
		system("PAUSE");
		return 1;
	}*/

	//smart ptrs
	HINSTANCE dll_handle = 0;

	try
	{		
		dll_handle = LoadLibraryA("H:\\Malistov\\Refactoring_1\\Debug\\ProcLib.dll");

		if (!dll_handle)
			throw std::exception();

		//получение указателя на функцию, создающую реализацию интерфейса
		CreateProcLibIO = (CreateProcLibIOFunc) GetProcAddress(dll_handle, "CreateProcLibIO");
		CreateProcLibProcess = (CreateProcLibProcessFunc)GetProcAddress(dll_handle, "CreateProcLibProcess");

		if ((!CreateProcLibIO) || (!CreateProcLibProcess))
			throw std::exception();

		//создание реализации интерфейса через функцию, полученную из dll. Вызов new происходит на стороне dll
		std::shared_ptr<IProcLibIO> image_io = std::shared_ptr<IProcLibIO>(CreateProcLibIO(), ReleasePointer());
		std::shared_ptr<IProcLibProcess> image_operate = std::shared_ptr<IProcLibProcess>
			(CreateProcLibProcess(), ReleasePointer());

		if ((!image_io) || (!image_operate))
			throw std::exception();

		//далее открытие файла, получение порогов, исправление гистограммы и сохранение результата
		std::shared_ptr<IContainer> container1 = std::shared_ptr<IContainer>(image_io->
			Open("H:\\Malistov\\Refactoring_1\\in.bmp"), ReleasePointer());

		std::shared_ptr<IContainer> container2 = 
			std::shared_ptr<IContainer>(image_operate->ProcessPicture(container1.get()), ReleasePointer());

		if (!image_io->Save("H:\\Malistov\\Refactoring_1\\out.bmp", container2.get()))
			throw std::exception();

		std :: cout << "Done" << std :: endl;
	}
	catch (const std::exception)
	{
		std::cout << "exception have been thrown" << std::endl;
		system("PAUSE");
		return 1;
	}


	system("PAUSE");

    return 0;
}

