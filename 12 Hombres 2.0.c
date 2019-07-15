#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define BUFFER 7 //Tamaño del tablero
#define FICHAS 12 //Numero de fichas
#define PROFUNDIDAD 5 //Profundidad limitada del arbol
#define BUFFER_MAX 99 //BUFFER Maximo en recorrido

//Tablero principal
char tablero[BUFFER][BUFFER];

//Bandera que activa la evaluacion requerida
int modo_evaluacion;

//Punteros a registro de fichas y juego
struct fichas *humano, *pc, *humano2;

//Arbol
struct nodo_arbol{
	//El valor de la jugada
	int ans;
	//Las dimensiones de la jugada
	int num;
	int let;
	//Las dimensiones de la ficha a mover
	int num2;
	int let2;
	//Bandera que indica si el nodo posee una jugada terminal
	int terminal;
	struct nodo_arbol *izq; //Hijo izquierdo
	struct nodo_arbol *der; //Hermanos del hijo izquierdo
	struct nodo_arbol *sgte; //Hermano siguiente
	char tablero_padre[BUFFER][BUFFER]; //Tablero del padre
};

//Fichas
struct fichas{
	int fichas;
	int capturadas;
	int valor_colocacion;
	int valor_ficha;
	int ficha_contrario;
	int fase;
	int rondas;
	int fichas_en_tab;
	int bandera_ultimajugada;
	char nombre[10];
};

//Inicio del tablero
void inicializar(){
	system("cls");
	printf("\n\n\n\n\tLOS 12 HOMBRES - MENU PRINCIPAL\n\n\n");
	printf("\n\n \t\t 1. Iniciar Juego HUM v.s PC\n\n \t\t 2. Iniciar Juego PC v.s HUM ");
	printf("\n\n \t\t 3. Iniciar Juego HUM v.s HUM \n\n \t\t 4. Reglas del juego \n\n \t\t 5. Cambiar funcion de evaluacion \n\n \t\t 0. Salir");
	printf(" \n\n \t\t >");
	int i,j;
	//Cerar el tablero
	for(i=0; i<BUFFER; i++){
		for(j=0; j<BUFFER; j++){
			tablero[i][j] = 0;
		}

		//Conocer las posiciones restringidas

		tablero[0][1] = '#';
		tablero[0][2] = '#';
		tablero[0][4] = '#';
		tablero[0][5] = '#';

		tablero[1][0] = '#';
		tablero[1][2] = '#';
		tablero[1][4] = '#';
		tablero[1][6] = '#';

		tablero[2][0] = '#';
		tablero[2][1] = '#';
		tablero[2][5] = '#';
		tablero[2][6] = '#';

		tablero[3][3] = '#';

		tablero[4][0] = '#';
		tablero[4][1] = '#';
		tablero[4][5] = '#';
		tablero[4][6] = '#';

		tablero[5][0] = '#';
		tablero[5][2] = '#';
		tablero[5][4] = '#';
		tablero[5][6] = '#';

		tablero[6][1] = '#';
		tablero[6][2] = '#';
		tablero[6][4] = '#';
		tablero[6][5] = '#';

	}
	
	humano = (struct fichas *) malloc (sizeof(struct fichas));
	humano2 = (struct fichas *) malloc (sizeof(struct fichas));
	pc = (struct fichas *) malloc (sizeof(struct fichas));
	humano->fichas = pc->fichas = humano2->fichas = FICHAS;
	humano->capturadas = pc->capturadas = humano2->capturadas = 0;
	pc->fichas_en_tab = 0;
	humano->bandera_ultimajugada = humano2->bandera_ultimajugada = pc->bandera_ultimajugada = 0;
	
	strncpy( pc->nombre, "PC", 3 );
	strncpy( humano->nombre, "HUM", 3 );
}

//Imprimir el tablero en consola
void imprimir_tablero(){
	int i,j,k;
	int a = 65;
	printf("\n\n\n\n\t");
	for(i=0; i<BUFFER; i++, a++){
		printf("\t  %c  ", a);
	}
	printf("\n\n");
	for(i=0; i<BUFFER; i++){
		for(j=0; j<BUFFER; j++){
			if(tablero[i][j] == '#'){
				if(j==0){
					printf("\t\t  |--");
				}else{
					if(j==6){
						printf("-----|   %d", j - i + 1);
					}else{
						printf("-----|--");
					}
				}
			}else{
				if(j==0){
					printf("\t\t |%c|-", tablero[i][j]);
				}else{
					if(j==6){
						printf("----|%c|  %d", tablero[i][j], j - i + 1);
					}else{
						printf("----|%c|-", tablero[i][j]);
					}

				}
			}
		}
		printf("\n");
		if(i<BUFFER-1){
			for(k=0; k<BUFFER; k++){
				if(k==0){
					printf("\t\t  |  ");
				}else{
					printf("     |  ");
				}
			}
		}
		printf("\n");
		if(i<BUFFER-1){
			for(k=0; k<BUFFER; k++){
				if(k==0){
					printf("\t\t  |  ");
				}else{
					printf("     |  ");
				}
			}
		}
		printf("\n");
	}
	printf("");
}

//Imprimir un tablero sin detalles graficos
void imprimir_matriz(char tab[BUFFER][BUFFER]){
	int i, j;
	printf("\n\n\n\n");
	for(i=0; i<BUFFER; i++){
		for(j=0; j<BUFFER; j++){
			printf(" %d ", tab[i][j]);
		}
		printf("\n");
	}
}

//Copia un tablero de original a imagen
void copiar_tablero(char tablero_original[BUFFER][BUFFER], char tablero_imagen[BUFFER][BUFFER]){
	int i, j;
	for(i=0; i<BUFFER; i++){
		for(j=0; j<BUFFER; j++){
			tablero_imagen[i][j] = tablero_original[i][j];
		}
	}
}

//Funciones de impresion del arbol
void imprimir_arbol(struct nodo_arbol *ptr){
	static int con = 0;
	while(ptr != NULL){
		if(ptr->terminal == 1){
		//printf("Hijo %d:", con++);
		//printf("%d,%d", ptr->num, ptr->let);
			imprimir_matriz(ptr->tablero_padre);
		}
		imprimir_arbol(ptr->izq);
		ptr = ptr->sgte;	
	}
	
}

//Imprime la jugada de los hijos
void imprimir_hijos(struct nodo_arbol *ptr){
	int con = 1;
	printf("Hijo %d: ", con++);
	printf("%d,%d\n", ptr->izq->num, ptr->izq->let);
	ptr = ptr->der;
	while(ptr != NULL){
		printf("Hijo %d: ", con++);
		printf("%d,%d\n", ptr->num, ptr->let);
		ptr = ptr->sgte;
	}
}

//Imprime la jugada de los hijos
void imprimir_hijos_ans(struct nodo_arbol *ptr){
	int con = 1;
	printf("Hijo %d ans: ", con++);
	printf("%d\n", ptr->izq->ans);
	//imprimir_matriz(ptr->tablero_padre);
	ptr = ptr->izq->sgte;
	while(ptr != NULL){
		printf("Hijo %d ans: ", con++);
		printf("%d\n", ptr->ans);
		//imprimir_matriz(ptr->tablero_padre);
		ptr = ptr->sgte;
	}
}

//Imprime el tablero de los hijos
void imprimir_hijos_tableros(struct nodo_arbol *ptr){
	int con = 1;
	struct nodo_arbol *hijo;
	hijo = ptr->izq;
	while(hijo != NULL){
		printf("Hijo %d: ", con++);
		imprimir_matriz(hijo->tablero_padre);
		printf("\n");
		hijo = hijo->sgte;
	}
}

//Imprime el tablero de juego del ultimo hijo
void imprimir_ultimo_hijo(struct nodo_arbol *ptr){
	int ban = 0;
	
	while(!ban){
		if(ptr->sgte == NULL){
			ptr = ptr->izq;
		}else{
			ptr = ptr->sgte;
		}
		if(ptr->izq == NULL && ptr->sgte == NULL){
			ban = 1;
		}
	}
	printf("\nUltimo hijo: ");
	imprimir_matriz(ptr->tablero_padre);
	printf("\nValor: %d", ptr->ans);
	printf("\n");

}

//Funcion que coloca una ficha en el tablero
int colocar_ficha(int jugador, int let, int num){
	if(let < 0 || num < 0){
		return(0);
	}
	if(jugador){
		//Humano
		if(tablero[num][let]){
			//Error, lugar ocupado o invalido
			printf("\n\n\tERROR: Posicion invalida\n\n");
			system("pause");
			return(0);
		}else{
			tablero[num][let] = 'O';
			return(1);
		}
	}else{
		//Pc o humano2
		if(tablero[num][let]){
			//Error, lugar ocupado o invalido
			printf("\n\n\tERROR: Posicion invalida\n\n");
			system("pause");
			return(0);
		}else{
			tablero[num][let] = 'X';
			return(1);
		}
	}
}

