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
 * @file    user/syscall.c
 * @brief   Implementación de funciones y estructuras para las llamadas al sistema
 * @author  Enzo Belmonte <ebelmonte@frba.utn.edu.ar>
 * @date    2025-07-18
 */
#include "defines.h"
#include "user/syscall.h"

__attribute__((section(".text"))) int my_printf(const char *buf)
{
    int ret = -1; // Valor de retorno por defecto en caso de error
    if (buf != NULL)
    {
        __asm__("MOV R0, %0" : : "r"(buf));
        __asm__("SVC %0" : : "i"(SYS_WRITE));
        __asm__("MOV %0, R0" : "=r"(ret));
    }
    return ret;
}
