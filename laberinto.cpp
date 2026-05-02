// LIBRERIAS NECESARIAS PARA EL FUNCIONAMIENTO DEL PROGRAMA


// "#include" sirve para llamar librerias (Que son cajas de herramientas)
#include <iostream>   // Se encarga de la comunicacion con el usuario (Flujo de entrada y salida)
#include <vector>     // Sirve para crear contenedores dinamicos, permitiendo que su tamanho sea modificado durante la ejecucion del programa
// Le dice al programa cada vez que pida un martillo de la caja roja, no me hagas decir CajaRoja::Marillo. solo di martillo y me entiendes
#include <chrono>     // Es Como un cronometro digital que puede medir el tiempo de manera ultra precisa milisegundos o menos
#include <random>     // Es como una dado electronico, que sirve para obtener numeros aleatorios
#include <stack>      // Acomoda los datos en una pila (tipo de platos) donde el ultimo en entrar es el primero en salir
#include <queue>      // Ordena los datos en una cola (tipo de espera) Donde el primero en llegar es el primero en salir
#include <algorithm>  // Sirve para llamar o traer funcionen ya echas (Ayuda a no reinventar la rueda y reducir codigo)

using namespace std; // Ahorra codigo y hace mas comodo la sintaxis en proyectos pequenhos



//----------------------------------------------------------------
//----------------------------------------------------------------
// Funcion para leer tamano desde parametros o entrada del usuario
//----------------------------------------------------------------
//----------------------------------------------------------------

// Definimos que el tipo de dato a devolver es un par de enteros (x,y)
pair<int, int> obtenerTamano(int argc, char* argv[]) {
    // Datos: * int argc es la cantidad de argumentos que se pasan al programa por línea de comando (maximo 3, nombre del programa, y filas y columnas)
    //        * char* argv[] es un arreglo de cadenas con los argumentos (Guarda la posicion del dato en la memoria)


    int filas = 11, columnas = 11; // Tamano por defecto
    
    if (argc >= 3) { // Si el usuario ingreso los datos de la filas y columnas al programa juntos entonces...
        // Leer desde argumentos de linea de comandos
        filas = atoi(argv[1]);      // El primer dato seria para las filas 
        columnas = atoi(argv[2]);   // El segundo dato para las columas
                                    // Primer dato o dato 0 seria para el nombre del programa

    } else {
        // Preguntar al usuario y solicita la informicon
        cout << ">> Bienvenido al Laberinto Maestro!" << endl;
        cout << "Ingresa el tamano del laberinto:" << endl;
        cout << "Filas (numeros impares funcionan mejor): ";
        cin >> filas;
        cout << "Columnas (numeros impares funcionan mejor): ";
        cin >> columnas;
        
        // Asegurar que sean numeros impares para mejor generacion
        if (filas % 2 == 0) filas++;        // el % devuelve el resto de una division
        if (columnas % 2 == 0) columnas++;  // si es igual a 0 es paar  si no impar
                                            // si es par entonces le suma 1, para volverlo impar
    }
    
    // Validar tamano minimo
    if (filas < 5) filas = 5;       // Si la fila es menor a 5, se pone 5 por defecto
    if (columnas < 5) columnas = 5; // Si la columna es menor a 5, se pone 5 por defecto
    
    return make_pair(filas, columnas); // Devuelve un par de enteros (x,y) con los calores de las filas y columnas
}                                      // make_pair es como una cajita de 2 compartimientos, se puede entregar completa y s=luego leer ambos valores            




// Estructura para representar una coordenada (fila, columna)

struct Punto { // Crea un molde para definir las posiciones con sus coordenadas en el laberinto (X ; Y)

    //(fila , columna)
    int  x, y;
    // Receta para crear un punto/coordenada del mapa
    Punto(int x = 0, int y = 0)  :  x(x), y(y) {} // (CONSTRUCTOR)
    // Sino se dan valores el      | Asigna los valores de X e Y a sus variables correspondientes(antes de meterlas en el mapa)
    // punto/coordenada sera (0,0)
    
    //----------------------------------------------------------------
    // Condicional que comparada si 2 puntos o coordenadas son iguales
    //----------------------------------------------------------------
    //(Son iguales cuando sus coordenadas x e y son idénticas)
    bool operator==(const Punto& otro) const {
        return x == otro.x && y == otro.y; // Aca se compara la coordenada "X" e "Y" con las de otro punto.
    }       

