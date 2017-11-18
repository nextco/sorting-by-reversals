/*
ibrs fuerza bruta entre todos los elementos
cl ibrs-bf.cpp
ibrs-bf.exe < input/fix.data - Bug Detected
*/
#include <iostream> // cout 
// #include <algorithm> // std::swap
#include <random> //
#include <string> // stoi
#include <chrono> // diferencia de tiempo 

using namespace std;
void imprimir_array(int * array, int n){	
	for (int i = 0; i != n; i++){
		cout << array[i];
		if(i != n - 1){ // No poner coma al final
			cout << ", ";	
		}		
	}
	cout << " => ";
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
*/
bool tiene_al_menos_un_strip_decreciente(int *&array, int n){
	
	// cout <<"tiene_al_menos_un_strip_decreciente -> " << endl;

	// El primer valor y el ultimo valor la permutación son crecientes por definicion
	for (int i = 1; i != (n - 1) ; i++){
		// 		
		// Son valores consecutivos crecientes
		if( array[i] - array[i - 1] == 1 ){
			// 1 2 3 4 5 ... n
			// cout << "Strip creciente (+): " << array[ i - 1 ] << " " <<  array[i] << endl;
		}else if(array[i] - array[i - 1] == -1 ){ // Si no son crecientes podrian ser decrecientes
			// n n-1 ... 1
			// cout << "Strip decreciente (-): " << array[ i - 1 ] << " " <<  array[i] << endl;
			// tiene_decreciente = true;
			return true;
		}

		// Son valores intercalados como 1 3 5 2 4 => no hay strips consecutivos decrecientes
		// Por definicion 3, 5, 2, 4 son strips decrecientes
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
void encontrar_reversion_que_minimize_bp(int * array, int n, int& mejor_rango_izquierda, int& mejor_rango_derecha ){
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
						// Hacer reversion y salir de la funcion
						revertir_sub_array(array, mejor_rango_izquierda, mejor_rango_derecha);
						return;
					}
				}

			}
		}			
	}

	revertir_sub_array(array, mejor_rango_izquierda, mejor_rango_derecha);
};
/*
Elegir el segmento creciente que optimize el numero de bp (disminuya)
tratamos de seleccionar 2 y si no se puede revertimos un solo segmento
*/
void revertir_strip_creciente(int * array, int n, int& mejor_rango_izquierda, int& mejor_rango_derecha ){
	// cout << "revertir_strip_creciente: " << endl;
	
	int maximo_bp_a_eliminar = 0;
	int bp_originales = numero_de_breakpoints(array, n);

	mejor_rango_izquierda = 1;
	// Hasta encontrar una secuencia de un strip creciente
	// [1 2 3 4 6 7 5] -> Ok, comienza en uno y termina en 4
	for (int i = 1; i != n-1; i++){
		if( array[i + 1] - array[i] == 1 ){
			// 1 2 3 4 .. n
			mejor_rango_izquierda = i;

			// Encontrar limite superior de strip
			mejor_rango_derecha = mejor_rango_izquierda + 1;
			for (int j = mejor_rango_derecha; j != n-1; j++){
				if( array[mejor_rango_derecha + 1] - array[mejor_rango_derecha] == 1 ){
					mejor_rango_derecha++;
				}else{					
					i = j;
					break; // Salir de for y probamos la reversion en este segmento
				}
			}

			// Probar reversion en ese rango y escoger la mejor
			// cout << endl << "Encontro rango izquierda y derecha x-avo intento: Entre (a, b) " << endl;
			// cout << endl << "(mri = " << array[mejor_rango_izquierda] << ", mrd = " << array[mejor_rango_derecha];

			// Contar cuantos breakpoints son eliminados por la reversion p(i,j)				
			// Copiar array para probar combinaciones, esto es porque en c++ todos los arrays se 
			// pasan por referencia y no queremos que se modifique nuestro array original
				
			int* probar_reversion_array = nullptr;
			copiar_array(array, probar_reversion_array, n);
			
			revertir_sub_array(probar_reversion_array, mejor_rango_izquierda, mejor_rango_derecha);
			
			int bp_eliminados = bp_originales - numero_de_breakpoints(probar_reversion_array, n);
			if( bp_eliminados > maximo_bp_a_eliminar ){
				maximo_bp_a_eliminar = bp_eliminados;

				// Determinar el mejor caso, dos bp eliminados
				if(maximo_bp_a_eliminar == 2){
					// Hacer reversion y salir de la funcion
					revertir_sub_array(array, mejor_rango_izquierda, mejor_rango_derecha);
					return;
				}
			}
			// cout << ", bp_eliminados: " << bp_eliminados <<  endl;
			// bp_eliminados: -1 (Significa que la reversion creciente no elimina ningun breakpoint y probamos el siguiente siempre tratamos de eliminar dos bp si se puede)
		}
	}

	revertir_sub_array(array, mejor_rango_izquierda, mejor_rango_derecha);

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
	// tiene_al_menos_un_strip_decreciente(array, n);
	
	while( numero_de_breakpoints(array, n) > 0){

		int mejor_rango_izquierda = 0;
		int mejor_rango_derecha = 0;

		// Tiene un strip decreciente?
		// cout << "Strip decreciente (-): (0=false, 1=true) " << tiene_al_menos_un_strip_decreciente(array, n) << endl;
				
		// if π has a decreasing strip (Linea 2)
		if( tiene_al_menos_un_strip_decreciente(array, n) ){
			// Among all reversals, choose reversal ρ minimizing b(π · ρ) (Linea 3)
			encontrar_reversion_que_minimize_bp( array, n, mejor_rango_izquierda, mejor_rango_derecha );			
		}else{
			// Choose a reversal ρ that flips an increasing strip in π (Linea 5)
			// Solo quedan strips crecientes
			revertir_strip_creciente( array, n, mejor_rango_izquierda, mejor_rango_derecha);
			// exit(-1); // Debug Inteligente		
        }

        d_pi++;
                
        imprimir_array(array, n);
        cout << " | bp_restantes = " << numero_de_breakpoints(array, n) ;
        cout << " (mri = " << array[mejor_rango_izquierda] << ", mrd = " << array[mejor_rango_derecha] << " )" << endl;
        
                
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

