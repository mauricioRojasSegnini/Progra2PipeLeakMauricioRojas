#include <stdbool.h>
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

/*
 *Esta es la estructura para las fuentes de agua y los drenajes 
 * contiene la fila y la columna y la direccion en la que se encuentra 
 */
typedef struct waterTapsOrDrains{
	int xRow;
	int yColumn;
	char dir;
}TapsOrDrains;
/*
 * esta es la estructura para los requerimientos que solicita el usuario para ejecutar el programa
 */
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

//subrutinas del programa
void goTo(int **matriz,int row, int column, char dir,TapsOrDrains *tapsOrDrainsArray, int taps, int drains, int matrizRows, int matrizColumns,int **copyMatriz,Requirements game);
void actionsMenu( int** matriz,char action[], TapsOrDrains *tapsOrDrainsArray, int taps, int drains,int matrizRows,int matrizColumns, Requirements game);
void destruirMatriz(int **matriz, const int matrizRows);
int** crearMatriz(const int matrizRows, const int matrizColumns);
int cargarMatriz(int ** matriz, const int matrizRows, const int matrizColumns);
int cargarMatrizDeFile(int ** matriz, const int matrizRows, const int matrizColumns,FILE *fp);
void printGame(int **matriz, const int matrizRows, const int matrizColumns, char printFormat[], char folder[]);
void validateLevel(int** matriz, int row, int column, char dir,TapsOrDrains *tapsOrDrainsArray,int taps, int drains, int matrizRows, int matrizColumns,int **copyMatriz,Requirements game);
void printLeak(int x, int y, char dir,Requirements game);
void solveLevel( int** matriz, int row, int column, char dir,TapsOrDrains *tapsOrDrainsArray,int taps, int drains, int matrizRows, int matrizColumns,int **copyMatriz,Requirements game);
void verify(int **matriz,int row, int column, char dir,TapsOrDrains *tapsOrDrainsArray, int taps, int drains, int matrizRows, int matrizColumns,int **copyMatriz,Requirements game);
int** copy(int** matriz, int matrizRows, int matrizColumns);
int ** rotatePipe(int row,int column, char dir, int**matriz);
void printHeader(int matrizRows,int matrizColumns,int taps, int drains,TapsOrDrains *tapsOrDrainsArray, int **matriz);
int** resetCopyMatriz(int **copyMatriz,int matrizRows, int matrizColumns);
/*
 * this is the main program where it contains all the subroutines required to solve, convert or validate the level
 * and the subroutine to print in the format that the user wants  
 * Efe:
 * Mod:
 * Req: it needs the arguments of the action, the print format, the read format and the file name 
 */
