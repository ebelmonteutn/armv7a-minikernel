/*
 * Copyright (c) 2025 Enzo Belmonte
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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
