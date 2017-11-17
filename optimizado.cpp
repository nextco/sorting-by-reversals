/*
cl ibrs-optimizado.cpp
ibrs-optimizado.cpp < hard.data
*/
#include <iostream> // cout 
#include <algorithm> // std::swap
#include <random> //
#include <string> // stoi
#include <chrono> // diferencia de tiempo 
#include <vector> // struct strip

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
    cout << "Permutacion?" << endl;
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
Elegir el segmento creciente que optimize el numero de bp (disminuya)
tratamos de seleccionar 2 y si no se puede revertimos un solo segmento
*/
/*
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
*/


// BreakPoint
struct BreakPoint {
	int izquierda; // Posicion
	int derecha;

  	// Constructor (2 parametros: izquierda, derecha).
  	BreakPoint(int izquierda, int derecha) : izquierda(izquierda), derecha(derecha){}
};

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
void obtener_breakpoints(int *&array, int n, vector<BreakPoint>& breakpoints){	
	// No comparar el último valor
	for (int i = 0; i != (n - 1) ; i++){
		if(array[i] != (array[i + 1] + 1) && array[i] != (array[i + 1] - 1) ){
			breakpoints.push_back( BreakPoint(i, i + 1));
		}
	}
}

int numero_de_breakpoints(vector<BreakPoint>& breakpoints){
    return breakpoints.size();
}

bool tiene_breakpoints(vector<BreakPoint>& breakpoints){
	if( breakpoints.size() != 0 ){
		return true;
	}
	return false;
}

void imprimir_breakpoints(vector<BreakPoint>& breakpoints, int *&array){
	cout << "bp: ";
	for ( int k = 0; k != breakpoints.size(); k++) {
        cout << "(" << array[ breakpoints[k].izquierda ] << " " << array[ breakpoints[k].derecha ] <<  ")"; // Valores

        if( k != breakpoints.size() - 1 ) {
            cout << ", ";
        }
    }
    cout << " => ";
}


// Strip
struct Strip {
	int izquierda; // Posicion
	int derecha;
	string etiqueta; // Creciente - Decreciente
	
  	// Constructor (3 parametros: izquierda, derecha, etiqueta).
  	Strip(int izquierda, int derecha, string etiqueta) : izquierda(izquierda), derecha(derecha), etiqueta(etiqueta) {}
};

void imprimir_strips(vector<Strip> &strips, int *&array){    
    for ( int k = 0; k != strips.size(); k++) {
        // Indices
        // cout << strips[k].etiqueta << " (" <<  strips[k].izquierda << " " << strips[k].derecha <<  ")" ;
        cout << " (" <<  array[ strips[k].izquierda ] << " " << array[ strips[k].derecha ] <<  ") => " << strips[k].etiqueta ; // Valores

        if( k != strips.size() - 1 ) {
            // cout << ", ";
            cout << endl;
        }
    }
    // cout << " => ";
    cout << endl;
}

int numero_de_strips(vector<Strip>& strips){
    return strips.size();
}

