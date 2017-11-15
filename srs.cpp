/*
cl srs.cpp
./srs.exe < input.data
*/

#include <iostream> // cout 
#include <algorithm> // std::swap
#include <random> //
#include <string> // stoi
#include <chrono> // diferencia de tiempo 

using namespace std;
void print_array(int * array, int n){
	for (int i = 0; i != n; i++){
		// cout << "array["<<i<<"] = " << array[i] << endl;
		cout << array[i] << "\t";
	}
	cout << endl;
}

void read_permutation_from_stdin(int *&array, int& n){
	cout << "read_permutation_from_stdin -> " << endl;

	cout << "Ingresa n: "; cin >> n;
    
    cout << "Permutacion? i.e : 1, 2, 3, ..., n" << endl;
    array = new int[n](); // Inicializar todos a zero	
	for (int i = 0; i != n; i++){
		cin >> array[i];
	}

	// Verificar entradas	
	for (int i = 0; i != n; i++){
		if(! (array[i] > 0 && array[i] <= n) ){
			cout << "Elemento invalido en permutacion :";
			cout << array[i];
			exit(-1); 
		}
	}
}

bool is_permutation_identity(int *&array, int n){
	// cout << "is_permutation_identity -> " ;
	
	int sequential_check = 1;
	for (int i = 0; i != n; i++){
		if (sequential_check != array[i] ){			
			return false;
		}
		sequential_check++;
	}
	return true;
}

/*
	Input: [1,2,3,4,5]
	Izquierda: 0, Derecha: 4
	Output: [5,4,3,2,1]
	Estado: Se que va servir para el improved, en este no se usa
*/	
void reverse_sub_array(int *& array, int left, int right){
	// cout <<"reverse_sub_array(" << left << ", " << right << ")" << endl;

	int half = (right - left + 1 ) / 2; // No tomar el elemento central, si existe
	for (int i = 0; i != half; i++){
		// Swap Values				
		int t = array[left + i];
		array[left + i] = array[right - i];
		array[right - i] = t;		
		// swap(array[left + i], array[right - i]); // :S 
	}

}

/*
Buscar elemento en sub array comenzando a buscar desde un elemento de la izquierda
	Input: [1,2,3,4,5], left: 3(comienzo de búsqueda)
	Target: 5	
	Output: 4 (Posición)
*/
int search_element_in_array_from_left(int *& array, int n, int target, int left){
	for(int i = left; i != n; i++){
		if( array[i] == target ){
			return i;
		}
	}
}

/*
Algoritmo:
Examine each position i of the permutation from left to right
I At each position, if pi != i , do a reversal such that pi(sub-i) = i

Teoría
i.e Para la permutación identidad (4 genes secuenciales)
pi(sub-1)  = 1 (Comienza en 1)
pi(sub-2)  = 2
pi(sub-3)  = 3
pi(sub-4)  = 4

Reversal o Inversiones
Problema de la distancia de reversión d(sub-pi);
Encontrar la más pequeña serie de reversiones para una permutación

Ref. 
// Neil C. Jones, Pavel A. Pevzner-An Introduction to Bioinformatics Algorithms 2004.pdf
// Pag 130
*/
void simple_reversal_sort(int *&array, int n, int& d_pi){
	d_pi = 0; // Init 
	cout << "simple_reversal_sort: " << endl;
	
	int i = 1; // Valor esperado (VE); comienza en 1, tomado de la permutación identidad

	for (int k = 0; k < n - 1; k++){
		int j = array[k]; // Valor Obtenido (VO)
		
		// cout << "VE-VO -> ( i = " << i << ", j = " << j << " )" << endl ; // Valor Esperado - Valor Obtenido
		if( i != j ){
			// Buscar en que posición esta el valor esperado y hacer una reversión
			int posicion_valor_esperado =  search_element_in_array_from_left(array, n, i, k);
			// cout << "valor_esperado (posicion) = " << posicion_valor_esperado << endl;

			// cout << "p (inversion - elementos) = (" << array[k] << ", "<<  array[posicion_valor_esperado] << ")"<< endl  ;
			reverse_sub_array(array, k, posicion_valor_esperado );
			print_array(array, n);
			d_pi++;
		}
		
		// Verificar si la permutación es identidad
		if( is_permutation_identity(array, n) ){
			return;
		}

		i++; // Comparar el siguiente elemento de la permutación
	}
}


int main(int argc, const char* argv[]) {	
	cout << endl << "./srs.exe < input.data" <<  endl;

	int* array = nullptr;	
	int n = 0; // Tamaño de permutación
	int d_pi = 0; // Número de reversiones

	read_permutation_from_stdin(array, n);		
	cout << "Valores Iniciales " << endl; print_array(array, n);
	
	// Calcular tiempo transcurrido.
  	auto t1 = chrono::high_resolution_clock::now();
  		simple_reversal_sort(array, n, d_pi);		
  	auto t2 = chrono::high_resolution_clock::now();

	cout << endl << "d_pi = " <<  d_pi << endl; 
	cout << "Tiempo ejecucion (ns) = " << chrono::duration_cast<chrono::nanoseconds>(t2-t1).count() << endl;
}

