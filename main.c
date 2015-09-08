/*
 * FreeModbus Libary: AVR Demo Application
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * Modfications Copyright (C) 2006 Michał.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: demo.c,v 1.7 2006/06/15 15:38:02 wolti Exp $
 */

/* ----------------------- AVR includes -------------------------------------*/
#include <avr/io.h>
#include <avr/interrupt.h>

/* ----------------------- Modbus includes ----------------------------------*/
#include "modbus/include/mb.h"
#include "mbport.h"

/* ----------------------- Defines ------------------------------------------*/
#define REG_INPUT_START 1000
#define REG_INPUT_NREGS 4

/* ----------------------- Static variables ---------------------------------*/
static USHORT usRegInputStart = REG_INPUT_START;
static USHORT usRegInputBuf[REG_INPUT_NREGS];

/* ----------------------- Start implementation -----------------------------*/
int main(void) {
	const UCHAR ucSlaveID[] = { 0xAA, 0xBB, 0xCC };
	eMBErrorCode eStatus;

	eStatus = eMBInit(MB_ASCII, 0x0A, 0, 38400, MB_PAR_NONE);

	eStatus = eMBSetSlaveID(0x34, TRUE, ucSlaveID, 3);
	sei( );

	/* Enable the Modbus Protocol Stack. */
	eStatus = eMBEnable();

	for (;;) {
		(void) eMBPoll();

		/* Here we simply count the number of poll cycles. */
		usRegInputBuf[0]++;
	}
}

eMBErrorCode eMBRegInputCB(UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs) {
	eMBErrorCode eStatus = MB_ENOERR;
	int iRegIndex;

	if ((usAddress >= REG_INPUT_START) && (usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS)) {
		iRegIndex = (int) (usAddress - usRegInputStart);
		while (usNRegs > 0) {
			*pucRegBuffer++ = (unsigned char) (usAddress >> 8);
			*pucRegBuffer++ = (unsigned char) (usAddress & 0xFF);
//				*pucRegBuffer++ = usAddress;
//            *pucRegBuffer++ =
//                ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
//            *pucRegBuffer++ =
//                ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );

			usNRegs--;
		}
	} else {
		eStatus = MB_ENOREG;
	}

	return eStatus;
}

uint16_t holding[100];

eMBErrorCode eMBRegHoldingCB(UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode) {
	uint16_t i;

	if (usAddress < 1000 || usAddress + (usNRegs - 1) > 1099) return MB_ENOREG;
	usAddress -= 1000;

	for (i = 0; i < usNRegs; i++) {

		if (eMode == MB_REG_READ) {

			pucRegBuffer[i * 2 + 0] = (unsigned char) (holding[i + usAddress] >> 8);
			pucRegBuffer[i * 2 + 1] = (unsigned char) (holding[i + usAddress] & 0xFF);

		} else if (eMode == MB_REG_WRITE) {

			holding[i + usAddress] = (pucRegBuffer[i * 2 + 0] << 8) | pucRegBuffer[i * 2 + 1];

		}

	}

	return MB_ENOERR;
}

eMBErrorCode eMBRegCoilsCB(UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode) {
	return MB_ENOREG;
}

eMBErrorCode eMBRegDiscreteCB(UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete) {
	return MB_ENOREG;
}
