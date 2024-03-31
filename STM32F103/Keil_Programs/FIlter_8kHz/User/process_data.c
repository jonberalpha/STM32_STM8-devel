#include "armv20_std.h"
#include "arm_math.h"
//-----------------Speicheradressen der Audiowerte------------//
q15_t Audio_IN[2], Audio_OUT[2];

q15_t in_1, in_2;
q15_t out_1, out_2;

/*------------------------------ Function Prototypes -------------------------*/

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void process_data(void)
{
	in_1=Audio_IN[0];						//Audiowerte in eine hilfsvariable speichern fuer filter-Upro
	in_2=Audio_IN[1];		

	out_1 = in_1;
	out_2 = in_2;					        //Ausgang=Eingang, Durchverbindung
 
  Audio_OUT[0] = out_1;					//bearbeiteten Werte auf die DMA2 Adresse spielen
  Audio_OUT[1] = out_2;	
}