// Retornar intervalos con los strips crecientes y decrecientes
void obtener_strips(int *&array, int n, vector<BreakPoint>& breakpoints, vector<Strip>& strips_crecientes, vector<Strip>& strips_decrecientes){
	// cout << "obtener_strips: " << endl;
	
	int rango_izquierda = 0;
	int rango_derecha = 0;

	// cout << "breakpoints.size() = " << breakpoints.size() << endl;

	// Analizar Strips en base a los breakpoints
	// Minimo numero de strips = 3, bp = 2, cuando la permutacion no es identidad
	for ( int i = 0; i != breakpoints.size(); i++) {

		// cout << "bp (pos): " <<  breakpoints[i].izquierda << " " << breakpoints[i].derecha << endl;
		// cout << "i: " << i << endl;

		// Para el primer y ultimo bp el analisis es especial
		// Primer bp
		if( i == 0 ){
			// cout << " analisis primer bp , i = " << i << endl;
			// En caso que el bp sea un numero solitario
			// 0 5 1 2 3 4 6 => 0 | 5 
			rango_izquierda = 0;

			if( array[ breakpoints[0].izquierda] == 0){ // Esta solo - creciente				
				rango_derecha = breakpoints[0].izquierda; // 0
				strips_crecientes.push_back( Strip(rango_izquierda, rango_derecha , "creciente"));

			}else if( array[breakpoints[i].izquierda] - array[breakpoints[i].izquierda - 1 ] == 1 ){ // Creciente
				rango_derecha = breakpoints[i].izquierda;
				strips_crecientes.push_back( Strip(rango_izquierda, rango_derecha , "creciente"));
			}
			
			// imprimir_strips(strips_crecientes, array);
		}

		
		if( i < breakpoints.size() - 1 && i > 0 ){ // No tomar cuando son sólo dos bp => bp1 | bp2 => inicio y final no hay intermedios
			// cout << "analisis bp intermedios , i = " << i << endl;

			// Derecha del breakpoints anterior
			rango_izquierda = breakpoints[i - 1].derecha;

			if( array[breakpoints[i].izquierda] - array[breakpoints[i].izquierda - 1 ] == 1){ // creciente
				rango_derecha = breakpoints[i].izquierda;
				strips_crecientes.push_back( Strip(rango_izquierda, rango_derecha , "creciente"));
			}else if( array[breakpoints[i].izquierda] - array[breakpoints[i].izquierda - 1 ] == - 1 ){
				rango_derecha = breakpoints[i].izquierda;
				strips_decrecientes.push_back( Strip(rango_izquierda, rango_derecha , "decreciente"));
			}else{
				// Valores solitarios
				rango_derecha = breakpoints[i].izquierda;
				strips_decrecientes.push_back( Strip(rango_izquierda, rango_derecha , "decreciente"));
			}
		}

		if( i == breakpoints.size() - 1 ){
			// cout << "analisis bp final, i = " << i << endl;
			// Derecha del breakpoints anterior
			rango_izquierda = breakpoints[i - 1].derecha;

			if( array[breakpoints[i].izquierda] - array[breakpoints[i].izquierda - 1 ] == 1){ // a su izquierda
				rango_derecha = breakpoints[i].izquierda;
				strips_crecientes.push_back( Strip(rango_izquierda, rango_derecha , "creciente"));
			}else if( array[breakpoints[i].izquierda] - array[breakpoints[i].izquierda - 1 ] == - 1 ){
				rango_derecha = breakpoints[i].izquierda;
				strips_decrecientes.push_back( Strip(rango_izquierda, rango_derecha , "decreciente")); // a su izquierda
			}else{
				// Valores solitarios
				rango_derecha = breakpoints[i].izquierda;
				strips_crecientes.push_back( Strip(rango_izquierda, rango_derecha , "creciente"));
			}

			// bp + 1 strips
			// por definicion
			if( array[breakpoints[i].derecha] -  array[breakpoints[i].izquierda] != 1 ){
				// Valor solitario
				rango_izquierda = breakpoints[i].derecha;
				rango_derecha = rango_izquierda;

				strips_crecientes.push_back( Strip(rango_izquierda, rango_derecha , "creciente"));				
			}
		}
        
    }
}

vector<Strip> unir_strips(vector<Strip>& strips_crecientes, vector<Strip>& strips_decrecientes){
	vector<Strip> strips;

	for ( int i = 0; i != strips_crecientes.size(); i++) {
		strips.push_back( strips_crecientes[i] );
	}

	for ( int i = 0; i != strips_decrecientes.size(); i++) {
		strips.push_back( strips_decrecientes[i] );
	}

	return strips;
}

/*
Retorna verdadero si la permutación tiene un strip decreciente
*/
bool tiene_al_menos_un_strip_decreciente(vector<Strip> &strips_decrecientes){	
	// cout <<"tiene_al_menos_un_strip_decreciente -> " << endl;
	if( strips_decrecientes.size() > 0 ){
		return true;
	}
	else{
		return false;
	}
}

