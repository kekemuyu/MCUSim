/*
 * Copyright (c) 2017, 2018, The MCUSim Contributors
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the MCUSim or its parts nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef MSIM_AVR_M328P_H_
#define MSIM_AVR_M328P_H_ 1

/* Include headers specific to the ATMega328P */
#define _SFR_ASM_COMPAT 1
#define __AVR_ATmega328P__ 1

#include <stdio.h>
#include <stdint.h>

#include "mcusim/avr/io.h"
#include "mcusim/avr/sim/m328/m328_ioregs.h"
#include "mcusim/avr/sim/io_regs.h"
#include "mcusim/avr/sim/sim.h"
#include "mcusim/avr/sim/simcore.h"

int MSIM_M328PSetFuse(struct MSIM_AVR *mcu, struct MSIM_AVRConf *cnf);
int MSIM_M328PSetLock(struct MSIM_AVR *mcu, struct MSIM_AVRConf *cnf);
int MSIM_M328PUpdate(struct MSIM_AVR *mcu, struct MSIM_AVRConf *cnf);

/* ATMega328P Fuse Low Byte */
enum MSIM_AVRFuseLowByte {
	AVR_FLB_CKSEL0,
	AVR_FLB_CKSEL1,
	AVR_FLB_CKSEL2,
	AVR_FLB_CKSEL3,
	AVR_FLB_SUT0,
	AVR_FLB_SUT1,
	AVR_FLB_CKOUT,
	AVR_FLB_CKDIV8
};
enum MSIM_AVRFuseHighByte {
	AVR_FHB_BOOTRST,
	AVR_FHB_BOOTSZ0,
	AVR_FHB_BOOTSZ1,
	AVR_FHB_EESAVE,
	AVR_FHB_WDTON,
	AVR_FHB_SPIEN,
	AVR_FHB_DWEN,
	AVR_FHB_RSTDISBL
};
enum MSIM_AVRFuseExtended {
	AVR_FEXT_BODLEVEL0,
	AVR_FEXT_BODLEVEL1,
	AVR_FEXT_BODLEVEL2,
};

const static struct MSIM_AVR ORIG_M328P = {
	.name = "ATmega328P",
	.signature = { SIGNATURE_0, SIGNATURE_1, SIGNATURE_2 },
	.pc = 0x0000,
	.pc_bits = 14,
	.freq = 1000000,		/* Clock frequency, in Hz */
	.clk_source = AVR_INT_CAL_RC_CLK,
	.lockbits = 0x3F,
	.regs_num = 32,			/* # of general purpose registers */
	.ioregs_num = 224,		/* # of I/O registers */
	.xmega = 0,			/* XMEGA AVR flag */
	.reduced_core = 0,		/* Reduced AVR core flag */
	.spm_pagesize = SPM_PAGESIZE,
	.flashstart = 0x0000,
	.flashend = FLASHEND,
	.ramstart = RAMSTART,
	.ramend = RAMEND,
	.ramsize = 2048,
	.e2start = 0x0000,
	.e2end = E2END,
	.e2size = 1024,
	.e2pagesize = E2PAGESIZE,
	.sfr_off = __SFR_OFFSET,
	.set_fusef = MSIM_M328PSetFuse,
	.set_lockf = MSIM_M328PSetLock,
	.tick_perf = MSIM_M328PUpdate,
	.fuse = { LFUSE_DEFAULT, HFUSE_DEFAULT, 0xFF },
	.bls = {			/* Bootloader config */
		.start = 0x7000,
		.end = 0x7FFF,
		.size = 4096,
	},
	.intr = {
		.reset_pc = 0x0000,
		.ivt = 0x0002,
	},
};

#endif /* MSIM_AVR_M328P_H_ */