int main(int args_count, char *args[]){
	//crear un array para las fuentes de agua y los drenajes
	TapsOrDrains* tapsOrDrainsArray;
	// crear la estructura del juego
    Requirements game;
    //crear la matriz para la tuberia
    int** matriz;
    //crear las variables 
    int waterTaps, drains, matrizRows, matrizColumns;
    int error=0;
    int rowWaterTap, columnWaterTap;
	int rowDrain, columnDrain;
	char dir;
    
    //indetify what arguments are giving 
    strcpy(game.printFormat,"-ot" ); //default format
    strcpy(game.folderName,""); //default format
    //repetir hasta el fin de los argumentos
    for(int indice=1; indice< args_count; indice++){
		// si el usuario ingreso alguno de las acciones del programa
        if( ((strcmp(args[indice],"validate" ) == 0) ) || ( (strcmp(args[indice],"solve" ) == 0)) || ((strcmp(args[indice],"convert" ) == 0) ) ){
            //asignarle el argumento a la variable de acciones
            strcpy(game.action,args[indice]);
        }else{
			// si el usuario ingreso alguno de los fomatos de lectura del programa
			if( ((strcmp(args[indice],"-it" ) == 0)) || ((strcmp(args[indice],"-ib" ) == 0)) ){
				//asignarle el argumento ingresado a l a variable de lectura
				strcpy(game.readingFormat,args[indice]);
			}else{
				// si el usuario ingreso alguno de los fomatos de salida del programa
				if( (strcmp(args[indice],"-ot" ) == 0) || (strcmp(args[indice],"-ob" )== 0)|| (strcmp(args[indice],"-o" )== 0)) {
					//asignarle el argumento ingresado a la variable de salida
					strcpy(game.printFormat,args[indice]);
				}else{
					//si ingreso un nombre de archivo 
					//asignarle a la variable del nombre de archivo
					strcpy(game.folderName,args[indice]);
				}
			}
		}
    }
    //if the arguments is -it it reads from the standar input
    if(strcmp(game.readingFormat,"-it" ) == 0){
		//si no ingreso un archivo de lectura entonces
		if(strcmp(game.folderName,"" ) == 0){
			//leer desde la entrada estandar
			scanf("%d%d%d%d", &matrizRows, &matrizColumns, &waterTaps, &drains);
			tapsOrDrainsArray = malloc((waterTaps+drains) * sizeof *tapsOrDrainsArray);
			//repetir hasta la cantidad de fuentes de agua
			for(int numOfWTaps=0;numOfWTaps<waterTaps;numOfWTaps++){
				scanf("%d %d %c",&rowWaterTap,&columnWaterTap,&dir);
				tapsOrDrainsArray[numOfWTaps].xRow=rowWaterTap;
				tapsOrDrainsArray[numOfWTaps].yColumn=columnWaterTap;
				tapsOrDrainsArray[numOfWTaps].dir =dir;
			}
			//repetir hasta la cantidad de drenajes
			for(int numOfDrains=waterTaps;numOfDrains<(waterTaps+drains);numOfDrains++){
				scanf("%d %d %c",&rowDrain,&columnDrain,&dir);
				tapsOrDrainsArray[numOfDrains].xRow=rowDrain;
				tapsOrDrainsArray[numOfDrains].yColumn=columnDrain;
				tapsOrDrainsArray[numOfDrains].dir =dir;
			}
			matriz= crearMatriz(matrizRows, matrizColumns);
			error=cargarMatriz(matriz, matrizRows, matrizColumns);
		}else{
			//si ingreso un archivo de lectura
			FILE *fp;
			char blanco;
			fp = fopen(game.folderName, "r");
			//leer los datos de entrada
			matrizRows=fgetc(fp)-48;
			blanco=fgetc(fp);
			matrizColumns=fgetc(fp)-48;
			blanco=fgetc(fp);
			waterTaps=fgetc(fp)-48;
			blanco=fgetc(fp);
			drains=fgetc(fp)-48;
			blanco=fgetc(fp);
			blanco=fgetc(fp);
			tapsOrDrainsArray = malloc((waterTaps+drains) * sizeof *tapsOrDrainsArray);
			//leer las fuentes de agua
			for(int numOfWTaps=0;numOfWTaps<waterTaps;numOfWTaps++){
				rowWaterTap=fgetc(fp)-48;
				blanco=fgetc(fp);
				columnWaterTap=fgetc(fp)-48;
				blanco=fgetc(fp);
				dir=fgetc(fp);
				blanco=fgetc(fp);
				tapsOrDrainsArray[numOfWTaps].xRow=rowWaterTap;
				tapsOrDrainsArray[numOfWTaps].yColumn=columnWaterTap;
				tapsOrDrainsArray[numOfWTaps].dir =dir;
			}
			//leer los drenajes
			for(int numOfDrains=waterTaps;numOfDrains<(waterTaps+drains);numOfDrains++){
				rowDrain=fgetc(fp)-48;
				blanco=fgetc(fp);
				columnDrain=fgetc(fp)-48;
				blanco=fgetc(fp);
				dir=fgetc(fp);
				blanco=fgetc(fp);
				tapsOrDrainsArray[numOfDrains].xRow=rowDrain;
				tapsOrDrainsArray[numOfDrains].yColumn=columnDrain;
				tapsOrDrainsArray[numOfDrains].dir =dir;
			}
			//crear la matriz de tuberia 
			matriz= crearMatriz(matrizRows, matrizColumns);
			//leer los valores de las tuberias
			error=cargarMatrizDeFile(matriz, matrizRows, matrizColumns,fp);
			fclose(fp);
			
			
		}
		
	}else{
		//if the argument is
	}
	//if theres an error, stop
	if(error!=0){
		return 0;
	}else{
		//subroutine to do the action required 
		actionsMenu(matriz, game.action, tapsOrDrainsArray,waterTaps,drains, matrizRows, matrizColumns, game);
		printGame(matriz,matrizRows, matrizColumns, game.printFormat, game.folderName);
		destruirMatriz(matriz, matrizRows);
	}
    return 0;
}
/*
 * Efe: Esta es la subrutina para ejecutar las accion que el usuario ingreso en los argumentos 
 * Mod: no modifica solo ejecuta subrutinas
 * Req: requiere de la matriz de tuberias, la accion requerida, las fuentes de agua y los drenajes, y los parametros de la matriz
 */
