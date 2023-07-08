#define I2C_STM32F10X_MOD

/*_______________________________ INCLUDES ___________________________________*/
#include "I2C_STM32F10X.h"

/*______________________________ SUBROUTINES _________________________________*/
void i2c_init(I2C_TypeDef* registerStruct, bool remap, bool fastMode,  
     uint32_t apb1_clock)
{
	// Enable clock for Port B and alternate functions
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPBEN + RCC_APB2ENR_AFIOEN);
	
	// Enable clock for the I2C interface
	if(registerStruct == I2C1)
	{
		SET_BIT(RCC->APB1ENR, RCC_APB1ENR_I2C1EN);
	}
	else if (registerStruct == I2C2)
	{
		SET_BIT(RCC->APB1ENR, RCC_APB1ENR_I2C2EN);
	}
    
	// Disable the peripheral
	CLEAR_BIT(registerStruct->CR1, I2C_CR1_PE);

	// Configure timing
	MODIFY_REG(registerStruct->CR2, I2C_CR2_FREQ, apb1_clock/1000000);
	if(fastMode)
	{
		MODIFY_REG(registerStruct->CCR, I2C_CCR_CCR, apb1_clock/800000);
		MODIFY_REG(registerStruct->TRISE, I2C_TRISE_TRISE, apb1_clock/4000000+1);
	}
	else
	{
		MODIFY_REG(registerStruct->CCR, I2C_CCR_CCR, apb1_clock/200000);
		MODIFY_REG(registerStruct->TRISE, I2C_TRISE_TRISE, apb1_clock/1000000+1);
	}

	// Enable the peripheral
	SET_BIT(registerStruct->CR1, I2C_CR1_PE);

	// Configure the I/O pins for alternate function open-drain 2 MHz
	if(registerStruct == I2C1)
	{
		if(remap)
		{
			// PB8=SCL, PB9=SDA
			SET_BIT(AFIO->MAPR, AFIO_MAPR_I2C1_REMAP);
			MODIFY_REG(GPIOB->CRH, GPIO_CRH_CNF8 + GPIO_CRH_MODE8, GPIO_CRH_CNF8_0 + GPIO_CRH_CNF8_1 + GPIO_CRH_MODE8_1);
			MODIFY_REG(GPIOB->CRH, GPIO_CRH_CNF9 + GPIO_CRH_MODE9, GPIO_CRH_CNF9_0 + GPIO_CRH_CNF9_1 + GPIO_CRH_MODE9_1);
		}
		else
		{
			// PB6=SCL, PB7=SDA
			CLEAR_BIT(AFIO->MAPR, AFIO_MAPR_I2C1_REMAP);
			MODIFY_REG(GPIOB->CRL, GPIO_CRL_CNF6 + GPIO_CRL_MODE6, GPIO_CRL_CNF6_0 + GPIO_CRL_CNF6_1 + GPIO_CRL_MODE6_1);
			MODIFY_REG(GPIOB->CRL, GPIO_CRL_CNF7 + GPIO_CRL_MODE7, GPIO_CRL_CNF7_0 + GPIO_CRL_CNF7_1 + GPIO_CRL_MODE7_1);
		}
	}
	else if(registerStruct == I2C2)
	{
		// PB10=SCL, PB11=SDA
		MODIFY_REG(GPIOB->CRH, GPIO_CRH_CNF10 + GPIO_CRH_MODE10, GPIO_CRH_CNF10_0 + GPIO_CRH_CNF10_1 + GPIO_CRH_MODE10_1);
		MODIFY_REG(GPIOB->CRH, GPIO_CRH_CNF11 + GPIO_CRH_MODE11, GPIO_CRH_CNF11_0 + GPIO_CRH_CNF11_1 + GPIO_CRH_MODE11_1);
	}
}

