#ifndef TECLADO_H
        #define TECLADO_H

#define BOTAO_PORT    LPC_GPIO2
//
#define INICIO   0
#define BOTAO_1  6
#define BOTAO_2  7
#define BOTAO_3  8
#define BOTAO_4  9
#define BOTAO_5  10
unsigned int checaBotao(void);

//void DebounceTeclas (void) ;

void InicializaBotoes(void);
void debounceTeclas(void);

#endif //TECLADO_H
