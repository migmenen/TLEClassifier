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
	char in;
	cout << "Use random numbers? (if not same num used) y/n:";
	cin >> in;
	bool randomNumbers = in == 'y';

	if (cases < 1) 
	{
		cout << "Wrong cases input, needs to be greater than 0" << endl;
		return 1;
	}

	int length = 10;
	ofstream file;
	file.open("casos_33%.txt");
	file << cases << endl;

	int len = length *.3f;
	for (int i = 0; i < cases; i++)
	{
		for (int k = 0; k < len; k++)
		{
			int num = randomNumbers ? RandomNum(0, 9) : len;
			file << num;
		}
		file << endl;
	}
	file << "-1";
	file.close();
	file.open("casos_66%.txt");
	file << cases << endl;

	len = length * .6f;
	for (int i = 0; i < cases; i++)
	{
		for (int k = 0; k < len; k++)
		{
			int num = randomNumbers ? RandomNum(0, 9) : len;
			file << num;
		}
		file << endl;
	}
	file << "-1";
	file.close();
	file.open("casos_99%.txt");
	file << cases << endl;

	len = length * .9f;
	for (int i = 0; i < cases; i++)
	{
		for (int k = 0; k < len; k++)
		{
			int num = randomNumbers ? RandomNum(0, 9) : len;
			file << num;
		}
		file << endl;
	}
	file << "-1";
	file.close();
}