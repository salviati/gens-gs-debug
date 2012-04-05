#include "vdp_io.h"
#include "vdp_rend.h"

// Starscream 68000 core.
#include "gens_core/cpu/68k/star_68k.h"

// More C includes
#include <stdio.h>
#include <errno.h>
#include <ctype.h>


// Duplicated from mdp_host_gens_mem.c
/* Byteswapping macros. */
#include "libgsft/gsft_byteswap.h"

#if GSFT_BYTEORDER == GSFT_LIL_ENDIAN

/* Little-endian memory macros. */
#define MEM_RW_8_BE(ptr, address)	(((uint8_t*)(ptr))[(address) ^ 1])
#define MEM_RW_8_LE(ptr, address)	(((uint8_t*)(ptr))[(address)])

#else

/* Big-endian memory macros. */
#define MEM_RW_8_BE(ptr, address)	(((uint8_t*)(ptr))[(address)])
#define MEM_RW_8_LE(ptr, address)	(((uint8_t*)(ptr))[(address) ^ 1])

#endif

/* Endian-neutral memory macros. */
#define MEM_RW_16(ptr, address)		(((uint16_t*)(ptr))[(address) >> 1])

/* TODO: Optimize 32-bit reads/writes for their respective architectures. */

#define MEM_READ_32_BE(ptr, address)	\
	(((((uint16_t*)(ptr))[(address) >> 1]) << 16) | (((uint16_t*)(ptr))[((address) >> 1) + 1]))

#define MEM_READ_32_LE(ptr, address)	\
	(((((uint16_t*)(ptr))[((address) >> 1) + 1]) << 16) | (((uint16_t*)(ptr))[(address) >> 1]))

#define MEM_WRITE_32_BE(ptr, address, data)					\
do {										\
	((uint16_t*)(ptr))[(address) >> 1]       = (((data) >> 16) & 0xFFFF);	\
	((uint16_t*)(ptr))[((address) >> 1) + 1] = ((data) & 0xFFFF);		\
} while (0)

#define MEM_WRITE_32_LE(ptr, address, data)					\
do {										\
	((uint16_t*)(ptr))[((address) >> 1) + 1] = (((data) >> 16) & 0xFFFF);	\
	((uint16_t*)(ptr))[(address) >> 1]       = ((data) & 0xFFFF);		\
} while (0)

void moncontrol(int);
void mcleanup();
void monstartup(unsigned long lowpc, unsigned long highpc);
void mcount(unsigned long frompc, unsigned long selfpc);

int monrunning;
void mcleanup_hook() {
	if (monrunning) {
		mcleanup();
		monrunning = 0;
	}
}

void VDP_Wrong(unsigned char reg, unsigned short val) {
	static FILE *fp;
	static int timer;

	int c = (int)val;
	int callee, caller;
	unsigned long lowpc, highpc;
	int control;

	int d0;
	unsigned char *ptr;
	int i;
	
	static int added_mcleanup_hook;
	
	if (!added_mcleanup_hook) {
		atexit(mcleanup_hook);
		added_mcleanup_hook = 1;
	}

	if (fp == NULL) {
		fp = fopen("log.txt", "w");
		if (fp == NULL ){
			perror("VDP_Wrong: failed to open log file.\n");
			perror(strerror(errno));
			exit(errno);
		}
	}
	
	switch (reg) {
		case 0x1c: // stuff provided by doc/prof/gens.s
			switch (val) {
				// FIXME: params are better read from stack, rather than regs. compatible with 68k calling conv and simplifies gens.s.
				case 0: // mcount
					callee = MEM_READ_32_BE(Ram_68k,(main68k_context.areg[7]) & 0xffff); //selfpc
					caller = MEM_READ_32_BE(Ram_68k,(main68k_context.areg[6]+4) & 0xffff); // frompc
					fprintf(fp, "VDP_Wrong: mcount(0x%x,0x%x)\n", caller, callee);
					mcount(caller, callee);
					
					break;
				case 1://monstartup
					lowpc = MEM_READ_32_BE(Ram_68k,(main68k_context.areg[7]+4) & 0xffff);
					highpc = MEM_READ_32_BE(Ram_68k,(main68k_context.areg[7]+8) & 0xffff);
					fprintf(fp, "VDP_Wrong: monstartup(0x%x,0x%x)\n", lowpc, highpc);
					monstartup(lowpc, highpc);
					monrunning = 1;
					break;
				case 2: //moncontrol
					control = MEM_READ_32_BE(Ram_68k,(main68k_context.areg[7]+4) & 0xffff);
					fprintf(fp, "VDP_Wrong: moncontrol(%d)\n", control);
					moncontrol(control);
					monrunning = control;
					break;
				case 3: //moncleanup
					mcleanup();
					monrunning = 0;
					break;
				case 4://gens_print
					d0 = MEM_READ_32_BE(Ram_68k,(main68k_context.areg[7]+4) & 0xffff);

					if (d0 >= 0xe00000 && d0 <= 0xffffff) {
						ptr = &Ram_68k[d0 & 0xffff];
					} else if (d0 >= 0 && d0 <= 0x3fffff) {
						ptr = &Rom_Data[d0];
					} else {
						fprintf(fp, "VDP_Wrong: illegal address: 0x%x\n", d0);
						return;
					}
					

					for(i=0; ;i++) {
						if((c = (int)MEM_RW_8_BE(ptr, i)) == 0) break;
						fputc((int)c, fp);
					}
					fflush(fp);
					break;
			}
		
		// kmod compatibility
		case 0x1e:
			if (c == 0) {
				fflush(fp);
				break;
			}
			if (isprint(c)) {
				fputc(c, fp);
			}
		break;
		
		case 0x1f:
			if (val & 0x80) {
				timer = 0;
			} else {
				fprintf(fp, "VDP_Wrong: %.10d cycles has elapsed\n", main68k_readOdometer() + timer - 66480);
			}

			if (val & 0x40) timer = 0;

			break;
	}
}
