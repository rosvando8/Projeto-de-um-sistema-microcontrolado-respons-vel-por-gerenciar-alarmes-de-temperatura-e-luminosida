
#include "teclado.h"
#include <LPC11xx.h>			/* LPC11xx Peripheral Registers */
#include "system_LPC11xx.h"

#define BOTAO_PORT    LPC_GPIO2
#define INICIO 0
#define BOTAO_1  6
#define BOTAO_2  7
#define BOTAO_3  8
#define BOTAO_4  9
#define BOTAO_5  10

 char serial;

void InicializaBotoes (void)
{

     BOTAO_PORT ->DIR &= ~(1<<BOTAO_1); // entrada digital
     BOTAO_PORT ->DIR &= ~(1<<BOTAO_2);
     BOTAO_PORT ->DIR &= ~(1<<BOTAO_3);
     BOTAO_PORT ->DIR &= ~(1<<BOTAO_4);
     BOTAO_PORT ->DIR &= ~(1<<BOTAO_5);
}

InicializaSerial();


unsigned int checaBotao(void)
{
    serial= checaSerial();

     if (( (BOTAO_PORT->DATA &(1<<6 )) ==0 ) || ( serial == 'w'))// testar se esta em 0
     {
         return BOTAO_1;

      }
      else if  (( ( BOTAO_PORT->DATA  &(1<<7) ) == 0 ) || ( serial == 'a'))
      {
         return BOTAO_2;

      }
      else if  (( ( BOTAO_PORT->DATA & (1<<8)) == 0  ) || ( serial == 'z') )
      {
        return BOTAO_3;

      }
      else if  (( ( BOTAO_PORT->DATA  &(1<<9) )== 0  ) || ( serial == 'd') )
      {
          return BOTAO_4;

      }
      else if  (( ( BOTAO_PORT->DATA   &(1<<10) ) ==0 ) || ( serial == 's') )
      {

        return BOTAO_5;

      }
}

void debounceTeclas()
{
        unsigned int tecla;
        unsigned char ValAtual;
        unsigned tempo;
        unsigned char ValTemp;

         tecla=checaBotao();
         ValAtual = tecla;
         if (ValAtual == tecla){  //quando acontecer alguma mudança, aguarda até estabilizar
            ValTemp = tecla;
         }
            tempo = 22;
		 if (tempo > 0){
			if (ValTemp == tecla){	// se não mudar continua a contar
				tempo--;
			}
			else{
				ValTemp = tecla;	// se mudar, atualiza o sistema e reinicia o tempo
				tempo = 22;
			}
		}
		ValAtual = ValTemp;	//valor atualizado;
}
