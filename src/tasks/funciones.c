/*!
 * @file    funciones.c
 * @brief   Funciones para el cálculo del número de Fibonacci, conjetura de Collatz y factorización de numeros primos.
 * @author  Enzo Belmonte <ebelmonte@frba.ut.edu.ar>
 * @date    Fecha: 11/07/2025
 */
#include "tasks/funciones.h"
#include "tasks/utils.h"
#include "defines.h"
#include "utils/console_utils.h"
#include <stddef.h>

__attribute__((section(".text"))) unsigned int fibonacci(unsigned int n)
{
    unsigned int ret;
    if (n == 0)
    {
        ret = 0;
    }
    else if (n == 1)
    {
        ret = 1;
    }
    else
    {
        ret = fibonacci(n - 1) + fibonacci(n - 2);
    }
    return ret;
}

__attribute__((section(".text"))) unsigned int conjetura_collatz(unsigned int n)
{
    unsigned int ret;
    if (n % 2 == 0)
    {
        ret = div(n, 2);
    }
    else
    {
        ret = 3 * n + 1;
    }
    return ret;
}

__attribute__((section(".text"))) void factorizacion_primos(unsigned int n, unsigned int *factores)
{
    unsigned int i = 0, j = 0;
    if (factores != NULL)
    {
        for (i = 2; i <= raiz_cuadrada_int(n); i++)
        {
            while (n == (i * div(n, i)))
            {
                factores[j] = i;
                n = div(n, i);
                j++;
            }
        }
        if (n > 1)
        {
            factores[j] = n;
        }
    }
}
