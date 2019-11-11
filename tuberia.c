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

void actionsMenu( int** matriz,char action[]);
void destruirMatriz(int **matriz, const int matrizRows);
int** crearMatriz(const int matrizRows, const int matrizColumns);
void cargarMatriz(int ** matriz, const int matrizRows, const int matrizColumns);
void printGame(int **matriz, const int matrizRows, const int matrizColumns);

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
typedef struct waterTapsOrDrains{
	int x;
	int y;
	int dir;
}TapsOrDrains;

/*
 * this is the main program where it contains all the subroutines required to solve, convert or validate the level
 * and the subrputine to print in the format that the user wants  
 * Efe:
 * Mod:
 * Req: it needs the arguments of the action, the print format, the read format and the file name 
 */
int main(int args_count, char *args[]){
    Requirements game;
    int** matriz;
    int waterTaps, drains, matrizRows, matrizColumns, rowWaterTap, columnWaterTap, rowDrain, columnDrain;
    char dir;
    //indetify what arguments are giving 
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
        if(strcmp(args[indice],"-ot" ) == 0){
		}
    }
    //if the arguments is -it it reads from the standar input
    if(strcmp(game.readingFormat,"-it" ) == 0){
		scanf("%d%d%d%d", &matrizRows, &matrizColumns, &waterTaps, &drains);
		
		matriz= crearMatriz(matrizRows, matrizColumns);
		cargarMatriz(matriz, matrizRows, matrizColumns);
	}else{
		//if the argument is
	}
	//subroutine to do the action required 
	actionsMenu(matriz,game.action);
	printGame(matriz,matrizRows,matrizColumns);
	destruirMatriz(matriz, matrizRows);
    return 0;
}
/*
 * Efe:
 * Mod:
 * Req:
 */
void actionsMenu(int** matriz, char action[]){
	 if(strcmp( action,"validate" ) == 0){
		printf("subrutina para validar\n");
		
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
void cargarMatriz(int ** matriz, const int matrizRows, const int matrizColumns){
    char letra;
    int num;
    for(int filas_cont=0;filas_cont<matrizRows; filas_cont++){
        for(int columna_cont=0;columna_cont<matrizColumns; columna_cont++){
			if(scanf("%d",&num)==1){
				matriz[filas_cont][columna_cont]=num;
			}else{
				scanf("%c",&letra);
				
				if(letra=='A'){
					printf("letra%c\n",letra);
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
				}
				matriz[filas_cont][columna_cont]=num;
			}
        }
    }

}
/*
 * Efe:
 * Mod:
 * Req:
 */
void printGame(int **matriz, const int matrizRows, const int matrizColumns){
    for(int filas_cont=0;filas_cont<matrizRows; filas_cont++){
        for(int columna_cont=0;columna_cont<matrizColumns; columna_cont++){
            printf("%d ",matriz[filas_cont][columna_cont]);        
		}
        putchar('\n');
    }
}



