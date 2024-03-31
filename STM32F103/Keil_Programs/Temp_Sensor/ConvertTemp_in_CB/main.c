#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define sw 0     //1=pos.|0=neg. BSP. HEX Tempwert
#if sw
#define HEX_LSB 0x37
#define HEX_MSB 0x00
#else
#define HEX_LSB 0xCE
#define HEX_MSB 0xFF
#endif

/*
9bit:
+ LSB 0x37
  MSB 0x00

- LSB 0xCE
  MSB 0xFF

12bit:
+ LSB 0x91
  MSB 0x01

- LSB 0x6F
  MSB 0xFE
*/

int main()
{
    char buffer[20];
    unsigned char temp=0x00,datareg[2]={HEX_LSB,HEX_MSB}; //pos. 0x37, neg.
    float ftemp=0;

    if(datareg[1] == 0) // Positive Temp.-Werte
    {
        ftemp = datareg[0] / 2;
    }
    else // Neg. Temp.-Werte
    {
        temp = (~datareg[0]) + 1; // 2er-Komplement
        temp = temp / 2;  //:2
        ftemp = temp; //to_dez
        ftemp = ftemp*(-1); //neg. Vorzeichen
    }

    sprintf(buffer,"Temp: %5.1f",ftemp);
    strcat(buffer,"\n");
    printf(buffer);

    return 0;
}
