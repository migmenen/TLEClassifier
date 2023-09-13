#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

int RandomNum(int min, int max)
{
	return min + rand() % (max - min);
}

int main()
{
	int cases = 100000;
	cout << "Input number of cases per lenght ( 3 lengths considered .3, .6 and .9): ";
	cin >> cases;
	cout << endl;

	if (cases < 1) 
	{
		cout << "Wrong cases input, needs to be greater than 0" << endl;
		return 1;
	}

	int length = 50;
	ofstream file;
	file.open("casos_33%.txt");

	int len = length *.3f;
	for (int i = 0; i < cases; i++)
	{
        file << len << endl;
		for (int k = 0; k < len; k++)
		{
			for(int w = 0; w < len; w++)
            {
                if(k==w) file << "1 ";
                else file << "0 ";
            }
            file << endl;
		}
	}
	file << "0";
	file.close();
	file.open("casos_66%.txt");

	len = length * .6f;
	for (int i = 0; i < cases; i++)
	{
        file << len << endl;
		for (int k = 0; k < len; k++)
		{
			for(int w = 0; w < len; w++)
            {
                if(k==w) file << "1 ";
                else file << "0 ";
            }
            file << endl;
		}
	}
	file << "0";
	file.close();
	file.open("casos_99%.txt");

	len = length * .9f;
	for (int i = 0; i < cases; i++)
	{
        file << len << endl;
		for (int k = 0; k < len; k++)
		{
			for(int w = 0; w < len; w++)
            {
                if(k==w) file << "1 ";
                else file << "0 ";
            }
            file << endl;
		}
	}
	file << "0";
	file.close();
}