void imprimir_vector(vector<int> &v, int *&array){
    for ( int k = 0; k != v.size(); k++) {
        // Indices
        cout << " (" <<  array[ v[k] ] << ") " ; 
        if( k != v.size() - 1 ) {
            // cout << ", ";
            cout << endl;
        }
    }
    // cout << " => ";
    cout << endl;
}

/*
Probar cada intervalo decreciente para ver si conduce a una reversión que 
elimina dos bp de forma optima, si no se puede eliminamos un bp
*/
void encontrar_reversion_que_minimize_bp(int * array, int n, vector<Strip> &strips, vector<BreakPoint> &breakpoints, int& mejor_rango_izquierda, int& mejor_rango_derecha ){
	// cout << "encontrar_reversion_que_minimize_bp: " << endl;
	
	mejor_rango_izquierda = 0; mejor_rango_derecha = 0;
	int maximo_bp_a_eliminar = 0;

	int bp_originales = numero_de_breakpoints(breakpoints);
	cout << "bp_originales = " << bp_originales << endl;

	
	// Obtener los valores izquierdas y derechas de los strips
	vector<int> izquierdas, derechas;
	for(int i = 0; i != strips.size(); i++){
		// Fix Bug - No tomar extremos 0 , n-1
		if( strips[i].izquierda > 0 && strips[i].derecha < n - 1 ){
			izquierdas.push_back( strips[i].izquierda );
			derechas.push_back( strips[i].derecha );	
		}
	}

	// cout << "izquierdas = " << endl;
	// imprimir_vector(izquierdas, array);
	// cout << "derechas = " << endl;
	// imprimir_vector(derechas, array);

	// Probamos todas las combinaciones posibles de reversiones entre izquierda => derecha	
	int rango_izquierda = 0, rango_derecha = 0;
	// cout << "Posibles combinaciones en strip: " << endl;
	
	// cout << "n "<< n << endl;

	for(int i = 0; i != izquierdas.size(); i++){
		for(int j = 0; j != derechas.size(); j++){
			
			rango_izquierda = izquierdas[i]; // i
			rango_derecha = derechas[j]; // j
						
			// Detectar combinaciones validas
			if( rango_izquierda != rango_derecha  ){
				// cout << rango_izquierda << " "<<  rango_derecha << endl;
				
				
				// Evaluar la inversión p(i, j)
				// de forma optima eliminar dos breakpoints si no se puede eliminamos sólo uno
			
				// Copiar array para probar combinaciones, esto es porque en c++ todos los arrays se 
				// pasan por referencia y no queremos que se modifique nuestro array original
				int* probar_reversion_array = nullptr;
				copiar_array(array, probar_reversion_array, n);
				
				revertir_sub_array(probar_reversion_array, rango_izquierda, rango_derecha);
				
				// Calcular bp para nueva_reversion
				vector<BreakPoint> bp;
				obtener_breakpoints(probar_reversion_array, n, bp);				
				int bp_eliminados = bp_originales - numero_de_breakpoints(bp);
				// cout << "bp_eliminados: " << bp_eliminados <<  endl;
				
				if( bp_eliminados > maximo_bp_a_eliminar ){
					// cout << "1 bp como minimo: "<< endl;
					maximo_bp_a_eliminar = bp_eliminados;

					mejor_rango_izquierda = rango_izquierda;
					mejor_rango_derecha = rango_derecha;

					// Determinar el mejor caso, dos bp eliminados
					if(maximo_bp_a_eliminar == 2){
						// Hacer reversion y salir de la funcion
						// cout << "2 bp optimo: "<< endl;
						// cout << " (mri = " << array[mejor_rango_izquierda] << ", mrd = " << array[mejor_rango_derecha] << " )" << endl;
						revertir_sub_array(array, mejor_rango_izquierda, mejor_rango_derecha);
						return;
					}
				}

				
				
			}
			
		}
	}

	// cout << " (mri = " << array[mejor_rango_izquierda] << ", mrd = " << array[mejor_rango_derecha] << " )" << endl;			
	revertir_sub_array(array, mejor_rango_izquierda, mejor_rango_derecha);

};


