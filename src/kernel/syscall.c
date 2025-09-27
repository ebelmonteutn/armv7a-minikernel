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
