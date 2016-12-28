#include "ProcessingLibImplementation.h"

//Функция, создающая и возвращающая реализацию интефейса
//extern "C" __declspec(dllexport) нужно чтобы функцию можно было найти через GetProcAddress в основной программе
extern "C" __declspec(dllexport) IProcLibIO* CreateProcLibIO()
{
	return new ProcLibIO();
}

extern "C" __declspec(dllexport) IProcLibProcess* CreateProcLibProcess()
{
	return new ProcLibProcess();
}