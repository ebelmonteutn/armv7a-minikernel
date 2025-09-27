/*!
 * @file    board_init.h
 * @brief   Declaración de funciones para la inicialización de la placa
 * @author  Enzo Belmonte <ebelmonte@frba.utn.edu.ar>
 * @date    2025-05-29
 */
#ifndef BOARD_INIT_H_
#define BOARD_INIT_H_

/*!
 * @brief Esta función se encarga de inicializar el sistema de interrupciones y el temporizador.
 *
 * @return None
 */
void board_init(void);

/*!
 * @brief Esta función se encarga de detener el CPU.
 *
 * @return None
 */
void halt_cpu(void);

#endif // BOARD_INIT_H_