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
