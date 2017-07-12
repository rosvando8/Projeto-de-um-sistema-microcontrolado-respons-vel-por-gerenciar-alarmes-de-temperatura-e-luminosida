#ifndef SERIAL_H
        #define SERIAL_H

#include <LPC11xx.h>			/* LPC11xx Peripheral Registers */
#include "system_LPC11xx.h"
#include "adc.h"

char RecebeSerial(void);

void InicializaSerial(void);
char RecebeSerial(void);
void EnviaSerial (void);

#endif
