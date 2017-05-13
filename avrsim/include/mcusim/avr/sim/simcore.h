/*
 * mcusim - Interactive simulator for microcontrollers.
 * Copyright (C) 2017 Dmitry Salychev <darkness.bsd@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef MSIM_AVR_SIMCORE_H_
#define MSIM_AVR_SIMCORE_H_ 1

#include <stdint.h>

#include "mcusim/avr/sim/sim.h"
#include "mcusim/avr/sim/bootloader.h"

#ifdef __cplusplus
extern "C" {
#endif

void MSIM_SimulateAVR(struct MSIM_AVR *mcu);

void MSIM_UpdateSREGFlag(struct MSIM_AVR *mcu,
			 enum MSIM_AVRSREGFlag flag,
			 uint8_t set_f);

uint8_t MSIM_ReadSREGFlag(struct MSIM_AVR *mcu,
			  enum MSIM_AVRSREGFlag flag);

void MSIM_StackPush(struct MSIM_AVR *mcu, uint8_t val);

uint8_t MSIM_StackPop(struct MSIM_AVR *mcu);

#ifdef __cplusplus
}
#endif

#endif /* MSIM_AVR_SIMCORE_H_ */
