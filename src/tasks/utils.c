/*!
 * @file    utils.c
 * @brief   Funciones utilitarias para cálculos matemáticos.
 * @author  Enzo Belmonte <ebelmonte@frba.ut.edu.ar>
 * @date    Fecha: 11/07/2025
 */
#include <stdint.h>
#include "tasks/utils.h"
#include "defines.h"
#include "utils/console_utils.h"

__attribute__((section(".text"))) int raiz_cuadrada_int(unsigned int x)
{
    unsigned int b, h, last;
    int ret = -1; // Valor de retorno por defecto en caso de error
    if (x == 0 || x == 1)
    {
        ret = x; 
    }
    else if (x > 1)
    {
        b = x;
        h = 0;
        last = 0;
        while (b != last)
        {
            last = b;
            h = div(x, b);          
            b = div(b + h, 2);      
        }
        ret = b;
    }
    return ret;
}
