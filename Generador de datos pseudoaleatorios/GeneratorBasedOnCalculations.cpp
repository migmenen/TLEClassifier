#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>


using namespace std;

const int NUM_LOTES = 3;//numero de ejecuciones (tamaño del array) { [(tam,tiem)],tipo }
const float SIZE_PERCENTS[NUM_LOTES]{ 0.10f, 0.5f, 1.0f };//{.1, .35, .60, .85}

typedef enum { CTE, LOGN, N, NLOGN, N2, N3 }TiempoEjecucion;
const int NUM_TIEMPOS_EJ = 6;

const int MIN_RUIDO = -5, MAX_RUIDO = 25;

string all_file_name = "casos_all";
ofstream allFile;
string file_names[] = { "casos_cte", "casos_logn", "casos_n", "casos_nlogn", "casos_n2", "casos_n3" };
const int NUM_FILES = 7;

const int MAX_CTE_MUL = 15;
const int MIN_CTE_MUL = 1;

const int CASOS_POR_CTE = 100000;

const long double DIVISORES[NUM_TIEMPOS_EJ] = { 10000, 100000, 1000000000, 1000000000, 100000000000, 10000000000 };

const int NUM_DECIMALES = 6;

/*int RANGOS_POR_TIPO[NUM_TIEMPOS_EJ][2] = { 
	{700000, 300000},	//cte
	{300000, 225000},	//log
	{200000, 150000},	//n
	{100000, 75000},	//nlogn
	{16000, 12000},	//n2
	{800, 600}		//n3
};*/
int RANGOS_POR_TIPO[NUM_TIEMPOS_EJ][2] = {
	{700000, 100},	//cte
	{300000, 100},	//log
	{200000, 100},	//n
	{100000, 100},	//nlogn
	{16000, 100},	//n2
	{800, 100}		//n3
};

long double CalculaTiempo(int CTE_MULT, long int tamaño, int tipo, int porcentajeRuido)
{
	if (tipo == CTE)
	{
		long double tiempoRuido = porcentajeRuido * CTE_MULT;

		return tiempoRuido + CTE_MULT;
	}
	else if (tipo == N)
	{
		long double tiempoBase = (CTE_MULT * tamaño);
		long double tiempoRuido = porcentajeRuido * tiempoBase;

		return tiempoBase + tiempoRuido;
	}
	else if (tipo == N2)
	{
		long double tiempoBase = (CTE_MULT * tamaño*tamaño);
		long double tiempoRuido = porcentajeRuido * tiempoBase;

		return tiempoBase + tiempoRuido;
	}
	else if (tipo == N3)
	{
		long double tiempoBase = (CTE_MULT * tamaño*tamaño*tamaño);
		long double tiempoRuido = porcentajeRuido * tiempoBase;

		return tiempoBase + tiempoRuido;
	}
	else if (tipo == LOGN)
	{
		long double tiempoBase = (CTE_MULT * log(tamaño));
		long double tiempoRuido = porcentajeRuido * tiempoBase;

		return tiempoBase + tiempoRuido;
	}
	else if (tipo == NLOGN)
	{
		long double tiempoBase = (CTE_MULT * tamaño * log(tamaño));
		long double tiempoRuido = porcentajeRuido * tiempoBase;

		return tiempoBase + tiempoRuido;
	}
}

void EjecutaCaso(int tipo, int cte, ofstream &outFile)
{
	//genero un tamaño aleatorio
	int tam_min = RANGOS_POR_TIPO[tipo][1], tam_max = RANGOS_POR_TIPO[tipo][0];
	long int tamano_maximo = tam_min + (rand()*rand()) % (tam_max- tam_min);

	//long double times[NUM_LOTES];
	//long int tamanos[NUM_LOTES];

	for (int i = 0; i < NUM_LOTES; i++)
	{
		long int tamano = SIZE_PERCENTS[i] * tamano_maximo;
		//genero un % de ruido aleatorio
		int ruido = MIN_RUIDO + rand() % (MAX_RUIDO - MIN_RUIDO);

		long double time = CalculaTiempo(cte, tamano, tipo, ruido) / DIVISORES[tipo];
		if (time < 0) time *= -1;
		if (time == 0) { i--; continue; }

		outFile << tamano << ", " << time << " ,";// << t << endl;
		allFile << tamano << ", " << time << " ,";// << t << endl;
	}


	int t = (int)tipo;
	outFile << t << endl;
	allFile << t << endl;
}

int main(int argc, char *argv[])
{
	ofstream file;

	allFile.open(all_file_name + ".txt");
	for (int w = 0; w < NUM_LOTES; w++)
	{
		allFile << "tamano,tiempo,";
	}
	allFile << "tipo" << endl;
	allFile << fixed << setprecision(NUM_DECIMALES);

	for (int tipo = 0; tipo < NUM_TIEMPOS_EJ; tipo++)//para cada tipo de ejecucion ( logn, n, nlogn, ...)
	{
		file.open(file_names[tipo] + ".txt");
		for (int w = 0; w < NUM_LOTES; w++)
		{
			file << "tamano,tiempo,";
		}
		file << "tipo" << endl;
		file << fixed << setprecision(NUM_DECIMALES);


		for (int CTE_MULT = MIN_CTE_MUL; CTE_MULT < MAX_CTE_MUL; CTE_MULT++)//para todas las cte_mul en el rango
		{
			for (int k = 0; k < CASOS_POR_CTE; k++)
			{
				EjecutaCaso(tipo, CTE_MULT, file);
			}
			printf("One CTE done!, ");
		}
		file.close();
		printf("One file done!\n");
	}
	allFile.close();

	system("pause");
	return 0;
}