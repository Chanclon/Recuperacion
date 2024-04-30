#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
using namespace std;

//constantes
int const LONG_CARRETERA = 10; //la modificaras a tu gusto, distancoa --> 0 hasta (Lng.crrtra -1)
int const MAX_PASOS = 3; // siempre sera positivo, en modo depuracion no habrá limit
int const TIEMPO_PARADO = 2; // tiempo que está parado un coche tras un pinchazo
bool const DEBUG = 0; // 0 --> normal, 1 --> depuración


//dibujos
const char CHAR_LINEA_HORIZONTAL = 205; // ˭
const char CHAR_ESQUINA_SUPERIOR_IZQUIERDA = 201; // ╔═
const char CHAR_ESQUINA_SUPERIOR_DERECHA = 187; // ═╗
const char CHAR_ESQUINA_INFERIOR_IZQUIERDA = 200; // ╚═
const char CHAR_ESQUINA_INFERIOR_DERECHA = 188; // ═╝
const char CHAR_LINEA_VERTICAL = 186; // ║
const char CHAR_COCHE = 254; // ■
const char CHAR_CLAVO = 193; // ┴
const char CHAR_SORPRESA = 63; // ?
const char CHAR_NORMAL = 32; // ‘ ‘

typedef enum { NORMAL, CLAVO, SORPRESA, COCHE }tTipoPosicion;
typedef tTipoPosicion  tCarretera[LONG_CARRETERA]; //tCarretera podra tener 0(normal), 1(clavo), 2(sorpresa), 3(coche)

bool cargaCarretera(tCarretera carretera);
tTipoPosicion stringToEnum(string s);
void iniciaCarril(tCarretera carretera);
void dibujaLineaHorizontalSuperior();
void dibujaLineaHorizontalInferior();
void dibujaCarril(const tCarretera carretera, int posCoche);
void dibujaCarretera(const tCarretera carretera, int posCoche);
int buscaSiguientePosSorpresa(const tCarretera carretera, int posCoche, int& n);
int buscaSorpresa(const tCarretera carretera, int desde, const int incr);
int avanza(int posCoche);
bool haLlegado(int posCoche);
void avanzaCarril(const tCarretera carretera, int& posCoche, int& tiempoParado);


bool cargaCarretera(tCarretera carretera)
{
	bool abierto = false;
	ifstream archivo;
	string nombre;
	cout << "Introduzca nombre archivo ";
	cin >> nombre;
	archivo.open(nombre);
	if (archivo.is_open()) //no se que es lo que leerá exactamente
	{
		string tipo = "";
		int cantidad;
		int n;
		while (tipo != "XX")
		{
			archivo >> tipo;
			tTipoPosicion posicion = stringToEnum(tipo);
			archivo >> cantidad;
			for (int i = 0; i < cantidad; i++)
			{
				archivo >> n;
				carretera[n] = posicion;
			}
		}
		archivo.close();
		abierto = true;
	}

	return abierto;
}

tTipoPosicion stringToEnum(string s)
{
	tTipoPosicion enumerado = NORMAL;
	if (s == "CLAVO") enumerado = CLAVO;
	else {
		if (s == "SORPRESA") enumerado = SORPRESA;
	}
	return enumerado;
}

void iniciaCarril(tCarretera carretera) { // pone todas las posiciones de la carretera a valor NORMAL.
	for (int i = 0; i < LONG_CARRETERA; i++)	carretera[i] = NORMAL;
}


void dibujaLineaHorizontalSuperior() /* dibuja la línea superior de la carretera, teniendo en cuenta la longitud de la misma. ╔══════════╗*/
{
	cout << CHAR_ESQUINA_SUPERIOR_IZQUIERDA;
	for (int i = 0; i < LONG_CARRETERA; i++) cout << CHAR_LINEA_HORIZONTAL;
	cout << CHAR_ESQUINA_SUPERIOR_DERECHA << endl;
}


void dibujaLineaHorizontalInferior() { /*dibuja la línea inferior de la carretera, teniendo en cuenta la longitud de la misma. ╚══════════╝*/
	cout << CHAR_ESQUINA_INFERIOR_IZQUIERDA;
	for (int i = 0; i < LONG_CARRETERA; i++) cout << CHAR_LINEA_HORIZONTAL;
	cout << CHAR_ESQUINA_INFERIOR_DERECHA;
	cout << "\n";
}


