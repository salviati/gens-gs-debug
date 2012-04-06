#include "vdp_io.h"
#include "vdp_rend.h"

#include "gens/prof/gmon.h"

#include "gens_core/mem/mem_m68k.h"

// Starscream 68000 core.
#include "gens_core/cpu/68k/star_68k.h"

// More C includes
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>

void moncontrol(int);
void mcleanup();
void monstartup(unsigned long lowpc, unsigned long highpc);
void mcount(unsigned long frompc, unsigned long selfpc);

void mcleanup_hook() {
	if (_gmonparam.state == GMON_PROF_ON) {
		mcleanup();
	}
}


static inline unsigned int _M68K_RL(unsigned int addr) {
	unsigned int val = M68K_RW(addr);
	val <<= 16;
	val |= M68K_RW(addr + 2);
	return val;
}

void VDP_Wrong(unsigned char reg, unsigned short val) {
	static FILE *fp;
	static int timer;

	int c = (int)val;
	int callee, caller;
	unsigned long lowpc, highpc;
	int control;

	int straddr;
	
	static int added_mcleanup_hook;
	
	if (!added_mcleanup_hook) {
		atexit(mcleanup_hook);
		added_mcleanup_hook = 1;
	}

	if (fp == NULL) {
		fp = fopen("log.txt", "w");
		if (fp == NULL ){
			perror("VDP_Wrong: failed to open log file.");
			perror(strerror(errno));
			exit(errno);
		}
	}
	
	switch (reg) {
		case 0x1c: // stuff provided by doc/prof/gens.s
			switch (val) {
				case 0: // mcount
					callee = _M68K_RL(main68k_context.areg[7]); //selfpc
					caller = _M68K_RL(main68k_context.areg[6]+4); // frompc
					fprintf(fp, "VDP_Wrong: mcount(0x%x,0x%x)\n", caller, callee);
					mcount(caller, callee);
					
					break;
				case 1://monstartup
					lowpc = _M68K_RL(main68k_context.areg[7]+4);
					highpc = _M68K_RL(main68k_context.areg[7]+8);
					fprintf(fp, "VDP_Wrong: monstartup(0x%x,0x%x)\n", lowpc, highpc);
					monstartup(lowpc, highpc);
					break;
				case 2: //moncontrol
					control = _M68K_RL(main68k_context.areg[7]+4);
					fprintf(fp, "VDP_Wrong: moncontrol(%d)\n", control);
					moncontrol(control);
					break;
				case 3: //moncleanup
					mcleanup();
					break;
				case 4://gens_print
					for(straddr = _M68K_RL(main68k_context.areg[7]+4); ;straddr++) {
						if((c = (int)M68K_RB(straddr)) == 0) break;
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
