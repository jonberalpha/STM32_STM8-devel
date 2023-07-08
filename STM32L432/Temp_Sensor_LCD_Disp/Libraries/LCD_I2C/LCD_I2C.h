#ifndef LCD_I2C_H_INCLUDED
#define LCD_I2C_H_INCLUDED

/*_______________________________ INCLUDES ___________________________________*/
#include <stm32f10x.h>
#include "I2C_STM32F10X.h"
#include "DELAY_STM32F10X.h"

#ifdef  LCD_I2C_MOD
#define EXPORT
#else
#define EXPORT extern
#endif

/*___________________________ EXPORTED FUNCTIONS ____________________________*/
EXPORT void lcd_i2c_send_cmd(char cmd);
EXPORT void lcd_i2c_send_data(char data);
EXPORT void lcd_i2c_clear(void);
EXPORT void lcd_i2c_put_curs(int row, int col);
EXPORT void lcd_i2c_init(void);
EXPORT void lcd_i2c_send_string(char *str);

#undef EXPORT
#endif /* __LCD_I2C_H */