//Funcion que mueve la ficha de un lugar a otro
int mover_ficha(int jugador, int let, int num, int nuevo_let, int nuevo_num, int modo, int volar){
	int i, saltos, ban = 1;
	
	//Si se quiere volar no se verifica nada
	if(volar){
		if(jugador){
			//Humano
			if(tablero[nuevo_num][nuevo_let]){
				//Error, lugar ocupado o invalido
				return(0);
			}else{
				if(modo){
					tablero[nuevo_num][nuevo_let] = 'O';
					tablero[num][let] = 0;
				}
				return(1);
			}
		}else{
			//Pc o humano2
			if(tablero[nuevo_num][nuevo_let]){
				//Error, lugar ocupado o invalido
				return(0);
			}else{
				if(modo){
					tablero[nuevo_num][nuevo_let] = 'X';
					tablero[num][let] = 0;
				}
				return(1);
			}
		}
	}
	
	//Si se quiere mover en fila
	//Cuento cuatas casillas intermedias hay, y todas deben ser '#'
	if(num == nuevo_num){
		if(let < nuevo_let){
			saltos = nuevo_let - let;
			for(i = 1; i < saltos; i++){
				if(tablero[num][let + i] != '#'){
					return 0;
				}
				if(num == 3 && let + i == 3){
					return 0;
				}
			}
		}else{
			saltos = let - nuevo_let;
			for(i = 1; i < saltos; i++){
				if(tablero[num][nuevo_let + i] != '#'){
					return 0;
				}
				if(num == 3 && nuevo_let + i == 3){
					return 0;
				}
			}
		}
	}

	//Si se quiere mover en columna
	//Cuento cuatas casillas intermedias hay, y todas deben ser '#'
	if(let == nuevo_let){
		if(num < nuevo_num){
			saltos = nuevo_num - num;
			for(i = 1; i < saltos; i++){
				if(tablero[num + i][let] != '#'){
					return 0;
				}
				if(num + i == 3 && let == 3){
					return 0;
				}
			}
		}else{
			saltos = num - nuevo_num;
			for(i = 1; i < saltos; i++){
				if(tablero[nuevo_num + i][let] != '#'){
					return 0;
				}
				if(nuevo_num + i == 3 && let == 3){
					return 0;
				}
			}
		}
	}

	//Si se quiere mover en diagonal
	//Cuento cuatas casillas intermedias hay, y todas deben ser '#'
	//Si restadas las dimensiones da 0, o sumadas da 7, estas en una de las cuatro diagonales
	if(num != nuevo_num && let != nuevo_let){
		ban = 0;
		if((let + num == 6) && (nuevo_let + nuevo_num == 6)){
			ban = 1;
			if(num < nuevo_num){
				int saltos_num = nuevo_num - num;
				int saltos_let = let - nuevo_let;
				if(saltos_num != 1 && saltos_let != 1){
					return 0;
				}
			}else{
				int saltos_num = num - nuevo_num;
				int saltos_let = nuevo_let - let;
				if(saltos_num != 1 && saltos_let != 1){
					return 0;
				}
			}
		}else{
			if((let - num == 0) && (nuevo_let - nuevo_num == 0)){
				ban = 1;
				if(num < nuevo_num){
					int saltos_num = nuevo_num - num;
					int saltos_let = nuevo_let - let;
					if(saltos_num != 1 && saltos_let != 1){
						return 0;
					}
			}else{
					int saltos_num = num - nuevo_num;
					int saltos_let = let - nuevo_let;
					if(saltos_num != 1 && saltos_let != 1){
						return 0;
					}
				}
			}
		}
	}

	if(ban == 0){
		return 0;
	}

	if(jugador){
		//Humano
		if(tablero[nuevo_num][nuevo_let]){
			//Error, lugar ocupado o invalido
			return(0);
		}else{
			if(modo){
				tablero[nuevo_num][nuevo_let] = 'O';
				tablero[num][let] = 0;
			}
			return(1);
		}
	}else{
		//Pc o humano2
		if(tablero[nuevo_num][nuevo_let]){
			//Error, lugar ocupado o invalido
			return(0);
		}else{
			if(modo){
				tablero[nuevo_num][nuevo_let] = 'X';
				tablero[num][let] = 0;
			}
			return(1);
		}
	}
}

//Devuelve la posicion de columna
int columna(char let){
	switch(let){
		case 'a':
			return(0);
		case 'A':
			return(0);
		case 'b':
			return(1);
		case 'B':
			return(1);
		case 'c':
			return(2);
		case 'C':
			return(2);
		case 'd':
			return(3);
		case 'D':
			return(3);
		case 'e':
			return(4);
		case 'E':
			return(4);
		case 'f':
			return(5);
		case 'F':
			return(5);
		case 'g':
			return(6);
		case 'G':
			return(6);
		default:
			printf("\n\n\tERROR: Columna invalida\n\n");
			system("pause");
			return(-1);
	}
}

//Devuelve la posicion de fila
int fila(char num){
	switch(num){
		case '1':
			return(6);
		case '2':
			return(5);
		case '3':
			return(4);
		case '4':
			return(3);
		case '5':
			return(2);
		case '6':
			return(1);
		case '7':
			return(0);
		default:
			printf("\n\n\tERROR: Fila invalida\n\n");
			system("pause");
			return(-1);
	}
}

//Verifica si la ficha esta en mil
int verificar_mil(int jugador, int let, int num){
	int i = 0, con = 0;

	//Control en columna
		//Control especial
		if(let == 3){
			if(num == 0 || num == 1 || num == 2){
				for(i=0, con = 0; i<3; i++){
					if(tablero[i][let] == jugador){
						con++;
					}
				}
				//Hizo mil
				if(con == 3){
					return(1);
				}	
			}else{
				for(i=4, con = 0; i<BUFFER; i++){
					if(tablero[i][let] == jugador){
						con++;
					}
				}
				//Hizo mil
				if(con == 3){
					return(1);
				}
			}
		}else{
			//Control normal
			for(i=0, con = 0; i<BUFFER; i++){
				if(tablero[i][let] == jugador){
					con++;
				}
			}
			//Hizo mil
			if(con == 3){
				return(1);
			}
		}

	//Control en fila
		//Control especial
		if(num == 3){
			if(let == 0 || let == 1 || let == 2){
				for(i=0, con = 0; i<3; i++){
					if(tablero[num][i] == jugador){
						con++;
					}
				}
				//Hizo mil
				if(con == 3){
					return(1);
				}
			}else{
				for(i=4, con = 0; i<BUFFER; i++){
					if(tablero[num][i] == jugador){
						con++;
					}
				}
				//Hizo mil
				if(con == 3){
					return(1);
				}
			}
		}else{
			//Control normal
			for(i=0, con = 0; i<BUFFER; i++){
				if(tablero[num][i] == jugador){
					con++;
				}
			}
			//Hizo mil
			if(con == 3){
				return(1);
			}
		}

	//Control en diagonales
	//Si restadas las dimensiones da 0, o sumadas da 6, estas en una de las cuatro diagonales a verificar
	if(let + num == 6){
		if(tablero[0][6] == jugador
			&&tablero[1][5] == jugador
				&&tablero[2][4] == jugador
					&& (num == 0 || num == 1 || num == 2)){
			return(1);
		}
		if(tablero[6][0] == jugador
			&&tablero[5][1] == jugador
				&&tablero[4][2] == jugador
					&& (num == 4 || num == 5 || num == 6)){
			return(1);
		}
	}else{
		if(let - num == 0){
			if(tablero[0][0] == jugador
				&&tablero[1][1] == jugador
					&&tablero[2][2] == jugador
						&& (let == 0 || let == 1 || let == 2)){
				return(1);
			}
			if(tablero[6][6] == jugador
				&&tablero[5][5] == jugador
					&&tablero[4][4] == jugador
						&& (let == 4 || let == 5 || let == 6)){
				return(1);
			}
		}
	}

	//Retorna falso, no se hizo mil
	return(0);
}

//Verifica si todas las fichas estan en mil
int verificar_mil_todas(int jugador){
	int i, j;
	for(i = 0; i<BUFFER; i++){
		for(j = 0; j<BUFFER; j++){
			if(tablero[i][j] == jugador){
				//Si la ficha no esta en mil, retorna falso
				if(!verificar_mil(jugador, j, i)){
					return 0;
				}
			}
		}
	}
	//Todas las fichas estan en mil
	return 1;
}

//Verifica si la ficha posee movimientos validos
int verificar_movimiento(int jugador, int let, int num){
	int i, j, ficha;
	//Si selecciono humano
	if(jugador){
		ficha = 79;
		//Verifica si selecciono su ficha
		if(tablero[num][let] != ficha){
			return 0;
		}
	}else{
		ficha = 88;
		//Verifica si selecciono su ficha
		if(tablero[num][let] != ficha){
			return 0;
		}
	}
		
	//Verifico si puedo mover esa ficha seleccionada	
	for(i = 0; i<BUFFER; i++){
		for(j = 0; j<BUFFER; j++){
			if(tablero[i][j] == 0){
				if(mover_ficha(jugador, let, num, j, i, 0,0)){
					return 1;		
				}
			}
		}
	}
	
	//La ficha no se puede mover
	printf("\n\tLa ficha seleccionada es invalida\n");
	return 0;
}

//Verifica si todas las fichas poseen movimientos validos 
int verificar_movimiento_todas(int jugador){
	int i, j, nuevo_i, nuevo_j, ficha;
	if(jugador){
		ficha = 79;
	}else{
		ficha = 88;
	}
	//Busco todas las fichas de jugador
	for(i = 0; i<BUFFER; i++){
		for(j = 0; j<BUFFER; j++){
			if(tablero[i][j] == ficha){
				//Buscamos todos los lugares vacios y vemos si puede mover ahi
				for(nuevo_i = 0; nuevo_i<BUFFER; nuevo_i++){
					for(nuevo_j = 0; nuevo_j<BUFFER; nuevo_j++){
						if(tablero[nuevo_i][nuevo_j] == 0){
							//Si una ficha tiene un moviminto valido, retorna verdadero
							if(mover_ficha(jugador, j, i, nuevo_j, nuevo_i, 0,0)){
								return 1;
							}
						}
					}
				}
			}
		}
	}
	//Todas las fichas estan bloqueadas
	return 0;
}

//Funcion que pide seleccionar una ficha para mover
int pedir_posicion(struct fichas *p, char *nueva_pos, char *pos){
	system("cls");
	system("cls");
	printf("\n\n\n \tLOS 12 HOMBRES\n\n \t%s selecciono la ficha en %c%c\n", p->nombre, pos[0], pos[1]);
	printf("\n\n\n\n");
	imprimir_tablero();
	printf("\n\tEliga una posicion para mover esa ficha (ColumnaFila) -> ");
	scanf("%s", nueva_pos);
	//Funcion que coloca la ficha en su nueva posicion, si la nueva posicion no es valida, pide de nuevo
	
	if(mover_ficha(p->valor_colocacion, columna(pos[0]), fila(pos[1]), columna(nueva_pos[0]), fila(nueva_pos[1]), 1, 0)){
		return 1;
	}else{
		//Vuelvo a pedir
		printf("\n\n\tERROR POSICION INVALIDA\n\n");
		system("pause");
		pedir_posicion(p, nueva_pos, pos);
	}
	return 1;
}

