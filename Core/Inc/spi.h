/*
 * spi.h
 *
 *  Created on: Jun 11, 2020
 *      Author: Kostiantyn
 */

#ifndef INC_SPI_H_
#define INC_SPI_H_

#include "stdint.h"

void SPI_Init(void);
void SPI_Write(uint8_t *buff, uint32_t len);
void SPI_Read(uint8_t *buff, uint32_t len);
void SPI_ReadWrite(uint8_t *o_buff, uint8_t *i_buff, uint32_t len);
void SPI_Wait(void);

#endif /* INC_SPI_H_ */
