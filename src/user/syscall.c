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