//Funcion que pide seleccionar una ficha para mover
int pedir_posicion_volar(struct fichas *p, char *nueva_pos, char *pos){
	system("cls");
	printf("\n\n\n \tLOS 12 HOMBRES\n\n \t%s selecciono la ficha en %c%c\n", p->nombre, pos[0], pos[1]);
	printf("\n\n\n\n");
	imprimir_tablero();
	printf("\n\tEliga una posicion para mover esa ficha (ColumnaFila) -> ");
	scanf("%s", nueva_pos);
	int i = fila(pos[1]); columna(pos[0]);
	int j = columna(pos[0]);
	int i2 = fila(nueva_pos[1]);
	int j2 = columna(nueva_pos[0]);
	//Funcion que coloca la ficha en su nueva posicion, si la nueva posicion no es valida, pide de nuevo
	if( tablero[i2][j2] == 0){
		printf("\n\n\n MOVER A  %c%c\n", p->nombre, i2, j2);
		tablero[i2][j2] = p->valor_ficha;
		tablero[i][j] = 0;
		return 1;
	}else{
		//Vuelvo a pedir
		printf("\n\n\tERROR POSICION INVALIDA\n\n");
		system("pause");
		pedir_posicion(p, nueva_pos, pos);
	}
	return 1;
}

//Funcion que verifica si existe un ganador
int verificar_ganador(struct fichas *humano, struct fichas *humano2, int jugadas_finales){
	//Si alguno captura 10 fichas, gana
	//Si ambos repitieron la jugada voladora 10 veces, empate
	if(jugadas_finales == 20){
		system("cls");
		printf(" EMPATE! \n\n");
		printf("\n\n\n\tFIN DEL JUEGO \n\n\n");
		imprimir_tablero();
		system("pause");
		return 1;
	}
	if(humano->capturadas == FICHAS - 2 || humano2->capturadas == FICHAS - 2){
		system("cls");
		printf("\n\n\n\t%s", humano->capturadas==FICHAS-2?humano->nombre:humano2->nombre);
		printf(" GANA LA PARTIDA\n\n");
		printf("\n\n\n\tFIN DEL JUEGO \n\n\n");
		imprimir_tablero();
		system("pause");
		return 1;
	}else{
		return 0;
	}
	return 0;
}

//Funcion que pide seleccionar una ficha para sacar una vez hecho un mil
void sacar_ficha(struct fichas *p){
	char pos[3];
	system("cls");
	printf("\n\n\n \tLOS 12 HOMBRES\n\n \tTurno %s [%c]\n\n \tHizo MIL!\n", p->nombre, p->valor_ficha);
	printf("\n\n");
	imprimir_tablero();
	printf("\n\tEliga una ficha del contrario para capturar (ColumnaFila) -> ");
	scanf("%s", pos);
	//Si todas las fichas estan en mil, saca
	//Si la ficha seleccionada no esta en mil, saca
	if( (verificar_mil_todas(p->ficha_contrario))||
	(!(verificar_mil(p->ficha_contrario, columna(pos[0]), fila(pos[1]))) && !(verificar_mil_todas(p->ficha_contrario))) ){
		if(tablero[fila(pos[1])][columna(pos[0])] == p->ficha_contrario){
			tablero[fila(pos[1])][columna(pos[0])] = 0;
			system("cls");
			printf("\n\n\n \tLOS 12 HOMBRES\n\n \tTurno %s [%c]\n\n \tFicha extraida en %c%c\n", p->nombre, pos[0], pos[1]);
			printf("\n\n");
			p->capturadas++;
			//Verificamos si el jugador saco una ficha en la ultima jugada de la 1 fase
			//Si ambos no tinen fichas, es la ultima jugada
			if(!humano->fichas && !humano2->fichas){
				humano2->bandera_ultimajugada = 1;
			}
			imprimir_tablero();
			system("pause");
		}else{
			printf("\n\n\tERROR POSICION INVALIDA\n\n");
			system("pause");
			sacar_ficha(p);
		}
	}else{
		printf("\n\n\tERROR FICHA CONTRARIA EN MIL\n\n");
		system("pause");
		sacar_ficha(p);
	}
}

//Funcion principal para colocar una ficha
void coloca_humano(struct fichas *p){
	char pos[3];
	system("cls");
	printf("\n\n\n \tLOS 12 HOMBRES\n\n \t\t\t\t\t\t\tTurno %s [%c]\n\n \tPosee %d fichas restantes", p->nombre, p->valor_ficha, p->fichas);
	printf("\n\n \tCapturo %d fichas del contrario \n", p->capturadas);
	imprimir_tablero();
	printf("\n\tEliga una posicion para insertar (ColumnaFila) -> ");
	scanf("%s", pos);
	fflush(stdin);
	//Si la posicon esta disponible, inserta ahi
	if(colocar_ficha(p->valor_colocacion, columna(pos[0]), fila(pos[1]))){
		system("cls");
		printf("\n\n\n \tLOS 12 HOMBRES\n\n\n \t%s coloco una ficha en %c%c\n", p->nombre, pos[0], pos[1]);
		printf("\n\n\n");
		p->fichas--;
		imprimir_tablero();
		system("pause");
		//Si la ficha hizo mil, saca una del contrario
		if(verificar_mil(p->valor_ficha, columna(pos[0]), fila(pos[1]))){
			//Llama a funcion seleccionar una ficha para sacar
			sacar_ficha(p);
		}
	}else{
		coloca_humano(p);
	}
}

//Funcion principal para mover una ficha
void mueve_humano(struct fichas *p){
	char pos[3];
	char nueva_pos[3];
	system("cls");
	printf("\n\n\n \tLOS 12 HOMBRES\n\n \t\t\t\t\t\t\tTurno %s [%c]", p->nombre, p->valor_ficha);
	printf("\n\n \tCapturo %d fichas del contrario \n\n\n", p->capturadas);
	imprimir_tablero();
	printf("\n\tEliga una ficha para mover (ColumnaFila) -> ");
	scanf("%s", pos);
	fflush(stdin);
	//Selecciona una ficha suya y que se pueda mover
	if(verificar_movimiento(p->valor_colocacion, columna(pos[0]), fila(pos[1]))){
		system("cls");
		printf("\n\n\n \tLOS 12 HOMBRES\n\n\n \t%s selecciono la ficha en %c%c\n", p->nombre, pos[0], pos[1]);
		printf("\n\n\n");
		imprimir_tablero();
		//Selecciono una posicion adyacente y valida, luego muevo
		if(pedir_posicion(p, nueva_pos, pos)){
			system("cls");
			printf("\n\n\n \tLOS 12 HOMBRES\n\n \t%s movio su ficha de %c%c a %c%c\n", p->nombre, pos[0], pos[1], nueva_pos[0], nueva_pos[1]);
			printf("\n\n\n\n");
			imprimir_tablero();
			system("pause");
			//Si la ficha movida forma mil, saca ficha del contrario
			if(verificar_mil(p->valor_ficha, columna(nueva_pos[0]), fila(nueva_pos[1]))){
				sacar_ficha(p);
			}
		}else{
			//Si no, vuelvo a pedir
			printf("\n\n\tERROR POSICION INVALIDA\n\n");
			system("pause");
			mueve_humano(p);
		}
	}else{
		//Si no, vuelvo a pedir
		printf("\n\n\tERROR SELECCION INVALIDA\n\n");
		system("pause");
		mueve_humano(p);
	}
}

//Funcion para volar las fichas
void volar_humano(struct fichas *p){
	char pos[3];
	char nueva_pos[3];
	system("cls");
	printf("\n\n\n \tLOS 12 HOMBRES\n\n \t\t\t\t\t\t\tTurno %s [%c]", p->nombre, p->valor_ficha);
	printf("\n\n \tCapturo %d fichas del contrario \n\n\n", p->capturadas);
	imprimir_tablero();
	printf("\n\tEliga una ficha para volar (ColumnaFila) -> ");
	scanf("%s", pos);
	fflush(stdin);
	//Selecciona una ficha suya 
	if(tablero[fila(pos[1])][columna(pos[0])] == p->valor_ficha){
		system("cls");
		printf("\n\n\n \tLOS 12 HOMBRES\n\n\n \t%s selecciono la ficha en %c%c\n", p->nombre, pos[0], pos[1]);
		printf("\n\n\n");
		imprimir_tablero();
		//Selecciono una posicion adyacente y valida, luego muevo
		if(pedir_posicion_volar(p, nueva_pos, pos)){
			system("cls");
			printf("\n\n\n \tLOS 12 HOMBRES\n\n \t%s movio su ficha de %c%c a %c%c\n", p->nombre, pos[0], pos[1], nueva_pos[0], nueva_pos[1]);
			printf("\n\n\n\n");
			imprimir_tablero();
			system("pause");
			//Si la ficha movida forma mil, saca ficha del contrario
			if(verificar_mil(p->valor_ficha, columna(nueva_pos[0]), fila(nueva_pos[1]))){
				sacar_ficha(p);
			}
		}else{
			//Si no, vuelvo a pedir
			printf("\n\n\tERROR POSICION INVALIDA\n\n");
			system("pause");
			mueve_humano(p);
		}
	}else{
		//Si no, vuelvo a pedir
		printf("\n\n\tERROR SELECCION INVALIDA\n\n");
		system("pause");
		mueve_humano(p);
	}
}

