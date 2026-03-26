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
 * @file    uart.c
 * @brief   Implementación de funciones para el manejo de la UART
 * @author  Enzo Belmonte <ebelmonte@frba.utn.edu.ar>
 * @date    2025-12-10
 */
#include "board/uart.h"
#include "defines.h"

__attribute__((section(".text"))) void __uart_init(uint32_t id)
{
    // Inicialización básica de la UART si es necesaria
}

__attribute__((section(".text"))) void uart_putc(_uart_t *uart, unsigned int c)
{
    // Esperar si la FIFO de transmisión está llena (Bit 5 de FR)
    while (uart->FR & (1 << 5));
    uart->DR = c;
}

__attribute__((section(".text"))) unsigned int uart_getc(_uart_t *uart)
{
    // Esperar si la FIFO de recepción está vacía (Bit 4 de FR)
    while (uart->FR & (1 << 4));
    return uart->DR;
}
