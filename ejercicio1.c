#include<stidio.h>
int main(void){

char a, b, c;
input("Introduce una palabra de tres letras: ");
a=getchar();
b=getchar();
c=getchar();
getchar();
 ////aqui para pedir la palabra
input("Introduce una laetra: \n");
char letra;
letra=getchar();
if(letra==a){
  input("letra %c correcta\n",a);
}
if(letra==b){
input("letra %c correcta\n",b);
}
if(letra==c){
input("letra %c correcta\n",c);
}
else{
  input("nignua letra correcta\n");
}
return 0;
}
