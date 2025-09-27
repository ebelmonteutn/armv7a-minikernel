/*!
 * @file    board_init.c
 * @brief   Implementación de funciones para la inicialización de la placa
 * @author  Enzo Belmonte <ebelmonte@frba.utn.edu.ar>
 * @date    2025-06-17
 */
#include "defines.h"

__attribute__((section(".text"))) void board_init(void)
{
    __gic_init();
    __timer_init();
    __uart_init(0);
    scheduler_init();
}

__attribute__((section(".text"))) void halt_cpu(void)
{
    while (1)
    {
        HALT_CPU;
    }
}
