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
 * @file    timer.h
 * @brief   Declaración de funciones para el manejo del Timer
 * @author  Enzo Belmonte <ebelmonte@frba.utn.edu.ar>
 * @date    2025-12-10
 */
#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

/* Direcciones base del Timer (SP804) en RealView PB-A8 */
#define TIMER0_1_BASE 0x10011000
#define TIMER2_3_BASE 0x10012000

typedef struct {
    volatile uint32_t LOAD;    // 0x00 Load Register
    volatile uint32_t VALUE;   // 0x04 Current Value Register
    volatile uint32_t CONTROL; // 0x08 Control Register
    volatile uint32_t INTCLR;  // 0x0C Interrupt Clear Register
    volatile uint32_t RIS;     // 0x10 Raw Interrupt Status Register
    volatile uint32_t MIS;     // 0x14 Masked Interrupt Status Register
    volatile uint32_t BGLOAD;  // 0x18 Background Load Register
} _timer_t;

void __timer_init(void);

#endif // TIMER_H_
