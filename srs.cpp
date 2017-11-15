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
void imprimir_array(int * array, int n){
	for (int i = 0; i != n; i++){
		// cout << "array["<<i<<"] = " << array[i] << endl;
		cout << array[i] << "\t";
	}
	cout << endl;
}

void leer_permutacion(int *&array, int& n){
	cout << "leer_permutacion -> " << endl;

	cout << "Ingresa n: "; cin >> n;
    
    cout << "Permutacion? p.e. : 1, 2, 3, ..., n" << endl;
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

bool es_permutacion_identidad(int *&array, int n){
	// cout << "es_permutacion_identidad -> " ;
	
	int verificador_secuencial = 1;
	for (int i = 0; i != n; i++){
		if (verificador_secuencial != array[i] ){			
			return false;
		}
		verificador_secuencial++;
	}
	return true;
}

/*
	Input: [1,2,3,4,5]
	Izquierda: 0, Derecha: 4
	Output: [5,4,3,2,1]
	Estado: Se que va servir para el improved, en este no se usa
*/	
void revertir_sub_array(int *& array, int izquierda, int derecha){
	// cout <<"revertir_sub_array(" << izquierda << ", " << derecha << ")" << endl;
	/*
	 1 2 3 4 5 6 
	 1 2 <- maximo_indice_reversion
	*/
	int maximo_indice_reversion = (derecha - izquierda + 1 ) / 2; // No tomar el elemento central, si existe

	for (int i = 0; i != maximo_indice_reversion; i++){
		// Swap Values				
		int t = array[izquierda + i];
		array[izquierda + i] = array[derecha - i];
		array[derecha - i] = t;		
		// swap(array[izquierda + i], array[derecha - i]); // :S 
	}

}

/*
Buscar elemento en sub array comenzando a buscar desde un elemento de la izquierda
	Input: [1,2,3,4,5], izquierda: 3(comienzo de búsqueda)
	Target: 5	
	Output: 4 (Posición)
*/
int buscar_elemento_en_array_desde_la_izquierda(int *& array, int n, int target, int izquierda){
	for(int i = izquierda; i != n; i++){
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
			int posicion_valor_esperado =  buscar_elemento_en_array_desde_la_izquierda(array, n, i, k);
			// cout << "valor_esperado (posicion) = " << posicion_valor_esperado << endl;

			// cout << "p (inversion - elementos) = (" << array[k] << ", "<<  array[posicion_valor_esperado] << ")"<< endl  ;
			revertir_sub_array(array, k, posicion_valor_esperado );
			imprimir_array(array, n);
			d_pi++;
		}
		
		// Verificar si la permutación es identidad
		if( es_permutacion_identidad(array, n) ){
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

	leer_permutacion(array, n);		
	cout << "Valores Iniciales " << endl; imprimir_array(array, n);
	
	// Calcular tiempo transcurrido.
  	auto t1 = chrono::high_resolution_clock::now();
  		simple_reversal_sort(array, n, d_pi);		
  	auto t2 = chrono::high_resolution_clock::now();

	cout << endl << "d_pi = " <<  d_pi << endl; 
	cout << "Tiempo ejecucion (ns) = " << chrono::duration_cast<chrono::nanoseconds>(t2-t1).count() << endl;
}
