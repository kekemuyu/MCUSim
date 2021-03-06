/*
 * This file is part of MCUSim, an XSPICE library with microcontrollers.
 *
 * Copyright (C) 2017-2019 MCUSim Developers, see AUTHORS.txt for contributors.
 *
 * MCUSim is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MCUSim is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/* Unit tests for AVR Instruction Set */
#include <inttypes.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "mcusim/mcusim.h"
#include "mcusim/config.h"
#include "mcusim/log.h"
#include "mcusim/avr/sim/private/macro.h"
#include "mcusim/avr/sim/private/io_macro.h"

#define TEST_PREF		"files/XlingFirmware"
#define CONF_FILE		TEST_PREF ".ft.conf"
#define GDB_RSP_PORT		12750
#define FRM_TEST		1
#define SREGR			(*mcu->sreg)

#define RESTORE_MCU() do {						\
	_ctl = _orig;							\
	_avr = _orig;							\
} while (0)

/* Structure to keep a PC value and a dump of the data memory. */
struct dm_checkpoint {
	uint32_t pc;
	char dump[1024];
};

static MSIM_AVR _orig;
static MSIM_AVR _ctl;
static MSIM_AVR _avr;
static MSIM_AVR *mcu = &_avr;
static MSIM_AVR *ctl = &_ctl;

static MSIM_CFG conf = {
	.firmware_test = FRM_TEST,
	.rsp_port = GDB_RSP_PORT,
};

