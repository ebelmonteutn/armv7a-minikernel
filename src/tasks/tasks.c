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
 * @file    tasks.c
 * @brief   Implementación de las tareas
 * @author  Enzo Belmonte <ebelmonte@frba.utn.edu.ar>
 * @date    2025-07-21
 */
#include "defines.h"
#include "tasks/funciones.h"

__attribute__((section(".tcb_data"))) uint32_t global_tarea1 = 0;
__attribute__((section(".tcb_data"))) uint32_t global_tarea2 = 0;

__attribute__((section(".tareaidle_text"))) void tarea_idle(void)
{
    while (1)
    {
        HALT_CPU;
    }
}

__attribute__((section(".tarea1_text"))) void tarea1(void)
{
    uint32_t i = 0;
    while (1)
    {
        my_printf("Prueba de funciones:\n");
        my_printf("Cálculo del número de Fibonacci:\n");
        for (i = 0; i < 10; i++)
        {
            printf("Fibonacci(%u) = %u\n", i, fibonacci(i));
        }
    }
}

__attribute__((section(".tarea2_text"))) void tarea2(void)
{
    uint32_t i = 0;
    uint32_t num = 0;
    while (1)
    {
        my_printf("Conjetura de Collatz:\n");
        for (i = 1; i < 4; i++)
        {
            num = i;
            printf("Conjetura de Collatz(%u): ", i);
            while (num != 1)
            {
                printf("%u ", num);
                num = conjetura_collatz(num);
            }
            my_printf("1\n");
        }
    }
}

__attribute__((section(".tarea3_text"))) void tarea3(void)
{
    uint32_t i = 0;
    uint32_t num = 0;
    unsigned int factores[20]; // Array para almacenar factores primos
    while (1)
    {
        my_printf("Factorización de números primos:\n");
        num = 28; // Ejemplo de número a factorizar
        printf("Factores primos de %u: ", num);
        factorizacion_primos(num, factores);
        for (i = 0; factores[i] != 0; i++)
        {
            printf("%u ", factores[i]);
        }
        my_printf("\n");
        my_printf("Factorización completa.\n");
    }
}
