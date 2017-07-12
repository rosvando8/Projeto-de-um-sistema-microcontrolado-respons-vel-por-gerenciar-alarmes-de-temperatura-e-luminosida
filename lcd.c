

#include "lcd.h"
#include <LPC11xx.h>			/* LPC11xx Peripheral Registers */
#include "system_LPC11xx.h"




void lcdString(char * txt) {
    while (*txt != 0) {
        LCD_caractere(*txt);
        txt++;
    }
}

void bit_set( volatile uint32_t *p_uiValor, unsigned char v_ucBit )
{
	(*p_uiValor) |= (1 << v_ucBit);
}

void bit_clr( volatile uint32_t *p_uiValor, unsigned char v_ucBit )
{
	(*p_uiValor) &= ~(1 << v_ucBit);
}

void delay_ms( unsigned int v_uiTempo )
{
	unsigned int vuiI1;

	do
	{
			// Gerar um atraso aproximado a 1 milissegundo, clock de 48MHz
			for( vuiI1 = 0; vuiI1 < 3000; vuiI1++ );

			v_uiTempo--;

	} while( v_uiTempo > 0 );

}

void LCD_iniciar()
{
        LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);                // habilitar o clock para o módulo GPIO (UM10398, 3.5.14)
        bit_set( &LPC_GPIO0->DIR, 6 );
        bit_set( &LPC_GPIO0->DIR, 8 );
        bit_set( &LPC_GPIO0->DIR, 9 );
        bit_set( &LPC_GPIO1->DIR, 5 );
        bit_set( &LPC_GPIO0->DIR, 11 );
        bit_set( &LPC_GPIO1->DIR, 0 );
        bit_set( &LPC_GPIO1->DIR, 1 );
        bit_set( &LPC_GPIO1->DIR, 2 );
        bit_set( &LPC_GPIO0->DIR, 2 );
        bit_set( &LPC_GPIO0->DIR, 3 );
        bit_set( &LPC_GPIO2->DIR, 4 );

        bit_clr( &LPC_GPIO0->DATA, 2 );
        bit_clr( &LPC_GPIO0->DATA, 3);

       LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16);                // habilitar o clock para o módulo IOCON (UM10398, 3.5.14)
       LPC_IOCON->R_PIO1_0        = 0x81;
       LPC_IOCON->R_PIO1_1        = 0x81;
       LPC_IOCON->R_PIO1_2        = 0x81;
       LPC_IOCON->R_PIO0_11       = 0x81;

	// Esperar pela inicialização do visor
        delay_ms(1000);

        // Enviar a sequência 0b0011****
        LCD_comando(0x30);
        delay_ms(5);

        // Enviar a sequência 0b0011****
        LCD_comando(0x30);
        delay_ms(5);

        // Enviar a sequência 0b0011****
        LCD_comando(0x30);
        delay_ms(5);

        // Requisitar modo de 8 bits, 2 linhas e fonte 5x10
        LCD_comando(0x3C);
        delay_ms(20);

        // Ligar o visor, ligar o cursor e colocá-lo piscando
        LCD_comando(0x0F);
        delay_ms(20);

        // Limpar o visor
        LCD_comando(0x01);
        delay_ms(20);

        // I/D = 1, S=0
        LCD_comando(0x06);
        delay_ms(20);
}

void LCD_comando(char v_cComando)
{
// Sinalizar o envio de um comando
   bit_clr( &LPC_GPIO0->DATA, 2 );


   // Enviar o comando aos pinos de dados D0 a D7
   //LCD_PORTA_DADOS->DATA = v_cComando;

   if ( ( v_cComando & 0x01 ) == 0 )

       bit_clr( &LPC_GPIO0->DATA, 6 );

    else

       bit_set( &LPC_GPIO0->DATA, 6 );

    if ( ( v_cComando & 0x02 ) == 0 )

       bit_clr( &LPC_GPIO0->DATA, 8 );

    else

       bit_set( &LPC_GPIO0->DATA, 8 );

    if ( ( v_cComando & 0x04 ) == 0 )

       bit_clr( &LPC_GPIO0->DATA, 9 );

    else

       bit_set( &LPC_GPIO0->DATA, 9 );

    if ( ( v_cComando & 0x08 ) == 0 )

       bit_clr( &LPC_GPIO1->DATA, 5 );

    else

       bit_set( &LPC_GPIO1->DATA, 5 );

    if ( ( v_cComando & 0x10 ) == 0 )

       bit_clr( &LPC_GPIO0->DATA, 11 );

    else

       bit_set( &LPC_GPIO0->DATA, 11 );


   if ( ( v_cComando & 0x20 ) == 0 )

       bit_clr( &LPC_GPIO1->DATA, 0 );

    else

       bit_set( &LPC_GPIO1->DATA, 0 );

   if ( ( v_cComando & 0x40 ) == 0 )

       bit_clr( &LPC_GPIO1->DATA, 1 );

    else

       bit_set( &LPC_GPIO1->DATA, 1 );

   if ( ( v_cComando & 0x80 ) == 0 )

       bit_clr( &LPC_GPIO1->DATA, 2 );

    else

       bit_set( &LPC_GPIO1->DATA, 2 );

   delay_ms(1);

   // Enviar o sinal de dados válidos (pulso positivo no pino E)
   bit_set( &LPC_GPIO0->DATA, 3 );
   delay_ms(1);
   bit_clr( &LPC_GPIO0->DATA, 3 );
}


void LCD_caractere( char v_cCaractere )
{
	// Sinalizar o envio de Xum caractere
   bit_set( &LPC_GPIO0->DATA, 2 );


   // Enviar o caractere aos pinos de dados D0 a D7
   //LCD_PORTA_DADOS->DATA = v_cCaractere;

      if ( ( v_cCaractere & 0x01 ) == 0 )

       bit_clr( &LPC_GPIO0->DATA, 6 );

    else

       bit_set( &LPC_GPIO0->DATA, 6 );

    if ( ( v_cCaractere & 0x02 ) == 0 )

       bit_clr( &LPC_GPIO0->DATA, 8 );

    else

       bit_set( &LPC_GPIO0->DATA, 8 );

    if ( ( v_cCaractere & 0x04 ) == 0 )

       bit_clr( &LPC_GPIO0->DATA, 9 );

    else

       bit_set( &LPC_GPIO0->DATA, 9 );

    if ( ( v_cCaractere & 0x08 ) == 0 )

       bit_clr( &LPC_GPIO1->DATA, 5 );

    else

       bit_set( &LPC_GPIO1->DATA, 5 );

    if ( ( v_cCaractere & 0x10 ) == 0 )

       bit_clr( &LPC_GPIO0->DATA, 11 );

    else

       bit_set( &LPC_GPIO0->DATA, 11 );


   if ( ( v_cCaractere & 0x20 ) == 0 )

       bit_clr( &LPC_GPIO1->DATA, 0 );

    else

       bit_set( &LPC_GPIO1->DATA, 0 );

   if ( ( v_cCaractere & 0x40 ) == 0 )

       bit_clr( &LPC_GPIO1->DATA, 1 );

    else

       bit_set( &LPC_GPIO1->DATA, 1 );

   if ( ( v_cCaractere & 0x80 ) == 0 )

       bit_clr( &LPC_GPIO1->DATA, 2 );

    else

       bit_set( &LPC_GPIO1->DATA, 2 );
   delay_ms(1);

   // Enviar o sinal de dados válidos (pulso positivo no pino E)
   bit_set( &LPC_GPIO0->DATA, 3 );
   delay_ms(1);
   bit_clr( &LPC_GPIO0->DATA, 3 ); // era data deixei com dir
}
