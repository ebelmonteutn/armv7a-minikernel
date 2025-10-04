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
 * @file    syscall.h
 * @brief   Declaración de funciones y estructuras para las llamadas al sistema
 * @author  Enzo Belmonte <ebelmonte@frba.utn.edu.ar>
 * @date    2025-07-18
 */
#ifndef SYSCALL_H_
#define SYSCALL_H_

#include "defines.h"
#include <stdint.h>

// Forward declaration para evitar dependencia circular
struct _uart_t;

typedef unsigned short umode_t;

typedef long int blksize_t;

typedef enum
{
    SYS_WRITE = 4, // Escribe datos en un descriptor de archivo
} svc_call_t;

/*!
 * @brief Funcion que escribe en pantalla.
 *
 * @param[in] buf Buffer de datos a escribir.
 * 
 * @return	  Devuelve la cantidad de bytes escritos o -1 en error.
 */
int sys_my_printf(const char *buf);

/*!
 * @brief Funcion para manejar la interrupción del temporizador 0.
 *
 * @param[in] num Número de la llamada al sistema.
 * @param[in] arg1 Primer argumento de la llamada al sistema.
 * @param[in] sp_irq Variable de entrada que recibe el stack pointer.
 * @return	  Devuelve el stack pointer original.
 */
uint32_t *C_SVC_handler(uint32_t svc_num, uint32_t *regs);

#endif /* SYSCALL_H_ */