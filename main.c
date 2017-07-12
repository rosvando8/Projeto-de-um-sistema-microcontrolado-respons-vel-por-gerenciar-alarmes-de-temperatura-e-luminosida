
#include <LPC11xx.h>			/* LPC11xx Peripheral Registers */
#include "system_LPC11xx.h"
#include "teclado.h"
#include "lcd.h"
#include "adc.h"
#include "alarme.h"
#include "TrataSerial.h"

#define PORTA_LED   LPC_GPIO2
#define PINO_LED    4

// Protótipos locais
void bit_set( volatile uint32_t *p_iValor, unsigned char v_ucBit );
void bit_clr( volatile uint32_t *p_iValor, unsigned char v_ucBit );
void delay_ms( unsigned int v_uiTempo );
void LCD_iniciar();
void LCD_comando( char v_cComando );
void LCD_caractere( char v_cCaractere );
void InicializaBotoes ( void);
unsigned int checaBotao(void);
void InicializaAD();
int LeAD();
void debounceTeclas(void);



// Variáveis Globais
unsigned int checaAlarme(void);
void InicializaSerial(void);
char RecebeSerial(void);
char  checaSerial(void);

unsigned int LDR;
unsigned int LDR_alarme_alto =80;
unsigned int LDR_alarme_baixo=10;

unsigned int POT;
unsigned int TEMP_alarme_alto=80;
unsigned int TEMP_alarme_baixo=10;

unsigned int ativaLDR =1;
unsigned int ativaTEMP=1;
unsigned int Valores_serial=1;

unsigned int tempo_proc = 1;

char recmensagem=0;
char indmensagem=0;

char L[2];
char P[2];

