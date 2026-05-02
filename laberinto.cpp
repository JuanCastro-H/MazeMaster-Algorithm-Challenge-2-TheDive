// LIBRERIAS NECESARIAS PARA EL FUNCIONAMIENTO DEL PROGRAMA


// "#include" sirve para llamar librerias (Que son cajas de herramientas)
#include <iostream>   // Se encarga de la comunicacion con el usuario (Flujo de entrada y salida)
#include <vector>     // Sirve para crear contenedores dinamicos, permitiendo que su tamanho sea modificado durante la ejecucion del programa
// Le dice al programa cada vez que pida un martillo de la caja roja, no me hagas decir CajaRoja::Marillo. solo di martillo y me entiendes
#include <chrono>     // Es Como un cronometro digital que puede medir el tiempo de manera ultra precisa milisegundos o menos
#include <random>     // Es como una dado electronico, que sirve para obtener numeros aleatorios


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