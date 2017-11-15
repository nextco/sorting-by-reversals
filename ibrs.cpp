/*
cl ibrs.cpp
ibrs.exe < ibrs-clase.data
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
		cout << array[i] << ", ";
	}
	cout << endl;
}

void leer_permutacion(int *&array, int& n){
	cout << "leer_permutacion -> " << endl;

	cout << "Ingresa n: "; cin >> n; 
	int max_permutation_value = n;

	n = n + 2; // Para agregar dos elementos al inicio y al final
    cout << "Permutacion? i.e : 5, 1, 2, 3, 4, ..." << endl;
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
			// cout << "bp: " << array[i] << " " << array[i + 1] << endl;
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
			// cout << "posible strip decreciente en: " << array[ i - 1 ] << " " <<  array[i] << " " << array[i + 1] << endl;
			return true;			
		}
	}
	return false;
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
Copia un array los valores de a en b
*/
void copiar_array(int *& a, int *& b, int n){
	b = new int[n](); // Inicializar valores a zero

	for(int i = 0; i != n; i++){
		b[i] = a[i];
	}	
}
/*
Mejor segmento para revertir probando todas las combinaciones posibles
*/
void elegir_segmento_reversion(int * array, int n, int& mejor_rango_izquierda, int& mejor_rango_derecha ){
	// cout << "elegir_inversion_fuerza_bruta: " << endl;

	mejor_rango_izquierda = 0; mejor_rango_derecha = 0;
	int maximo_bp_a_eliminar = 0;

	int bp_originales = numero_de_breakpoints(array, n);
	// cout << "bp_originales = " << bp_originales << endl;

	for(int i = 1; i != n - 2; i++){
		for(int j = i + 1; j != n - 1; j++){
			// Evaluar la inversión p(i, j)
			// de forma optima eliminar dos breakpoints si no se puede eliminamos sólo uno			
			if ( abs(array[i] - array[j + 1]) == 1 || abs(array[i - 1] - array[j] ) == 1) {
				// Contar cuantos breakpoints son eliminados por la reversion p(i,j)
				
				// Copiar array para probar combinaciones, esto es porque en c++ todos los arrays se 
				// pasan por referencia y no queremos que se modifique nuestro array original
				int* probar_reversion_array = nullptr;
				copiar_array(array, probar_reversion_array, n);
				
				revertir_sub_array(probar_reversion_array, i, j);
				
				int bp_eliminados = bp_originales - numero_de_breakpoints(probar_reversion_array, n);
				if( bp_eliminados > maximo_bp_a_eliminar ){
					maximo_bp_a_eliminar = bp_eliminados;

					mejor_rango_izquierda = i;
					mejor_rango_derecha = j;

					// Determinar el mejor caso, dos bp eliminados
					if(maximo_bp_a_eliminar == 2){
						return;
					}
				}

			}
		}			
	}
};
/*
Elegir el segmento creciente que esta más a la izquierda del array 
excluyendo el primer y ultimo elemento
*/
void elegir_segmento_creciente(int * array, int n, int& mejor_rango_izquierda, int& mejor_rango_derecha ){
	// cout << "elegir_segmento_creciente: " << endl;
	
	mejor_rango_izquierda = 1; 

    while ( mejor_rango_izquierda < n - 1 && array[mejor_rango_izquierda] + 1 != array[mejor_rango_izquierda+1] ){
    	mejor_rango_izquierda++;
    }
    
    mejor_rango_derecha = mejor_rango_izquierda + 1;
    while( mejor_rango_derecha < n - 2 && array[mejor_rango_derecha] + 1 == array[mejor_rango_derecha + 1]){
    	mejor_rango_derecha++;
    }        
};

/*
Algoritmo:

Ref. 
// Neil C. Jones, Pavel A. Pevzner-An Introduction to Bioinformatics Algorithms 2004.pdf
// Pag 136
*/
void improved_breakpoint_reversal_sort(int *&array, int n, int& d_pi){
	d_pi = 0; // Init 
	// cout << "improved_breakpoint_reversal_sort: " << endl;

	cout << "bp_originales = " << numero_de_breakpoints(array, n) << endl << endl;	 
	while( numero_de_breakpoints(array, n) > 0){

		int mejor_rango_izquierda = 0;
		int mejor_rango_derecha = 0;

		// Tiene un strip decreciente?
		if( tiene_un_strip_decreciente(array, n) ){
			elegir_segmento_reversion(array, n, mejor_rango_izquierda, mejor_rango_derecha );
			
			cout << endl << "(mri = " << mejor_rango_izquierda << ", mrd = " << mejor_rango_derecha;
			revertir_sub_array(array, mejor_rango_izquierda, mejor_rango_derecha);

		}else{
			// Solo quedan strips crecientes
			elegir_segmento_creciente(array, n, mejor_rango_izquierda, mejor_rango_derecha);

			cout << endl <<  "(mri = " << mejor_rango_izquierda << ", mrd = " << mejor_rango_derecha;
			revertir_sub_array(array, mejor_rango_izquierda, mejor_rango_derecha);
        }
        d_pi++;
        cout << "), bp_restantes = " << numero_de_breakpoints(array, n) << endl;
        imprimir_array(array, n);
        
	}


}


int main(int argc, const char* argv[]) {	
	cout << endl << "./brs.exe < input.data" <<  endl;

	int* array = nullptr;	
	int n = 0; // Tamaño de permutación
	int d_pi = 0; // Número de reversiones

	leer_permutacion(array, n);		
	cout << "Valores Iniciales " << endl; imprimir_array(array, n);
	

	// Calcular tiempo transcurrido.
  	auto t1 = chrono::high_resolution_clock::now();
  		improved_breakpoint_reversal_sort(array, n, d_pi);		
  	auto t2 = chrono::high_resolution_clock::now();

	cout << endl << "d_pi = " <<  d_pi << endl; 
	cout << "Tiempo ejecucion (ns) = " << chrono::duration_cast<chrono::nanoseconds>(t2-t1).count() << endl;
}

