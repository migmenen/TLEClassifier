#include <stdio.h>
#include <cmath>
#include <chrono>
#include <string>
#include <fstream>
#include <time.h>
#include <algorithm>

using namespace std;
using namespace chrono;

const int ORDER_AMOUNT = 6;//cte, logn, n, nlogn, n2, n3
const int RUNS_PER_ORDER[ORDER_AMOUNT]{ 5000, 5000, 5000, 2000, 100, 50 };
//n3 - 500runs w 1000-100 sizes => exec time: 9:34h 87%
//n3 - 250runs w 1000-500 sizes => exec time: 10h 88%

const int SIZE_RANGE_PER_TYPE[ORDER_AMOUNT][2] = {
	{700000, 100},	//cte
	{300000, 100},	//log
	{200000, 100},	//n
	{100000, 100},	//nlogn
	{10000, 100},	//n2
	{1000, 500}		//n3
};
const int PERCENTS_SIZE = 4;
//const float percents[PERCENTS_SIZE]{ 0.1f, 0.45f, 0.95f };
const float percents[PERCENTS_SIZE]{ 0.25f, 0.5f, 0.75f, 1.0f };
const int NUMBER_OF_OPERATIONS = 3;
const int NUMBER_OF_CONSTANTS_CONSIDERED = 10;//Appart from 1 ( this will increase the amount of outputs => RUNS_PER_ORDER * NUM_CTE_CONSIDERED )
const int MAX_NUMBER_OF_CONSTANTS = 10;
float CONSTANTS[MAX_NUMBER_OF_CONSTANTS]{ 0.5f, 1.69f, 2.3f, 3.75f, 5.15f, 6.9f, 7.45f, 8.34f, 9.81f, 10.0f };
enum CONSTANTTYPE { MODTIME, MODSIZE, MODTIMEANDSIZE, ALL };
const CONSTANTTYPE cteType = ALL;
enum OPERATIONDIFFICULTY { EASY, MID, HARD };
const OPERATIONDIFFICULTY dif = HARD;
const int SECTION_AMOUNT = 20;//TODO: maybe pick this number depending on the order or the size difference between min n max size and runs...
enum EXECUTION_TYPE { FULLRANDOM, SAMPLEDRANDOM };
const EXECUTION_TYPE executionType = SAMPLEDRANDOM;

void HandleConsole(int order, int cur, int total, int &curPercent)
{
	int percent = cur * 100 / total;
	if (percent == curPercent) return;
	curPercent = percent;

	system("CLS");
	for (int i = 0; i < order; i++) printf("DONE WITH ORDER: %i\n", i);
	char c = '%';
	printf("%i %c OF ORDER %i", percent, c, order);
}