int main(void)
{
    unsigned int port;   // portugues
    unsigned int ing;  // ingles

    unsigned int aux;   //auxiliar do menu
    unsigned int botao;

    char L_alarme_alto[2];
    char L_alarme_baixo[2];
    char T_alarme_alto[2];
    char T_alarme_baixo[2];
    char T_proc[2];

    aux = INICIO;

    // FUNC=2, MODE=0 e ADMODE=0, diferencial 7.4.37

    // Inicializações
    LCD_iniciar();
    InicializaBotoes();
    InicializaAD();
    InicializaSerial();

    // Laço de repetição da aplicação
    //

    while(1)
    {
        debounceTeclas();
        LDR = LeLDR();
        LDR = (LDR >> 6) & (0x3FF);
        LDR = (LDR*99)/1023 ;
        L[0] = LDR / 10;
        L[1] = LDR % 10;
        L[0] += '0'; // Digitos ASCII
        L[1] += '0';

        POT = LePOT();
        POT = (POT >> 6) & (0x3FF);
        POT = (POT *10*33)/ 1023;    //Temperatura
        P[0] = POT / 10;
        P[1] = POT % 10;
        P[0] += '0'; // dígitos ASCII
        P[1] += '0';

        L_alarme_alto[0] = LDR_alarme_alto/10;
        L_alarme_alto[1] = LDR_alarme_alto % 10;
        L_alarme_alto[0] += '0';
        L_alarme_alto[1] += '0';

        L_alarme_baixo[0] = LDR_alarme_baixo/10;
        L_alarme_baixo[1] = LDR_alarme_baixo % 10;
        L_alarme_baixo[0] += '0';
        L_alarme_baixo[1] += '0';

        T_alarme_alto[0] = TEMP_alarme_alto/10;
        T_alarme_alto[1] = TEMP_alarme_alto% 10;
        T_alarme_alto[0] += '0';
        T_alarme_alto[1] += '0';

        T_alarme_baixo[0] = TEMP_alarme_baixo/10;
        T_alarme_baixo[1] = TEMP_alarme_baixo% 10;
        T_alarme_baixo[0] += '0';
        T_alarme_baixo[1] += '0';

        T_proc[0] = tempo_proc/10;
        T_proc[1] = tempo_proc%10;
        T_proc[0] += '0';
        T_proc[1] += '0';

        botao = checaBotao();

        switch (aux)
        {
            //maquina de estados do menu
        case 0:                     //tela de inicio

            LCD_comando(0x01);
            lcdString("PORTUGUESE [<]");
            LCD_comando(0xC0);
            lcdString("ENGLISH    [>]");
            LCD_comando(0x0C);
            checaBotao();
            if (botao == BOTAO_2)
            {
                port= 1;
                aux = 1;
            }
            else if(botao == BOTAO_4)
            {
                ing=1;
                aux =1;
            }
            else
            {
                aux=0;

            }

            break;

        case 1:
            //valroes atuais alarme
            if (port==1)
            {
                // portugues
                LCD_comando(0x01);
                lcdString("LDR=");
                LCD_caractere(L[0]);
                LCD_caractere(L[1]);
                lcdString("% ");
                lcdString("TEMP=");
                LCD_caractere(P[0]);
                LCD_caractere(P[1]);
                lcdString("C");

                LCD_comando(0xC0);  //PULA LINHA

                lcdString("Config Alarme[o]");
                LCD_comando(0x0C);
                checaBotao();
                if(botao == BOTAO_5)
                {
                    aux = 2;            //configurar ldr
                }
                else
                {
                    aux = 1;
                }
            }
            else if (ing ==1) //ingles
            {
                // ingles
                LCD_comando(0x01);
                lcdString("LDR=");
                LCD_caractere(L[0]);
                LCD_caractere(L[1]);
                lcdString("% ");
                lcdString("TEMP=");
                LCD_caractere(P[0]);
                LCD_caractere(P[1]);
                lcdString("C");
                LCD_comando(0xC0);  //PULA LINHA
                lcdString("Set Alarm    [o]");
                LCD_comando(0x0C);
                checaBotao();
                if(botao == BOTAO_5)
                {
                    aux = 2;            //configurar ldr
                }
                else
                {
                    aux = 1;
                }
            }
            break;

        case 2:                         //opção de configurar ldr e temp
            if (port==1)  //portugues
            {
                LCD_comando(0x01);
                lcdString("LDR [+]   ");
                lcdString("VLT[<]");
                LCD_comando(0xC0);
                lcdString("Temp[-] ");
                lcdString("Tempo[>]");
                LCD_comando(0x0C);
                checaBotao();
                if(botao == BOTAO_1)
                {
                    aux = 3;            //configura ldr
                }
                else if(botao == BOTAO_3)
                {
                    aux = 6;            //configurar temp

                }
                else if(botao == BOTAO_2)
                {
                    aux = 1;

                }
                else if(botao == BOTAO_4)   //tempo
                {
                    aux = 19;

                }
                else
                {
                    aux =2;
                }

            }
            else if (ing ==1)
            {
                LCD_comando(0x01);
                lcdString("LDR [+]   ");
                lcdString("OUT[<]");
                LCD_comando(0xC0);
                lcdString("Temp[-]  ");
                lcdString("Time[>]");
                LCD_comando(0x0C);
                checaBotao();
                if(botao == BOTAO_1)
                {
                    aux = 3;            //configura ldr
                }
                else if(botao == BOTAO_3)
                {
                    aux = 6;            //configurar temp

                }
                else if(botao == BOTAO_2)
                {
                    aux = 1;

                }
                else if(botao == BOTAO_4)
                {
                    aux = 19;

                }
                else
                {
                    aux =2;
                }

            }
            break;

        case 3:

            if (port ==1) //portugues
            {

                //ldr alto e baixo
                LCD_comando(0x01);
                lcdString("LDR A[+]  ");
                lcdString("ENT[o]");
                LCD_comando(0xC0);
                lcdString("LDR B[-]  ");
                lcdString("VLT[<]");
                LCD_comando(0x0C);
                checaBotao();
                if(botao == BOTAO_1)
                {
                    aux = 4;            //setar ldr alto

                }
                else if(botao == BOTAO_2)
                {
                    aux = 2;            //opção de configurar ldr temp

                }
                else if(botao == BOTAO_3)
                {
                    aux = 5;               //setar ldr baixo

                }
                else if(botao == BOTAO_5)
                {
                    aux = 9;                   //mostra valores atuais
                }
                else
                {
                    aux= 3;
                }

            }                       //fica na mesma tela

            else if (ing ==1) //ingles
            {
                //ldr alto e baixo
                LCD_comando(0x01);
                lcdString("LDR H[+]  ");
                lcdString("ENT[o]");
                LCD_comando(0xC0);
                lcdString("LDR L[-]  ");
                lcdString("OUT[<]");
                LCD_comando(0x0C);
                checaBotao();
                if(botao == BOTAO_1)
                {
                    aux = 4;            //setar ldr alto

                }
                else if(botao == BOTAO_2)
                {
                    aux = 2;            //opção de configurar ldr temp

                }
                else if(botao == BOTAO_3)
                {
                    aux = 5;               //setar ldr baixo

                }
                else if(botao == BOTAO_5)
                {
                    aux = 9;                   //mostra valores atuais
                }
                else
                {
                    aux= 3;
                }
            }
            break;

        case 4:
            //ldr alto
            if (port==1)
            {
                //portugues
                LCD_comando(0x01);
                lcdString("LDR ALTO  ");
                lcdString("VLT[<]");
                LCD_comando(0xc0);
                lcdString("LDR=");
                LCD_caractere(L_alarme_alto[0]);
                LCD_caractere(L_alarme_alto[1]);
                lcdString("%");
                lcdString("   ENT[o] ");

                LCD_comando(0x0C);
                checaBotao();
                if(botao == BOTAO_1)
                {
                    LDR_alarme_alto += 1; //incrementa
                    aux = 4;
                    if(LDR_alarme_alto >99)
                    {
                        LDR_alarme_alto-=100;  //se for maior que 99, vai para 0
                    }

                }
                else if(botao ==BOTAO_3)
                {
                    LDR_alarme_alto-=1;         //decrementa
                    aux = 4;
                    if(LDR_alarme_alto ==-1)  //se for menor que zero, vai para 99
                    {
                        LDR_alarme_alto+=100;

                    }
                }
                else if(botao == BOTAO_5)
                {
                    aux = 9;                    //mostra valores atuais

                }
                else if(botao == BOTAO_2)
                {
                    aux = 3;                    //tela anterior

                }
                else
                {
                    aux = 4;
                }     //fica na mesma tela
            }
            else if (ing ==1) //ingles
            {
                LCD_comando(0x01);
                lcdString("HIGH LDR  ");
                lcdString("OUT[<]");
                LCD_comando(0xc0);
                lcdString("LDR=");
                LCD_caractere(L_alarme_alto[0]);
                LCD_caractere(L_alarme_alto[1]);
                lcdString("%");
                lcdString("   ENT[o] ");
                LCD_comando(0x0C);
                checaBotao();
                if(botao == BOTAO_1)
                {
                    LDR_alarme_alto += 1;
                    aux = 4;
                    if(LDR_alarme_alto >99)
                    {
                        LDR_alarme_alto-=100;
                    }

                }
                else if(botao ==BOTAO_3)
                {
                    LDR_alarme_alto-=1;
                    aux = 4;
                    if(LDR_alarme_alto ==-1)
                    {
                        LDR_alarme_alto+=100;

                    }
                }
                else if(botao == BOTAO_5)
                {
                    aux = 9;                    //mostra valores atuais

                }
                else if(botao == BOTAO_2)
                {
                    aux = 3;                    //tela anterior

                }
                else
                {
                    aux = 4;
                }     //fica na mesma tela

            }
            break;

        case 5:                         //setar ldr baixo
            if (port == 1)
            {
                //portugues
                LCD_comando(0x01);
                lcdString("LDR BAIXO ");
                lcdString("VLT[<]");
                LCD_comando(0xc0);
                lcdString("LDR=");
                LCD_caractere(L_alarme_baixo[0]);
                LCD_caractere(L_alarme_baixo[1]);
                lcdString("%");
                lcdString("   ENT[o] ");

                LCD_comando(0x0C);
                checaBotao();
                if(botao == BOTAO_1)
                {
                    LDR_alarme_baixo += 1;
                    aux = 5;
                    if(LDR_alarme_baixo >99)
                    {
                        LDR_alarme_baixo-=100;

                    }
                }
                else if(botao ==BOTAO_3)
                {
                    LDR_alarme_baixo-=1;
                    aux = 5;
                    if(LDR_alarme_baixo ==-1)
                    {
                        LDR_alarme_baixo+=100;

                    }
                }
                else if(botao == BOTAO_5)
                {
                    aux = 9;                    //mostra valores atuais

                }
                else if(botao == BOTAO_2)
                {
                    aux = 3;

                }
                else
                {
                    aux = 5;
                }
            }
            else if (ing ==1)    // ingles
            {
                LCD_comando(0x01);
                lcdString("LOW LDR ");
                lcdString("  OUT[<]");
                LCD_comando(0xc0);
                lcdString("LDR=");
                LCD_caractere(L_alarme_baixo[0]);
                LCD_caractere(L_alarme_baixo[1]);
                lcdString("%");
                lcdString("   ENT[o] ");
                LCD_comando(0x0C);
                checaBotao();
                if(botao == BOTAO_1)
                {
                    LDR_alarme_baixo += 1;
                    aux = 5;
                    if(LDR_alarme_baixo >99)
                    {
                        LDR_alarme_baixo-=100;

                    }
                }
                else if(botao ==BOTAO_3)
                {
                    LDR_alarme_baixo-=1;
                    aux = 5;
                    if(LDR_alarme_baixo ==-1)
                    {
                        LDR_alarme_baixo+=100;

                    }
                }
                else if(botao == BOTAO_5)
                {
                    aux = 9;                    //mostra valores atuais

                }
                else if(botao == BOTAO_2)
                {
                    aux = 3;

                }
                else
                {
                    aux = 5;
                }

            }
            break;

        case 6:
            //configura temp
            if (port==1)
            {
                //PORTUGUES
                LCD_comando(0x01);
                lcdString("TEMP A[+] ");
                lcdString("ENT[o]");
                LCD_comando(0xC0);
                lcdString("TEMP B[-] ");
                lcdString("VLT[<]");
                LCD_comando(0x0C);
                checaBotao();
                if(botao == BOTAO_1)
                {
                    aux = 7;            //setar temp alto

                }
                else if(botao == BOTAO_2)
                {
                    aux = 2;            //opção de configurar ldr temp

                }
                else if(botao == BOTAO_3)
                {
                    aux = 8;               //setar temp baixo

                }
                else if(botao == BOTAO_5)
                {
                    aux = 9;                   //mostra valores atuais
                }
                else
                {
                    aux= 6;
                }                     //fica na mesma tela
            }
            else if (ing==1 )
            {
                LCD_comando(0x01);
                lcdString("H TEMP[+] ");
                lcdString("ENT[o]");
                LCD_comando(0xC0);
                lcdString("L TEMP [-]");
                lcdString("OUT[<]");
                LCD_comando(0x0C);
                checaBotao();
                if(botao == BOTAO_1)
                {
                    aux = 7;            //setar temp alto

                }
                else if(botao == BOTAO_2)
                {
                    aux = 2;            //opção de configurar ldr temp

                }
                else if(botao == BOTAO_3)
                {
                    aux = 8;               //setar temp baixo

                }
                else if(botao == BOTAO_5)
                {
                    aux = 9;
                    //mostra valores atuais
                }
                else
                {
                    aux= 6;
                }

            }
            break;

        case 7:
            //setar temp alto
            if (port==1)
            {
                //PORTUGUES
                LCD_comando(0x01);
                lcdString("TEMP ALTA ");
                //lcdString("DIM- ");
                lcdString("VLT[<]");
                LCD_comando(0xc0);
                lcdString("TEMP=");
                LCD_caractere(T_alarme_alto[0]);
                LCD_caractere(T_alarme_alto[1]);
                lcdString("C");
                lcdString("  ENT[o]");

                LCD_comando(0x0C);
                checaBotao();
                if(botao == BOTAO_1)
                {
                    TEMP_alarme_alto += 1;
                    aux = 7;
                    if(TEMP_alarme_alto >99)
                    {
                        TEMP_alarme_alto-=100;

                    }

                }
                else if(botao ==BOTAO_3)
                {
                    TEMP_alarme_alto-=1;
                    aux = 7;
                    if(TEMP_alarme_alto ==-1)
                    {
                        TEMP_alarme_alto+=100;

                    }
                }
                else if(botao == BOTAO_5)
                {
                    aux = 9;                    //mostra valores atuais

                }
                else if(botao == BOTAO_2)
                {
                    aux = 6;                    //tela anterior

                }
                else
                {
                    aux = 7;
                }
            }                   //fica na mesma tela
            else if (ing ==1)   //INGLES
            {
                LCD_comando(0x01);
                lcdString("HIGH TEMP ");
                lcdString("OUT[<]");
                LCD_comando(0xc0);
                lcdString("TEMP=");
                LCD_caractere(T_alarme_alto[0]);
                LCD_caractere(T_alarme_alto[1]);
                lcdString("C");
                lcdString("  ENT[o]");
                LCD_comando(0x0C);
                checaBotao();
                if(botao == BOTAO_1)
                {
                    TEMP_alarme_alto += 1;
                    aux = 7;
                    if(TEMP_alarme_alto >99)
                    {
                        TEMP_alarme_alto-=100;

                    }

                }
                else if(botao ==BOTAO_3)
                {
                    TEMP_alarme_alto-=1;
                    aux = 7;
                    if(TEMP_alarme_alto ==-1)
                    {
                        TEMP_alarme_alto+=100;

                    }
                }
                else if(botao == BOTAO_5)
                {
                    aux = 9;                    //mostra valores atuais

                }
                else if(botao == BOTAO_2)
                {
                    aux = 6;                    //tela anterior

                }
                else
                {
                    aux = 7;
                }
            }
            break;

        case 8:
            if (port ==1)
            {
                //PORTUGUES              //setar TEMP baixo
                LCD_comando(0x01);
                lcdString("TEMPBAIXA");
                lcdString("VLT[<]");
                LCD_comando(0xc0);
                lcdString("TEMP=");
                LCD_caractere(T_alarme_baixo[0]);
                LCD_caractere(T_alarme_baixo[1]);
                lcdString("C");
                lcdString("  ENT[o]");

                LCD_comando(0x0C);
                checaBotao();
                if(botao == BOTAO_1)
                {
                    TEMP_alarme_baixo += 1;
                    aux = 8;
                    if(TEMP_alarme_baixo >99)
                    {
                        TEMP_alarme_baixo-=100;

                    }
                }
                else if(botao ==BOTAO_3)
                {
                    TEMP_alarme_baixo-=1;
                    aux = 8;
                    if(TEMP_alarme_baixo ==-1)
                    {
                        TEMP_alarme_baixo+=100;

                    }
                }
                else if(botao == BOTAO_5)
                {
                    aux = 9;                    //mostra valores atuais

                }
                else if(botao == BOTAO_2)
                {
                    aux = 6;

                }
                else
                {
                    aux = 8;
                }
            }
            else if (ing ==1)   //INGLES
            {
                LCD_comando(0x01);
                lcdString("LOW TEMP  ");
                lcdString("VLT[<]");
                LCD_comando(0xc0);
                lcdString("TEMP=");
                LCD_caractere(T_alarme_baixo[0]);
                LCD_caractere(T_alarme_baixo[1]);
                lcdString("C");
                lcdString("  ENT[o]");

                LCD_comando(0x0C);
                checaBotao();
                if(botao == BOTAO_1)
                {
                    TEMP_alarme_baixo += 1;
                    aux = 8;
                    if(TEMP_alarme_baixo >99)
                    {
                        TEMP_alarme_baixo-=100;

                    }
                }
                else if(botao ==BOTAO_3)
                {
                    TEMP_alarme_baixo-=1;
                    aux = 8;
                    if(TEMP_alarme_baixo ==-1)
                    {
                        TEMP_alarme_baixo+=100;

                    }
                }
                else if(botao == BOTAO_5)
                {
                    aux = 9;                    //mostra valores atuais

                }
                else if(botao == BOTAO_2)
                {
                    aux = 6;

                }
                else
                {
                    aux = 8;
                }
            }
            break;

        case 9:

            if (port ==1)
            {
                //PORTUGUES
                LCD_comando(0x01);
                lcdString("LDR=");
                LCD_caractere(L[0]);
                LCD_caractere(L[1]);
                lcdString("% ");
                lcdString("TEMP=");
                LCD_caractere(P[0]);
                LCD_caractere(P[1]);
                lcdString("C");

                LCD_comando(0xc0);
                lcdString("Altera Alarme[o]");
                LCD_comando(0x0C);
                checaBotao();
                if(botao==BOTAO_5)
                {
                    aux = 2;
                }
                else
                {
                    aux = checaAlarme();
                }

            }
            else if (ing == 1)
            {
                LCD_comando(0x01);
                lcdString("LDR=");
                LCD_caractere(L[0]);
                LCD_caractere(L[1]);
                lcdString("% ");
                lcdString("TEMP=");
                LCD_caractere(P[0]);
                LCD_caractere(P[1]);
                lcdString("C");

                LCD_comando(0xc0);
                lcdString("Change Alarm[o]");
                LCD_comando(0x0C);
                checaBotao();
                if(botao==BOTAO_5)
                {
                    aux = 2;
                }
                else
                {
                    aux = checaAlarme();
                }

            }

            break;

        case 10:
            //alrme alto ldr
            if (port==1)
            {
                //PORTUGUES
                LCD_comando(0x01);
                lcdString("   ATENCAO");
                LCD_comando(0XC0);
                lcdString("   LUZ ALTA");
                LCD_comando(0x0c);
                aux = checaAlarme();
            }
            else if (ing ==1) //INGLES
            {
                LCD_comando(0x01);
                lcdString("   ATTENTION");
                LCD_comando(0XC0);
                lcdString("   HIGH LIGHT");
                LCD_comando(0x0c);
                aux = checaAlarme();
            }
//

            break;
        case 11:            //alrme baixo ldr

            if (port==1)
            {
                //PORTUGUES
                LCD_comando(0x01);
                lcdString("   ATENCAO");
                LCD_comando(0XC0);
                lcdString("   LUZ BAIXA");
                LCD_comando(0x0c);
                aux = checaAlarme();
            }
            else if (ing ==1) //ingles
            {
                LCD_comando(0x01);
                lcdString("   ATTENTION ");
                LCD_comando(0XC0);
                lcdString("   LOW LIGHT");
                LCD_comando(0x0c);
                aux = checaAlarme();
            }
            break;

        case 12:            //alarme alto temperatura

            if (port ==1)
            {
                //PORTUGUES
                LCD_comando(0x01);
                lcdString("   ATENCAO");
                LCD_comando(0XC0);
                lcdString("  TEMP ALTA ");
                LCD_comando(0x0c);
                aux = checaAlarme();
            }
            else if (ing ==1)
            {
                LCD_comando(0x01);
                lcdString("   ATTENTION");
                LCD_comando(0XC0);
                lcdString("  HIGH TEMP ");
                LCD_comando(0x0c);
                aux = checaAlarme();
            }

            break;

        case 13:            //alrme baixo temperatura
            if (port ==1)
            {
                LCD_comando(0x01);
                lcdString("   ATENCAO");
                LCD_comando(0XC0);
                lcdString("   TEMP BAIXA ");
                LCD_comando(0x0c);
                aux = checaAlarme();
            }
            else if (ing ==1)
            {
                LCD_comando(0x01);
                lcdString("   ATTENTION");
                LCD_comando(0XC0);
                lcdString("   LOW TEMP ");
                LCD_comando(0x0c);
                aux = checaAlarme();
            }
            break;

        case 14:            //LUZ E TEMP ALTOS

            if(port==1)
            {
                //PORTUGUES
                LCD_comando(0x01);
                lcdString("   ATENCAO ");
                LCD_comando(0XC0);
                lcdString("LUZ E TEMP ALTAs");
                LCD_comando(0x0c);
                aux = checaAlarme();
            }
            else if (ing ==1)
            {
                LCD_comando(0x01);
                lcdString("   ATTENTION ");
                LCD_comando(0XC0);
                lcdString("L AND T HIGH");
                LCD_comando(0x0c);
                aux = checaAlarme();
            }

            break;

        case 15:            //LUZ E TEMP BAIXOS

            if (port ==1)
            {
                LCD_comando(0x01);
                lcdString("   ATENCAO ");
                LCD_comando(0XC0);
                lcdString("LUZ E TEMP BAIXOS");
                LCD_comando(0x0c);
                aux = checaAlarme();
            }
            else if (ing ==1)
            {
                LCD_comando(0x01);
                lcdString("   ATTENTION ");
                LCD_comando(0XC0);
                lcdString("L AND T LOW");
                LCD_comando(0x0c);
                aux = checaAlarme();
            }

            break;

        case 16:
            //alrme baixo temperatura
            if (port==1)
            {
                LCD_comando(0x01);
                lcdString("  LUZ ALTA  E ");
                LCD_comando(0XC0);
                lcdString("  TEMP  BAIXA ");
                LCD_comando(0x0c);
                aux = checaAlarme();
            }
            else if (ing ==1 )
            {
                LCD_comando(0x01);
                lcdString("HIGH LIGHT AND");
                LCD_comando(0XC0);
                lcdString("  LOW TEMP ");
                LCD_comando(0x0c);
                aux = checaAlarme();
            }

            break;

        case 17:            //alrme baixo temperatura
            if (port == 1)
            {
                LCD_comando(0x01);
                lcdString("  LUZ BAIXA ");
                LCD_comando(0XC0);
                lcdString("  TEMP ALTA");
                LCD_comando(0x0c);
                aux = checaAlarme();
            }
            else if (ing ==1)
            {
                LCD_comando(0x01);
                lcdString("LOW LIGHT AND");
                LCD_comando(0XC0);
                lcdString(" HIGH TEMP");
                LCD_comando(0x0c);
                aux = checaAlarme();
            }

            break;

        case 18:            //alrme baixo temperatura
            if ( port==1)
            {
                LCD_comando(0x01);
                lcdString("ALARME BAIXO");
                LCD_comando(0XC0);
                lcdString("MAIOR QUE O ALTO");
                LCD_comando(0x0c);
                delay_ms(3000);
                aux = 9;
            }
            else if ( ing ==1)
            {
                LCD_comando(0x01);
                lcdString("LOW ALARM HIGHER");
                LCD_comando(0XC0);
                lcdString("THAN THE HIGH ");
                LCD_comando(0x0c);
                delay_ms(3000);
                aux = 9;
            }

            break;

        case 19:
            if (port==1)
            {
                //portugues
                LCD_comando(0x01);
                lcdString("CHECAGEM  ");
                lcdString("VLT[<]");
                LCD_comando(0xc0);
                lcdString("T=");
                LCD_caractere(T_proc[0]);
                LCD_caractere(T_proc[1]);
                lcdString("s");
                lcdString("     ENT[o] ");

                LCD_comando(0x0C);
                checaBotao();
                if(botao == BOTAO_1)
                {
                    tempo_proc += 1; //incrementa
                    aux = 19;
                    if(tempo_proc >99)
                    {
                        tempo_proc-=100;  //se for maior que 99, vai para 0
                    }

                }
                else if(botao ==BOTAO_3)
                {
                    tempo_proc-=1;         //decrementa
                    aux = 19;
                    if(tempo_proc ==-1)  //se for menor que zero, vai para 99
                    {
                        tempo_proc+=100;

                    }
                }
                else if(botao == BOTAO_5)
                {
                    aux = 9;                    //mostra valores atuais

                }
                else if(botao == BOTAO_2)
                {
                    aux = 2;                    //tela anterior

                }
                else
                {
                    aux = 19;
                }     //fica na mesma tela
            }
            else if (ing ==1) //ingles
            {
                LCD_comando(0x01);
                lcdString("CHECKTIME ");
                lcdString("OUT[<]");
                LCD_comando(0xc0);
                lcdString("LDR=");
                LCD_caractere(T_proc[0]);
                LCD_caractere(T_proc[1]);
                lcdString("s");
                lcdString("   ENT[o] ");
                LCD_comando(0x0C);
                checaBotao();
                if(botao == BOTAO_1)
                {
                    tempo_proc += 1; //incrementa
                    aux = 19;
                    if(tempo_proc >99)
                    {
                        tempo_proc-=100;  //se for maior que 99, vai para 0
                    }

                }
                else if(botao ==BOTAO_3)
                {
                    tempo_proc-=1;         //decrementa
                    aux = 19;
                    if(tempo_proc ==-1)  //se for menor que zero, vai para 99
                    {
                        tempo_proc+=100;

                    }
                }
                else if(botao == BOTAO_5)
                {
                    aux = 9;                    //mostra valores atuais

                }
                else if(botao == BOTAO_2)
                {
                    aux = 2;                    //tela anterior

                }
                else
                {
                    aux = 19;
                }     //fica na mesma tela
            }
            break;

        }//switch

    }
    return 0;
}
