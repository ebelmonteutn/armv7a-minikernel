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