void actionsMenu(int** matriz, char action[], TapsOrDrains *tapsOrDrainsArray,int taps, int drains,int matrizRows,int matrizColumns,Requirements game){
	//si el usuario ingreso la accion de validar
	//crear una copia de la matriz para ir marcando las celdas visitadas 
	int **copyMatriz=copy(matriz, matrizRows, matrizColumns);
	if(strcmp( action,"validate" ) == 0){
	//ejecutar subrutina para validar la matriz de tuberia
	validateLevel( matriz, tapsOrDrainsArray[0].xRow-1,tapsOrDrainsArray[0].yColumn-1, tapsOrDrainsArray[0].dir,tapsOrDrainsArray,taps,drains, matrizRows,matrizColumns,copyMatriz, game);
	}else{
		if(strcmp(action,"solve" ) == 0){
			//subrutina para resolver
			solveLevel( matriz, tapsOrDrainsArray[0].xRow-1,tapsOrDrainsArray[0].yColumn-1, tapsOrDrainsArray[0].dir,tapsOrDrainsArray,taps,drains, matrizRows,matrizColumns,copyMatriz, game);
			printHeader(matrizRows, matrizColumns, taps, drains, tapsOrDrainsArray, matriz);
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
 * Efe: Esta es la subrutina para crear la matriz de tuberias con malloc
 * Mod: crea la matriz de tuberias
 * Req: requiere de los parametros de tamano de la matriz
 */
int** crearMatriz(const int matrizRows, const int matrizColumns){
    int **matriz= malloc(matrizRows*sizeof(int *));
    for(int i=0; i<matrizRows; i++){
        matriz[i]= (int*)malloc(matrizColumns*sizeof(int));
    }
    return matriz;
}
/*
 * Efe: Esta es la subrutina para eliminar la memoria de la matriz creada
 * Mod: elimina la matriz de tuberia 
 * Req: requiere de la matriz
 */
void destruirMatriz(int **matriz, const int matrizRows){
	assert(matriz);
	for(int f=0;f<matrizRows;f++){
			free(matriz[f]);
	}
	free(matriz);
}
/*
 * Efe: Esta es la subrutina para cargar la matriz
 * Mod: modifica los valores de las celdas de la matriz por los valores de la entrada
 * Req: requiere de la matriiz y de los parametros de tamano
 */
int cargarMatriz(int ** matriz, const int matrizRows, const int matrizColumns){
    char letra;
    int num;
    //repetir por filas y columnas
    for(int filas_cont=0;filas_cont<matrizRows; filas_cont++){
        for(int columna_cont=0;columna_cont<matrizColumns; columna_cont++){
			//si es un numero entre 1 y 9
			if((scanf("%d",&num)==1)&&(num<=9)){
				//leer valor y asignarlo
				matriz[filas_cont][columna_cont]=num;
			}else{
				//si es una letra cambiarlo por el valor decimal 
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
 * Efe: esta es la subrutina para cargar la matriz si el usuario ingreso un archivo de lectura
 * Mod: carga la matriz con los valores de entrada 
 * Req: de la matriz de tuberia, los parametros de tamano y del archivo de lectura
 */
int cargarMatrizDeFile(int ** matriz, const int matrizRows, const int matrizColumns,FILE *fp){
	int num;
	char blanco;
    blanco=fgetc(fp);
    for(int filas_cont=0;filas_cont<matrizRows; filas_cont++){
        for(int columna_cont=0;columna_cont<matrizColumns; columna_cont++){
			num=fgetc(fp);
			blanco=fgetc(fp);
			if((num>=48)&&(num<=57)){
				//printf("%d\n", num-48);
				matriz[filas_cont][columna_cont]=(num-48);
			}else{
				//printf("%d\n", (int)num-55);
				matriz[filas_cont][columna_cont]=(int)num-55;
			}
        }
    }	
    return 0;
}
/*
 * Efe: imprime la matriz de tuberia
 * Mod:
 * Req: de la matriz y de los parametros de tamano, el formato de salida ingresado por el usuario y el folder de salida
 */
void printGame(int **matriz, const int matrizRows, const int matrizColumns, char printFormat[], char folder[]){
	// if the print format is -ot or its the default format, print in the standard output
	FILE * fichero;
	if(strcmp(folder,"" ) != 0){
		fichero = fopen( folder ,"a");
	}
	if( (strcmp( printFormat,"-ot" ) == 0)||(strcmp( printFormat,"" ) == 0) ){
		for(int filas_cont=0;filas_cont<matrizRows; filas_cont++){
			for(int columna_cont=0;columna_cont<matrizColumns; columna_cont++){
				//imprimir en el archivo
				if(strcmp(folder,"" ) != 0){
					char celda[12]="";
					sprintf(celda,"%d",matriz[filas_cont][columna_cont]);
					fputs(celda,fichero);
					fputs(" ",fichero);
					
				}else{
					//imprimir en formato nomal
					printf("%d ",matriz[filas_cont][columna_cont]);
				}
				    
			}
			if(strcmp(folder,"" ) != 0){ fputs("\n",fichero); }
			putchar('\n');
		}
	}
}
/*
 * Efe: Esta es la subrutina para validar la matriz de tuberia, si hay alguna fuga el programa lo va a detectar e indicar donde es la fuga 
 * Mod:
 * Req: de la matriz, parametros de tamanos de matriz, las funetes y drenajes, la matriz para las celdas visitadas y los requerimientos del juego
 */
void validateLevel(int** matriz, int row, int column, char dir,TapsOrDrains *tapsOrDrainsArray,int taps, int drains, int matrizRows, int matrizColumns,int **copyMatriz,Requirements game){
	int rowCell=row;
	int columnCell=column;
	//Subrutina para evaluar la celda del nivel
	//Asignarle el valor de la celda como visitada
	copyMatriz[rowCell][columnCell]=-1;
	//Condicionar si la celda es 0
	if( (matriz[rowCell][columnCell])==0){
		//Informar del error
		printf("leak %d %d %c\n",row+1, column+1, dir);
	}else
	//Condicionar si la celda es 1
	if(matriz[rowCell][columnCell]==1){
		//Cambiar el valor de la variable dirección por O
		goTo(matriz,rowCell,columnCell-1,'O',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
	}else
	//Condicionar si la celda es 2
	if(matriz[rowCell][columnCell]==2){
		//Ejecutar subrutina para ir al sur de la celda
		goTo(matriz,rowCell+1,columnCell,'S',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
	}else
	//Condicionar si la celda es 3
	if(matriz[rowCell][columnCell]==3){
		//Ejecutar subrutina para ir al oeste de la celda
		goTo(matriz,rowCell,columnCell-1,'O',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
		//Ejecutar subrutina para ir al sur de la celda
		goTo(matriz,rowCell+1,columnCell,'S',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
	}else
	//Condicionar si la celda es 4
	if(matriz[rowCell][columnCell]==4){
		//Ejecutar subrutina para ir al este de la celda
		goTo(matriz,rowCell,columnCell+1,'E',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
	}else
	//Condicionar si la celda es 5
	if(matriz[rowCell][columnCell]==5){
		//Ejecutar subrutina para ir al oeste de la celda
		goTo(matriz,rowCell,columnCell-1,'O',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
		//Ejecutar subrutina para ir al este de la celda
		goTo(matriz,rowCell,columnCell+1,'E',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
	}else
	//Condicionar si la celda es 6
	if(matriz[rowCell][columnCell]==6){
		//Ejecutar subrutina para ir al sur de la celda
		goTo(matriz,rowCell+1,columnCell,'S',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
		//Ejecutar subrutina para ir al este de la celda
		goTo(matriz,rowCell,columnCell+1,'E',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
	}else 
	//Condicionar si la celda es 7
	if(matriz[rowCell][columnCell]==7){
		//Ejecutar subrutina para ir al oeste de la celda
		goTo(matriz,rowCell,columnCell-1,'O',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
		//Ejecutar subrutina para ir al sur de la celda
		goTo(matriz,rowCell+1,columnCell,'S',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
		//Ejecutar subrutina para ir al este de la celda
		goTo(matriz,rowCell,columnCell+1,'E',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
	}else
	//Condicionar si la celda es 8
	if(matriz[rowCell][columnCell]== 8){
		//Ejecutar subrutina para ir al norte de la celda
		goTo(matriz,rowCell-1,columnCell,'N',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
	}else
	//Condicionar si la celda es 9
	if(matriz[rowCell][columnCell]==9){
		//Ejecutar subrutina para ir al norte de la celda
		goTo(matriz,rowCell-1,columnCell,'N',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
		//Ejecutar subrutina para ir al oeste de la celda
		goTo(matriz,rowCell,columnCell-1,'O',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
	}else
	//Condicionar si la celda es A	
	if(matriz[rowCell][columnCell]==10){
		//Ejecutar subrutina para ir al norte de la celda
		goTo(matriz,rowCell-1,columnCell,'N',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);	
		//Ejecutar subrutina para ir al sur de la celda
		goTo(matriz,rowCell+1,columnCell,'S',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
	}else
	//Condicionar si la celda es B
	if(matriz[rowCell][columnCell]==11 ){
		//Ejecutar subrutina para ir al norte de la celda
		goTo(matriz,rowCell-1,columnCell,'N',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
		//Ejecutar subrutina para ir al oeste de la celda
		goTo(matriz,rowCell,columnCell-1,'O',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
		//Ejecutar subrutina para ir al sur de la celda
		goTo(matriz,rowCell+1,columnCell,'S',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
	}else
	//Condicionar si la celda es C
	if(matriz[rowCell][columnCell]==12){
		//Ejecutar subrutina para ir al norte de la celda
		goTo(matriz,rowCell-1,columnCell,'N',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
		//Ejecutar subrutina para ir al este de la celda
		goTo(matriz,rowCell,columnCell+1,'E',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
	}else
	//Condicionar si la celda es 13
	if(matriz[rowCell][columnCell]==13){
		//Ejecutar subrutina para ir al norte de la celda
		goTo(matriz,rowCell-1,columnCell,'N',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
		//Ejecutar subrutina para ir al oeste de la celda
		goTo(matriz,rowCell,columnCell-1,'O',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
		//Ejecutar subrutina para ir al este de la celda
		goTo(matriz,rowCell,columnCell+1,'E',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
	}else
	//Condicionar si la celda es E
	if(matriz[rowCell][columnCell]==14){
		//Ejecutar subrutina para ir al norte de la celda
		goTo(matriz,rowCell-1,columnCell,'N',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
		//Ejecutar subrutina para ir al sur de la celda
		goTo(matriz,rowCell+1,columnCell,'S',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game); 
		//Ejecutar subrutina para ir al este de la celda
		goTo(matriz,rowCell,columnCell+1,'E',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
	}else
	//Condicionar si la celda es F
	if(matriz[rowCell][columnCell]==15){
		//Ejecutar subrutina para ir al norte de la celda
		goTo(matriz,rowCell-1,columnCell,'N',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
		//Ejecutar subrutina para ir al oeste de la celda
		goTo(matriz,rowCell,columnCell-1,'O',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);   
		//Ejecutar subrutina para ir al sur de la celda	
		goTo(matriz,rowCell+1,columnCell,'S',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game); 
		//Ejecutar subrutina para ir al este de la celda
		goTo(matriz,rowCell,columnCell+1,'E',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
	}
//Fin de la subrutina 
}

/*
 * Efe: Subrutina para ir a la direccion requerida y condiciona las celdas si hay alguna fuga la reporta
 * Mod:
 * Req: de la matriz, parametros de tamanos de matriz, las funetes y drenajes, la matriz para las celdas visitadas y los requerimientos del juego
 * Se ocupa por parámetros la fila y la columna y el punto cardinal 
 */
void goTo(int **matriz,int row, int column, char dir,TapsOrDrains *tapsOrDrainsArray, int taps, int drains, int matrizRows, int matrizColumns,int **copyMatriz,Requirements game){
	//Condicionar si la celda se salió del rango
	if((row>=0 && column>=0) &&(row<matrizRows&&column<matrizColumns)){
		//Condicionar si la celda fue visitada
		if(copyMatriz[row][column]==-1){
			//retornar
			return ;
		}
		//Condicionar si la celda es un cero
		if(matriz[row][column]==0){
			printLeak(row, column, dir, game);
			return ;
		}
		//Condicionar si la celda no tiene su punto cardinal opuesto
		if(dir=='N'){
			if( (matriz[row][column]==1) ||(matriz[row][column]==4) ||(matriz[row][column]==5) ||(matriz[row][column]==8) ||(matriz[row][column]==9) ||(matriz[row][column]==12) ||(matriz[row][column]==13) ){
				printLeak(row, column, dir, game);
				return ;
			}
			
		}else{
			if(dir=='O'){
				if( (matriz[row][column]==1) ||(matriz[row][column]==2) ||(matriz[row][column]==3) ||(matriz[row][column]==8) ||(matriz[row][column]==9) ||(matriz[row][column]==10) ||(matriz[row][column]==11) ){
							printLeak(row, column, dir, game);
							return ;
						}
			
			}else{
				if(dir=='E'){
					if( (matriz[row][column]==2) ||(matriz[row][column]==4) ||(matriz[row][column]==6) ||(matriz[row][column]==8) ||(matriz[row][column]==10) ||(matriz[row][column]==12) ||(matriz[row][column]==14) ){
							printLeak(row, column, dir, game);
							return ;
						}
				}else{
					if(dir=='S'){
						if( (matriz[row][column]==1) ||(matriz[row][column]==2) ||(matriz[row][column]==3) ||(matriz[row][column]==4) ||(matriz[row][column]==5) ||(matriz[row][column]==6) ||(matriz[row][column]==7) ){
							printLeak(row, column, dir, game);
							return ;
						}
					}else{
						return ;
					}
				}
			}	
		}
		//Condicionar si la celda tiene otro numero
		if(matriz[row][column]>=0 && matriz[row][column]<=15){
			//Ejecutar subrutina para evaluar la siguiente celda
			validateLevel(matriz, row, column, dir, tapsOrDrainsArray, taps,  drains,  matrizRows,  matrizColumns,copyMatriz, game);
		} 
	}else{
		int rowC=row+1;
		int colC=column+1;
		if(dir=='N'){
			rowC=rowC+1;
		}
		if(dir=='S'){
			rowC=rowC-1;
		}
		if(dir=='E'){
			colC=colC-1;
		}
		if(dir=='O'){
			colC=colC+1;
		}
		//Condicionar si la celda es la fuente o el desague
		for(int waterTapOrDrains=0; waterTapOrDrains<taps+drains; waterTapOrDrains++){
			if( (tapsOrDrainsArray[waterTapOrDrains].dir==dir)&&(tapsOrDrainsArray[waterTapOrDrains].xRow==rowC)&&(tapsOrDrainsArray[waterTapOrDrains].yColumn==colC) ){
				return ;
			}
		}
		printLeak(row, column, dir, game);
		return ;
	}
//Fin de la subrutina 
}
/*
 * Efe: imprime la fuga de tuberia
 * Mod:
 * Req: de la posicion de la fuga y la direccion
 */
void printLeak(int x, int y, char dir,Requirements game){
		FILE * fichero;
		fichero = fopen( game.folderName ,"a");
		int row=x+1;
		int col=y+1;
		if(strcmp(game.folderName,"" ) != 0){
			char rowNumber[12];
			char colNumber[12];
			if(dir=='N'){
				sprintf(rowNumber,"%d",row+1);
				sprintf(colNumber,"%d",col);
				fputs("leak ",fichero);
				fputs(rowNumber,fichero);
				fputs(" ",fichero);
				fputs(colNumber,fichero);
				fputs(" ",fichero);
				fputc(dir,fichero);
			}
			if(dir=='S'){
				sprintf(rowNumber,"%d",row-1);
				sprintf(colNumber,"%d",col);
				fputs("leak ",fichero);
				fputs(rowNumber,fichero);
				fputs(" ",fichero);
				fputs(colNumber,fichero);
				fputs(" ",fichero);
				fputc(dir,fichero);
			}
			if(dir=='E'){
				sprintf(rowNumber,"%d",row);
				sprintf(colNumber,"%d",col-1);
				fputs("leak ",fichero);
				fputs(rowNumber,fichero);
				fputs(" ",fichero);
				fputs(colNumber,fichero);
				fputs(" ",fichero);
				fputc(dir,fichero);
			}
			if(dir=='O'){
				sprintf(rowNumber,"%d",row);
				sprintf(colNumber,"%d",col+1);
				fputs("leak ",fichero);
				fputs(rowNumber,fichero);
				fputs(" ",fichero);
				fputs(colNumber,fichero);
				fputs(" ",fichero);
				fputc(dir,fichero);
			}
			fputc('\n',fichero);
		}else{
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
		
}
/*
 * Efe: copia la matriz de tuberia
 * Mod:
 * Req: de la matriz y de los parametros de tamano
 */
int** copy(int** matriz, int matrizRows, int matrizColumns){
	int **copy= malloc(matrizRows*sizeof(int *));
    for(int i=0; i<matrizRows; i++){
        copy[i]= (int*)malloc(matrizColumns*sizeof(int));
    }
	for(int row=0;row<matrizRows; row++){
		for(int col=0;col<matrizColumns; col++){
			copy[row][col]=matriz[row][col];
		}
	}
	return copy;
}
void solveLevel(int** matriz, int row, int column, char dir,TapsOrDrains *tapsOrDrainsArray,int taps, int drains, int matrizRows, int matrizColumns,int **copyMatriz,Requirements game){
	int rowCell=row;
	int columnCell=column;
	//Subrutina para evaluar la celda del nivel
	//Asignarle el valor de la celda como visitada
	copyMatriz[rowCell][columnCell]=-1;
	//Condicionar si la celda es 0
	if( (matriz[rowCell][columnCell])==0){
		//Informar del error
		return;
	}else
	//Condicionar si la celda es 1
	if(matriz[rowCell][columnCell]==1){
		//Cambiar el valor de la variable dirección por O
		verify(matriz,rowCell,columnCell-1,'O',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
	}else
	//Condicionar si la celda es 2
	if(matriz[rowCell][columnCell]==2){
		//Ejecutar subrutina para ir al sur de la celda
		verify(matriz,rowCell+1,columnCell,'S',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
	}else
	//Condicionar si la celda es 3
	if(matriz[rowCell][columnCell]==3){
		//Ejecutar subrutina para ir al oeste de la celda
		verify(matriz,rowCell,columnCell-1,'O',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
		//Ejecutar subrutina para ir al sur de la celda
		verify(matriz,rowCell+1,columnCell,'S',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
	}else
	//Condicionar si la celda es 4
	if(matriz[rowCell][columnCell]==4){
		//Ejecutar subrutina para ir al este de la celda
		verify(matriz,rowCell,columnCell+1,'E',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
	}else
	//Condicionar si la celda es 5
	if(matriz[rowCell][columnCell]==5){
		//Ejecutar subrutina para ir al oeste de la celda
		verify(matriz,rowCell,columnCell-1,'O',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
		//Ejecutar subrutina para ir al este de la celda
		verify(matriz,rowCell,columnCell+1,'E',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
	}else
	//Condicionar si la celda es 6
	if(matriz[rowCell][columnCell]==6){
		//Ejecutar subrutina para ir al sur de la celda
		verify(matriz,rowCell+1,columnCell,'S',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
		//Ejecutar subrutina para ir al este de la celda
		verify(matriz,rowCell,columnCell+1,'E',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
	}else 
	//Condicionar si la celda es 7
	if(matriz[rowCell][columnCell]==7){
		//Ejecutar subrutina para ir al oeste de la celda
		verify(matriz,rowCell,columnCell-1,'O',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
		//Ejecutar subrutina para ir al sur de la celda
		verify(matriz,rowCell+1,columnCell,'S',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
		//Ejecutar subrutina para ir al este de la celda
		verify(matriz,rowCell,columnCell+1,'E',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
	}else
	//Condicionar si la celda es 8
	if(matriz[rowCell][columnCell]== 8){
		//Ejecutar subrutina para ir al norte de la celda
		verify(matriz,rowCell-1,columnCell,'N',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
	}else
	//Condicionar si la celda es 9
	if(matriz[rowCell][columnCell]==9){
		//Ejecutar subrutina para ir al norte de la celda
		verify(matriz,rowCell-1,columnCell,'N',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
		//Ejecutar subrutina para ir al oeste de la celda
		verify(matriz,rowCell,columnCell-1,'O',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
	}else
	//Condicionar si la celda es A	
	if(matriz[rowCell][columnCell]==10){
		//Ejecutar subrutina para ir al norte de la celda
		verify(matriz,rowCell-1,columnCell,'N',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);	
		//Ejecutar subrutina para ir al sur de la celda
		verify(matriz,rowCell+1,columnCell,'S',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
	}else
	//Condicionar si la celda es B
	if(matriz[rowCell][columnCell]==11 ){
		//Ejecutar subrutina para ir al norte de la celda
		verify(matriz,rowCell-1,columnCell,'N',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
		//Ejecutar subrutina para ir al oeste de la celda
		verify(matriz,rowCell,columnCell-1,'O',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
		//Ejecutar subrutina para ir al sur de la celda
		verify(matriz,rowCell+1,columnCell,'S',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
	}else
	//Condicionar si la celda es C
	if(matriz[rowCell][columnCell]==12){
		//Ejecutar subrutina para ir al norte de la celda
		verify(matriz,rowCell-1,columnCell,'N',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
		//Ejecutar subrutina para ir al este de la celda
		verify(matriz,rowCell,columnCell+1,'E',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
	}else
	//Condicionar si la celda es 13
	if(matriz[rowCell][columnCell]==13){
		//Ejecutar subrutina para ir al norte de la celda
		verify(matriz,rowCell-1,columnCell,'N',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
		//Ejecutar subrutina para ir al oeste de la celda
		verify(matriz,rowCell,columnCell-1,'O',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
		//Ejecutar subrutina para ir al este de la celda
		verify(matriz,rowCell,columnCell+1,'E',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
	}else
	//Condicionar si la celda es E
	if(matriz[rowCell][columnCell]==14){
		//Ejecutar subrutina para ir al norte de la celda
		verify(matriz,rowCell-1,columnCell,'N',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
		//Ejecutar subrutina para ir al sur de la celda
		verify(matriz,rowCell+1,columnCell,'S',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game); 
		//Ejecutar subrutina para ir al este de la celda
		verify(matriz,rowCell,columnCell+1,'E',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
	}else
	//Condicionar si la celda es F
	if(matriz[rowCell][columnCell]==15){
		//Ejecutar subrutina para ir al norte de la celda
		verify(matriz,rowCell-1,columnCell,'N',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
		//Ejecutar subrutina para ir al oeste de la celda
		verify(matriz,rowCell,columnCell-1,'O',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);   
		//Ejecutar subrutina para ir al sur de la celda	
		verify(matriz,rowCell+1,columnCell,'S',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game); 
		//Ejecutar subrutina para ir al este de la celda
		verify(matriz,rowCell,columnCell+1,'E',tapsOrDrainsArray,taps, drains,  matrizRows,  matrizColumns,copyMatriz, game);
	}
	
}
void verify(int **matriz,int row, int column, char dir,TapsOrDrains *tapsOrDrainsArray, int taps, int drains, int matrizRows, int matrizColumns,int **copyMatriz,Requirements game){
	//Condicionar si la celda se salió del rango
	if((row>=0 && column>=0) &&(row<matrizRows&&column<matrizColumns)){
		//Condicionar si la celda fue visitada
		if(copyMatriz[row][column]==-1){
			//retornar
			return ;
		}
		//Condicionar si la celda es un cero
		if(matriz[row][column]==0){
			return;
		}
		//Condicionar si la celda no tiene su punto cardinal opuesto
		if(dir=='N'){
			if( (matriz[row][column]==1) ||(matriz[row][column]==4) ||(matriz[row][column]==5) ||(matriz[row][column]==8) ||(matriz[row][column]==9) ||(matriz[row][column]==12) ||(matriz[row][column]==13) ){
				matriz=rotatePipe(row, column, dir, matriz);
				solveLevel(matriz,tapsOrDrainsArray[0].xRow-1,tapsOrDrainsArray[0].yColumn-1, tapsOrDrainsArray[0].dir,tapsOrDrainsArray, taps, drains, matrizRows, matrizColumns, copyMatriz,game);
				return ;
			}
			
		}else{
			if(dir=='O'){
				if( (matriz[row][column]==1) ||(matriz[row][column]==2) ||(matriz[row][column]==3) ||(matriz[row][column]==8) ||(matriz[row][column]==9) ||(matriz[row][column]==10) ||(matriz[row][column]==11) ){
							matriz=rotatePipe(row, column, dir, matriz);
							solveLevel(matriz,tapsOrDrainsArray[0].xRow-1,tapsOrDrainsArray[0].yColumn-1, tapsOrDrainsArray[0].dir,tapsOrDrainsArray, taps, drains, matrizRows, matrizColumns, copyMatriz,game);
							return ;
						}
			
			}else{
				if(dir=='E'){
					if( (matriz[row][column]==2) ||(matriz[row][column]==4) ||(matriz[row][column]==6) ||(matriz[row][column]==8) ||(matriz[row][column]==10) ||(matriz[row][column]==12) ||(matriz[row][column]==14) ){
							matriz=rotatePipe(row, column, dir, matriz);
							solveLevel(matriz,tapsOrDrainsArray[0].xRow-1,tapsOrDrainsArray[0].yColumn-1, tapsOrDrainsArray[0].dir,tapsOrDrainsArray, taps, drains, matrizRows, matrizColumns, copyMatriz,game);
							return ;
						}
				}else{
					if(dir=='S'){
						if( (matriz[row][column]==1) ||(matriz[row][column]==2) ||(matriz[row][column]==3) ||(matriz[row][column]==4) ||(matriz[row][column]==5) ||(matriz[row][column]==6) ||(matriz[row][column]==7) ){
							matriz=rotatePipe(row, column, dir, matriz);
							solveLevel(matriz, tapsOrDrainsArray[0].xRow-1,tapsOrDrainsArray[0].yColumn-1, tapsOrDrainsArray[0].dir,tapsOrDrainsArray, taps,  drains,  matrizRows,  matrizColumns,copyMatriz, game);
							return ;
						}
					}else{
						return ;
					}
				}
			}	
		}
		//Condicionar si la celda tiene otro numero
		if(matriz[row][column]>=0 && matriz[row][column]<=15){
			//Ejecutar subrutina para evaluar la siguiente celda
			solveLevel(matriz, row, column, dir, tapsOrDrainsArray, taps,  drains,  matrizRows,  matrizColumns,copyMatriz, game);
		} 
	}else{
		int rowC=row+1;
		int colC=column+1;
		if(dir=='N'){
			rowC=rowC+1;
		}
		if(dir=='S'){
			rowC=rowC-1;
		}
		if(dir=='E'){
			colC=colC-1;
		}
		if(dir=='O'){
			colC=colC+1;
		}
		//Condicionar si la celda es la fuente o el desague
		for(int waterTapOrDrains=0; waterTapOrDrains<taps+drains; waterTapOrDrains++){
			if( (tapsOrDrainsArray[waterTapOrDrains].dir==dir)&&(tapsOrDrainsArray[waterTapOrDrains].xRow==rowC)&&(tapsOrDrainsArray[waterTapOrDrains].yColumn==colC) ){
				return ;
			}
		}
		rowC=rowC-1;
		colC=colC-1;
		matriz=rotatePipe(rowC, colC, dir, matriz);
		copyMatriz=resetCopyMatriz( copyMatriz, matrizRows,  matrizColumns);
		solveLevel(matriz, tapsOrDrainsArray[0].xRow-1,tapsOrDrainsArray[0].yColumn-1, tapsOrDrainsArray[0].dir,tapsOrDrainsArray, taps,  drains,  matrizRows,  matrizColumns,copyMatriz, game);
		return ;
	}
}
int ** rotatePipe(int row,int column, char dir, int**matriz){
	int celda=matriz[row][column];
	int rotaciones;
	int total=15;
	//printf("celda a rotar %d\n",celda);
	if(celda!=0 && celda!=15){
		if(celda%3==0){
			rotaciones=3;
			matriz[row][column]=(celda+rotaciones)%total;
		}
		if(celda%5==0){
			if(celda==5){
				matriz[row][column]=10;
			}else{
				matriz[row][column]=5;
			}
		}
		if((celda==7)||(celda==11)||(celda==13)||(celda==14)){
			if(celda==7){
				matriz[row][column]=11;
			}else{
				if(celda==11){
					matriz[row][column]=13;
				}else{
					if(celda==13){
						matriz[row][column]=14;
					}else{
						if(celda==14){
							matriz[row][column]=7;
						}
					}
				}
			}
		}
		if((celda==1)||(celda==2)||(celda==4)||(celda==8)){
			if(celda==1){
				matriz[row][column]=2;
			}else{
				if(celda==2){
					matriz[row][column]=4;
				}else{
					if(celda==4){
						matriz[row][column]=8;
					}else{
						if(celda==8){
							matriz[row][column]=1;
						}
					}
				}
			}
		}
		
	}else{
		printf("it cannot be solved\n");
	}
	return matriz;
}
/*
 * Subrutina para imprimir el encabezado de los datos de entrada
 * no modifica solo imprime
 */
void printHeader(int matrizRows,int matrizColumns,int taps, int drains,TapsOrDrains *tapsOrDrainsArray, int **matriz){
	printf("\n%d %d %d %d\n", matrizRows,matrizColumns, taps, drains);
	//imprimir hasta la cantidad de fuentes de agua
	for(int numOfWTaps=0;numOfWTaps<taps;numOfWTaps++){
		printf("%d %d %c\n",tapsOrDrainsArray[numOfWTaps].xRow,tapsOrDrainsArray[numOfWTaps].yColumn,tapsOrDrainsArray[numOfWTaps].dir);
	}
	//imprimir hasta la cantidad de drenajes
	for(int numOfDrains=taps;numOfDrains<(taps+drains);numOfDrains++){
		printf("%d %d %c\n",tapsOrDrainsArray[numOfDrains].xRow,tapsOrDrainsArray[numOfDrains].yColumn,tapsOrDrainsArray[numOfDrains].dir);
	}
	printf("\n");
}
int** resetCopyMatriz(int **copyMatriz,int matrizRows, int matrizColumns){
	for(int filas_cont=0;filas_cont<matrizRows; filas_cont++){
		for(int columna_cont=0;columna_cont<matrizColumns; columna_cont++){
			copyMatriz[filas_cont][columna_cont]=0;
		}
	}
				
	return copyMatriz;
}

