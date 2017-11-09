#include<stdio.h>
int main(void){

char a, b, c;
input("Introduce una palabra de tres letras: ");
a=getchar();
b=getchar();
c=getchar();
getchar();
 ////aqui para pedir la palabra
printf("Introduce una laetra: \n");
char letra;
letra=getchar();
if(letra==a){
  printf("letra %c correcta\n",a);
}
if(letra==b){
printf("letra %c correcta\n",b);
}
if(letra==c){
printf("letra %c correcta\n",c);
}
else{
printf("nignua letra correcta\n");
}
return 0;
}