/* Check points to stop and check data memory. */
static struct dm_checkpoint dm_checkpoints[] = {
	{ 0x0000, TEST_PREF ".dm_0_0000.hex" },
	{ 0x0240, TEST_PREF ".dm_1_240.hex" },
	{ 0x0241, TEST_PREF ".dm_2_241.hex" },
	{ 0x0242, TEST_PREF ".dm_3_242.hex" },
	{ 0x0243, TEST_PREF ".dm_4_243.hex" },
	{ 0x0244, TEST_PREF ".dm_5_244.hex" },
	{ 0x0245, TEST_PREF ".dm_6_245.hex" },
	{ 0x0246, TEST_PREF ".dm_7_246.hex" },
	{ 0x0247, TEST_PREF ".dm_8_247.hex" },
	{ 0x0248, TEST_PREF ".dm_9_248.hex" },
	{ 0x0249, TEST_PREF ".dm_10_249.hex" },
	{ 0x024A, TEST_PREF ".dm_11_24A.hex" },
	{ 0x024B, TEST_PREF ".dm_12_24B.hex" },
	{ 0x0251, TEST_PREF ".dm_13_251.hex" },
	{ 0x0252, TEST_PREF ".dm_14_252.hex" },
	{ 0x0253, TEST_PREF ".dm_15_253.hex" },
	{ 0x0254, TEST_PREF ".dm_16_254.hex" },
	{ 0x0259, TEST_PREF ".dm_17_259.hex" },
	{ 0x025F, TEST_PREF ".dm_18_25F.hex" },
	{ 0x0260, TEST_PREF ".dm_19_260.hex" },
	{ 0x0261, TEST_PREF ".dm_20_261.hex" },
	{ 0x0262, TEST_PREF ".dm_21_262.hex" },
	{ 0x0263, TEST_PREF ".dm_22_263.hex" },
	{ 0x0264, TEST_PREF ".dm_23_264.hex" },
	{ 0x0265, TEST_PREF ".dm_24_265.hex" },
	{ 0x0266, TEST_PREF ".dm_25_266.hex" },
	{ 0x0267, TEST_PREF ".dm_26_267.hex" },
	{ 0x0268, TEST_PREF ".dm_27_268.hex" },
	{ 0x026A, TEST_PREF ".dm_29_26A.hex" },
	{ 0x026B, TEST_PREF ".dm_30_26B.hex" },
	{ 0x026C, TEST_PREF ".dm_31_26C.hex" },
	{ 0x026D, TEST_PREF ".dm_32_26D.hex" },
	{ 0x026E, TEST_PREF ".dm_33_26E.hex" },
	{ 0x026F, TEST_PREF ".dm_34_26F.hex" },
	{ 0x0270, TEST_PREF ".dm_35_270.hex" },
	{ 0x0271, TEST_PREF ".dm_36_271.hex" },
	{ 0x0272, TEST_PREF ".dm_37_272.hex" },
	{ 0x0275, TEST_PREF ".dm_38_275.hex" },
	{ 0x0276, TEST_PREF ".dm_39_276.hex" },
	{ 0x0277, TEST_PREF ".dm_40_277.hex" },
	{ 0x0278, TEST_PREF ".dm_41_278.hex" },
	{ 0x0279, TEST_PREF ".dm_42_279.hex" },
	{ 0x027C, TEST_PREF ".dm_43_27C.hex" },
	{ 0x027D, TEST_PREF ".dm_44_27D.hex" },
	{ 0x027E, TEST_PREF ".dm_45_27E.hex" },
	{ 0x027F, TEST_PREF ".dm_46_27F.hex" },
	{ 0x0280, TEST_PREF ".dm_47_280.hex" },
	{ 0x0283, TEST_PREF ".dm_48_283.hex" },
	{ 0x0284, TEST_PREF ".dm_49_284.hex" },
	{ 0x0285, TEST_PREF ".dm_50_285.hex" },
	{ 0x0286, TEST_PREF ".dm_51_286.hex" },
	{ 0x0287, TEST_PREF ".dm_52_287.hex" },
	{ 0x0288, TEST_PREF ".dm_53_288.hex" },
	{ 0x028D, TEST_PREF ".dm_54_28D.hex" },
	{ 0x028E, TEST_PREF ".dm_55_28E.hex" },
	{ 0x028F, TEST_PREF ".dm_56_28F.hex" },
	{ 0x0290, TEST_PREF ".dm_57_290.hex" },
	{ 0x0291, TEST_PREF ".dm_58_291.hex" },
	{ 0x0576, TEST_PREF ".dm_59_576.hex" },
	{ 0x0577, TEST_PREF ".dm_60_577.hex" },
	{ 0x0578, TEST_PREF ".dm_61_578.hex" },
	{ 0x0579, TEST_PREF ".dm_62_579.hex" },
	{ 0x06C0, TEST_PREF ".dm_63_6C0.hex" },
	{ 0x06C1, TEST_PREF ".dm_64_6C1.hex" },
	{ 0x06C2, TEST_PREF ".dm_65_6C2.hex" },
	{ 0x06C3, TEST_PREF ".dm_66_6C3.hex" },
	{ 0x06C4, TEST_PREF ".dm_67_6C4.hex" },
	{ 0x06C5, TEST_PREF ".dm_68_6C5.hex" },
	{ 0x06C6, TEST_PREF ".dm_69_6C6.hex" },
	{ 0x06C7, TEST_PREF ".dm_70_6C7.hex" },
	{ 0x06C8, TEST_PREF ".dm_71_6C8.hex" },
	{ 0x06CD, TEST_PREF ".dm_72_6CD.hex" },
	{ 0x06CE, TEST_PREF ".dm_73_6CE.hex" },
	{ 0x06CF, TEST_PREF ".dm_74_6CF.hex" },
	{ 0x06D0, TEST_PREF ".dm_75_6D0.hex" },
	{ 0x06D1, TEST_PREF ".dm_76_6D1.hex" },
	{ 0x06D2, TEST_PREF ".dm_77_6D2.hex" },
	{ 0x06D3, TEST_PREF ".dm_78_6D3.hex" },
	{ 0x06D4, TEST_PREF ".dm_79_6D4.hex" },
	{ 0x06D5, TEST_PREF ".dm_80_6D5.hex" },
	{ 0x06D6, TEST_PREF ".dm_81_6D6.hex" },
	{ 0x06DB, TEST_PREF ".dm_82_6DB.hex" },
	{ 0x06DC, TEST_PREF ".dm_83_6DC.hex" },
	{ 0x06DD, TEST_PREF ".dm_84_6DD.hex" },
	{ 0x06DE, TEST_PREF ".dm_85_6DE.hex" },
	{ 0x06DF, TEST_PREF ".dm_86_6DF.hex" },
	{ 0x06E0, TEST_PREF ".dm_87_6E0.hex" },
	{ 0x06E1, TEST_PREF ".dm_88_6E1.hex" },
	{ 0x06E2, TEST_PREF ".dm_89_6E2.hex" },
	{ 0x06E3, TEST_PREF ".dm_90_6E3.hex" },
	{ 0x06E4, TEST_PREF ".dm_91_6E4.hex" },
	{ 0x06E9, TEST_PREF ".dm_92_6E9.hex" },
	{ 0x06EA, TEST_PREF ".dm_93_6EA.hex" },
	{ 0x06EB, TEST_PREF ".dm_94_6EB.hex" },
	{ 0x06EC, TEST_PREF ".dm_95_6EC.hex" },
	{ 0x06ED, TEST_PREF ".dm_96_6ED.hex" },
	{ 0x06EE, TEST_PREF ".dm_97_6EE.hex" },
	{ 0x06EF, TEST_PREF ".dm_98_6EF.hex" },
	{ 0x06F4, TEST_PREF ".dm_99_6F4.hex" },
	{ 0x06F5, TEST_PREF ".dm_100_6F5.hex" },
	{ 0x06F6, TEST_PREF ".dm_101_6F6.hex" },
	{ 0x057B, TEST_PREF ".dm_102_57B.hex" },
};
/* Index of the datamem check point */
static uint32_t dmcp = 0;

