/*
cl srs-benchmark.cpp
srs-benchmark.exe 100000
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


void crear_permutacion_identidad(int *&array, int n){
	// cout << "crear_permutacion_identidad -> " << endl;
	array = new int[n](); // Inicializar todos a cero
	
	int semilla_secuencial = 1;
	for (int i = 0; i != n; i++){	
		array[i] = semilla_secuencial++;
	}
}

void crear_permutacion_aleatoria(int *&array, int n){
	cout << "crear_permutacion_aleatoria -> " << endl;
	array = new int[n](); // Inicializar todos a cero
	
	crear_permutacion_identidad(array, n);

	// Mezclar valores, utiliza la funcion random_shuffle de la libreria algorithm
	random_shuffle(array, array + n);
}

/*
1,2,3,4,5,max -> 1,max,2,3,4,5
			  -> 1,2,3,max, 4,5	
*/
void crear_permutacion_especial(int *&array, int n){
	cout << "crear_permutacion_especial -> " << endl;
	array = new int[n](); // Inicializar todos a cero
	
	int max_value = n;
	
	// Random Value entre [0, n-2]
	random_device rd;
	mt19937 rng(rd());
	uniform_int_distribution<int> random_integer(0, n - 2);	
	
	// Insertar máximo valor en una posición aleatoria
	int posicion_aleatoria = random_integer(rng);
	// cout << "posicion_aleatoria = " << posicion_aleatoria << endl;

	int semilla_secuencial = 1; // 
	for (int i = 0; i != n; i++){
		if( posicion_aleatoria == i){
			array[i] = n;
			continue; // Siguiente iteracion
		}
		array[i] = semilla_secuencial++;
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

void simple_reversal_sort(int *&array, int n, int& d_pi){
	d_pi = 0; // Init 
	// cout << "simple_reversal_sort: " << endl;
	
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
			// swap_values_in_array(array, k, array[i] ); // Reverse un sólo valor
			// swap ( array[k], array[array[i]] ); // Counting Sort OP xD
			// imprimir_array(array, n);
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
	cout << endl << "./srs.exe number_of_elements" <<  endl;

	int* array = nullptr;	
	int n = stoi(argv[1]); // Tamaño de permutación
	int d_pi = 0; // Número de reversiones

	crear_permutacion_aleatoria(array, n);

	// Calcular tiempo transcurrido.
  	auto t1 = chrono::high_resolution_clock::now();  
  		simple_reversal_sort(array, n, d_pi);  				
  	auto t2 = chrono::high_resolution_clock::now();

	cout << endl << "d_pi = " <<  d_pi << endl; 
	cout << "Tiempo CPU (ms) = " << chrono::duration_cast<chrono::milliseconds>(t2-t1).count() << endl;
}