//Funciones para evaluaciones e inteligencia
//Verifica si una ficha puede moverse donde se pide en el tablero de un nodo del arbol
int mover_evaluacion(int jugador, int let, int num, int nuevo_let, int nuevo_num, char tab[BUFFER][BUFFER], int volar){
	int i, saltos, ban = 1;
	
	//Si se quiere volar
	if(volar){
		if(tab[nuevo_num][nuevo_let]){
			//Error, lugar ocupado o invalido
			return(0);
		}else{
			return(1);
		}
	}
	
	//Si se quiere mover en fila
	//Cuento cuatas casillas intermedias hay, y todas deben ser '#'
	if(num == nuevo_num){
		if(let < nuevo_let){
			saltos = nuevo_let - let;
			for(i = 1; i < saltos; i++){
				if(tab[num][let + i] != '#'){
					return 0;
				}
				if(num == 3 && let + i == 3){
					return 0;
				}
			}
		}else{
			saltos = let - nuevo_let;
			for(i = 1; i < saltos; i++){
				if(tab[num][nuevo_let + i] != '#'){
					return 0;
				}
				if(num == 3 && nuevo_let + i == 3){
					return 0;
				}
			}
		}
	}

	//Si se quiere mover en columna
	//Cuento cuatas casillas intermedias hay, y todas deben ser '#'
	if(let == nuevo_let){
		if(num < nuevo_num){
			saltos = nuevo_num - num;
			for(i = 1; i < saltos; i++){
				if(tab[num + i][let] != '#'){
					return 0;
				}
				if(num + i == 3 && let == 3){
					return 0;
				}
			}
		}else{
			saltos = num - nuevo_num;
			for(i = 1; i < saltos; i++){
				if(tab[nuevo_num + i][let] != '#'){
					return 0;
				}
				if(nuevo_num + i == 3 && let == 3){
					return 0;
				}
			}
		}
	}

	//Si se quiere mover en diagonal
	//Cuento cuatas casillas intermedias hay, y todas deben ser '#'
	//Si restadas las dimensiones da 0, o sumadas da 7, estas en una de las cuatro diagonales
	if(num != nuevo_num && let != nuevo_let){
		ban = 0;
		if((let + num == 6) && (nuevo_let + nuevo_num == 6)){
			ban = 1;
			if(num < nuevo_num){
				int saltos_num = nuevo_num - num;
				int saltos_let = let - nuevo_let;
				if(saltos_num != 1 && saltos_let != 1){
					return 0;
				}
			}else{
				int saltos_num = num - nuevo_num;
				int saltos_let = nuevo_let - let;
				if(saltos_num != 1 && saltos_let != 1){
					return 0;
				}
			}
		}else{
			if((let - num == 0) && (nuevo_let - nuevo_num == 0)){
				ban = 1;
				if(num < nuevo_num){
					int saltos_num = nuevo_num - num;
					int saltos_let = nuevo_let - let;
					if(saltos_num != 1 && saltos_let != 1){
						return 0;
					}
			}else{
					int saltos_num = num - nuevo_num;
					int saltos_let = let - nuevo_let;
					if(saltos_num != 1 && saltos_let != 1){
						return 0;
					}
				}
			}
		}
	}

	if(ban == 0){
		return 0;
	}

	if(tab[nuevo_num][nuevo_let]){
		//Error, lugar ocupado o invalido
		return(0);
	}else{
		return(1);
	}
}

//Verifica si la ficha esta en mil
int verificar_mil_evaluacion(int jugador, int let, int num, char tab[BUFFER][BUFFER]){
	int i = 0, con = 0;

	//Control en columna
		//Control especial
		if(let == 3){
			if(num == 0 || num == 1 || num == 2){
				for(i=0, con = 0; i<3; i++){
					if(tab[i][let] == jugador){
						con++;
					}
				}
				//Hizo mil
				if(con == 3){
					return(1);
				}	
			}else{
				for(i=4, con = 0; i<BUFFER; i++){
					if(tab[i][let] == jugador){
						con++;
					}
				}
				//Hizo mil
				if(con == 3){
					return(1);
				}
			}
		}else{
			//Control normal
			for(i=0, con = 0; i<BUFFER; i++){
				if(tab[i][let] == jugador){
					con++;
				}
			}
			//Hizo mil
			if(con == 3){
				return(1);
			}
		}

	//Control en fila
		//Control especial
		if(num == 3){
			if(let == 0 || let == 1 || let == 2){
				for(i=0, con = 0; i<3; i++){
					if(tab[num][i] == jugador){
						con++;
					}
				}
				//Hizo mil
				if(con == 3){
					return(1);
				}
			}
			for(i=4, con = 0; i<BUFFER; i++){
				if(tab[num][i] == jugador){
					con++;
				}
			}
			//Hizo mil
			if(con == 3){
				return(1);
			}
		}else{
			//Control normal
			for(i=0, con = 0; i<BUFFER; i++){
				if(tab[num][i] == jugador){
					con++;
				}
			}
			//Hizo mil
			if(con == 3){
				return(1);
			}
		}

	//Control en diagonales
	//Si restadas las dimensiones da 0, o sumadas da 6, estas en una de las cuatro diagonales a verificar
	if(let + num == 6){
		if(tab[0][6] == jugador
			&&tab[1][5] == jugador
				&&tab[2][4] == jugador
					&& (num == 0 || num == 1 || num == 2)){
			return(1);
		}
		if(tab[6][0] == jugador
			&&tab[5][1] == jugador
				&&tab[4][2] == jugador
					&& (num == 4 || num == 5 || num == 6)){
			return(1);
		}
	}else{
		if(let - num == 0){
			if(tab[0][0] == jugador
				&&tab[1][1] == jugador
					&&tab[2][2] == jugador
						&& (let == 0 || let == 1 || let == 2)){
				return(1);
			}
			if(tab[6][6] == jugador
				&&tab[5][5] == jugador
					&&tab[4][4] == jugador
						&& (let == 4 || let == 5 || let == 6)){
				return(1);
			}
		}
	}

	//Retorna falso, no se hizo mil
	return(0);
}

//Verifica si todas las fichas poseen movimientos validos 
int verificar_movimiento_evaluacion(int jugador, char tab[BUFFER][BUFFER]){
	int i, j, nuevo_i, nuevo_j, ficha;
	if(jugador){
		ficha = 79;
	}else{
		ficha = 88;
	}
	//Busco todas las fichas de jugador
	for(i = 0; i<BUFFER; i++){
		for(j = 0; j<BUFFER; j++){
			if(tab[i][j] == ficha){
				//Buscamos todos los lugares vacios y vemos si puede mover ahi
				for(nuevo_i = 0; nuevo_i<BUFFER; nuevo_i++){
					for(nuevo_j = 0; nuevo_j<BUFFER; nuevo_j++){
						if(tab[nuevo_i][nuevo_j] == 0){
							//Si una ficha tiene un moviminto valido, retorna verdadero
							if(mover_evaluacion(jugador, j, i, nuevo_j, nuevo_i, tab, 0)){
								return 1;
							}
						}
					}
				}
			}
		}
	}
	//Todas las fichas estan bloqueadas
	return 0;
}

//Funciones de evaluacion
/*  
	The evaluation functions were motivated by
	Simona-Alexandra PETCU, Stefan HOLBAN
	Computer Science and Engineering, Politehnica University of Timisoara 
*/

//Evaluacion de dos jugadas criticas y mas importantes
int evaluacion_critica(struct nodo_arbol *x){
	int i, j, let, num, mill_ban = 0;
	let = x->let;
	num = x->num;
	
	//Recorro el tablero buscando donde la pc puede hacer mill
	for(i = 0; i<BUFFER; i++){
		for(j = 0; j<BUFFER; j++){	
			if(x->tablero_padre[num][let] == 0){
				//Si esta vacia la posicion, vemos si existe un mill ahi
				if(verificar_mil_evaluacion(pc->valor_ficha, let, num, x->tablero_padre)){
					mill_ban = 1;
				}
			}
		}
	}
	
	if(mill_ban){
		x->ans = -999;
		printf("NUIV");
		system("PAUSE");
		return 1;
	}
	
	//Si al recorrer todo el tablero, la pc no puede hacer ningun mil
	//Coloca en donde el contrario si puede hacer
	
	//Recorro el tablero buscando donde el contrario puede hacer mill
	for(i = 0; i<BUFFER; i++){
		for(j = 0; j<BUFFER; j++){	
			if(x->tablero_padre[num][let] == 0){
				//Si esta vacia la posicion, vemos si existe un mill ahi
				if(verificar_mil_evaluacion(humano->valor_ficha, let, num, x->tablero_padre)){
					mill_ban = 1;
				}
			}
		}
	}
	
	if(mill_ban){
		x->ans = 999;
		printf("NUIV");
		system("PAUSE");
		return 1;
	}
	
	x->ans = 1;
	return 0;
	
}

// 1 Evalua quien hace "mil" en la jugada 
int e_mil_anterior(struct nodo_arbol *x){
	
	//Jugada mil para pc, retorna 1
	//Jugada mil para humano, retorna -1
	//Si no es una jugada mil para ninguno, retorna 0
	
	if(x->tablero_padre[x->num][x->let] == pc->valor_ficha 
		&& verificar_mil_evaluacion(pc->valor_ficha, x->let, x->num, x->tablero_padre)){
			return (-1);
	}else{
		if(x->tablero_padre[x->num][x->let] == humano->valor_ficha 
			&& verificar_mil_evaluacion(humano->valor_ficha , x->let, x->num, x->tablero_padre)){
			return (1);
		}else{
			return 0;
		}
	}
}

