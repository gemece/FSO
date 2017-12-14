//Guillermo Munoz Crespo 71959391G
//Maria Varona Gomez 12424680B

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <math.h>
#include <string.h>

#define TamBuffer2 5
#define Num_bucle 75
char **buffer1;
char **buffer2;
int TamBuffer1;
sem_t hay_dato_B1;
sem_t hay_espacio_B1;
sem_t hay_dato_B2;
sem_t hay_espacio_B2;

int capicua(char*num){
int esCapicua=1;
int i,j;
j=strlen(num)-1;
	for(i=0;i<strlen(num)/2&& esCapicua;i++,j--){
		if(*(num+i)!=*(num+j)){
			esCapicua=0;
		}
	}
	return esCapicua;
}


void *productor(void* arg1){
int sig_llenar_p=0;
char num1[4];

	srand(time(NULL));
	for(int i=0;i<75;i++){
		int num_alea=(rand() % 1000);
		sprintf(num1, "%d", num_alea);
		sem_wait(&hay_espacio_B1);
		strcpy(buffer1[sig_llenar_p],num1);
		sem_post(&hay_dato_B1);
		sig_llenar_p=(sig_llenar_p+1)%TamBuffer1;
	}
pthread_exit(NULL);
}



void *consumidor(void* arg2){
int sig_llenar_cf=0;
int sig_vaciar_c=0;
char num2[4];
char num_aux[30];
int dato_leido=0;
while(dato_leido<Num_bucle){
	dato_leido++;
	sem_wait(&hay_dato_B1);
	strcpy(num2,buffer1[sig_vaciar_c]);
	sem_wait(&hay_espacio_B2);
	if(capicua(num2)==1){
		strcpy(num_aux, "El numero ");
		strcat(num_aux, num2);
		strcat(num_aux, " es capicua\n");
	}else{
		strcpy(num_aux, "El numero ");
		strcat(num_aux, num2);
		strcat(num_aux, " no es capicua\n");
	}
	strcpy(buffer2[sig_llenar_cf],num_aux);
	sem_post(&hay_dato_B2);
	sem_post(&hay_espacio_B1);
	sig_vaciar_c=(sig_vaciar_c+1)%TamBuffer1;
	sig_llenar_cf=(sig_llenar_cf+1)%TamBuffer2;
}
pthread_exit(NULL);
}

void *consumidorFinal(void* arg3){
FILE* fichero;
if((fichero=fopen(arg3,"w"))==NULL){
	printf("ERROR. No se ha podido abrir el fichero");
	exit(-1);
}
int sig_vaciar_cf=0;
char num3[30];
int dato_leido=0;
while(dato_leido<Num_bucle){
	dato_leido++;
	sem_wait(&hay_dato_B2);
	strcpy(num3,buffer2[sig_vaciar_cf]);
	sem_post(&hay_espacio_B2);
	sig_vaciar_cf=(sig_vaciar_cf+1)%TamBuffer2;
	fputs(num3,fichero);
}
if(fclose(fichero)!=0){
	printf("ERROR. No se ha podido cerrar el fichero");
	exit(-1);
}
pthread_exit(NULL);
}


int main(int argc, char *argv[]){
	//validacion de que el nÂº de argumentos es el correcto
	if(argc != 3){
		printf("ERROR. Numero de argumentos invalido.\n");
		exit(-1);//me parece que tiene que ser exit -1	
	}
	//validacion de que el tamaÃ±o del buffer1 es el correcto
	if((TamBuffer1=atoi(argv[2]))<=0){
		printf("ERROR. El tamano del buffer debe ser mayor que 0");
		exit(-1);//exit -1	
	}
	//reservar memoria de manera dinamica
	if((buffer1=(char**)malloc(TamBuffer1*sizeof(char*)))==NULL){
		printf("ERROR. Al reservar memoria para el buffer1");
		exit(-1);
	}
	//reservo memoria dentro del vector
	for(int i =0;i<TamBuffer1;i++){
		if((buffer1[i]=(char*)malloc(4*sizeof(char)))==NULL){
		printf("ERROR. Al reservar memoria para el buffer1");
		exit(-1);
		}
	}
	if((buffer2=(char**)malloc(TamBuffer2*sizeof(char*)))==NULL){
		printf("ERROR. Al reservar memoria para el buffer1");
		exit(-1);
	}
	for(int i =0;i<TamBuffer2;i++){
		if((buffer2[i]=(char*)malloc(30*sizeof(char)))==NULL){
		printf("ERROR. Al reservar memoria para el buffer2");
		exit(-1);
		}
	}
	pthread_t producer, consumer, consumer_final;
	sem_init(&hay_dato_B1,0,0);
	sem_init(&hay_espacio_B1,0,TamBuffer1);
	sem_init(&hay_dato_B2,0,0);
	sem_init(&hay_espacio_B2,0,TamBuffer2);

	pthread_create(&producer,NULL,productor,(void*)NULL);
	pthread_create(&consumer,NULL,consumidor,(void*)NULL);
	pthread_create(&consumer_final,NULL,consumidorFinal,(void*)&argv[1]);	
	pthread_join(producer,NULL);
	pthread_join(consumer,NULL);
	pthread_join(consumer_final,NULL);
	free(buffer1);
	free(buffer2);
	return 0;
}