    // Dato importante: "(const Punto& otro) const" 
    // * "(const Punto& otro)" Va a recibir otro objeto como referencia
    // * "&" Significa no copio el objeto solo lo apunto (Es mas eficiente)
    // * "const" (al final) se asegura de que no se modifique el punto original
};




// CREACION VISUALIZACION Y RESOLUCION DEL LABERINTO 


class LaberintoMaestro { /*Este bloque define la clase LaberintoMaestro,
que es como un “molde/receta” con los datos que tiene el laberinto y qué puede hacer.*/

//----------------------------------------------------------------
// ATRIBUTOS PRIVADOS (Datos internos del laberinto)
//----------------------------------------------------------------

private:
    vector<vector<char>> laberinto; // Matriz que representa el mapa (Cada casilla puede ser un carácter "char")
//     ↑       ↑
//     |       └─ Cada fila es un vector de caracteres.
//     └─────────── El laberinto es un vector de filas.


    //--- Bloque de atributos privados de la clase  ---//

    int filas, columnas;    // Tamanho del laberinto.
    Punto entrada, salida;  // Coordenadas de Inicio/Fin.
    vector<Punto> solucion; // Lista de puntos que que guardan el camino desde la entrada a la salida.
    // Dados virtuales:
    mt19937 generador;      // Generador de numeros aleatorios (dados) ( Mersenne Twister)
    
    // Direcciones para moverse (arriba, derecha, abajo, izquierda)
    const vector<Punto> direcciones = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    //    | "Dame una caja donde pued guardar muchas coordenadas"

//----------------------------------------------------------------
// CONSTRUCTOR
//----------------------------------------------------------------

public:
    // Constructor  |Parametros    | Inicializamos directamente las variables de filas y columnas (El +2 es para compensar las capaz de muro)
    LaberintoMaestro(int f, int c) : filas(f + 2), columnas(c + 2) {

        //---------------------------------------------------
        // Inicializar generador de numero aleatorio con tiempo actual
        //---------------------------------------------------
//    |Dados . semilla|
        generador.seed(chrono::steady_clock::now().time_since_epoch().count()); 
            //  DATOS: * Libreria de tiempo
             //        * Reloj interno del pc
             //        * Momento actual
             //        * "epoch" es el punto de inicio de tiempo de las pc (generalmente 01/01/1970)
            //         * Calcula cuanto tiempo ha pasado desde ese momento hasta ahora
            //         * "count" convierte ese numero ramdons en entero

        
        
        
        // Rellena cada espacio del laberinto con "#"
        laberinto = vector<vector<char>>(filas, vector<char>(columnas, '#'));
                //  |Matriz<fila<lista.caracteres

        // Definir entrada y salida (considerando los bordes)
        entrada = Punto(1, 1);
        salida = Punto(filas - 2, columnas - 2);
    }
    

    
    //---------------------------------------------------------------------------
    // Verificar si una coordenada esta dentro del laberinto (sin contar bordes)
    //---------------------------------------------------------------------------

    bool esValida(int x, int y) const {
        return x >= 1 && x < filas - 1 && y >= 1 && y < columnas - 1;
    } // Si "X" no esta en la primera y ultima fila, y "Y" no esta en la primera y ultima columna entonces es valido
    


    //--------------------------------------------------------------------------
    // GENERADOR DEL LABERINTO QUE USA EL ALGORITMO DE BACKTRAKING (RETROCESO)
    //--------------------------------------------------------------------------

