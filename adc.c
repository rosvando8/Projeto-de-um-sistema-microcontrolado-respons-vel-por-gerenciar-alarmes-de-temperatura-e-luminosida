
#include "adc.h"
#include <LPC11xx.h>			/* LPC11xx Peripheral Registers */
#include "system_LPC11xx.h"

void InicializaAD()
{



        // Configurar o AD para amostrar
            LPC_SYSCON->SYSAHBCLKCTRL |= (1<<13); // habilitar o clock para o bloco AD (UM10398, 3.5.14)
            LPC_SYSCON->PDRUNCFG &= ~(1<<4); // ativar o AD (UM10398,3.5.36)
            LPC_IOCON->PIO1_10 = 0x01; // configurar o pino PIO1_10 (AD6) como entrada analógica 7.4.26
            LPC_IOCON->PIO1_11 = 0x01; // configurar o pino PIO1_11 (AD7) como entrada analógica 7.4.37
            LPC_IOCON->PIO1_4 = 0x01; // configurar o pino PIO1_4 (AD5) como entrada analógica 8.4.36
            // FUNC=2, MODE=0 e ADMODE=0, diferencial 7.4.37
}

int LeLDR()
{
            unsigned int ValorLDR;
             //LDR
            LPC_ADC->CR = 0x0C20; // selecionar o canal AD5 e configurar seu clock para 4 MHz (UM10398, 25.5.1)
            // iniciar uma conversão ( bits START=01) (UM10398, 25.5.1)
            LPC_ADC->CR |= (1<<24);
            // esperar pelo fim da conversão (bit DONE em 1) (UM10398, 25.5.4)
            while( (LPC_ADC->GDR < 0x7FFFFFFF));
            ValorLDR = LPC_ADC->GDR;
            delay_ms(100);



            return ValorLDR;
}

int LePOT()
{
            unsigned int ValorPOT;
            //POTENCIOMETRO
                LPC_ADC->CR = 0x0C40; // selecionar o canal AD6 e configurar seu clock para 4 MHz (UM10398, 25.5.1)
                // iniciar uma conversão ( bits START=01) (UM10398, 25.5.1)
                LPC_ADC->CR |= (1<<24);
                // esperar pelo fim da conversão (bit DONE em 1) (UM10398, 25.5.4)
                while( (LPC_ADC->GDR < 0x7FFFFFFF));
                    ValorPOT = LPC_ADC->GDR;
                    delay_ms(100);
                    return ValorPOT;
}


