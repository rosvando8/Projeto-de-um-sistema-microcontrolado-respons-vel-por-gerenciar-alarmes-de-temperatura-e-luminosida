
#include "alarme.h"
#include "serial.h"
#include "adc.h"
#include <LPC11xx.h>			/* LPC11xx Peripheral Registers */
#include "system_LPC11xx.h"
#include "lcd.h"
/*
#define PORTA_UART LPC_GPIO3
#define PINO_UART1  0 //txd
#define PINO_UART2  1 // rxd
#define PINO_UART3  2  //serial clock
#define PINO_UART4  3  //RI
*/
extern char L[2];
extern char P[2];
extern unsigned int Valores_serial;

void InicializaSerial(void)
{


	//
	// O procedimento SystemInit (CMSIS) configura o CLOCK principal
	// do dispositivo para 48 MHz a partir de um clock externo de
	// 12 MHz (cristal) e de seu circuito PLL interno.
	//
	// Para estipular outra configuração do clock principal,
	// altere as constantes SYSOSCCTRL_Val, WDTOSCCTRL_Val,
	// SYSPLLCTRL_Val, SYSPLLCLKSEL_Val, MAINCLKSEL_Val e
	// SYSAHBCLKDIV_Val presentes no arquivo system_LPC11xx.c
	//

	//
	// Configurar os demais periféricos
	//
	// Para se obter BR de 9600 com o periférico UART para um clock principal de 48MHz, foi utilizado:
	// - UARTCLKDIV de 4;
	// - DLM de 0;
	// - DLL de 52 (decimal);
	//

    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16);   // habilitar o clock para o bloco IOCON (UM10398, 3.5.14)
	LPC_IOCON->PIO1_7         |= 0x01;      // configurar o pino UART TXD (UM10398, 7.4.41)
	LPC_IOCON->PIO1_6         |= 0x01;      // configurar o pino UART RXD (UM10398, 7.4.41)

	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12);    // habilitar o clock para o bloco UART (UM10398, 3.5.14)
	LPC_SYSCON->UARTCLKDIV    |= 0x04;       // estipular o clock do módulo UART (divisor de 4) para gerar BR de 9600 (UM10398, 3.5.16)
	LPC_UART->LCR             |= (1<<7);     // habilitar o acesso aos registros do divisor de clock da UART com DLAB=1 (UM10398, 13.5.7)
	LPC_UART->DLM              = 0x0;        // DLM=0 (UM10398, 3.5.13)
	LPC_UART->DLL              = 0x34;       // DLL=52 (UM10398, 3.5.13)
	LPC_UART->FDR              = 0x21;       // DIVADDVAL = 1, MULVAL = 2 (UM10398, 3.5.15)
	LPC_UART->FCR             |= 0x01;       // habiltiar o FIFO (necessário para operar) (UM10398, 13.5.6)
	LPC_UART->LCR             =  0x03;       // estipular (UM10398, 13.5.7):
											 //  - tamanho de palavra de 8 bits (WLS=3);
											 //  - desabilitar paridade (PE=0);
											 //  - selecionar 1 bit de parada (SBS=0);
											 //  - sem caracter de quebra (BC=0);
											 //  - desabilitar o acesso aos registros de divisor de clock com (DLAB=0).
	LPC_UART->TER             |= 0x80;       // habilitar a transmissão (UM10398, 13.5.16)


	// Configurar a porta de saída para o LED
	//LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);    // habilitar o clock para o módulo GPIO (UM10398, 3.5.14)
	//PORTA_LED->DIR  |= (1 << PINO_LED);     // configurar o pino PTO0_3 como saída digital (UM10398, 12.3.2)
	//PORTA_LED->DATA = ~(1<<PINO_LED);       // iniciar o LED desligado

	 return;
}

char RecebeSerial(void)

{
    char c;
        if ( LPC_UART->LSR & 0x01 )   // verifica se esta recebendo dados
		{

			// obter o dado recebido (UM10398, 13.5.1)
            c = LPC_UART->RBR;
              return c;
                  // obter o dado recebido (UM10398, 13.5.1)

        }else{
           return -1;
        }
}

void Envia_Serial_Texto (char msgL [16])
{

    unsigned int i;

            for( i = 0; i < 16; i++ )
            {
			// Esperar pelo fim da transmissão atual (bit TEMT, UM10398, 13.5.9)
			while ( ( LPC_UART->LSR & ( 1 << 6 ) ) == 0 );

			 // transmitir o caractere da sequência (UM10398, 25.5.2)
			LPC_UART->THR  = msgL[i];

            }

		// Gerar um atraso entre a coleta de amostras
		for( i = 0; i < 0x2FFFFF; i++ );
}

void EnviaSerial (void)
{
    unsigned int j;
            //Manda para a serial os valores lidos nas entradas AD
    char Valor_serial[19];
    //LDR
     Valor_serial[0] = 13;
     Valor_serial[1] = 10;
     Valor_serial[2] = 76;  //L
     Valor_serial[3] = 68;  //D
     Valor_serial[4] = 82;  //R
     Valor_serial[5] = 61;  //=
     Valor_serial[6] = L[0];
     Valor_serial[7] = L[1];
     Valor_serial[8] = 37;   //%

     Valor_serial[9] = 32; //espaço
     Valor_serial[10] = 32;
     //TEMP
     Valor_serial[11] = 84;   //T
     Valor_serial[12] = 69;   //E
     Valor_serial[13] = 77;   //M
     Valor_serial[14] = 80;   //P
     Valor_serial[15] = 61;   //=
     Valor_serial[16] = P[0];
     Valor_serial[17] = P[1];
     Valor_serial[18] = 32;  //espaco
     Valor_serial[19] = 67;   //C

     //Valor_serial[20] = 13;
     //Valor_serial[21] = 10;

    if (Valores_serial ==1){

            for( j = 0; j < 20; j++ )
            {
			// Esperar pelo fim da transmissão atual (bit TEMT, UM10398, 13.5.9)
			while ( ( LPC_UART->LSR & ( 1 << 6 ) ) == 0 );

			 // transmitir o caractere da sequência (UM10398, 25.5.2)
			LPC_UART->THR  = Valor_serial[j];

            }

		// Gerar um atraso entre a coleta de amostras
		for( j = 0; j < 0x2FFFFF; j++ );

    }
}





