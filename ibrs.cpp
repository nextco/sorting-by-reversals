/*
cl ibrs.cpp
./ibrs.exe < input.data
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
	int max_permutation_value = n;

	n = n + 2; // Para agregar dos elementos al inicio y al final
    cout << "Permutacion? i.e : 1, 2, 3, ..., n" << endl;
    array = new int[n](); // Inicializar todos a zero

    /*
    Agregar marcadores al inicio y al final i.e
    6 5 4 3 2 1 -> 0 6 5 4 3 2 1 7
    */
    array[0] = 0; array[n - 1] = max_permutation_value + 1;
	for (int i = 1; i != (n - 1) ; i++){
		cin >> array[i];
	}

	// Verificar entradas	
	for (int i = 0; i != n; i++){
		if(! (array[i] >= 0 && array[i] <= (n + 1) ) ){
			cout << "Elemento invalido en permutacion :";
			cout << array[i];
			exit(-1); 
		}
	}
}
/*
 Devuelve el numero de breakpoins b(pi)
 i.e para n = 8
	0 2 1 3 4 5 8 7 6 9

	bp: 0 2
	bp: 1 3
	bp: 5 8
	bp: 6 9
    -> # bp = 4
	Debug: ibrs.exe < sd.data
*/
int numero_de_breakpoints(int *&array, int n){
	int bp = 0;
	// No comparar el último valor
	for (int i = 0; i != (n - 1) ; i++){
		if(array[i] != (array[i + 1] + 1) && array[i] != (array[i + 1] - 1) ){
			cout << "bp: " << array[i] << " " << array[i + 1] << endl;
			bp++;
		}
	}
    return bp;
}

/*
Retorna verdadero si la permutación tiene un strip decreciente
los strips de longitud uno son decrecientes por definicion

Debug: ibrs.exe < sd.data

*/
bool tiene_un_strip_decreciente(int *&array, int n){
	// El primer valor y el ultimo valor la permutación son crecientes por definicion	
	for (int i = 1; i != (n - 1) ; i++){
		// Algoritmo: el elemento i pertenece a un strip decreciente,
		// Si el elemento no pertece a un strip creciente
		if( (array[i - 1] + 1) != array[i] && (array[i] + 1) != array[i + 1] ){
			cout << "posible strip decreciente en: " << array[ i - 1 ] << " " <<  array[i] << " " << array[i + 1] << endl;
			return true;			
		}
	}
	return false;
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

Ref. 
// Neil C. Jones, Pavel A. Pevzner-An Introduction to Bioinformatics Algorithms 2004.pdf
// Pag 134
*/
void improved_breakpoint_reversal_sort(int *&array, int n, int& d_pi){
	d_pi = 0; // Init 
	cout << "improved_breakpoint_reversal_sort: " << endl;

	tiene_un_strip_decreciente(array,n);
	/*	 
	while( numero_de_breakpoints(array, n) > 0){
		// Tiene un strip decreciente?
		if( tiene_un_strip_decreciente(array,n) ){

		}
	}
	
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
	*/
}


int main(int argc, const char* argv[]) {	
	cout << endl << "./brs.exe < input.data" <<  endl;

	int* array = nullptr;	
	int n = 0; // Tamaño de permutación
	int d_pi = 0; // Número de reversiones

	leer_permutacion(array, n);		
	cout << "Valores Iniciales " << endl; imprimir_array(array, n);
	
	// number of breakpoins
	numero_de_breakpoints(array, n);
	
	// Calcular tiempo transcurrido.
  	auto t1 = chrono::high_resolution_clock::now();
  		improved_breakpoint_reversal_sort(array, n, d_pi);		
  	auto t2 = chrono::high_resolution_clock::now();

	cout << endl << "d_pi = " <<  d_pi << endl; 
	cout << "Tiempo ejecucion (ns) = " << chrono::duration_cast<chrono::nanoseconds>(t2-t1).count() << endl;
}

