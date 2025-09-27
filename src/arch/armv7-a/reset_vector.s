/*
 * @file    reset_vector.s
 * @brief   Codigo de Assembler para el vector de reinicio
 * @author  Enzo Belmonte <ebelmonte@frba.utn.edu.ar>
 * @date    2025-05-13
 */
.extern _start

.global reset_vector

.code 32
.section .reset_vector
reset_vector:
    B _start
.end
