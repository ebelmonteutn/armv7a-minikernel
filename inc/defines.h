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
 * @file    defines.h
 * @brief   Declaración de definiciones comunes para el proyecto.
 * @author  Enzo Belmonte <ebelmonte@frba.utn.edu.ar>
 * @date    2025-07-17
 */
#ifndef DEFINES_H_
#define DEFINES_H_

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include "board/gic.h"
#include "board/timer.h"
#include "board/uart.h"
#include "utils/console_utils.h"
#include "utils/low_level_cpu_access.h"
#include "irq/interrupciones.h"
#include "bsp/board_init.h"
#include "kernel/scheduler.h"
#include "kernel/syscall.h"
#include "user/syscall.h"
#include "tasks/tasks.h"

#define HALT_CPU __asm__("WFI")
#define NOP __asm__("NOP")

#endif // DEFINES_H_