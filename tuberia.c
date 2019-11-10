#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

void actionsMenu( char action[]);
void destruirMatriz(int **matriz, const int matrizRows);
int** crearMatriz(const int matrizRows, const int matrizColumns);
void cargarMatriz(int ** matriz, const int matrizRows, const int matrizColumns);
void imprimirMatriz(int **matriz, const int matrizRows, const int matrizColumns);
typedef struct Requirements{
    //Crear variable para la acción e inicializar variable el valor de nulo
    char action[40];
    //Crear variable para el nombre del archivo e inicializar variable el valor de nulo
    char printFormat[40];
    //Crear variable para el formato de lectura e inicializar variable el valor de nulo
    char readingFormat[40];
    char folderName[40];
}Requirements;
typedef struct waterTapsOrDrains{
	int x;
	int y;
	int dir;
}TapsOrDrains;


int main(int args_count, char *args[]){
    Requirements game;
    int waterTaps;
    int drains;
    int matrizRows;
    int matrizColumns;
    int rowWaterTap;
    int columnWaterTap;
    int rowDrain;
    int columnDrain;
    char dir;
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
    if(strcmp(game.readingFormat,"-it" ) == 0){
		scanf("%d%d%d%d", &matrizRows, &matrizColumns, &waterTaps, &drains);
		printf("leidos\n");
		int** matriz= crearMatriz(matrizRows, matrizColumns);
		cargarMatriz(matriz, matrizRows, matrizColumns);
		imprimirMatriz(matriz,matrizRows,matrizColumns);
		destruirMatriz(matriz, matrizRows);
	}else{
	}
	actionsMenu(game.action);
    //printGame(game.printFormat, game.folderName);

    return 0;
}
void actionsMenu(char action[]){
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
int** crearMatriz(const int matrizRows, const int matrizColumns){
    int **matriz= malloc(matrizRows*sizeof(int *));
    for(int i=0; i<matrizRows; i++){
        matriz[i]= (int*)malloc(matrizColumns*sizeof(int));
    }
    return matriz;
}
void destruirMatriz(int **matriz, const int matrizRows){
	assert(matriz);
	for(int f=0;f<matrizRows;f++){
			free(matriz[f]);
	}
	free(matriz);
}
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
void imprimirMatriz(int **matriz, const int matrizRows, const int matrizColumns){
    for(int filas_cont=0;filas_cont<matrizRows; filas_cont++){
        for(int columna_cont=0;columna_cont<matrizColumns; columna_cont++){
            printf("%d ",matriz[filas_cont][columna_cont]);        
		}
        putchar('\n');
    }
}