void dibujaCarril(const tCarretera carretera, int posCoche) { /*dibuja el contenido
del carril, es decir los clavos(┴), las sorpresas(? ) y el coche(■), que está en la posición posCoche.
El carril realmente ocupa dos líneas.En la primera se colocan las posiciones clavo y sorpresa, y en la
segunda línea se coloca el coche.De esta forma la visualización es más representativa.Observa
además que cada línea empieza y acaba con ǁ.El siguiente dibujo muestra el carril del archivo
“carriles.txt”, donde el coche está en la posición inicial.*/

	/*for (int s = 0; s < 2; s++) {
		cout << CHAR_LINEA_VERTICAL;
		switch (s)
		{
		case 0: for (int i = 0; i < LONG_CARRETERA - 2; i++) cout << i;
			break;

		case 1:
			for (int i = 0; i < LONG_CARRETERA - 2; i++) {
				if (carretera[i] == 0) cout << CHAR_NORMAL;
				else {
					if (carretera[i] == 1) cout << CHAR_CLAVO;
					else  cout << CHAR_SORPRESA;
				}
			}
		
			break;


		case 2:
			for (int i = 0; i < LONG_CARRETERA-2; i++)
			{
				if (posCoche == carretera[i]) cout << CHAR_COCHE;
				else cout << CHAR_NORMAL;
			}
			break;
		}
		cout << CHAR_LINEA_VERTICAL;
		cout << "\n";
	};
	*/
	cout << CHAR_LINEA_VERTICAL;
	for (int i = 0; i < LONG_CARRETERA ; i++) cout << i;
	cout << CHAR_LINEA_VERTICAL << endl<<CHAR_LINEA_VERTICAL;
	for (int i = 0; i < LONG_CARRETERA ; i++) {
		if (carretera[i] == CLAVO) cout << CHAR_CLAVO;
		else {
			if (carretera[i] == SORPRESA) cout << CHAR_SORPRESA;
			else cout << CHAR_NORMAL;
		}
	}
	cout << CHAR_LINEA_VERTICAL << endl << CHAR_LINEA_VERTICAL;
	for (int i = 0; i < LONG_CARRETERA ; i++)
	{
		if (i == posCoche) cout << CHAR_COCHE;
		else cout << CHAR_NORMAL;
	}
	cout << CHAR_LINEA_VERTICAL << endl;
}


bool esSorpresa(const tCarretera carretera, int posCoche) { /*devuelve true sí y sólo sí la posición posCoche es una posición sorpresa en la carretera.*/
	bool sorpresa = false;
	if (carretera[posCoche] == SORPRESA) sorpresa = true;
	return sorpresa;
}


bool esClavo(const tCarretera carretera, int posCoche) { /*devuelve true sí y sólo sí la posición posCoche contiene un clavo en la carretera.*/
	bool clavo = false;
	if (carretera[posCoche] == CLAVO) clavo = true;
	return clavo;
}


void dibujaCarretera(const tCarretera carretera, int posCoche) { /*utiliza las funciones anteriores para dibujar la carretera completa*/
	dibujaLineaHorizontalSuperior();
	dibujaCarril(carretera, posCoche);
	dibujaLineaHorizontalInferior();
}


int buscaSiguientePosSorpresa(const tCarretera carretera, int posCoche, int& n) {
	/*genera un número aleatorio entre 0 y 1 (ambos incluidos).Si sale 0, entonces busca la siguiente
	posición sorpresa y la devuelve.Si sale 1, busca la posición sorpresa anterior y la devuelve.En caso
	de no existir en el avance o retroceso, posición sorpresa, entonces devuelve 0. Para implementar
	esta función, implementa la función buscaSorpresa*/
	srand(time(NULL));
	n = rand() % 2;
	if (n == 0) n = -1;
	int siguientesorpresa = buscaSorpresa(carretera, posCoche, n);
	return siguientesorpresa;
}
int buscaSorpresa(const tCarretera carretera, int desde, const int incr) {/*que busca una posición sorpresa en la carretera, empezando desde la posición
	desde, en la dirección marcada por incr.El parámetro incr será 1 o - 1. El valor 1 indica avance
	y - 1 retroceso.Tanto si se avanza como si se retrocede, asegúrate de respetar las dimensiones de
	la carretera*/
	int possorpresa = -1; // si es igual a -1 no hay sorpresa
	while (desde < LONG_CARRETERA && desde > 0)
	{
		if (esSorpresa(carretera, desde)) possorpresa = desde;
		desde += incr;
	}
	return possorpresa;
}


