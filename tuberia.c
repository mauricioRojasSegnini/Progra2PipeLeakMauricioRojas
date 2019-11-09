#include <stdio.h>
#include <string.h>
void actionsMenu( char action[]);
typedef struct Requirements{
    //Crear variable para la acción e inicializar variable el valor de nulo
    char action[40];
    //Crear variable para el nombre del archivo e inicializar variable el valor de nulo
    char printFormat[40];
    //Crear variable para el formato de lectura e inicializar variable el valor de nulo
    char readingFormat[40];
    char folderName[40];
}Requirements;

int main(int args_count, char *args[]){
    Requirements game;
    //Crear subrutina para analizar los argumentos en la entrada
    //leerArgumentos(args_count, args, game);
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
        //if(){}
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
