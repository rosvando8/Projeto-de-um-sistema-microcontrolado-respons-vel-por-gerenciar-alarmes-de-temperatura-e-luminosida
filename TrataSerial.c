
#include "serial.h"
#include "adc.h"
#include "TrataSerial.h"
#include "teclado.h"
#include <LPC11xx.h>			/* LPC11xx Peripheral Registers */
#include "system_LPC11xx.h"
    unsigned char valor ;
    extern char recmensagem;
    extern char indmensagem;
    char mensagem [6];


    extern   unsigned int LDR_alarme_alto ;
    extern   unsigned int LDR_alarme_baixo;
    extern unsigned int LDR;
    extern unsigned int POT;

    extern   unsigned int TEMP_alarme_alto;
    extern   unsigned int TEMP_alarme_baixo;
    extern   unsigned int tempo_proc;

    extern unsigned int ativaLDR ;
    extern unsigned int ativaTEMP ;
    extern unsigned int Valores_serial;


char  checaSerial(void)
{
    int i;
    unsigned char c1;
    unsigned char c2;
    char serial;

    serial = RecebeSerial();

    if ( serial == -1 ){ // se não recebeu  não faz nada
            return;
    }else {
            if ( recmensagem == 0) {
                     if ( serial == '<' ){
                        mensagem[0] = serial;
                        indmensagem = 1;
                        recmensagem =1 ;
                    }
                    else {
                    return serial ;

                         }
            } else {
                    mensagem[indmensagem]=serial;
                    indmensagem = indmensagem + 1;

                    } if (indmensagem >= 6 ){
                            recmensagem =0;
                            indmensagem =0;
                            ProcessarMens();  // função que processa a mensagem ;
                    }

            return;
    }
}

void ProcessarMens(void)
{

    unsigned char c1;
    unsigned char c2;

        if ((mensagem[5]) !='>'){
                return;
        }

       // teste do codigo ascii

 // testar se esta ente 0 a 9 e a e f
            if ((( mensagem[4]<'0')|| (mensagem[4]>'9') )&&   ((mensagem[4]<'A')||( mensagem[4]>'F')))
            {
                return; // igora msg
            }   else if ((( mensagem[3]<'0')|| (mensagem[3]>'9') )&&   ((mensagem[3]<'A')||( mensagem[3]>'F')))
                {
                    return ; // ignora mensagem

        //codigo valido

                } else {  // codigo válido
                        if ( ( mensagem[3] >= 'A') &&  (mensagem[3] <= 'F')){

                            mensagem [3]='0'+('A'+10- mensagem [3]); // CONVERTET MSG 3
                        }

                        if ( ( mensagem[4] >= 'A')&&  (mensagem[4] <= 'F')){

                            mensagem [4]='0'+('A'+10- mensagem [4]); //CONVERTER MSG 4
                        }

                            valor  = (mensagem[3] - '0') << 4;
                            valor += (mensagem [4] - '0');
         //TESTE DO COMANDO NAO PRECISA CONVERTER
                            c1 = mensagem [1];
                            c2 = mensagem [2];
                                if ((c1== '0') && (c2=='1')){
                                    tempo_proc=valor;
                                }
                                else if ((c1== '1') && (c2=='0')){
                                        if ( valor == 0){
                                                ativaTEMP = 1;  //liga temp
                                                Envia_Serial_Texto(" TEMP ATIVO ");

                                        }else if(valor == 1){
                                                ativaTEMP = 0;
                                                Envia_Serial_Texto(" TEMP INATIVO ");
                                                    // LPC_IOCON->PIO1_10 = 0x00;  //desliga temp
                                        }

                                }else if ((c1 == '1') && (c2 == '1')){  //inferior temp
                                        TEMP_alarme_baixo = valor;

                                }else if ((c1 == '1') && (c2 == '2')){  // superior temp
                                        TEMP_alarme_alto = valor;
                                            if(valor == 100){
                                                TEMP_alarme_alto = 99;
                                            }
                                }else if ((c1 == '2') && (c2 == '0')){  //liga desliga ldr
                                          if ( valor == 0){
                                                ativaLDR = 1; // configurar o pino PIO1_4 (AD5) como entrada analógica 8.4.36
                                                Envia_Serial_Texto(" LDR ATIVO ");
                                          }else if(valor = 1){
                                                    ativaLDR = 0;
                                                    Envia_Serial_Texto(" LDR INATIVO ");

                                        }
                                  }else if ((c1 == '2') && (c2 == '1')){  //inferior ldr
                                      LDR_alarme_baixo = valor;

                                    }else if ((c1 == '2') && (c2 == '2')){  //superior ldr
                                            LDR_alarme_alto = valor;
                                                if(valor == 100){
                                                    LDR_alarme_alto = 99;
                                                }
                                    }else if ((c1 == '3')&&(c2 == '0')){
                                        if(valor ==0){
                                            Valores_serial = 1;
                                            }else if(valor ==1){
                                                Valores_serial = 0;
                                                delay_ms(100);
                                                Envia_Serial_Texto(" FIM VALORES ");
                                            }

                                    }

    }//else código valido
 }//char procesmensagem

