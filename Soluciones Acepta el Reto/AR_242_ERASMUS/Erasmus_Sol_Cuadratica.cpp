#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void resuelveCaso(long long int a, int b[]);

int main() {
	long long int numCasos;
	int *arrayCasos;

	cin >> numCasos;
	while (numCasos != 0) {
		arrayCasos = new int[numCasos];
		for (int i = 0; i < numCasos; ++i) {
			cin >> arrayCasos[i];
		}
		resuelveCaso(numCasos, arrayCasos);
		cin >> numCasos;
	}
	return 0;
}

void resuelveCaso(long long int a, int b[])
{
	long long int cont = 0, aux = 0;

	for (int i = 0; i < a; i++) {
      	//calculo el total
		aux = 0;
      	for (int k = i; k < a; k++)
		{
			aux += b[k];
		}
		//multiplico por la suma del total restante
		cont += aux * b[i];
	}
	cout << cont << endl;
}