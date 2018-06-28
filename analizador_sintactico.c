#include "analizador_sintactico.h"

/***********************************/

//raiz de la gramatica
void sintaxis()
{
	int siguiente[] = {COMA, R_CORCHETE, R_LLAVE, '\0'};
	//synchset de sincronizacion
	elemento(siguiente);
}

//puede ser un objeto o un vector
void elemento(int synchset[])
{
    int primero[] = {L_CORCHETE,L_LLAVE,'\0'};
    int siguiente[] = {COMA,R_CORCHETE,R_LLAVE, '\0'};

    check_in(primero, synchset);

    if(t.compLex == L_LLAVE)
        objeto(siguiente);
    else if(t.compLex == L_CORCHETE)
        vector(siguiente);

    check_in(siguiente, primero);
}

void vector(int synchset[])
{
	int primero[] = {L_CORCHETE, '\0'};
	int siguiente[] = {COMA, R_CORCHETE, R_LLAVE, '\0'};

	check_in(primero, synchset);

	if (t.compLex == L_CORCHETE){
		match(L_CORCHETE);
		vector_(siguiente);
	}

	check_in(siguiente, primero);

}

void vector_(int synchset[])
{
	int primero[] = {L_LLAVE, L_CORCHETE, R_CORCHETE, '\0'};

	int siguiente[] = {COMA, R_CORCHETE, R_LLAVE, L_CORCHETE, '\0'};

	check_in(primero, synchset);

	if ((t.compLex == L_LLAVE) || (t.compLex == L_CORCHETE)){
		
		lista_elemento(siguiente);
		match(R_CORCHETE);
	}
	else if(t.compLex == R_CORCHETE){
		match(R_CORCHETE);
		
	}

	check_in(siguiente, primero);
}

void lista_elemento(int synchset[])
{
	int primero[] = {L_LLAVE, L_CORCHETE, '\0'};
	int siguiente[] = {R_CORCHETE, '\0'};

	check_in(primero, synchset);

	if((t.compLex == L_LLAVE) || (t.compLex == L_CORCHETE)){
		elemento(siguiente);
		lista_elemento_(siguiente);
	}

	check_in(siguiente, primero);
}

void lista_elemento_(int synchset[])
{

	if(t.compLex == R_CORCHETE)
       return;

	int primero[] = {COMA,'\0'};
	int siguiente[] = {R_CORCHETE, '\0'};

	check_in(primero, synchset);

	

	if (t.compLex == COMA){
		match(COMA);
		elemento(siguiente);
		lista_elemento_(siguiente);
	}

	check_in(siguiente, primero);
}

void objeto(int synchset[])
{
	int primero[] = {L_LLAVE, '\0'};
	int siguiente[] = {COMA, R_CORCHETE, R_LLAVE, '\0'};
	
	check_in(primero, synchset);

	if (t.compLex == L_LLAVE) {
		match(L_LLAVE);
		objeto_(siguiente);
	}
	
	check_in(siguiente, primero);
}

void objeto_(int synchset[]){
	int primero[] = {STRING, R_LLAVE, '\0'};

	int siguiente[] = {COMA, R_CORCHETE, R_LLAVE, STRING, '\0'};

	check_in(primero, synchset);

	if (t.compLex == STRING){
		lista_atributo(synchset);
		match(R_LLAVE);
	} else if (t.compLex == R_LLAVE){
		match(R_LLAVE);
	}

	check_in(siguiente, primero);
}

void lista_atributo(int synchset[]){
	int primero[] = {STRING, '\0'};
	int siguiente[] = {R_LLAVE, '\0'};

	check_in(primero, synchset);

	if (t.compLex == STRING){
		atributo(siguiente);
		lista_atributo_(siguiente);
	}

	check_in(siguiente, primero);

}

void lista_atributo_(int synchset[]){

	if (t.compLex == R_LLAVE)
        return;

	int primero[] = {COMA, '\0'};
	int siguiente[] = {R_LLAVE, '\0'};

	check_in(primero, synchset);


	if (t.compLex == COMA){
		match(COMA);
		atributo(siguiente);
		lista_atributo_(siguiente);
	}

	check_in(siguiente, primero);
}

void atributo(int synchset[]){
	int primero[] = {STRING, '\0'};
	int siguiente[] = {COMA, R_LLAVE, '\0'};

	check_in(primero, synchset);

	if (t.compLex == STRING){
		nombre_atributo(siguiente);
		match(DOS_PUNTOS);
		valor_atributo(siguiente);
	}

	check_in(siguiente, primero);
}

void nombre_atributo(int synchset[]){
	int primero[] = {STRING, '\0'};
	int siguiente[] = {DOS_PUNTOS, '\0'};

	check_in(primero, synchset);

	if (t.compLex == STRING)
		match(STRING);

	check_in(siguiente, primero);
}

void valor_atributo(int synchset[]){
	int primero[] = {L_LLAVE, L_CORCHETE, STRING,
		NUMBER, PR_TRUE, PR_FALSE, PR_NULL, '\0'};
	int siguiente[] = {COMA, R_LLAVE, '\0'};

	check_in(primero, synchset);

	if (t.compLex == L_LLAVE){
		elemento(siguiente);
	}
	else if (t.compLex == L_CORCHETE) {
		elemento(siguiente);
	}
	else if (t.compLex == STRING){
		match(STRING);
	}
	else if (t.compLex == NUMBER){
		match(NUMBER);
	}
	else if (t.compLex == PR_TRUE){
		match(PR_TRUE);
	}
	else if (t.compLex == PR_FALSE){
		match(PR_FALSE);
	}
	else if (t.compLex == PR_NULL){
		match(PR_NULL);
	}

	check_in(siguiente, primero);

}

//si no esta en el synchset llama a lexema() e imprimer errores del checkeo de entrada
void scan(int synch[]){
    errorSintactico();
	int indice1 = 0;
    while(t.compLex != synch[indice1] && t.compLex != EOF){   
        if (synch[indice1] == '\0'){
            lexema();
            indice1=0;        
        }
        indice1++;
    }
    //el lexema que falta luego de salir
    lexema();
    return;    	   
}

//llama al siguiente elemento de la pila
void match(int expectedToken){
    if(t.compLex == expectedToken)
        lexema();
}


void check_in(int primero[], int siguiente[]){

    int indice1=0;
    if(t.compLex == EOF)
    	return;

    //entra cuando no hay errores
    while(primero[indice1] != '\0') 
    {
        if(t.compLex == primero[indice1])
            return;
        indice1++;
    }
 
 	indice1=0;    
    
    int synchset[]={1}; //acumulador
    
    while(primero[indice1] != '\0'){ 
        synchset[indice1] = primero[indice1];
        indice1++;
    }
    int indice2=0;

    while(siguiente[indice2] != '\0'){
        synchset[indice1] = siguiente[indice2];
        indice1++; //aumenta indice1
        indice2++; //aumenta indice2
    }

    scan(synchset);

}

/*muestra los errores en pantalla*/
void errorSintactico(){
	aceptado=0;
   	printf("\nError sintactico: Linea %d. Se encontro %s.\n", numLinea, t.lexema);
}



/*Main*/
int main (int argc,char* args[]){
    if(argc > 1)
    {
        if (!(archivo=fopen(args[1],"rt")))
        {
            printf("Fuente no encontrada.\n");
            exit(1);
        }
        lexema();
        sintaxis();
        if(aceptado) printf("Sintaxis correcta de la fuente.\n");
        fclose(archivo);
    }else{
        printf("Debe pasar como parametro el path al archivo fuente.\n");
        exit(1);
    }

    return 0;
}
