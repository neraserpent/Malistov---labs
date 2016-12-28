#include "ProcessingLibImplementation.h"

//�������, ��������� � ������������ ���������� ���������
//extern "C" __declspec(dllexport) ����� ����� ������� ����� ���� ����� ����� GetProcAddress � �������� ���������
extern "C" __declspec(dllexport) IProcLibIO* CreateProcLibIO()
{
	return new ProcLibIO();
}

extern "C" __declspec(dllexport) IProcLibProcess* CreateProcLibProcess()
{
	return new ProcLibProcess();
}