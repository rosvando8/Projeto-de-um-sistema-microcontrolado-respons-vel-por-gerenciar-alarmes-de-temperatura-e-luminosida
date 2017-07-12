#ifndef LCD_H
        #define LCD_H

#include <LPC11xx.h>			/* LPC11xx Peripheral Registers */
#include "system_LPC11xx.h"

void lcdString(char * txt);
void bit_set( volatile uint32_t *p_uiValor, unsigned char v_ucBit );
void bit_clr( volatile uint32_t *p_uiValor, unsigned char v_ucBit );
void delay_ms( unsigned int v_uiTempo );
void LCD_iniciar();
void LCD_comando(char v_cComando);
void LCD_caractere( char v_cCaractere );


#endif
