#include <iostream>
#include <Windows.h>
using namespace std;


typedef HANDLE(*LPFNDLLinit)(char *, char *);

class Project
{
public:
	Project(char *directory = "D:\\C\\Visual Studio 2015\\Projects\\SPO_LR5\\Catalog\\", char *outputFileName = "output.txt");
	void START();
private:
	char directory[MAX_PATH];
	char *outputFileName;
};