// 2 Diferencia entre el numero de "mil" de los jugadores
int e_dif_mil(struct nodo_arbol *x){
	int i, j, mil_p = 0, mil_h = 0, con_p = 0, con_h = 0; 
	
	//Recorro filas
	for(i = 0, con_p = 0, con_h = 0; i<BUFFER; i++){
		for(j = 0; j<BUFFER; j++){	
			if(x->tablero_padre[i][j] == pc->valor_ficha){
				con_p++;
			}else{
				if(x->tablero_padre[i][j] == humano->valor_ficha){
					con_h++;
				}
			}
			if(i == 3 && j == 3){
				con_p = con_h = 0;
			}
			if(con_p == 3){
				mil_p++;
			}
			if(con_h == 3){
				mil_h++;
			}
		}
	}
	
	//Recorro columnas
	for(j = 0, con_p = 0, con_h = 0; j<BUFFER; j++){
		for(i = 0; i<BUFFER; i++){	
			if(x->tablero_padre[i][j] == pc->valor_ficha){
				con_p++;
			}else{
				if(x->tablero_padre[i][j] == humano->valor_ficha){
					con_h++;
				}
			}
			if(i == 3 && j == 3){
				con_p = con_h = 0;
			}
			if(con_p == 3){
				mil_p++;
			}
			if(con_h == 3){
				mil_h++;
			}
		}
	}
	
	//Recorro diagonales
	for(i = 0, j = 0, con_p = 0, con_h = 0; i<BUFFER; i++, j++){
		if(x->tablero_padre[i][j] == pc->valor_ficha){
			con_p++;
		}else{
			if(x->tablero_padre[i][j] == humano->valor_ficha){
				con_h++;
			}
		}
		if(i == 3 && j == 3){
			con_p = con_h = 0;
		}
		if(con_p == 3){
			mil_p++;
		}
		if(con_h == 3){
			mil_h++;
		}
	}
	for(i = 0, j = 6, con_p = 0, con_h = 0; i<BUFFER; i++, j--){
		if(x->tablero_padre[i][j] == pc->valor_ficha){
			con_p++;
		}else{
			if(x->tablero_padre[i][j] == humano->valor_ficha){
				con_h++;
			}
		}
		if(i == 3 && j == 3){
			con_p = con_h = 0;
		}
		if(con_p == 3){
			mil_p++;
		}
		if(con_h == 3){
			mil_h++;
		}
	}
	
	return(mil_p - mil_h);
}

// 3 Diferencia entre el numero de fichas bloqueadas de los jugadores
int e_dif_bloqueados(struct nodo_arbol *x){
	int i, j, nuevo_i, nuevo_j, ficha, ficha2, con_bloqueados_pc = 0, con_bloqueados_hum = 0, ban_p, ban_h;
	ficha = pc->valor_ficha;
	ficha2 = pc->ficha_contrario;
	//Controlo todas las fichas
	for(i = 0; i<BUFFER; i++){
		for(j = 0; j<BUFFER; j++){	
			if(x->tablero_padre[i][j] == ficha || x->tablero_padre[i][j] == ficha2){
				ban_p = ban_h = 0;
				//Buscamos todos los lugares vacios y vemos si puede mover ahi
				for(nuevo_i = 0; nuevo_i<BUFFER; nuevo_i++){
					for(nuevo_j = 0; nuevo_j<BUFFER; nuevo_j++){
						if(x->tablero_padre[nuevo_i][nuevo_j] == 0){
							//Si la ficha es de la pc y puede mover, suma contador de pc
							if(x->tablero_padre[i][j] == ficha){
								if(mover_evaluacion(pc->valor_colocacion, j, i, nuevo_j, nuevo_i, x->tablero_padre, 0)){
									//La ficha de pc se puede mover
									ban_p = 1;
								}
							}
							//Si la ficha es de hum y puede mover, suma contador de pc
							if(x->tablero_padre[i][j] == ficha2){
								if(mover_evaluacion(humano->valor_colocacion, j, i, nuevo_j, nuevo_i, x->tablero_padre, 0)){
									//La ficha de hum se puede mover
									ban_h = 1;
								}
							}
						}
					}
				}
				if(!ban_p){
					con_bloqueados_pc++;	
				}
				if(!ban_h){
					con_bloqueados_hum++;
				}
			}
		}
	}
	return(con_bloqueados_pc - con_bloqueados_hum);
}

// 4 Diferencia entre el numero de fichas de los jugadores
int e_dif_fichas(struct nodo_arbol *x){
	int i, j, con_p = 0, con_h = 0;
	
	for(i = 0; i<BUFFER; i++){
		for(j = 0; j<BUFFER; j++){	
			if(x->tablero_padre[i][j] == pc->valor_ficha){
				con_p++;
			}else{
				if(x->tablero_padre[i][j] == humano->valor_ficha){
					con_h++;
				}
			}
		}
	}
	
	return(con_p - con_h);
}

// 5 Diferencia entre el numero de 2 fichas unidas donde se puede hacer un "mil"
int e_dif_2fichas(struct nodo_arbol *x){
	int i, j, con_p = 0, con_h = 0, vacio = 0 , posible_mil_p = 0, posible_mil_h = 0; 
	
	//Recorro filas
	for(i = 0, con_p = 0, con_h = 0; i<BUFFER; i++){
		for(j = 0; j<BUFFER; j++){	
			if(x->tablero_padre[i][j] == pc->valor_ficha){
				con_p++;
			}else{
				if(x->tablero_padre[i][j] == humano->valor_ficha){
					con_h++;
				}else{
					if(x->tablero_padre[i][j] == 0){
						vacio++;
					}
				}
			}
			if(i == 3 && j == 3){
				con_p = con_h = 0;
			}
			//Si en la fila hay dos fichas de pc, y un lugar vacio, es posible mil
			if(con_p == 2 && vacio == 1){
				posible_mil_p++;
			}
			//Si en la fila hay dos fichas de humano, y un lugar vacio, es posible mil
			if(con_h == 2 && vacio == 1){
				posible_mil_h++;
			}
		}
	}
	
	//Recorro columnas
	for(j = 0, con_p = 0, con_h = 0; j<BUFFER; j++){
		for(i = 0; i<BUFFER; i++){	
			if(x->tablero_padre[i][j] == pc->valor_ficha){
				con_p++;
			}else{
				if(x->tablero_padre[i][j] == humano->valor_ficha){
					con_h++;
				}else{
					if(x->tablero_padre[i][j] == 0){
						vacio++;
					}
				}
			}
			if(i == 3 && j == 3){
				con_p = con_h = 0;
			}
			//Si en la columna hay dos fichas de pc, y un lugar vacio, es posible mil
			if(con_p == 2 && vacio == 1){
				posible_mil_p++;
			}
			//Si en la columna hay dos fichas de humano, y un lugar vacio, es posible mil
			if(con_h == 2 && vacio == 1){
				posible_mil_h++;
			}
		}
	}
	
	//Recorro diagonales
	for(i = 0, j = 0, con_p = 0, con_h = 0; i<BUFFER; i++, j++){
		if(x->tablero_padre[i][j] == pc->valor_ficha){
			con_p++;
		}else{
			if(x->tablero_padre[i][j] == humano->valor_ficha){
				con_h++;
			}else{	
				if(x->tablero_padre[i][j] == 0){
					vacio++;
				}
			}
		}
		if(i == 3 && j == 3){
			con_p = con_h = 0;
		}
		//Si en la diagonal hay dos fichas de pc, y un lugar vacio, es posible mil
		if(con_p == 2 && vacio == 1){
			posible_mil_p++;
		}
		//Si en la diagonal hay dos fichas de humano, y un lugar vacio, es posible mil
		if(con_h == 2 && vacio == 1){
			posible_mil_h++;
		}
	}
	for(i = 0, j = 6, con_p = 0, con_h = 0; i<BUFFER; i++, j--){
		if(x->tablero_padre[i][j] == pc->valor_ficha){
			con_p++;
		}else{
			if(x->tablero_padre[i][j] == humano->valor_ficha){
				con_h++;
			}else{
				if(x->tablero_padre[i][j] == 0){
					vacio++;
				}	
			}
		}
		if(i == 3 && j == 3){
			con_p = con_h = 0;
		}
		//Si en la diagonal hay dos fichas de pc, y un lugar vacio, es posible mil
		if(con_p == 2 && vacio == 1){
			posible_mil_p++;
		}
		//Si en la diagonal hay dos fichas de humano, y un lugar vacio, es posible mil
		if(con_h == 2 && vacio == 1){
			posible_mil_h++;
		}
	}
	
	//return(posible_mil_p - posible_mil_h);
	return(posible_mil_h - posible_mil_p);
}

// 6 Evalua quien puede ganar la partida
int e_ganador(struct nodo_arbol *x){
	int i, j, con_p = 0, con_h = 0;
	
	//Jugada ganadora para pc, retorna 1
	//Jugada ganadora para humano, retorna -1
	//Si no es una jugada ganadora para ninguno, retorna 0
	
	//Contamos cuantas fichas tiene cada uno en esta jugada
	for(i = 0; i<BUFFER; i++){
		for(j = 0; j<BUFFER; j++){	
			if(x->tablero_padre[i][j] == pc->valor_ficha){
				con_p++;
			}else{
				if(x->tablero_padre[i][j] == humano->valor_ficha){
					con_h++;
				}
			}
		}
	}
	
	//Si alguno tine dos fichas, pierde
	if(con_p == 2){
		return -1;
	}else{
		if(con_h == 2){
			return 1;
		}
	}
	
	//Vemos si alguien se queda sin movimientos en esta jugada
	//Si la pc se queda sin movimientos retorna -1
	if(!verificar_movimiento_evaluacion(pc->valor_colocacion, x->tablero_padre)){
		return (-1);
	}
	
	//Si el jugador se queda sin movimientos retorna 1
	if(!verificar_movimiento_evaluacion(humano->valor_colocacion, x->tablero_padre)){
		return (1);
	}


	return (0);
}

