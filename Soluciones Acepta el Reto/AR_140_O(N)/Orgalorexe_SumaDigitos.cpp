#include<iostream>
using namespace std;

int sumaDigitos(int n);

int main() {
	int n;
	cin >> n;

	while (n > -1) {
		cout << " = " << sumaDigitos(n) << endl;
		cin >> n;
	}

	return 0;
}

/* { Pre: 0 <= n} */
int sumaDigitos(int n){
	//Complejidad
	// O(n)

	//Caso base
	if (n < 10) { 
		cout << n;
		return n;
	}

	//LLamada recursiva
	int ret = sumaDigitos(n / 10);
	cout << " + " << n % 10;
	return ret + (n % 10);
}
/* { Pos: } */