int i2c_communicate(I2C_TypeDef* registerStruct, uint8_t slave_addr, 
    uint8_t* send_buffer, int send_size, uint8_t* receive_buffer, int receive_size)
{
	int receive_count = -1;

	// shift the 7bit address to the right position
	slave_addr = slave_addr << 1;

	// Send data
	if(send_size > 0)
	{
		// Send START and slave address
		SET_BIT(registerStruct->CR1, I2C_CR1_START);                    // send START condition
		while(!READ_BIT(registerStruct->SR1, I2C_SR1_SB));              // wait until START has been generated
		WRITE_REG(registerStruct->DR, slave_addr);                      // send slave address
		while(!READ_BIT(registerStruct->SR1, I2C_SR1_ADDR))             // wait until address has been sent
		{
			if (READ_BIT(registerStruct->SR1, I2C_SR1_AF))                // Check AF-Bit (ACK-Failure)
			{
				// did not receive ACK after address
				goto error;
			}
		}

		READ_REG(registerStruct->SR2);																	// clear ADDR
		while(send_size > 0)
		{
			WRITE_REG(registerStruct->DR,*((uint8_t*)send_buffer));				// send 1 byte from buffer
			while(!READ_BIT(registerStruct->SR1, I2C_SR1_TXE))						// wait until Tx register is empty
			{
				if(READ_BIT(registerStruct->SR1, I2C_SR1_AF))               // Check AF-Bit (ACK-Failure)
				{
					// did not receive ACK after data byte
					goto error;
				}
			}
			send_buffer++;
			send_size--;
		}
		while(!READ_BIT(registerStruct->SR1, I2C_SR1_BTF))              // wait until last byte transfer has finished
		{
			if(READ_BIT(registerStruct->SR1, I2C_SR1_AF))                 // Check AF-Bit (ACK-Failure)
			{
				// did not receive ACK after data byte
				goto error;
			}
		}
	}
	
	// Sending succeeded, start counting the received bytes
	receive_count = 0;

	CLEAR_BIT(registerStruct->CR1, I2C_CR1_POS);                      // POS=0
	SET_BIT(registerStruct->CR1, I2C_CR1_ACK);                        // acknowledge each byte

	// Receive data
	if(receive_size > 0)
	{
		// Send (RE-)START and slave address
		SET_BIT(registerStruct->CR1, I2C_CR1_START);                     // send START condition
		while(!READ_BIT(registerStruct->SR1, I2C_SR1_SB));               // wait until START has been generated
		WRITE_REG(registerStruct->DR, slave_addr+1);                     // send slave address + read mode
		while(!READ_BIT(registerStruct->SR1, I2C_SR1_ADDR))              // wait until address has been sent
		{
			if(READ_BIT(registerStruct->SR1, I2C_SR1_AF))                  // Check AF-Bit (ACK-Failure)
			{
				// did not receive ACK after address
				goto error;
			}
		}

		if(receive_size > 2)
		{
			READ_REG(registerStruct->SR2);                                // clear ADDR
			while(receive_size > 3)
			{
				while(!READ_BIT(registerStruct->SR1, I2C_SR1_RXNE));        // wait until a data byte has been received
				*((uint8_t*)receive_buffer) = READ_REG(registerStruct->DR); // read data
				receive_size--;
				receive_count++;
				receive_buffer++;
			}
			while(!READ_BIT(registerStruct->SR1, I2C_SR1_BTF));           // wait until 2 bytes are received
			CLEAR_BIT(registerStruct->CR1, I2C_CR1_ACK);                  // prepare to send a NACK
			*((uint8_t*)receive_buffer) = READ_REG(registerStruct->DR);   // read the penultimate data byte
			receive_size--;
			receive_count++;
			receive_buffer++;
			__disable_irq();
			{
				SET_BIT(registerStruct->CR1, I2C_CR1_STOP);                 // prepare to send a STOP condition
				*((uint8_t*)receive_buffer) = READ_REG(registerStruct->DR); // read the last data byte
				receive_size--;
				receive_count++;
				receive_buffer++;
			}
			__enable_irq();
		}
		else if (receive_size == 2)
		{
			SET_BIT(registerStruct->CR1, I2C_CR1_POS);                    // NACK shall be applied to the next 
																																			 // byte, not the current byte
			__disable_irq();
			{
				READ_REG(registerStruct->SR2);                            // clear ADDR
				CLEAR_BIT(registerStruct->CR1, I2C_CR1_ACK);              // prepare to send a NACK
			}
			__enable_irq();
			while(!READ_BIT(registerStruct->SR1, I2C_SR1_BTF));          // wait until 2 bytes are received
			__disable_irq();
			{
				SET_BIT(registerStruct->CR1, I2C_CR1_STOP);               // prepare to send a STOP condition
				*((uint8_t*)receive_buffer) = READ_REG(registerStruct->DR); // read the penultimate data byte
				receive_size--;
				receive_count++;
				receive_buffer++;
			}
			__enable_irq();
		}
		else if(receive_size == 1)
		{
			CLEAR_BIT(registerStruct->CR1, I2C_CR1_ACK);                  // prepare to send a NACK
			__disable_irq();
			{
				READ_REG(registerStruct->SR2);                            // clear ADDR
				SET_BIT(registerStruct->CR1, I2C_CR1_STOP);               // prepare to send a STOP condition
			}
			__enable_irq();
			while(!READ_BIT(registerStruct->SR1, I2C_SR1_RXNE));         // wait until a data byte has been received
		}

		*((uint8_t*)receive_buffer) = READ_REG(registerStruct->DR);         // read the last data byte
		receive_size--;
		receive_count++;
		receive_buffer++;
		while(READ_BIT(registerStruct->SR1, I2C_CR1_STOP));              // wait until STOP has been generated
	}
	else if((receive_size == 0) && (send_size > 0))
	{
		SET_BIT(registerStruct->CR1, I2C_CR1_STOP);                       // send STOP condition
		while(READ_BIT(registerStruct->CR1, I2C_CR1_STOP));              // wait until STOP has been generated
	}

	return receive_count;

	error:
	SET_BIT(registerStruct->CR1, I2C_CR1_STOP);                           // send STOP condition
	while(READ_BIT(registerStruct->CR1, I2C_CR1_STOP));                  // wait until STOP has been generated
	CLEAR_BIT(registerStruct->CR1, I2C_CR1_PE);                           // restart the I2C interface clear all error flags
	SET_BIT(registerStruct->CR1, I2C_CR1_PE);
	
	return receive_count;
}
