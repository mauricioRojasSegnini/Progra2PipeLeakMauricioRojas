
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
/*
 * @mauricioRojasSegnini and @
 * This is the game's program similar to Pipe Mania where it finds a solution for the level 
 * The player must build or rebuild a pipe through which a fluid, usually water, will pass through.
 * The pipe is built on a matrix area, where fluid arises from one or more sources and must reach one or more destinations.
 * The program receives the status of a game in progress by standard input or in files. The content in the standard input or in the files could be text or binary.
 * The program must validate, resolve, or convert the level it receives according to the user's choice.
 * v.1
*/

typedef struct waterTapsOrDrains{
	int x;
	int y;
	char dir;
}TapsOrDrains;

typedef struct Requirements{
    //create variable for the action required 
    char action[40];
    //create variable for the print format required 
    char printFormat[40];
    //create variable for the print format required
    char readingFormat[40];
    //create a variable for the file name 
    char folderName[40];
}Requirements;

void goTo(int **matriz,int row, int column, char dir,TapsOrDrains *tapsOrDrainsArray, int taps, int drains, int matrizRows, int matrizColumns);
void actionsMenu( int** matriz,char action[], TapsOrDrains *tapsOrDrainsArray, int taps, int drains,int matrizRows,int matrizColumns);
void destruirMatriz(int **matriz, const int matrizRows);
int** crearMatriz(const int matrizRows, const int matrizColumns);
int cargarMatriz(int ** matriz, const int matrizRows, const int matrizColumns);
void printGame(int **matriz, const int matrizRows, const int matrizColumns, char printFormat[]);
void validateLevel(int** matriz, int row, int column, char dir,TapsOrDrains *tapsOrDrainsArray,int taps, int drains, int matrizRows, int matrizColumns);
void printLeak(int x, int y, char dir);
/*
 * this is the main program where it contains all the subroutines required to solve, convert or validate the level
 * and the subrputine to print in the format that the user wants  
 * Efe:
 * Mod:
 * Req: it needs the arguments of the action, the print format, the read format and the file name 
 */
int main(int args_count, char *args[]){
	TapsOrDrains* tapsOrDrainsArray;
    Requirements game;
    int** matriz;
    int waterTaps, drains, matrizRows, matrizColumns, rowWaterTap, columnWaterTap, rowDrain, columnDrain;
    int error=0;
    //indetify what arguments are giving 
    strcpy(game.printFormat,"-ot" ); //default format
    for(int indice=1; indice< args_count; indice++){
        if( ((strcmp(args[indice],"validate" ) == 0) ) || ( (strcmp(args[indice],"solve" ) == 0)) || ((strcmp(args[indice],"convert" ) == 0) ) ){
            strcpy(game.action,args[indice]);
        }
        if( ((strcmp(args[indice],"-it" ) == 0)) || ((strcmp(args[indice],"-ib" ) == 0)) ){
            strcpy(game.readingFormat,args[indice]);
        }
        if( (strcmp(args[indice],"-ot" ) == 0) || (strcmp(args[indice],"-ob" )== 0)|| (strcmp(args[indice],"-o" )== 0)) {
            strcpy(game.printFormat,args[indice]);
        }
        
    }
    //if the arguments is -it it reads from the standar input
    if(strcmp(game.readingFormat,"-it" ) == 0){
		int x,y;
		char dir;
		scanf("%d%d%d%d", &matrizRows, &matrizColumns, &waterTaps, &drains);
		tapsOrDrainsArray = malloc((waterTaps+drains) * sizeof *tapsOrDrainsArray);
		for(int numOfWTaps=0;numOfWTaps<waterTaps;numOfWTaps++){
			scanf("%d %d %c",&x,&y,&dir);
			tapsOrDrainsArray[numOfWTaps].x=x;
			tapsOrDrainsArray[numOfWTaps].y=y;
			tapsOrDrainsArray[numOfWTaps].dir =dir;
		}
		for(int numOfDrains=waterTaps;numOfDrains<(waterTaps+drains);numOfDrains++){
			scanf("%d %d %c",&x,&y,&dir);
			tapsOrDrainsArray[numOfDrains].x=x;
			tapsOrDrainsArray[numOfDrains].y=y;
			tapsOrDrainsArray[numOfDrains].dir =dir;
		}
		matriz= crearMatriz(matrizRows, matrizColumns);
		error=cargarMatriz(matriz, matrizRows, matrizColumns);
	}else{
		//if the argument is
	}
	//if theres an error, stop
	if(error!=0){
		return 0;
	}else{
		//subroutine to do the action required 
		actionsMenu(matriz, game.action, tapsOrDrainsArray,waterTaps,drains, matrizRows, matrizColumns);
		printGame(matriz,matrizRows, matrizColumns, game.printFormat);
		destruirMatriz(matriz, matrizRows);
	}
    return 0;
}
/*
 * Efe:
 * Mod:
 * Req:
 */
