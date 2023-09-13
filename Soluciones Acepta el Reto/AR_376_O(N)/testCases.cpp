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

	int length = 1000;
	ofstream file;
	file.open("casos_33%.txt");

	int len = length *.3f;
	int min = len * 0.95;
	int max = len * 1.05;
	for (int i = 0; i < cases; i++)
	{
		int num = randomNumbers ? RandomNum(min, max) : len;
		file << num << endl;

		int rnd = RandomNum(1, 1000);
		file << rnd;
		for (int j = 1; j < num; j++)
		{
			rnd = RandomNum(1, 1000);
			file << " " << rnd;
		}
		file << endl;
	}
	file << "0";
	file.close();
	file.open("casos_66%.txt");

	len = length * .6f;
	min = len * 0.95;
	max = len * 1.05;
	for (int i = 0; i < cases; i++)
	{
		int num = randomNumbers ? RandomNum(min, max) : len;
		file << num << endl;

		int rnd = RandomNum(1, 1000);
		file << rnd;
		for (int j = 1; j < num; j++)
		{
			rnd = RandomNum(1, 1000);
			file << " " << rnd;
		}
		file << endl;
	}
	file << "0";
	file.close();
	file.open("casos_99%.txt");

	len = length * .9f;
	min = len * 0.95;
	max = len * 1.05;
	for (int i = 0; i < cases; i++)
	{
		int num = randomNumbers ? RandomNum(min, max) : len;
		file << num << endl;

		int rnd = RandomNum(1, 1000);
		file << rnd;
		for (int j = 1; j < num; j++)
		{
			rnd = RandomNum(1, 1000);
			file << " " << rnd;
		}
		file << endl;
	}
	file << "0";
	file.close();
}