//Funcion principal de evaluacion
int e(struct nodo_arbol *x){
	
	int e;
	
	//Evaluation function for Phase 1 e = 18 * (1) + 26 * (2) + 1 * (3) + 9 * (4) + 10 * (5) + 7 * (6);
	//Evaluation function for Phase 2 = 14 * (1) + 43 * (2) + 10 * (3) + 11 * (4) + 8 * (7) + 1086 * (8);
	//Evaluation function for Phase 3 = 16 * (1) + 10 * (5) + 1 * (6) + 1190 * (8);
	
	if(pc->fase == 1){
		e = 18 * (e_mil_anterior(x)) + 
			26 * (e_dif_mil(x)) + 
			1 * (e_dif_bloqueados(x)) + 
			9 * (e_dif_fichas(x)) + 
			10 * (e_dif_2fichas(x));
	}else{
		if(pc->fase == 2){
			e = 14 * (e_mil_anterior(x)) + 
				43 * (e_dif_mil(x)) + 
				10 * (e_dif_bloqueados(x)) + 
				11 * (e_dif_fichas(x)) + 
				1086 * (e_ganador(x));
		}else{
			e = 16 * (e_mil_anterior(x)) + 
				10 * (e_dif_2fichas(x)) + 
				1190 * (e_ganador(x));
		}
	}
	
	if(modo_evaluacion){
		return (e);
	}
	
	//e(x) = E(x) si juega pc
	//e(x) = -E(x) si juega hum
	if(x->tablero_padre[x->num][x->let] == pc->valor_ficha){
		return (-e);
	}else{
		return (e);
	}
}

//Funcion que retorna el mayor entre dos enteros
int max(int a, int b){
	if(a > b){
		return a;
	}else{
		return b;
	}
}

//Funcion de corte alpha-beta en la busqueda de la mejor jugada
int alpha_beta(struct nodo_arbol *x, int nivel, int lb, int d){
	struct nodo_arbol *x_hijo;
	if(x->izq == NULL || !nivel){
		return(e(x));
	}

	x->ans = lb;
	for(x_hijo = x->izq; x_hijo != NULL; x_hijo = x_hijo->sgte){
		x->ans = max(x->ans, -alpha_beta(x_hijo, nivel - 1, -d, -(x->ans)));
		if(x->ans >= d){
			return(x->ans);	
		}
	}
	
	return(x->ans);
}

//Agrega un nodo en la lista de hermanos
void addnode(struct nodo_arbol **ptr, struct nodo_arbol *nuevo){
	struct nodo_arbol **pp, *actual;
	pp=ptr;
	while((actual=*pp) != NULL){
		pp= &(actual->sgte);
	}
	
	*pp=nuevo;
	nuevo->sgte=actual;
}

//Crea los hijos del arbol de juego
void crear_hijos(struct nodo_arbol *ptr, int ficha){
	
	//Variables para crear hijos
	int i, j;
	struct nodo_arbol *nuevo;
	struct nodo_arbol *nuevo2;
	
	//Tablero de referencia
	char tablero_referencia[BUFFER][BUFFER];
	
	//Copiamos el tablero actual a la referencia
	copiar_tablero(ptr->tablero_padre, tablero_referencia);
	
	//Creamos los hijos a partir de la referencia
	for(i = 0; i < BUFFER; i++){
		for(j = 0; j < BUFFER; j++){
			if(tablero_referencia[i][j] == 0){
				//tablero_arbol[i][j] = 'X';
				//Veo si hay hijo izquierdo
				if(ptr->izq == NULL){
					//Nuevo nodo izquierdo
					nuevo = (struct nodo_arbol *) malloc (sizeof(struct nodo_arbol));
					nuevo->let = j;
					nuevo->num = i;
					nuevo->ans = 0;
					nuevo->terminal = 0;
					nuevo->der = NULL;
					nuevo->izq = NULL;
					nuevo->sgte = NULL;
					//Guardamos el tablero del padre, con la jugada del hijo
					copiar_tablero(ptr->tablero_padre, nuevo->tablero_padre);
					nuevo->tablero_padre[nuevo->num][nuevo->let] = ficha;
					//Verifica si la jugada no es una terminal
					nuevo->terminal = e_mil_anterior(nuevo);
					//printf("%d", nuevo->terminal);
					//Agrego un hijo izquierdo al arbol
					
					//Si en el nodo creado se hizo mil
					if(verificar_mil_evaluacion(ficha, j, i, nuevo->tablero_padre)){
						nuevo->terminal = 1;
					}
					
					ptr->izq = nuevo;
				}else{
					//Nuevo nodo derecho
					nuevo2 = (struct nodo_arbol *) malloc (sizeof(struct nodo_arbol));
					nuevo2->let = j;
					nuevo2->num = i;
					nuevo2->ans = 0;
					nuevo2->terminal = 0;
					nuevo2->der = NULL;
					nuevo2->izq = NULL;
					nuevo2->sgte = NULL;
					//Guardamos el tablero del padre, con la jugada del hijo
					copiar_tablero(ptr->tablero_padre, nuevo2->tablero_padre);
					nuevo2->tablero_padre[nuevo2->num][nuevo2->let] = ficha;
					//Verifica si la jugada no es una terminal
					nuevo2->terminal = e_mil_anterior(nuevo2);
						
					//Si en el nodo creado se hizo mil
					if(verificar_mil_evaluacion(ficha, j, i, nuevo2->tablero_padre)){
						nuevo2->terminal = 1;
					}
					
					//printf("%d", nuevo2->terminal);
					//Agrego un hijo derecho al arbol, en la lista de hermanos
					addnode(&(ptr->der), nuevo2);
				}
				//El tablero de referencia indica que esa jugada ya esta calculada
				tablero_referencia[i][j] = ficha;
				
				//Hago que el hijo izquierdo pueda acceder a su hermano
				ptr->izq->sgte = ptr->der;
			}			
		}
	}
}

//Funcion recursiva que crea nuevos hijos para hojas/terminales
void crear_hijos_terminales(struct nodo_arbol *ptr, int ficha){

	//Recorre todo el arbol, y crea hijos para los nodos terminales
	while(ptr != NULL){
		crear_hijos_terminales(ptr->izq, ficha);
		/*
			Si el nodo es una hoja que debe seguir generando hijos
		  	porque no es una jugada terminal (una ganadora o una jugada en mil)
		  	debe crear mas hijos
		  	&& ptr->terminal == 0
		  	
		*/
		
		if(ptr->izq == NULL && !ptr->terminal){
			//Envia el nodo terminal para crearle mas hijos
			crear_hijos(ptr, ficha);
			//imprimir_matriz(ptr->tablero_padre);
			//printf("\n\n%d,%d", ptr->num, ptr->let);
		}
		ptr = ptr->sgte;
	}
	
}

//Crea un nuevo arbol de juego en cada turno
void crear_arbol(struct nodo_arbol **ptr_arbol, int type){
	int i, j;
	
	//Borramos el arbol si es que existia y creamos uno nuevo
	struct nodo_arbol *liberar = *ptr_arbol;
	free(liberar);
	
	struct nodo_arbol *ptr = (struct nodo_arbol *) malloc (sizeof(struct nodo_arbol));
	ptr->num = -1;
	ptr->let = -1;
	ptr->ans = 0;
	ptr->izq = NULL;
	ptr->der = NULL;
	ptr->sgte = NULL;
	*ptr_arbol = ptr;
	
	//Vemos que tipo de arbol se quiere crear
	//1 = arbol colocar
	//2 = arbol mover
	
	if(type == 1){
		//Copiamos el tablero actual del juego al tablero auxiliar de la raiz
		copiar_tablero(tablero, ptr->tablero_padre);
		crear_hijos(ptr, pc->valor_ficha);
		
		//Creamos los hijos de la raiz
		pc->rondas = 2 + pc->fichas_en_tab;
		//Calculo hasta que nivel ir dependiendo del progreso del juego
		if(pc->rondas > PROFUNDIDAD){
			pc->rondas = 5;
		}
		
		for(i = 0; i < pc->rondas; i++){
			crear_hijos_terminales(ptr, pc->ficha_contrario);
			crear_hijos_terminales(ptr, pc->valor_ficha);
		}
		
	}
}

//Funcion que crea hijos para el arbol de movimiento
void crear_hijos_movimiento(struct nodo_arbol *ptr, int ficha, int volar){
	//Variables para crear hijos
	int i, j, i2, j2;
	struct nodo_arbol *nuevo;
	
	//Seleccionamos todas las fichas de la pc
	for(i = 0; i<BUFFER; i++){
		for(j = 0; j<BUFFER; j++){
			//Selecciona una ficha
			if(ptr->tablero_padre[i][j] == ficha){
				//Seleccionamos todas los espacios vacios
				for(i2 = 0; i2<BUFFER; i2++){
					for(j2 = 0; j2<BUFFER; j2++){
						//Seleccionamos un espacio vacio
						if(ptr->tablero_padre[i2][j2] == 0 && tablero[i2][j2] == 0){
							//Si la ficha seleccionada se puede mover en el espacio vacio crea un hijo
							if(mover_evaluacion(ficha, j, i, j2, i2, ptr->tablero_padre, volar)){
								nuevo = (struct nodo_arbol *) malloc (sizeof(struct nodo_arbol));
								nuevo->let = j;
								nuevo->num = i;
								nuevo->ans = 0;
								nuevo->let2 = j2;
       							nuevo->num2 = i2;
								nuevo->der = NULL;
								nuevo->izq = NULL;
								nuevo->sgte = NULL;
								//Guardamos el tablero del padre, con la jugada del hijo
								copiar_tablero(ptr->tablero_padre, nuevo->tablero_padre);
								nuevo->tablero_padre[i][j] = 0;
								nuevo->tablero_padre[i2][j2] = ficha;
								//Agrego un hijo derecho al arbol, en la lista de hermanos
								addnode(&(ptr->izq), nuevo);
							}	
						}
					}
				}
			}
		}
	}
}

//Funcion recursiva que crea nuevos hijos para hojas/terminales
void crear_hijos_terminales_movimiento(struct nodo_arbol *ptr, int ficha, int volar){

	//Recorre todo el arbol, y crea hijos para los nodos terminales
	while(ptr != NULL){
		crear_hijos_terminales_movimiento(ptr->izq, ficha, volar);	
		if(ptr->izq == NULL){
			//Envia el nodo terminal para crearle mas hijos
			crear_hijos_movimiento(ptr, ficha, volar);
			//imprimir_matriz(ptr->tablero_padre);
			//printf("\n\n%d,%d", ptr->num, ptr->let);
		}
		ptr = ptr->sgte;
	}
	
}