    // "Void" no devuelve nada la funcion
    void generarLaberinto() { // "Cerebro constructor del laberito"

        cout << ">> Generando laberinto..." << endl;
        
        // Stack para el algoritmo de backtracking
        stack<Punto> pila; // Crea una pila de coordenadas
        vector<vector<bool>> visitado(filas, vector<bool>(columnas, false));
        // Crea una matriz llamada "visitado" que marca si cada celda del laberinto fue visitada o no (Todas inician en False)

        
        // Empezar desde la entrada
        Punto actual = entrada;               // El punto de inicio es la entrda del laberintoq
        laberinto[actual.x][actual.y] = ' ';  // Lo marca como camino libre " "
        visitado[actual.x][actual.y] = true;  // Lo Marca como visitado con True
        pila.push(actual);                    // Lo mete en la pila para empezar a explorar desde ahi
        
        // Bucle que va a recorrer los caminos hasta que se acaben
        while (!pila.empty()) { // Mientras haya puntos/coordenadas en la pila seguimos explorando

            actual = pila.top(); // El explorador mira donde esta actualmente
            
            //------------------------------------------------------------------
            // Buscar vecinos no visitados (a distancia 2 para crear pasillos)
            //------------------------------------------------------------------

             // Crea una lista vacia llamada "vecinosLibres" para guardar coordenadas/puntos de las casillas vecinas
            vector<Punto> vecinosLibres;           
            
             // Bucle que recorre las direcciones de movimiento (⬆️ ⬅️ ⬇️ ➡️)
            for (const Punto& dir : direcciones) { //  No vamos a modificar dir (direccion), y solo lo tomaremos como referencia

                int nx = actual.x + dir.x * 2;     // Se toma la posicion actuaal (fila y columna), se le suma el movimiento (dir)
                int ny = actual.y + dir.y * 2;     // Y se le multiplica por 2 (para saltar una celdaa y crear un pasillo ancho)
                
                if (esValida(nx, ny) && !visitado[nx][ny]) {   // Verifica si la posicion es valida (no sale del laberinto) y si no ha sido visitada aun
                    vecinosLibres.push_back(Punto(nx, ny));    // Agrega entonces esa coordenadas a vecinos libres, pero al final de la lista (Para explorarla despues)
                }
            }
            

            if (!vecinosLibres.empty()) { // Si hay vecinos libres para moverse entonces... (El "!" cambia el valor de la condicion al contrario)
                                                            // .empy(si el contenedor esta vacio = True, sino False)
                //------------------------------
                // Elegir un vecino aleatorio
                //------------------------------

                // Generador de numeros aleatorios uniformes
                uniform_int_distribution<int> dist(0, vecinosLibres.size() - 1); // "dist(0, vecinosLibres.size() - 1)" rango en el que va a tomar el numero
                                                                                 // "vecinosLibres.size()" devuelve cuantos numeros hay guardados (se le resta uno por que vectores empieza en 0)

                Punto siguiente = vecinosLibres[dist(generador)]; // Se guarda el vecino elegido al azar en la variable "siguente" que va a ser de tipo punto
                
                // Crear camino hacia el vecino elegido
                int mx = actual.x + (siguiente.x - actual.x) / 2; // Se calcula la posicion (fila y columna) del punto intermedio del salto
                int my = actual.y + (siguiente.y - actual.y) / 2; // Que esta entre la posicion actual y la siguente a la que vamos a saltar
                
                // Bloque que marca el caamino del laberinto
                laberinto[mx][my] = ' ';                    // Habre el muro intermedio (Entre la posicion en donde estabamos y donde caemos)
                laberinto[siguiente.x][siguiente.y] = ' ';  // Habre la posicion donde caemos
                visitado[siguiente.x][siguiente.y] = true;  // Marca la celda como visitada
                
                pila.push(siguiente); // Guarda el camino (punto/coordenada) en una pila

            } else { // Sino
                // No hay vecinos libres, retroceder
                pila.pop(); // Con este retrocedemos a una posicion anterior para buscar vecinos no explorados
            }
        }
        
        // Asegurar que entrada y salida sean caminos libres
        laberinto[entrada.x][entrada.y] = 'E'; // Coloco el simbolo de entrad "E" en la coordenada/punto de la entrada
        laberinto[salida.x][salida.y] = 'S';   // Coloco el simbolo de la salida "S" en la coordenada/punto de la salida
        
        cout << "OK Laberinto generado exitosamente!" << endl; // Mensaje
    }



