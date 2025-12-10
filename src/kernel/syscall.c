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
 * @file    kernel/syscall.c
 * @brief   Implementación de funciones y estructuras para las llamadas al sistema
 * @author  Enzo Belmonte <ebelmonte@frba.utn.edu.ar>
 * @date    2025-07-18
 */
#include "defines.h"
#include "board/uart.h"

__attribute__((section(".text"))) int sys_my_printf(const char *buf)
{
    int i = -1; // Valor de retorno por defecto en caso de error
    _uart_t *UART0 = (_uart_t *)UART0_ADDR;
    if (buf != NULL)
    {
        i = 0;
        while (buf[i] != '\0')
        {
            uart_putc(UART0, (unsigned int)(buf[i]));
            i++;
        }
    }
    return i;
}

__attribute__((section(".text"))) uint32_t *C_SVC_handler(uint32_t svc_num, uint32_t *sp_irq)
{
    // Extraemos los argumentos de la syscall desde los registros
    uint32_t arg0; // r0

    if (sp_irq != NULL)
    {
        arg0 = sp_irq[2]; // r0
        switch (svc_num)
        {
        case SYS_WRITE:
            sp_irq[2] = sys_my_printf((const char *)arg0);
            break;
        default:
            NOP;
            break;
        }
    }

    return sp_irq;
}
