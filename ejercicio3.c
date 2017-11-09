//  ** Programa que genera la tabla de multiplicar **
// Este c�digo NO est� completo. 
// Las partes que faltan est�n se�alizadas mediante el s�mbolo: "***"
// Completarle, compilarle y ejecutarle 
#include <stdio.h>
int main (void)
{
  int i, j;
  int k;


  for (i = 1; i < j; i++)
    {

      printf ("Tabla del %d\n", i);
      j =0 ;
      do
	{
	  printf ("%1d * %1d =", i, j);
	  k = i * j;
	  printf ("% 3d\n", k);
	  j++;
	}
      while (j<=10);
      printf ("\nPulsa <Enter> para continuar");
      getchar();
    }

  return;
}