    //----------------------------------------------------------------
    // Resolver laberinto usando BFS (Busqueda en Anchura)
    //----------------------------------------------------------------
    bool resolverLaberinto() {
        cout << ">> Resolviendo laberinto..." << endl;
        
        queue<Punto> cola; // Aca se guardan las posiciones del laberinto que faltan por explorar en un formato de cola (supermercado)

        vector<vector<bool>> visitado(filas, vector<bool>(columnas, false)); // Sirve para verificar si ya pasamos por una coordenada/punto asignandole True o False
        vector<vector<Punto>> padre(filas, vector<Punto>(columnas, Punto(-1, -1))); // Guarda un objeto Punto (coordenada) que representa de dónde venimos
                                                        // Inicialmente todas las posiciones están en (-1, -1) eso indica “sin padre”
        
        cola.push(entrada); // Ponemos que el primer numero a visitar sea la coordenada de la entrada
        visitado[entrada.x][entrada.y] = true; // Y marcamos la entrada como visitada
        
        // Este bucle seguira explornado siempre que aun hayan datos guardados en la cola hasta acabarlos
        while (!cola.empty()) {
            Punto actual = cola.front(); // Saca el primer elemento que entro a la cola y aun no salio
            cola.pop(); // Y luego lo elimina (para que quede el siguente como el primero)
            
            // Llegamos a la salida?...
            if (actual == salida) { 

                // Reconstruir el camino
                solucion.clear(); // Se asegura de borrar cualquier contenido previo que pudiera quedar en solucion
                Punto temp = salida; // Crea una variable de coordenas y mete las coordenadas del camino hacia la salida
                // analogia: Es como empezar desde el final siguendo las migas de pan que dejaste
                // Dato: (temp-temporal) por que se va a usar de manera temporal para el calculo

                while (!(temp.x == -1 && temp.y == -1)) { // Va a repetir el bucle mientras la variable temp, no sea valor (-1, -1) (Que significa que ya no hay padre)
                    solucion.push_back(temp);      // Mete el punto/coordenad actual la variable solucion (.push_back(temp) = gregar un elemento al final del vector (temp).)
                    temp = padre[temp.x][temp.y];  // Nos da la celda anterior al punto donde estamos (padre es una matriz de puntos, temp es un punto de esa mtiz)
                        // Ej: Si llegamos a la celda (3,5) desde (2,5), entonces padre[3][5] = (2,5).
                }
                
                // Invertir para tener el camino desde entrada hasta salida
                reverse(solucion.begin(), solucion.end()); // Ivertimos el orden de los datos de la variable solucion para que vaya de la entrada a la salida (Por que la construimos alrevez)
                                                            // "solucion.begin()"" es el inicio del vector
                                                            // "solucion.end()"" es el final 

                cout << "EXITO! Laberinto resuelto! Camino encontrado." << endl;
                return true; // Indicamos que la funcion tuvo exito
            }
            
            // Explorar vecinos
            for (const Punto& dir : direcciones) { // Vamos a recorrer cada direcion a la que nos podemos mover en la selda actual
                
                // Caalculamos las coordenadas del vecino
                int nx = actual.x + dir.x;  // sumando respectivamente la fila y columna actual mas
                int ny = actual.y + dir.y;  // la fila y columna del proximo movimiento
                
                // VERIFICAMOS SI LA CELDA VECINA ES VALIDA (Si no esta fuera del laberinto, aun no la visitamos, y es un camino libro o la salida)
                if (esValida(nx, ny) && !visitado[nx][ny] && 
                    (laberinto[nx][ny] == ' ' || laberinto[nx][ny] == 'S')) { // Entonces...
                    
                    visitado[nx][ny] = true;  // La marcamos como visitadaa
                    padre[nx][ny] = actual;   // Guardmos de donde vinimos para reconstruir el camino luego
                    cola.push(Punto(nx, ny)); // Mete la selda vacia/valida a la cola del BFS para analiarla mas tarde
                }
            }
        }
        
        // SI EL LABERINTO LLEGA A NO TENER SOLUCION
        cout << "ERROR: No se encontro solucion al laberinto." << endl;
        return false;
    }
    




//----------------------------------------------------------------
// Funcion principal
//----------------------------------------------------------------

int main(int argc, char* argv[]) {
    // Datos: * int argc, argumentos pasados al programa desde la linea de comandos | cuantos ingredientes hay en una bolsa
    //        * char* argv[] cada posicion apunta  aun string                       | es la lista de ingredientes


    //----------------------------------------------------------------
    // BIENVENIDA PARA EL USUARIO CON IMPRESION DE TEXTO
    //----------------------------------------------------------------
    cout << ">> EL LABERINTO MAESTRO: Codigo, Estrategia y una Salida" << endl;
    cout << "=======================================================" << endl;
    
    //-- Obtener tamano del laberinto --//
    pair<int, int> tamano = obtenerTamano(argc, argv); // Llamamos a la funcion obtener tamano (que devolvera un pair (x,y))
    int filas = tamano.first;                          // Numero de filas     .first (accede al primer valor del pair)
    int columnas = tamano.second;                      // Numero de columnas  .second (accede al segundo valor del pair)
    

    // Mensaje
    cout << ">> Creando laberinto de " << filas << "x" << columnas << "..." << endl;
    return 0; // LE dice a c++ que todo salio bien y el programa termino su ejecucion
}