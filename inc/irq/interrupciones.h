/*!
 * @file    interrupciones.h
 * @brief   Declaración de funciones de manejo de interrupciones
 * @author  Enzo Belmonte <ebelmonte@frba.utn.edu.ar>
 * @date    2025-06-17
 */
#ifndef INTERRUPCIONES_H_
#define INTERRUPCIONES_H_

/*!
 * @brief Funcion para identificar la interrupción.
 *
 * @return	  Devuelve el número de interrupción identificada.
 */
unsigned int identify_IRQ(void);

/*!
 * @brief Funcion para manejar las interrupciones.
 *
 * @param[in] sp Variable de entrada que recibe el stack pointer.
 * @return	  Devuelve el stack pointer original.
 */
uint32_t *C_IRQ_handler(uint32_t *sp_irq);

/*!
 * @brief Funcion para inicializar la placa.
 *
 * @return	  None
 */
uint32_t *TIMER0_IRQHandler(uint32_t *sp_irq);

#endif /* INTERRUPCIONES_H_ */