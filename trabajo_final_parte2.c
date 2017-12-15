//Guillermo Munoz Crespo 71959391G
//Maria Varona Gomez 12424680B

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <math.h>
#include <string.h>
// preguntar si esta bien  declarar esto en preprocesador
#define TamBuffer2 5
#define Num_bucle 75
char **buffer1;
char **buffer2;
int TamBuffer1;
//Declaro los semaforos
sem_t hay_dato_B1;
sem_t hay_espacio_B1;
sem_t hay_dato_B2;
sem_t hay_espacio_B2;
//Declaro los semaforos mutex
pthread_mutex_t buffer_mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t buffer_mutex2 = PTHREAD_MUTEX_INITIALIZER;
//Declaro las variables como variables compartidas para los hilos consumidores
int sig_llenar_cf=0;
int sig_vaciar_c=0;
//Declaro el num hilos que va a meter como argumento
int num_hilos;
int dato_leido=0;

//Funcion que comprueba si el numero es capicua o no
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

//Hilo productor que va a generar 75 numeros
void *productor(void* arg1){
int sig_llenar_p=0;
char num1[4];
	//Inicializamos el random en funcion del tiempo
	srand(time(NULL));
	for(int i=0;i<75;i++){
		//Genera un numero aleatorio entre 0 y 999
		int num_alea=(rand() % 1000);
		//pasa de un int a un String
		sprintf(num1, "%d", num_alea);
		//Espera a recibir la seÃ±al del semaforo consumidor de que hay espacio en el buffer1
		sem_wait(&hay_espacio_B1);
		strcpy(buffer1[sig_llenar_p],num1);
		//Semaforo que envia la seÃ±al de que hay dato en el buffer1
		sem_post(&hay_dato_B1);
		sig_llenar_p=(sig_llenar_p+1)%TamBuffer1;
	}
//Finaliza el hilo productor
pthread_exit(NULL);
}


//Hilos consumidores
void *consumidor(void* arg2){
int *hilos=(int *) arg2;
char num2[4];
char num_aux[30];

while(dato_leido<Num_bucle){
	//Semafaro que espera a er si hay dato en el buffer1
	sem_wait(&hay_dato_B1);
	
	pthread_mutex_lock(&buffer_mutex1);
	strcpy(num2,buffer1[sig_vaciar_c]);
	sig_vaciar_c=(sig_vaciar_c+1)%TamBuffer1;
	dato_leido++;
	pthread_mutex_unlock(&buffer_mutex1);
	//Semafaro que espera a ver si hay dato en el buffer2
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
	
	pthread_mutex_lock(&buffer_mutex2);
	strcpy(buffer2[sig_llenar_cf],num_aux);
	sig_llenar_cf=(sig_llenar_cf+1)%TamBuffer2;
	pthread_mutex_unlock(&buffer_mutex2);
	//Semaforo que manda la seÃ±al de que hay dato en el buffer2
	sem_post(&hay_dato_B2);
	//Semaforo que manda la saÃ±al de que hay espacio en el buffer1
	sem_post(&hay_espacio_B1);
	/*printf("Hilo numero %d\n",*hilos+1);
	fflush(stdout);*/
}
//Finaliza el hilo consumidor
pthread_exit(NULL);
}



void *consumidorFinal(void* arg3){
FILE* fichero;
//Validacion de que el fichero se ha abierto correctamente
if((fichero=fopen(arg3,"w"))==NULL){
	printf("ERROR. No se ha podido abrir el fichero");
	exit(-1);
}

int sig_vaciar_cf=0;
char num3[30];
int dato_leido=0;

while(dato_leido<Num_bucle){
	dato_leido++;
	//Semaforo que espera a que le manden la seÃ±al de que hay dato en el buffer2
	sem_wait(&hay_dato_B2);
	strcpy(num3,buffer2[sig_vaciar_cf]);
	//Semaforo que manda la seÃ±al de que hay espacio en el buffer2
	sem_post(&hay_espacio_B2);
	sig_vaciar_cf=(sig_vaciar_cf+1)%TamBuffer2;
	//Va escribiendo en el fichero si el numero es capicua o no
	fputs(num3,fichero);
}

//validacion que el fichero se ha cerrado correctamente
if(fclose(fichero)!=0){
	printf("ERROR. No se ha podido cerrar el fichero");
	exit(-1);
}
//Finaliza el hilo consumidor final
pthread_exit(NULL);
}


int main(int argc, char *argv[]){
	//validacion de que el numero de argumentos es el correcto
	if(argc != 4){
		printf("ERROR. Numero de argumentos invalido.\n");
		exit(-1);//me parece que tiene que ser exit -1	
	}
	//validacion de que el tamaÃ±o del buffer1 es el correcto
	if((TamBuffer1=atoi(argv[1]))<=0){
		printf("ERROR. El tamano del buffer debe ser mayor que 0");
		exit(-1);//exit -1	
	}
	
	//reservar memoria de manera dinamica para el buffer 1
	if((buffer1=(char**)malloc(TamBuffer1*sizeof(char*)))==NULL){
		printf("ERROR. Al reservar memoria para el buffer1");
		exit(-1);
	}
	//reservo memoria de manera dinamica dentro del puntero
	for(int i =0;i<TamBuffer1;i++){
		if((buffer1[i]=(char*)malloc(4*sizeof(char)))==NULL){
		printf("ERROR. Al reservar memoria para el buffer1");
		exit(-1);
		}
	}
	//reservo memoria dinamica para el buffer 2
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
	//validacion de que el numero de hilos consumidores es mayor que 0
	if((num_hilos=atoi(argv[3]))<=0){
		printf("ERROR. Numero de hilos consumidores insuficientes");
		exit(-1);
	}
	printf("%d",num_hilos);
	fflush(stdout);
	//declaro los procesos tanto de productor,consumidor y consumidor final
	pthread_t producer, consumer[num_hilos], consumer_final;

	//Aqui inicializo los semÃ¡foros
	sem_init(&hay_dato_B1,0,0);
	sem_init(&hay_espacio_B1,0,TamBuffer1);
	sem_init(&hay_dato_B2,0,0);
	sem_init(&hay_espacio_B2,0,TamBuffer2);
	
	//Aqui creo el proceso del productor
	pthread_create(&producer,NULL,productor,(void*)NULL);
	int j;
	int hilo[num_hilos];	
	//Aqui creo todos los procesos hilos del consumidor
	for(j=0;j<num_hilos;j++){
		hilo[j]=j;
		pthread_create(&consumer[j],NULL,consumidor,(void*)&hilo[j]);
	}
	//Aqui creo el proceso del consumidor final 
	pthread_create(&consumer_final,NULL,consumidorFinal,(void*)argv[2]);
	//Aqui espero a que los procesos que he creado terminen
	pthread_join(producer,NULL);
	for(int j=0;j<num_hilos;j++){
		pthread_join(consumer[j],NULL);
		
	}		
	pthread_join(consumer_final,NULL);
	//libero la memoria dinamica que he reservado para los buffers
	free(buffer1);
	free(buffer2);
	return 0;
}