//Crea un nuevo arbol de juego en cada turno
void crear_arbol_movimiento(struct nodo_arbol **ptr_arbol, int volar){
	int i, j;
	
	//Borramos el arbol si es que existia y creamos uno nuevo
	struct nodo_arbol *liberar = *ptr_arbol;
	free(liberar);
	
	struct nodo_arbol *ptr = (struct nodo_arbol *) malloc (sizeof(struct nodo_arbol));
	ptr->num = -1;
	ptr->let = -1;
	ptr->num2 = -1;
	ptr->let2 = -1;
	ptr->ans = 0;
	ptr->izq = NULL;
	ptr->der = NULL;
	ptr->sgte = NULL;
	*ptr_arbol = ptr;
	
	//Vemos que tipo de arbol se quiere crear
	//1 = arbol colocar
	//2 = arbol mover
	
	//Copiamos el tablero actual del juego al tablero auxiliar de la raiz
	copiar_tablero(tablero, ptr->tablero_padre);
	crear_hijos_movimiento(ptr, pc->valor_ficha, volar);
	
	if(!volar){
		crear_hijos_terminales_movimiento(ptr, pc->ficha_contrario, volar);
		crear_hijos_terminales_movimiento(ptr, pc->valor_ficha, volar);
	
		crear_hijos_terminales_movimiento(ptr, pc->ficha_contrario, volar);
		crear_hijos_terminales_movimiento(ptr, pc->valor_ficha, volar);
	}

}

//Funcion que sacar una ficha la pc
void saca_ficha_pc(struct nodo_arbol *ptr){
	int i, j, aux, i_r = 0;		
	
	//Tablero donde guardamos las jugadas evaluadas despues de sacar la ficha
	char tabla_referencia[BUFFER][3];
	
	for(i = 0; i<BUFFER; i++){
		for(j = 0; j<BUFFER; j++){
			if(ptr->tablero_padre[i][j] == pc->ficha_contrario){
				ptr->tablero_padre[i][j] = 0;
				tabla_referencia[i_r][0] = i;
				tabla_referencia[i_r][1] = j;
				tabla_referencia[i_r][2] = e(ptr);
				ptr->tablero_padre[i][j] = pc->valor_ficha;
				i_r++;
			}
		}
	}
	
	for(i = 1; i < i_r; i++){
		if(	tabla_referencia[0][2] < tabla_referencia[i][2] ){
			tabla_referencia[0][2] = tabla_referencia[i][2];
			tabla_referencia[0][0] = tabla_referencia[i][0];
			tabla_referencia[0][1] = tabla_referencia[i][1];
		}
	}
	
	i = tabla_referencia[0][0];
	j = tabla_referencia[0][1];
	
	if((verificar_mil_todas(pc->ficha_contrario))
		|| (!(verificar_mil(pc->ficha_contrario, j, i)) 
			&& !(verificar_mil_todas(pc->ficha_contrario)))){
		if(tablero[i][j] == pc->ficha_contrario){
			system("cls");
			printf("\n\n\n \tLOS 12 HOMBRES\n\n\n \t%s saco una ficha en %c%d\n", pc->nombre, j + 'a' , 7 - i);
			printf("\n\n\n");
			pc->capturadas++;
			//Verificamos si el jugador saco una ficha en la ultima jugada de la 1 fase
			//Si ambos no tinen fichas, es la ultima jugada
			if(!pc->fichas && !humano->fichas){
				pc->bandera_ultimajugada = 1;
			}
			tablero[i][j] = 0;
			imprimir_tablero();
		}else{
			system("cls");
			printf("\n\n\n \tLOS 12 HOMBRES\n\n\n \t%s saco una ficha en %c%d\n", pc->nombre, j + 'a' , 7 - i);
			printf("\n\n\n");
			pc->capturadas++;
			//Verificamos si el jugador saco una ficha en la ultima jugada de la 1 fase
			//Si ambos no tinen fichas, es la ultima jugada
			if(!pc->fichas && !humano->fichas){
				pc->bandera_ultimajugada = 1;
			}
			tablero[i][j] = 0;
			imprimir_tablero();
		}
	}else{
		saca_ficha_pc(ptr);
	}

}

//Funcion que realiza la jugada del hijo mejor evaluado
int colocar_ficha_pc(struct nodo_arbol *ptr_arbol){
	
	struct nodo_arbol *ptr_hijo;
	
	//Recorremos todos los hijos hasta encontrar el nodo
	for(ptr_hijo = ptr_arbol->izq; ptr_hijo != NULL; ptr_hijo = ptr_hijo->sgte){
		if(ptr_hijo->ans == -ptr_arbol->ans || ptr_hijo->ans == 0){
			colocar_ficha(pc->valor_colocacion, ptr_hijo->let, ptr_hijo->num);
			system("cls");
			printf("\n\n\n \tLOS 12 HOMBRES\n\n\n \t%s coloco una ficha en %c%d\n", pc->nombre, ptr_hijo->let + 'a' , 7 - ptr_hijo->num);
			printf("\n\n\n");
			pc->fichas--;
			imprimir_tablero();
			
			//Verifia si la pc hizo mil
			if(verificar_mil(pc->valor_ficha, ptr_hijo->let, ptr_hijo->num)){
				printf("LA PC HIZO MIL\n");
				system("PAUSE");
				//Funcion que crea un nuevo arbol, eligiendo que ficha sacar
				saca_ficha_pc(ptr_hijo);
			}
			return 1;
		}
	}
}

//Funcion que realiza el movimiento del hijo mejor evaluado
int mover_ficha_pc(struct nodo_arbol *ptr_arbol, int volar){
	struct nodo_arbol *ptr_hijo;
	
	//Recorremos todos los hijos hasta encontrar el nodo
	for(ptr_hijo = ptr_arbol->izq; ptr_hijo != NULL; ptr_hijo = ptr_hijo->sgte){
		if(ptr_hijo->ans == -ptr_arbol->ans || ptr_hijo->ans == 0){
			mover_ficha(pc->valor_colocacion, ptr_hijo->let, ptr_hijo->num, ptr_hijo->let2, ptr_hijo->num2, 1, volar);
			system("cls");
			printf("\n\n\n \tLOS 12 HOMBRES\n\n\n \t%s movio una ficha de %c%d a %c%d\n", pc->nombre, ptr_hijo->let + 'a' , 7 - ptr_hijo->num, ptr_hijo->let2 + 'a' , 7 - ptr_hijo->num2);
			printf("\n\n\n");
			imprimir_tablero();
			//Verifia si la pc hizo mil
			if(verificar_mil(pc->valor_ficha, ptr_hijo->let2, ptr_hijo->num2)){
				printf("LA PC HIZO MIL\n");
				system("pause");
				//Funcion que crea un nuevo arbol, eligiendo que ficha sacar
				saca_ficha_pc(ptr_hijo);
			}
			return 1;
		}
	}
	return 0;
}

//Funcion para la IA donde coloca una ficha
void coloca_pc(struct nodo_arbol **ptr_arbol){
	
	//Cuadno es el turno de la pc, se crea el arbol para luego decidir la mejor jugada posible
	crear_arbol(ptr_arbol, 1);
	
	//Una vez creado el arbol en la fase de colocacion, se llama al corte alpha-beta evaluando cada nodo
	alpha_beta(*ptr_arbol, 5, -99999999, 99999999);
	
	//Realiza la jugada del hijo que posee el nodo ans de la raiz
	colocar_ficha_pc(*ptr_arbol);
	//imprimir_arbol(*ptr_arbol);
	//printf("\n Mejor nodo evaluado: ans = %d \n", (*ptr_arbol)->ans);
	//imprimir_hijos_ans((*ptr_arbol));
	//imprimir_ultimo_hijo(*ptr_arbol);
	system("pause");
	
}

//Funcion para la IA donde mueve una ficha (falta)*
void mueve_pc(struct nodo_arbol **ptr_arbol, int volar){
	
	struct nodo_arbol *ptr_hijo;
	ptr_hijo = (*ptr_arbol)->izq;
	
	//Cuadno es el turno de la pc, se crea el arbol para luego decidir la mejor jugada posible
	crear_arbol_movimiento(ptr_arbol, volar);
	
	//Modo Balanceado
	if(modo_evaluacion){
		for(ptr_hijo = (*ptr_arbol)->izq; ptr_hijo != NULL; ptr_hijo = ptr_hijo->sgte){
			evaluacion_critica(ptr_hijo);
		}
	}
	
	//Una vez creado el arbol en la fase de colocacion, se llama al corte alpha-beta evaluando cada nodo
	alpha_beta(*ptr_arbol, 5, -99999999, 99999999);
	
	//Realiza la jugada del hijo que posee el nodo ans de la raiz
	if(!mover_ficha_pc(*ptr_arbol, volar)){
		for( ; ptr_hijo->ans < 100 ; ptr_hijo = ptr_hijo->sgte){
			
		}
		mover_ficha(pc->valor_colocacion, ptr_hijo->let, ptr_hijo->num, ptr_hijo->let2, ptr_hijo->num2, 1, volar);
		system("cls");
		printf("\n\n\n \tLOS 12 HOMBRES\n\n\n \t%s movio una ficha de %c%d a %c%d\n", pc->nombre, ptr_hijo->let + 'a' , 7 - ptr_hijo->num, ptr_hijo->let2 + 'a' , 7 - ptr_hijo->num2);
		printf("\n\n\n");
		imprimir_tablero();
		//Verifia si la pc hizo mil
		if(verificar_mil(pc->valor_ficha, ptr_hijo->let2, ptr_hijo->num2)){
			printf("LA PC HIZO MIL\n");
			system("pause");
			//Funcion que crea un nuevo arbol, eligiendo que ficha sacar
			saca_ficha_pc(ptr_hijo);
		}
		
	}
	
	//imprimir_arbol(*ptr_arbol);
	//printf("\n Mejor nodo evaluado: ans = %d \n", (*ptr_arbol)->ans);
	//imprimir_hijos_ans((*ptr_arbol));
	//imprimir_ultimo_hijo(*ptr_arbol);
	system("pause");
	
}

