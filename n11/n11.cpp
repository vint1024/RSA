// n11.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include "gmp.h"
#include <iostream>
//#include <conio.h>
#include "rsa.h"
#include <fstream>
//#pragma library ("gmp.lib");
#pragma comment (lib, "gmp.lib")

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	int i = 0;
	cout << "encoding(1) / decoding(2)  > ";
	cin >> i;
	char in[1024], out[1024], key[1024];
	if (i == 1) 
	{
		cout << "Enter file name for input  data. > "; cin >> in;
		cout << "Enter file name for output data. > "; cin >> out;
		rsa(in,out);
	}
	if (i == 2)
	{
		cout << "Enter file name for input  data. > "; cin >> in;
		cout << "Enter file name for output data. > "; cin >> out;
		cout << "Enter file name for key    data. > "; cin >> key;
		rsa(in,out,key);
	}
	if ((i>2)||(i<1))cout <<"ERROR! Command not found.";
	system("pause");
	return 0;
}