int avanza(int posCoche) { /* dada la posición actual del coche posCoche, devuelve la nueva
		posición del coche tras el avance.Si estamos en el modo normal, entonces se genera un número
		aleatorio entre 1 y MAX_PASOS, ambos valores incluidos, que se suma a la posición actual del coche
		posCoche, devolviendo dicho valor como resultado.En el modo depuración, el número de pasos a
		avanzar lo introduce el usuario, de forma que, si al calcular la nueva posición del coche nos sale un
		valor negativo, entonces se devuelve 0. En ambos modos, si al avanzar el coche sobrepasa la meta,
		se devuelve como posición del coche LONG_CARRETERA*/
	int ncasillas;
	if (DEBUG)
	{
		cout << "Por favor introduzca el numero de pasos que desea avanzar o retroceder";
		cin >> ncasillas;
		ncasillas += posCoche;
		if (ncasillas < 0) ncasillas = 0;
	}
	else {
		srand(time(NULL));
		cout << "Por favor presione enter para continuar";

		ncasillas = posCoche + rand() % 1 + MAX_PASOS;
		cout << "Has avanzado " << ncasillas;
	}
	if (ncasillas > LONG_CARRETERA) ncasillas = LONG_CARRETERA;
	return ncasillas;
}


bool haLlegado(int posCoche) {
	return (posCoche >= LONG_CARRETERA);
}/*devuelve true sí y sólo sí posCoche es mayor o igual aLONG_CARRETERA.*/


void avanzaCarril(const tCarretera carretera, int& posCoche, int& tiempoParado) { /*Dibuja la carretera.
		Si el coche no está parado por un pinchazo, se calcula su avance(utilizando la función avanza).
		Seguidamente, se analiza la nueva posición alcanzada.Si se ha llegado a la meta, se dibuja de nuevo la carretera.
		En otro caso, si ha caído en una sorpresa, se dibuja la carretera, y la posición del coche pasa a ser la siguiente posición sorpresa,
		que se calcula invocando a buscaSiguientePosSorpresa.Si ha caído en un clavo, se actualiza su tiempo
		parado al valor de la constante TIEMPO_PARADO.Hay que informar al usuario de lo que va
		ocurriendo.Mira los ejemplos que aparecen al final.*/

	dibujaCarretera(carretera, posCoche);
	if (tiempoParado > 0) cout << "Tienes que esperar " << tiempoParado << " turnos";
	else {
		cout << "Presiona enter para continuar";
		cin.ignore();
		cin.get();
		posCoche = avanza(posCoche);
		

		if (posCoche == LONG_CARRETERA) {
			dibujaCarretera(carretera, posCoche);
			cout << "HAS GANADO" << endl;

		}
		else if (esSorpresa(carretera, posCoche))
		{
			int sentido;
			cout << "HAS CAIDO EN UNA SORPRESA, PASAS A LA SIGUIENTE SORPRESA" << endl;
			posCoche = buscaSiguientePosSorpresa(carretera, posCoche, sentido);

			if (sentido == 1) cout << "Avanzando a la siguiente sorpresa" << endl;
			else cout << "Retrocediendo a la anterior sorpresa" << endl;

			dibujaCarretera(carretera, posCoche);
		}
		else if (esClavo(carretera, posCoche)){
			cout << "Has pinchado, estarás " << TIEMPO_PARADO << " sin moverte";
			tiempoParado = TIEMPO_PARADO;
		}
	}


}


void iniciaCoche(int& posCoche, int& tiempoParado) /*pone a valor 0 ambos parámetros.*/
{
	posCoche = 0;
	tiempoParado = 0;
}

void simulaCarrera(const tCarretera carretera) { /*inicia la posición del coche y su
tiempo parado a 0, utilizando la función anterior.A continuación, mientras no llegue el coche a la
meta, invoca a la función avanzaCarril.Cuando termina el bucle, manda un mensaje indicando
que la simulación ha terminado*/
	int posCoche;
	int Tiempoparao;
	iniciaCoche(posCoche, Tiempoparao);
	while (!haLlegado(posCoche))
	{
		avanzaCarril(carretera, posCoche, Tiempoparao);
	}
	cout << "SIMULACION TERMINADA";
}

int main() /*Carga la configuración de la carretera de un archivo de texto, cuyo nombre se pide por consola. Si la carga
ha sido correcta, se invoca a simulaCarrera para realizar la simulación. Una vez terminada la simulación,
se pregunta al usuario si desea ejecutar una nueva simulación. En caso afirmativo se repite el proceso. En
otro caso el programa termina.*/
{
	tCarretera carretera;

	while (!cargaCarretera(carretera)) cout << "Nombre del archivo no reconocido" << endl;

	simulaCarrera(carretera);

}
