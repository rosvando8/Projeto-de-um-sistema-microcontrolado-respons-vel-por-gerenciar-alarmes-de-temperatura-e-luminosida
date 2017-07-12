
#include "alarme.h"
#include "adc.h"
#include <LPC11xx.h>			/* LPC11xx Peripheral Registers */
#include "system_LPC11xx.h"

extern   unsigned int LDR;
extern   unsigned int LDR_alarme_alto ;
extern   unsigned int LDR_alarme_baixo;
extern   unsigned int POT;
extern   unsigned int TEMP_alarme_alto;
extern   unsigned int TEMP_alarme_baixo;
extern unsigned int ativaLDR;
extern unsigned int ativaTEMP;
extern unsigned int tempo_proc;

unsigned int checaAlarme(void)
{
    unsigned int cont;


    if(tempo_proc!=1){
    tempo_proc = tempo_proc*600;//delay inicial de 1s pois o sistema ja tem um atraso de mais ou menos 1s
     delay_ms(tempo_proc);
    }


    EnviaSerial();
        if(LDR_alarme_baixo >= LDR_alarme_alto){
            LDR_alarme_baixo = 10;
            LDR_alarme_alto= 80;

            return 18;   // tela avisaodno que alarme baixo esta maior q alto
        }
        if(TEMP_alarme_baixo >= TEMP_alarme_alto){
            TEMP_alarme_baixo = 10;
            TEMP_alarme_alto=80;
            return 18;
        }
            if ((ativaLDR ==1)&&(ativaTEMP ==1)){
                if ((LDR > LDR_alarme_alto) && (POT > TEMP_alarme_alto) ){ // ldr e pot alto

                return 14;

                }

                else if ((LDR <LDR_alarme_baixo) && (POT < TEMP_alarme_baixo )){  //ldr e  pot baixo

                    return 15;
                }

                else if ((LDR > LDR_alarme_alto) && (POT <TEMP_alarme_baixo )){ //ldr alto e pot baixo

                    return 16;

                } else if ((LDR < LDR_alarme_baixo) && (POT > TEMP_alarme_alto )){ // ldr baixo  e pot alto

                    return 17;

                }else if (POT > TEMP_alarme_alto ){

                    return 12;

                }else if(POT < TEMP_alarme_baixo ){

                    return 13;

                } else if(LDR > LDR_alarme_alto){

                    return 10;

                }else if(LDR < LDR_alarme_baixo){

                    return 11;

                }  else

                    return 9;//else continua mesmo lugar

        //fim do ldr e temp ativados
            }else if((ativaLDR ==1)&&(ativaTEMP ==0)){
                if(LDR > LDR_alarme_alto){

                        return 10;

                    }else if(LDR < LDR_alarme_baixo){

                        return 11;

                    } else
                        return 9;

                }else if((ativaLDR ==0)&&(ativaTEMP ==1)){
                    if (POT > TEMP_alarme_alto ){

                        return 12;

                    }else if(POT < TEMP_alarme_baixo ){

                        return 13;

                    } else

                        return 9;
                }else if((ativaLDR ==0)&&(ativaTEMP ==0)){
                        return 9;
                }
}