/*
Algoritmo:

Ref. 
// Neil C. Jones, Pavel A. Pevzner-An Introduction to Bioinformatics Algorithms 2004.pdf
// Pag 136
*/
void optimized_breakpoint_reversal_sort(int *&array, int n, int& d_pi){
	d_pi = 0; // Init 
	
	cout << "optimized_breakpoint_reversal_sort: " << endl;	

	// Obtener bp y strips
	vector<BreakPoint> bp;
	obtener_breakpoints(array, n, bp);
	int bp_restantes = numero_de_breakpoints(bp);

	// cout << "bp_originales = " << bp_restantes << endl << endl;

	 
	while( bp_restantes != 0 ){

		int mejor_rango_izquierda = 0;
		int mejor_rango_derecha = 0;

		// Obtener bp y strips para cada permutacion
		vector<BreakPoint> breakpoints;
		obtener_breakpoints(array, n, breakpoints);

		vector<Strip> strips_crecientes;
		vector<Strip> strips_decrecientes;
		obtener_strips(array, n, breakpoints, strips_crecientes, strips_decrecientes);
		vector<Strip> strips = unir_strips(strips_crecientes, strips_decrecientes );
		
		// imprimir_strips(strips, array);
		// cout << "Strip decreciente: (0=false, 1=true) " << tiene_al_menos_un_strip_decreciente( strips_decrecientes ) << endl;
		
		// if π has a decreasing strip (Linea 2)
		if( tiene_al_menos_un_strip_decreciente( strips_decrecientes ) ){
			// Among all reversals, choose reversal ρ minimizing b(π · ρ) (Linea 3)
			encontrar_reversion_que_minimize_bp( array, n, strips, breakpoints, mejor_rango_izquierda, mejor_rango_derecha );			
		}else{
			// Choose a reversal ρ that flips an increasing strip in π (Linea 5)
			// Solo quedan strips crecientes
			// revertir_strip_creciente( array, n, mejor_rango_izquierda, mejor_rango_derecha);
			// exit(-1); // Debug Inteligente		
        }

        d_pi++;

        // Recalcular # de bp        		
		vector<BreakPoint> bp_reversion_optima;
		obtener_breakpoints(array, n, bp_reversion_optima);
		bp_restantes = numero_de_breakpoints(bp_reversion_optima);

		imprimir_array(array, n);
        cout << " | bp_restantes = " << bp_restantes ;
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
	
	/*
	// Analisis de la permutacion Inicial
	cout << endl << "Analisis de la permutacion" << endl;
	vector<BreakPoint> breakpoints;
	obtener_breakpoints(array, n, breakpoints);
	// cout << "tiene_breakpoints (1=true, 0=false) = " << tiene_breakpoints(breakpoints); cout << endl;
	// cout << "cuantos_breakpoints = " << numero_de_breakpoints(breakpoints) << endl;
	cout << "imprimir_breakpoints => " << endl; imprimir_breakpoints(breakpoints, array); cout << endl;

	vector<Strip> strips_crecientes;
	vector<Strip> strips_decrecientes;
	obtener_strips(array, n, breakpoints, strips_crecientes, strips_decrecientes);
	cout << "imprimir_strips -> " << endl;
	// imprimir_strips(strips_crecientes, array); imprimir_strips(strips_decrecientes, array);
	
	vector<Strip> strips = unir_strips(strips_crecientes, strips_decrecientes );
	imprimir_strips(strips, array);
	cout << "numero_de_strips -> " << numero_de_strips(strips);
	*/


	
	// Calcular tiempo transcurrido.
  	auto t1 = chrono::high_resolution_clock::now();
  		optimized_breakpoint_reversal_sort(array, n, d_pi);		
  	auto t2 = chrono::high_resolution_clock::now();

	cout << endl << "d_pi = " <<  d_pi << endl; 
	cout << "Tiempo ejecucion (ns) = " << chrono::duration_cast<chrono::nanoseconds>(t2-t1).count() << endl;
}