void actionsMenu(int** matriz, char action[], TapsOrDrains *tapsOrDrainsArray,int taps, int drains,int matrizRows,int matrizColumns){
	 if(strcmp( action,"validate" ) == 0){
		validateLevel( matriz, tapsOrDrainsArray[0].x-1,tapsOrDrainsArray[0].y-1, tapsOrDrainsArray[0].dir,tapsOrDrainsArray,taps,drains, matrizRows,matrizColumns);
	}else{
		if(strcmp(action,"solve" ) == 0){
			printf("subrutina para resolver\n");
		}else{
			if(strcmp(action,"convert" ) == 0){
				printf("subrutina para convertir\n");
			}else{
				printf("error\n");
			}
		}
	}
  
}
/*
 * Efe:
 * Mod:
 * Req:
 */
int** crearMatriz(const int matrizRows, const int matrizColumns){
    int **matriz= malloc(matrizRows*sizeof(int *));
    for(int i=0; i<matrizRows; i++){
        matriz[i]= (int*)malloc(matrizColumns*sizeof(int));
    }
    return matriz;
}
/*
 * Efe:
 * Mod:
 * Req:
 */
void destruirMatriz(int **matriz, const int matrizRows){
	assert(matriz);
	for(int f=0;f<matrizRows;f++){
			free(matriz[f]);
	}
	free(matriz);
}
/*
 * Efe:
 * Mod:
 * Req:
 */
int cargarMatriz(int ** matriz, const int matrizRows, const int matrizColumns){
    char letra;
    int num;
    for(int filas_cont=0;filas_cont<matrizRows; filas_cont++){
        for(int columna_cont=0;columna_cont<matrizColumns; columna_cont++){
			if((scanf("%d",&num)==1)&&(num<=9)){
				matriz[filas_cont][columna_cont]=num;
			}else{
				scanf("%c",&letra);
				
				if(letra=='A'){
					num=10;
				}else if(letra=='B'){
					num=11;
				}else if(letra=='C'){
					num=12;
				}else if(letra=='D'){
					num=13;
				}else if(letra=='E'){
					num=14;
				}else if(letra=='F'){
					num=15;
				}else{
					printf("wrong number");
					return 1; // return error
				}
				matriz[filas_cont][columna_cont]=num;
			}
        }
    }
	return 0;

}
/*
 * Efe:
 * Mod:
 * Req:
 */
