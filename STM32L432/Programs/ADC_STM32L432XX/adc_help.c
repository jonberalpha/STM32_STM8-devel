uint32_t adcResult = 0;

/*
I solved this - finally. If anyone gets into the same place. 
I had set the SYSCLK as the ADC clock source, but this needs to be setup in RCC->CCIPR[29:28].
*/

/* Configure the clocks - using MSI as SYSCLK @16MHz */
RCC->CR             &=  0xFFFFFF07;     //Clear ~MSIRANGE bits and MSIRGSEL bit
RCC->CR             |=  0x00000089;     //Set MSI to 16MHz and MSIRGSEL bit
char *dataPtr = NULL;

//init ADC1
ADC1->CR     &= 0xDFFFFFFF;      //Take ADC out of deep power down - i break at this point to allow enough time - doesn't help
ADC1->CR     |= 0x10000000;    //Enable ADC1 votage regulator
RCC->AHB2ENR |= 0x00002001;    //Enable the ADC clock, and GPIOA clk
GPIOA->ASCR  |= 0x00000001;    //Connect analog switch to GPIOA[0]
GPIOA->MODER |= 0x00000003;    //Set A0 for analog input mode
ADC1->ISR    |= 0x00000001;    //Clear the ADRDY bit in the ADCx_ISR register by writing ‘1’.
ADC1->SQR1   |= 0x00000040;    //Set for a sequence of 1 conversion on CH0


while (1)
{
        ADC1->CR |= 0x00000004;       //Convst
        while(!(ADC1->ISR & 0x4));
        adcResult = ADC1->DR;
        sprintf(dataPtr, "%d", adcResult);
}