/*
 * Perform AVR simulation with occasional stops to compare actual
 * data memory with HEX dumps (obtained from the Atmel Studio 7 with
 * MemoryLogger plugin).
 */
static void
check_datamem(void **state)
{
	/* Compare the whole datamem (0x900 for m328p) */
	const size_t memsz = mcu->ramend + 1;

	/* Initialize data memory */
	if (dm_checkpoints[dmcp].pc == 0U) {
		snprintf(LOG, LOGSZ, "init datamem: step=%" PRIu32 ", pc=0x%"
		         PRIx32 ", size=0x%" PRIx64 " (%" PRIu64 ")",
		         dmcp, mcu->pc, memsz, memsz);
		MSIM_LOG_INFO(LOG);

		MSIM_AVR_LoadDataMem(mcu, dm_checkpoints[dmcp].dump);
		dmcp++;
	}

	do {
		int rc = MSIM_AVR_SimStep(mcu, FRM_TEST);

		/* Stop if there was an error during simulation step. */
		if (rc != 0) {
			MSIM_LOG_FATAL("simulation error, terminating...");
			assert_int_equal(rc, 0);
		}

		/* Skip checking if it's not a checkpoint */
		if (mcu->pc != dm_checkpoints[dmcp].pc) {
			continue;
		}

		/*
		 * Data memory can be uninitialized if we didn't have
		 * a dump file for PC == 0x0000.
		 */
		if (dmcp == 0U) {
			snprintf(LOG, LOGSZ, "init datamem: step=%" PRIu32
			         ", pc=0x%" PRIx32 ", size=0x%" PRIx64 " (%"
			         PRIu64 ")", dmcp, mcu->pc, memsz, memsz);
			MSIM_LOG_INFO(LOG);

			MSIM_AVR_LoadDataMem(mcu, dm_checkpoints[dmcp].dump);
			dmcp++;
			continue;
		}

		snprintf(LOG, LOGSZ, "comparing datamem: step=%" PRIu32
		         ", pc=0x%" PRIx32 ", size=0x%" PRIx64 " (%"
		         PRIu64 ")...", dmcp, mcu->pc, memsz, memsz);
		MSIM_LOG_INFO(LOG);

		/* Load DM from a file to the 'control' MCU */
		MSIM_AVR_LoadDataMem(ctl, dm_checkpoints[dmcp].dump);

		/*
		 * Compare data memories of the 'current' and
		 * 'control' MCUs.
		 */
		assert_memory_equal(mcu->dm, ctl->dm, memsz);
		MSIM_LOG_INFO("Done");

		dmcp++;
	} while (dmcp < ARRSZ(dm_checkpoints));
}

static int
restore_mcu(void **state)
{
	RESTORE_MCU();
	return 0;
}

int
main(void)
{
	int rc = 0;

	const struct CMUnitTest tests[] = {
		cmocka_unit_test_setup(check_datamem, restore_mcu),
	};

	MSIM_CFG_PrintVersion();

	do {
		/* Debug output from MCUSim */
		MSIM_LOG_SetLevel(MSIM_LOG_LVLDEBUG);

		/* Read config file */
		rc = MSIM_CFG_Read(&conf, CONF_FILE);
		if (rc != 0) {
			break;
		}

		/* Force firmware test option */
		conf.firmware_test = 1;

		/* Initialize AVR MCU */
		rc = MSIM_AVR_Init(mcu, &conf);
		_orig = *mcu;
		_ctl = *mcu;

		/* Don't write any registers to VCD */
		mcu->vcd.regs[0].i = -1;

		/* Force running state */
		mcu->state = AVR_RUNNING;
	} while (0);

	return rc != 0 ? rc : cmocka_run_group_tests(tests, NULL, NULL);
}
