/*
 *  ArduLogic - Low Speed Logic Analyzer using the Arduino Hardware
 *
 *  Copyright (C) 2011  Clifford Wolf <clifford@clifford.at>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "ardulogic.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void writevcd(const char *file)
{
	FILE *f = fopen(file, "w");

	if (f == NULL) {
		fprintf(stderr, "Can't open VCD file `%s': %s\n", file, strerror(errno));
		exit(1);
	}

	struct decoder_desc *decoder = NULL;
//	if (decode == DECODE_SPI)
//		decoder = &decoder_spi;
//	if (decode == DECODE_I2C)
//		decoder = &decoder_i2c;
	if (decode == DECODE_JTAG)
		decoder = &decoder_jtag;

	fprintf(f, "$comment Created by ArduLogic $end\n");
	fprintf(f, "$var reg 1 c clk $end\n");
	for (int i = 0; i < TOTAL_PIN_NUM; i++)
		if ((pins[i] & PIN_CAPTURE) != 0)
			fprintf(f, "$var reg 1 p%d %s $end\n", i, pin_names[i]);
	if (decoder)
		decoder->vcd_defs(f);
	fprintf(f, "$enddefinitions\n");

	if (samples.size() == 0) {
		fprintf(f, "#0 $dumpall");
		for (int i = 0; i < TOTAL_PIN_NUM; i++)
			if ((pins[i] & PIN_CAPTURE) != 0)
				fprintf(f, " xp%d", i);
		if (decoder)
			decoder->vcd_init(f);
		fprintf(f, " $end\n");
		fclose(f);
		return;
	}

	fprintf(f, "#0 $dumpall 0c");
	for (int i = 0; i < TOTAL_PIN_NUM; i++)
		if ((pins[i] & PIN_CAPTURE) != 0)
			fprintf(f, " %dp%d", (samples[0] & (1 << i)) != 0, i);
	if (decoder)
		decoder->vcd_init(f);
	fprintf(f, " $end\n");

	for (size_t i = 1; i < samples.size(); i++) {
		fprintf(f, "#%zd 1c", i*10);
		for (int j = 0; j < TOTAL_PIN_NUM; j++) {
			if ((pins[j] & PIN_CAPTURE) == 0)
				continue;
			if (((samples[i-1] ^ samples[i]) & (1 << j)) == 0)
				continue;
			fprintf(f, " %dp%d", (samples[i] & (1 << j)) != 0, j);
		}
		if (decoder)
			decoder->vcd_step(f, i);
		fprintf(f, "\n#%zd 0c\n", i*10+3);
	}
	fprintf(f, "#%zd\n", samples.size());

	fclose(f);
}