int RandomSize(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

int RandomSize(int order)
{
	int max = SIZE_RANGE_PER_TYPE[order][0];
	int min = SIZE_RANGE_PER_TYPE[order][1];
	return RandomSize(min, max);
}

void CalculateSizePerSection(int order, int sizeSections, int &sizePerSection, int &runsPerSection)
{
	int max = SIZE_RANGE_PER_TYPE[order][0];
	int min = SIZE_RANGE_PER_TYPE[order][1];
	int runsForOrder = RUNS_PER_ORDER[order];
	runsPerSection = runsForOrder / sizeSections;
	sizePerSection = (max - min) / sizeSections;
}

void operations(int amount)
{
	while (amount > 0)
	{
		switch (dif)
		{
			case EASY:
				9999999 + 999999;
				break;
			case MID:
				9999999 * 999999;
				break;
			case HARD:
				int rnd = 10 + rand() % 10;
				log(rnd);
				break;
		}
		amount--;
	}
}

#pragma region Order Methods

void constant(int size, int numOperations)
{
	operations(numOperations);
}

void logarithmic(int size, int numOperations)
{
	while (size > 0)
	{
		constant(size, numOperations);
		size /= 2;
	}
}

void linear(int size, int numOperations)
{
	for (int i = 0; i < size; i++)
	{
		constant(size, numOperations);
	}
}

void linearlogarithmic(int size, int numOperations)
{
	for (int i = 0; i < size; i++)
	{
		logarithmic(size, numOperations);
	}
}

void cuadratic(int size, int numOperations)
{
	for (int i = 0; i < size; i++)
	{
		linear(size, numOperations);
	}
}

void cubic(int size, int numOperations)
{
	for (int i = 0; i < size; i++)
	{
		cuadratic(size, numOperations);
	}
}

#pragma endregion Order Methods

void HandleConstants(int order, ofstream &file, int size[PERCENTS_SIZE], duration<double, std::milli> times[PERCENTS_SIZE], int numConstantsConsidered, CONSTANTTYPE cteType)
{
	if (cteType == MODTIME || cteType == ALL)
	{
		for (int k = 0; k < min(MAX_NUMBER_OF_CONSTANTS, numConstantsConsidered); k++)
		{
			for (int j = 0; j < PERCENTS_SIZE; j++)
			{
				double time = times[j].count() * CONSTANTS[k];
				file << size[j] << ", " << time << ", ";
			}
			file << order << endl;
		}
	}
	if (cteType == MODSIZE || cteType == ALL)
	{
		for (int k = 0; k < min(MAX_NUMBER_OF_CONSTANTS, numConstantsConsidered); k++)
		{
			for (int j = 0; j < PERCENTS_SIZE; j++)
			{
				int modSize = size[j] * CONSTANTS[k];
				file << modSize << ", " << times[j].count() << ", ";
			}
			file << order << endl;
		}
	}
	if (cteType == MODTIMEANDSIZE || cteType == ALL)
	{
		for (int k = 0; k < min(MAX_NUMBER_OF_CONSTANTS, numConstantsConsidered); k++)
		{
			for (int j = 0; j < PERCENTS_SIZE; j++)
			{
				double time = times[j].count() * CONSTANTS[k];
				int modSize = size[j] * CONSTANTS[k];
				file << modSize << ", " << time << ", ";
			}
			file << order << endl;
		}
	}
}

void RunCaseOfOrder(int order, int N, ofstream &file, int numOperations, int numConstantsConsidered)
{
	void(*methodList[ORDER_AMOUNT])(int, int) { constant, logarithmic, linear, linearlogarithmic, cuadratic, cubic };
	duration<double, std::milli> times[PERCENTS_SIZE];
	int size[PERCENTS_SIZE];
	for (int j = 0; j < PERCENTS_SIZE; j++)
	{
		size[j] = N * percents[j];
		auto t1 = high_resolution_clock::now();
		methodList[order](size[j], numOperations);
		auto t2 = high_resolution_clock::now();
		times[j] = t2 - t1;
		file << size[j] << ", " << times[j].count() << ", ";
	}
	file << order << endl;

	HandleConstants(order, file, size, times, numConstantsConsidered, cteType);
}

void CallPro(int order, ofstream &file, int ops, int numConstantsConsidered)
{
	int runs = RUNS_PER_ORDER[order];
	int percent = -1;
	for (int i = 0; i < runs; i++)
	{
		RunCaseOfOrder(order, RandomSize(order), file, ops, numConstantsConsidered);
		HandleConsole(order, i, runs, percent);
	}
}

void CallSampledPRO(int order, ofstream &file, int ops, int sections, int numConstantsConsidered)
{
	int sizePerSection, runsPerSection;
	CalculateSizePerSection(order, sections, sizePerSection, runsPerSection);

	int minOrderSize = SIZE_RANGE_PER_TYPE[order][1];
	int runsForOrder = RUNS_PER_ORDER[order];
	int percent = -1;
	int counter = 0;

	for (int k = 0; k < sections; k++)
	{
		int minSize = minOrderSize + (sizePerSection * k);
		int maxSize = minOrderSize + (sizePerSection * (k + 1));

		for (int i = 0; i < runsPerSection; i++)
		{
			int N = RandomSize(minSize, maxSize);
			RunCaseOfOrder(order, N, file, ops, numConstantsConsidered);
			HandleConsole(order, ++counter, runsForOrder, percent);
		}
	}
}

int main2()
{
	srand(time(0));//randomize rand seed for different outcomes each time

	string all_file_name = "casos_ss_O(";
	for (int w = 0; w < ORDER_AMOUNT; w++) all_file_name += RUNS_PER_ORDER[w] > 0 ? to_string(w)+"," : "";
	all_file_name = all_file_name.substr(0, all_file_name.length()-1) + ")_";
	for (int w = 0; w < 5; w++) all_file_name += to_string(rand() % 10);

	ofstream allFile;
	allFile.open(all_file_name + ".txt");
	if (!allFile.is_open()) return 1;

	for (int order = 0; order < ORDER_AMOUNT; order++)
	{
		switch (executionType)
		{
			case FULLRANDOM:
				CallPro(
					order,
					allFile,
					NUMBER_OF_OPERATIONS,
					NUMBER_OF_CONSTANTS_CONSIDERED
				);
				break;
			case SAMPLEDRANDOM:
				CallSampledPRO(
					order,
					allFile,
					NUMBER_OF_OPERATIONS,
					SECTION_AMOUNT,
					NUMBER_OF_CONSTANTS_CONSIDERED
				);
			break;
		}
	}
	return 0;
}

//NEW STUFF
/*void CallWithNonLoopOperations(int order, int size, int numOperations, ofstream &file)
{
	void(*methodList[ORDER_AMOUNT])(int, int) { constant, logarithmic, linear, linearlogarithmic, cuadratic, cubic };

	for (int j = 0; j < PERCENTS_SIZE; j++)
	{
		int N = size * percents[j];
		auto t1 = high_resolution_clock::now();
		//if (order - 2 > 0) methodList[order - 2](N, numOperations);
		methodList[order](N, numOperations);
		auto t2 = high_resolution_clock::now();
		duration<double, std::milli> ms_double = t2 - t1;

		file << size << ", " << ms_double.count() << ", ";
	}
	file << order << endl;
}

void CallSampledWithNonLoopOperations(int order, ofstream &file, int ops, int sections)
{
	int sizePerSection, runsPerSection;
	CalculateSizePerSection(order, sections, sizePerSection, runsPerSection);

	int minOrderSize = SIZE_RANGE_PER_TYPE[order][1];
	int runsForOrder = RUNS_PER_ORDER[order];
	int percent = -1;
	int counter = 0;

	for (int k = 0; k < sections; k++)
	{
		int minSize = minOrderSize + (sizePerSection * k);
		int maxSize = minOrderSize + (sizePerSection * (k + 1));

		for (int i = 0; i < runsPerSection; i++)
		{
			int N = RandomSize(minSize, maxSize);
			CallWithNonLoopOperations(order, N, ops, file);
			HandleConsole(order, ++counter, runsForOrder, percent);
		}
	}
}

int main()
{
	void(*methodList[ORDER_AMOUNT])(int, int) { constant, logarithmic, linear, linearlogarithmic, cuadratic, cubic };

	srand(time(0));//randomize rand seed for different outcomes each time

	string all_file_name = "casos_ss_";
	for (int w = 0; w < 5; w++) all_file_name += to_string(rand() % 10);

	ofstream allFile;
	allFile.open(all_file_name + ".txt");
	if (!allFile.is_open()) return 1;

	for (int order = 0; order < ORDER_AMOUNT; order++)
	{
		CallSampledWithNonLoopOperations(order, allFile, NUMBER_OF_OPERATIONS, SECTION_AMOUNT);
	}
	return 0;
}*/

//OLDER MAIN STUFF
/*
void DoCTE(int N, ofstream& file, int numOperations)
{
	for (int j = 0; j < 3; j++)
	{
		int size = N * percents[j];
		auto t1 = high_resolution_clock::now();
		constant(size, numOperations);
		auto t2 = high_resolution_clock::now();
		duration<double, std::milli> ms_double = t2 - t1;
		file << size << ", " << ms_double.count() << ", ";
	}
	file << "0" << endl;
}

void DoLOG(int N, ofstream& file, int numOperations)
{
	for (int j = 0; j < 3; j++)
	{
		int size = N * percents[j];
		auto t1 = high_resolution_clock::now();
		logarithmic(size, numOperations);
		auto t2 = high_resolution_clock::now();
		duration<double, std::milli> ms_double = t2 - t1;
		file << size << ", " << ms_double.count() << ", ";
	}
	file << "1" << endl;
}

void DoN(int N, ofstream& file, int numOperations)
{
	for (int j = 0; j < 3; j++)
	{
		int size = N * percents[j];
		auto t1 = high_resolution_clock::now();
		linear(size, numOperations);
		auto t2 = high_resolution_clock::now();
		duration<double, std::milli> ms_double = t2 - t1;
		file << size << ", " << ms_double.count() << ", ";
	}
	file << "2" << endl;
}

void DoNLOG(int N, ofstream& file, int numOperations)
{
	for (int j = 0; j < 3; j++)
	{
		int size = N * percents[j];
		auto t1 = high_resolution_clock::now();
		linearlogarithmic(size, numOperations);
		auto t2 = high_resolution_clock::now();
		duration<double, std::milli> ms_double = t2 - t1;
		file << size << ", " << ms_double.count() << ", ";
	}
	file << "3" << endl;
}

void DoN2(int N, ofstream& file, int numOperations)
{
	for (int j = 0; j < 3; j++)
	{
		int size = N * percents[j];
		auto t1 = high_resolution_clock::now();
		cuadratic(size, numOperations);
		auto t2 = high_resolution_clock::now();
		duration<double, std::milli> ms_double = t2 - t1;
		file << size << ", " << ms_double.count() << ", ";
	}
	file << "4" << endl;
}

void DoN3(int N, ofstream& file, int numOperations)
{
	for (int j = 0; j < 3; j++)
	{
		int size = N * percents[j];
		auto t1 = high_resolution_clock::now();
		cubic(size, numOperations);
		auto t2 = high_resolution_clock::now();
		duration<double, std::milli> ms_double = t2 - t1;
		file << size << ", " << ms_double.count() << ", ";
	}
	file << "5" << endl;
}

void CallDoMethodWithSampledSizes(int order, ofstream &file, int ops, void(*func)(int, ofstream&, int), int sections)
{
	int sizePerSection, runsPerSection;
	CalculateSizePerSection(order, sections, sizePerSection, runsPerSection);

	int minOrderSize = SIZE_RANGE_PER_TYPE[order][1];
	int runsForOrder = RUNS_PER_ORDER[order];
	int percent = -1;
	int counter = 0;

	for (int k = 0; k < sections; k++)
	{
		int minSize = minOrderSize + (sizePerSection * k);
		int maxSize = minOrderSize + (sizePerSection * (k+1));

		for (int i = 0; i < runsPerSection; i++)
		{
			func(RandomSize(minSize, maxSize), file, ops);
			HandleConsole(order, ++counter, runsForOrder, percent);
		}
	}
}

void CallDoMethod(int order, ofstream &file, int ops, void(*func)(int, ofstream&, int))
{
	int runs = RUNS_PER_ORDER[order];
	int percent = -1;
	for (int i = 0; i < runs; i++)
	{
		func(RandomSize(order), file, ops);
		HandleConsole(order, i, runs, percent);
	}
}

int main()
{
	void(*doMethodList[ORDER_AMOUNT])(int, ofstream&, int) { DoCTE, DoLOG, DoN, DoNLOG, DoN2, DoN3 };

	srand(time(0));//randomize rand seed for different outcomes each time

	string all_file_name = "casos_ss_";
	for (int w = 0; w < 5; w++) all_file_name += to_string(rand() % 10);

	ofstream allFile;
	allFile.open(all_file_name  + ".txt");
	if (!allFile.is_open()) return 1;

	constant(100000, 10);//this is just to discard first run that is always trash!!!

	for (int order = 0; order < ORDER_AMOUNT; order++)
	{
		switch (executionType)
		{
			case FULLRANDOM:
				CallDoMethod(
					order,
					allFile,
					NUMBER_OF_OPERATIONS, //NUMBER OF OPERATIONS
					doMethodList[order]
				);
				break;
			case SAMPLEDRANDOM:
				CallDoMethodWithSampledSizes(
					order,
					allFile,
					NUMBER_OF_OPERATIONS,
					doMethodList[order],
					SECTION_AMOUNT//NUMBER OF SECTIONS
				);
				break;
		}
	}
	return 0;
}

*/
