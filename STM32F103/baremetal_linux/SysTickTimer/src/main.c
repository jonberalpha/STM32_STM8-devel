//==============================================================================
// Includes
//==============================================================================
#include "stm32f1xx.h"
#include <stdbool.h>

//==============================================================================
// Local Variables
//==============================================================================
volatile uint32_t sysTick = 0;

//==============================================================================
// Local Function Prototypes
//==============================================================================
void initialize(void);
void delayMs(uint32_t ms);

//==============================================================================
// Initialization
//==============================================================================
void initialize(void) {

	SystemCoreClock = 8000000U;

	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN // Enable PortC
			| RCC_APB2ENR_AFIOEN; // Enable A.F.Z

	// Pin Remaps
	AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE; // JTAG is disabled

	// GPIO Settings
	GPIOC->CRH = 0x44244444; // LED connected to PC13
	GPIOC->BSRR = (1 << 13); // LED is inverted, turn it off.

	// SysTick Settings
	SysTick_Config(SystemCoreClock / 1000);

	// Initialization Done Signal
	GPIOC->BSRR = (1 << (13 + 16));
	delayMs(200);
	GPIOC->BSRR = (1 << 13);
}

//===========================================================================
// Main
//===========================================================================
int main(void) {

	initialize();

	while (true) {
		delayMs(500);
		GPIOC->BSRR = (1 << (13 + 16));
		delayMs(500);
		GPIOC->BSRR = (1 << 13);
	}
}

//===========================================================================
// Interrupt Handlers
//===========================================================================

void SysTick_Handler(void)
{
	++sysTick;
}

//===========================================================================
// Local Functions
//===========================================================================

void delayMs(uint32_t ms) {
	uint32_t start = sysTick;
	while (sysTick - start < ms);
}
