#include <stdio.h>
int main(void){
float a,b;
printf("Introduce los dos numeros: ");
scanf("%f %f\n", &a,&b);
int cont=0;
do{
if(a>b){
   a=a/10;
   if(a<b){
   a=a*10
   cont=1;
   }
   if(1>a>0){
   a=a*10;
   cont=1;
   }
  }while(cont=0);
   printf(" %.2f\n %.2f\n");
  return 0;
  }
