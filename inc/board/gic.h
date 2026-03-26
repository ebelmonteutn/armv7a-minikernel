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
 * @file    gic.h
 * @brief   Declaración de funciones para el manejo del GIC (Generic Interrupt Controller)
 * @author  Enzo Belmonte <ebelmonte@frba.utn.edu.ar>
 * @date    2025-12-10
 */
#ifndef GIC_H_
#define GIC_H_

#include <stdint.h>

// Direcciones base del GIC para RealView PB-A8 (Verificar según documentación específica)
#define GIC_DIST_BASE   0x1E001000
#define GIC_CPU_BASE    0x1E000100

void __gic_init(void);
void gic_enable_interrupt(uint32_t irq_id);
void gic_disable_interrupt(uint32_t irq_id);
void gic_acknowledge_interrupt(uint32_t *irq_id);
void gic_end_interrupt(uint32_t irq_id);

#endif // GIC_H_