//Pide el nombre de los dos jugadores en HUM VS HUM
void pedir_nombre(struct fichas *p, struct fichas *p2){
	system("cls");
	printf("\n\n\n\n\tLOS 12 HOMBRES\n\n\n");
	printf("\n\n \t\t Ingrese nombre para HUMANO 1:");
	printf(" \n\n \t\t >");
	scanf("%s", p->nombre);
	fflush(stdin);
	system("cls");
	printf("\n\n\n\n\tLOS 12 HOMBRES\n\n\n");
	printf("\n\n \t\t Ingrese nombre para HUMANO 2:");
	printf(" \n\n \t\t >");
	scanf("%s", p2->nombre);
	fflush(stdin);
}

//Funcion que brinda info
void ayuda(){
	printf("\n\n\n\n\tLOS 12 HOMBRES - REGLAS\n\n\n");
	printf("\tPara ayuda sobre como jugar visite:\n");
	printf("\thttps://es.wikipedia.org/wiki/Juego_del_molino\n\n");
	system("pause>nul"); 
}

//Funcion que cambia la evaluacion
void cambiar_evaluacion(){
	if(modo_evaluacion){
		modo_evaluacion = 0;
		printf("\n\n\t\tFuncion cambiada: MODO DEFENSA\n\n");
	}else{
		modo_evaluacion = 1;
		printf("\n\n\t\tFuncion cambiada: MODO ATAQUE\n\n");
	}
	system("pause>nul"); 
}

//Funciones de modo de juego
void hum_hum(struct nodo_arbol **arbol_colocar){
	int op = 1, jugadas_finales = 0;
	humano->valor_colocacion = 1;
	humano2->valor_colocacion = 0;
	humano->valor_ficha = 79;
	humano2->valor_ficha = 88;
	humano->ficha_contrario = 88;
	humano2->ficha_contrario = 79;
	//Fase 1
	while(op){
		//Si humano tiene fichas, coloca
		if(humano->fichas){
			coloca_humano(humano);
		}
		//Si humano2 tiene fichas, coloca
		if(humano2->fichas){
			coloca_humano(humano2);
		}
		//Si ambos se quedan sin fichas, salimos del primer ciclo
		if(!humano->fichas && !humano2->fichas){
			op = 0;
		}
		pc->fichas_en_tab++;
	}
	//Si hum2 saco una ficha en su ultima jugada, mueve primero hum1
	if(humano2->bandera_ultimajugada){
		mueve_humano(humano);	
	}
	//Fase 2
	while(!op){
		//Si humano2 aun no capturo 10 fichas y posee movimientos, mueve
		if(humano2->capturadas != 10 && verificar_movimiento_todas(humano2->valor_ficha)){
			//Si humano2 tiene 3 fichas, puede volar
			if(FICHAS - humano->capturadas == 3){
				if(FICHAS - humano->capturadas == 3){
					jugadas_finales++;
				}
				volar_humano(humano2);
			}else{
				mueve_humano(humano2);
			}
		}else{
			//Si humano2 quedo sin movimientos, humano2 gana
			humano->capturadas = FICHAS - 2;
			printf("\n\n\tSIN MOVIMIENTOS\n\n");
			system("pause");
		}
		//Si no hay ganador, sigue el juego
		if(!verificar_ganador(humano, humano2, jugadas_finales)){
			//Si humano aun no capturo 10 fichas y posee movimientos, mueve
			if(humano->capturadas != 10 && verificar_movimiento_todas(humano->valor_ficha)){
				//Si humano tiene 3 fichas, puede volar
				if(FICHAS - humano2->capturadas == 3){
					if(FICHAS - humano->capturadas == 3){
						jugadas_finales++;
					}
					volar_humano(humano);
				}else{
					mueve_humano(humano);
				}
			}else{
				//Si humano se quedo sin movimientos, humano gana
				humano2->capturadas = FICHAS - 2;
				printf("\n\n\tSIN MOVIMIENTOS\n\n");
				system("pause");
			}
		}					
	op = verificar_ganador(humano, humano2, jugadas_finales);
	}
}

void hum_pc(struct nodo_arbol **arbol_colocar){
	int op = 1, jugadas_finales = 0;
	humano->valor_colocacion = 1;
	pc->valor_colocacion = 0;
	humano->valor_ficha = 79;
	pc->valor_ficha = 88;
	humano->ficha_contrario = 88;
	pc->ficha_contrario = 79;
	while(op){
		//Fase 1 de colocacion
		pc->fase = 1;
		if(humano->fichas){
			coloca_humano(humano);
		}
		if(pc->fichas){
			coloca_pc(arbol_colocar);
		}
		//Si ambos se quedan sin fichas, salimos del primer ciclo
		if(!humano->fichas && !pc->fichas){
			op = 0;
		}
	}
	//Fase 2 de movimiento
	pc->fase = 2;
	//Si pc saco una ficha en su ultima jugada, mueve primero humano
	if(pc->bandera_ultimajugada){
		mueve_humano(humano);	
	}
	while(!op){
		//Si pc aun no capturo 10 fichas y posee movimientos, mueve
		if(pc->capturadas != 10 && verificar_movimiento_todas(pc->valor_ficha)){
			//Si pc tiene 3 fichas, puede volar
			if(FICHAS - humano->capturadas == 3){
				if(FICHAS - pc->capturadas == 3){
					jugadas_finales++;
				}
				pc->fase = 3;
				mueve_pc(arbol_colocar, 1);
			}else{
				mueve_pc(arbol_colocar, 0);
			}
		}else{
			//Si pc se quedo sin movimientos, humano gana
			humano->capturadas = FICHAS - 2;
			printf("\n\n\tSIN MOVIMIENTOS\n\n");
			system("pause");
		}
		//Si no hay ganador, sigue el juego
		if(!verificar_ganador(humano, pc, jugadas_finales)){
			//Si humano aun no capturo 10 fichas y posee movimientos, mueve
			if(humano->capturadas != 10 && verificar_movimiento_todas(humano->valor_ficha)){
				//Si humano tiene 3 fichas, puede volar
				if(FICHAS - pc->capturadas == 3){
					if(FICHAS - humano->capturadas == 3){
						jugadas_finales++;
					}
					volar_humano(humano);
				}else{
					mueve_humano(humano);
				}
			}else{
				//Si humano se quedo sin movimientos, pc gana
				pc->capturadas = FICHAS - 2;
				printf("\n\n\tSIN MOVIMIENTOS\n\n");
				system("pause");
			}	
		}					
	op = verificar_ganador(humano, pc, jugadas_finales);
	}
}

void pc_hum(struct nodo_arbol **arbol_colocar){
	int op = 1, jugadas_finales = 0;
	pc->valor_colocacion = 1;
	humano->valor_colocacion = 0;
	pc->valor_ficha = 79;
	humano->valor_ficha = 88;
	pc->ficha_contrario = 88;
	humano->ficha_contrario = 79;
	while(op){
		//Fase 1 de colocacion
		pc->fase = 1;
		if(pc->fichas){
			coloca_pc(arbol_colocar);
		}
		if(humano->fichas){
			coloca_humano(humano);
		}
		//Si ambos se quedan sin fichas, salimos del primer ciclo
		if(!pc->fichas && !humano->fichas){
			op = 0;
		}
	}
	//Fase 2 de movimiento
	pc->fase = 2;
	//Si el humano saco una ficha en su ultima jugada, mueve primero pc
	if(humano->bandera_ultimajugada){
		mueve_pc(arbol_colocar, 0);		
	}
	while(!op){
		//Si humano aun no capturo 10 fichas y posee movimientos, mueve
		if(humano->capturadas != 10 && verificar_movimiento_todas(humano->valor_ficha)){
			//Si humano tiene 3 fichas, puede volar
			if(FICHAS - pc->capturadas == 3){
				if(FICHAS - humano->capturadas == 3){
					jugadas_finales++;
				}
				volar_humano(humano);
			}else{
				mueve_humano(humano);
			}
		}else{
			//Si se quedo sin movimientos, humano2 gana
			pc->capturadas = FICHAS - 2;
			printf("\n\n\tSIN MOVIMIENTOS\n\n");
			system("pause");
		}
		//Si no hay ganador, sigue el juego
		if(!verificar_ganador(humano, pc, jugadas_finales)){
			//Si humano2 aun no capturo 10 fichas y posee movimientos, mueve
			if(pc->capturadas != 10 && verificar_movimiento_todas(pc->valor_ficha)){
				//Si pc tiene 3 fichas, puede volar
				if(FICHAS - humano->capturadas == 3){
					if(FICHAS - pc->capturadas == 3){
						jugadas_finales++;
					}
					pc->fase = 3;
					mueve_pc(arbol_colocar, 1);
				}else{
					mueve_pc(arbol_colocar, 0);
				}
			}else{
				//Si se quedo sin movimientos, humano gana
				humano->capturadas = FICHAS - 2;
				printf("\n\n\tSIN MOVIMIENTOS\n\n");
				system("pause");
			}
		}					
		op = verificar_ganador(humano, pc, jugadas_finales);
	}
}

int main(void){
	int op;
	modo_evaluacion = 1;
	//Puntero a la raiz del arbol para colocar
	struct nodo_arbol *arbol_colocar = NULL;
	do{
		inicializar();
		scanf("%d", &op);
		switch(op){
			//HUMANO VS PC
			case 1:
				hum_pc(&arbol_colocar);
				break;
			//PC VS HUMANO
			case 2:
				pc_hum(&arbol_colocar);
				break;
			//HUMANO VS HUMANO
			case 3:
				pedir_nombre(humano, humano2);
				hum_hum(&arbol_colocar);
				break;
			case 4:
				ayuda();
				break;
			case 5:
				cambiar_evaluacion();
				break;
			case 0:
				break;
			default:
				printf("\n\nERROR: Opcion Invalida");
		}
	}while(op);
}