void printGame(int **matriz, const int matrizRows, const int matrizColumns, char printFormat[]){

	// if the print format is -ot or its the default format, print in the standard output
	if( (strcmp( printFormat,"-ot" ) == 0)||(strcmp( printFormat,"" ) == 0) ){
		for(int filas_cont=0;filas_cont<matrizRows; filas_cont++){
			for(int columna_cont=0;columna_cont<matrizColumns; columna_cont++){
				printf("%d ",matriz[filas_cont][columna_cont]);        
			}
			putchar('\n');
		}
	}
}
void validateLevel(int** matriz, int row, int column, char dir,TapsOrDrains *tapsOrDrainsArray,int taps, int drains, int matrizRows, int matrizColumns){
	int rowCell=row;
	int columnCell=column;
	//Subrutina para evaluar la celda del nivel
	//Asignarle el valor de la celda como visitada
	//Condicionar si la celda es 0
	if( (matriz[rowCell][columnCell])==0){
		//Informar del error
		printf("leak %d %d %c\n",row, column, dir);
	}
	/*
	Condicionar si la celda es 1
		Cambiar el valor de la variable dirección por O
		Ejecutar subrutina para ir al oeste de la celda (fila, columna-1, direcion)
	Condicionar si la celda es 2
		Cambiar el valor de la variable dirección por S
		Ejecutar subrutina para ir al sur de la celda
		*/
	//Condicionar si la celda es 3
	if(matriz[rowCell][columnCell]==3){
		//Cambiar el valor de la variable dirección por O
		//Ejecutar subrutina para ir al oeste de la celda
		//Cambiar el valor de la variable dirección por S
		//Ejecutar subrutina para ir al sur de la celda
		//goTo(matriz,rowCell+1,columnCell+0,'S',tapsOrDrainsArray,taps, drains);
	}else
		
		/*
	Condicionar si la celda es 4
		Cambiar el valor de la variable dirección por E
		Ejecutar subrutina para ir al este de la celda
		*/
	//Condicionar si la celda es 5
	if(matriz[rowCell][columnCell]==5){
		//Cambiar el valor de la variable dirección por O
		//Ejecutar subrutina para ir al oeste de la celda
		//Cambiar el valor de la variable dirección por E
		//Ejecutar subrutina para ir al este de la celda
	}else
		
	//Condicionar si la celda es 6
	if(matriz[rowCell][columnCell]==6){
		//Cambiar el valor de la variable dirección por S
		//Ejecutar subrutina para ir al sur de la celda
		//Cambiar el valor de la variable dirección por E
		//Ejecutar subrutina para ir al este de la celda
	}else 
	if(matriz[rowCell][columnCell]==7){
	//Condicionar si la celda es 7
		//Cambiar el valor de la variable dirección por O
		//Ejecutar subrutina para ir al oeste de la celda
		//goTo(matriz,rowCell,columnCell-1,'O',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns);
		//Cambiar el valor de la variable dirección por S
		goTo(matriz,rowCell+1,columnCell,'S',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns);
		//Ejecutar subrutina para ir al sur de la celda
		//Cambiar el valor de la variable dirección por E
		//Ejecutar subrutina para ir al este de la celda
		goTo(matriz,rowCell,columnCell+1,'E',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns);
	}else
		/*
	Condicionar si la celda es 8
		Cambiar el valor de la variable dirección por N
		Ejecutar subrutina para ir al norte de la celda
	Condicionar si la celda es 9
		Cambiar el valor de la variable dirección por N
		Ejecutar subrutina para ir al norte de la celda
		Cambiar el valor de la variable dirección por O
		Ejecutar subrutina para ir al oeste de la celda
	Condicionar si la celda es A
		Cambiar el valor de la variable dirección por N
		Ejecutar subrutina para ir al norte de la celda
		Cambiar el valor de la variable dirección por S
		Ejecutar subrutina para ir al sur de la celda
	Condicionar si la celda es B
		Cambiar el valor de la variable dirección por N
		Ejecutar subrutina para ir al norte de la celda
		Cambiar el valor de la variable dirección por O
		Ejecutar subrutina para ir al oeste de la celda
		Cambiar el valor de la variable dirección por S
		Ejecutar subrutina para ir al sur de la celda
		*/
	//Condicionar si la celda es 12
	if(matriz[rowCell][columnCell]==12){
		//Cambiar el valor de la variable dirección por N
		//Ejecutar subrutina para ir al norte de la celda
		goTo(matriz,rowCell-1,columnCell,'N',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns);
		//Cambiar el valor de la variable dirección por E
		//Ejecutar subrutina para ir al este de la celda
		goTo(matriz,rowCell,columnCell+1,'E',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns);
	}else
	//Condicionar si la celda es 13
	if(matriz[rowCell][columnCell]==13){
		//Cambiar el valor de la variable dirección por N
		//Ejecutar subrutina para ir al norte de la celda
		//Cambiar el valor de la variable dirección por O
		//Ejecutar subrutina para ir al oeste de la celda
		//Cambiar el valor de la variable dirección por E
		//Ejecutar subrutina para ir al este de la celda
	}
		/*
	Condicionar si la celda es E
		Cambiar el valor de la variable dirección por N
		Ejecutar subrutina para ir al norte de la celda
		Cambiar el valor de la variable dirección por S
		Ejecutar subrutina para ir al sur de la celda 
		Cambiar el valor de la variable dirección por E
		Ejecutar subrutina para ir al este de la celda
	Condicionar si la celda es F
		Cambiar el valor de la variable dirección por N
		Ejecutar subrutina para ir al norte de la celda
		Cambiar el valor de la variable dirección por O
		Ejecutar subrutina para ir al oeste de la celda
		Cambiar el valor de la variable dirección por S    
		Ejecutar subrutina para ir al sur de la celda
		Cambiar el valor de la variable dirección por E
		Ejecutar subrutina para ir al este de la celda
Fin de la subrutina 
 */
}

//Subrutina para ir a la direccion requerida 
//Se ocupa por parámetros la fila y la columna y el punto cardinal 
void goTo(int **matriz,int row, int column, char dir,TapsOrDrains *tapsOrDrainsArray, int taps, int drains, int matrizRows, int matrizColumns){
	//Condicionar si la celda se salió del rango
	if((row>=0 && column>=0) &&(row<matrizRows&&column<matrizColumns)){
		//Condicionar si la celda es un cero
		if(matriz[row][column]==0){
			printLeak(row, column, dir);
			//printf("leak in %d %d %c\n",row, column, dir);
			return;
		}
		//Condicionar si la celda no tiene su punto cardinal opuesto
			//imprimir fuga la fila y la columna y la etiqueta
			//retornar
		//Condicionar si la celda fue visitada
			//retornar
		//Condicionar si la celda tiene otro numero
		if(matriz[row][column]>=0 && matriz[row][column]<=15){
			//Ejecutar subrutina para evaluar la siguiente celda
			//Mandar por parámetros la fila y la columna actual
			validateLevel(matriz, row, column, dir, tapsOrDrainsArray, taps,  drains,  matrizRows,  matrizColumns);
		} 
	}else{
		//Condicionar si la celda es la fuente o el desague
		for(int waterTapOrDrains=0; waterTapOrDrains<taps+drains; waterTapOrDrains++){
			if( (tapsOrDrainsArray[waterTapOrDrains].dir==dir) ){
				return;
			}
		}
		printf("leak %d %d %c\n",row, column, dir);
	}
//Fin de la subrutina 
}
void printLeak(int x, int y, char dir){
		int row=x+1;
		int col=y+1;
		if(dir=='N'){
			printf("leak %d %d %c\n",row+1, col, dir);
		}
		if(dir=='S'){
			printf("leak %d %d %c\n",row-1, col, dir);
		}
		if(dir=='E'){
			printf("leak %d %d %c\n",row, col-1, dir);
		}
		if(dir=='O'){
			printf("leak %d %d %c\n",row, col+1, dir);
		}
}





