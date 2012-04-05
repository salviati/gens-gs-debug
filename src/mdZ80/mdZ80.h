/**********************************************************/
/*                                                        */
/* Z80 emulator 0.99                                      */
/* Copyright 2002 Stéphane Dallongeville                  */
/* Used for the genesis emulation in Gens                 */
/*                                                        */
/**********************************************************/

#ifndef __MDZ80_H
#define __MDZ80_H


#include "libgsft/gsft_fastcall.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/****************************/
/* Structures & definitions */
/****************************/


typedef uint8_t FASTCALL Z80_RB(uint32_t adr);
typedef uint16_t FASTCALL Z80_RW(uint32_t adr);
typedef void FASTCALL Z80_WB(uint32_t adr, uint8_t data);
typedef void FASTCALL Z80_WW(uint32_t adr, uint16_t data);


struct _Z80_context
{
	union
	{
		struct
		{
			uint8_t A;
			uint8_t F;
			uint8_t x;
			uint8_t FXY;
		} b;
		struct
		{
			uint16_t AF;
			uint16_t FXYW;
		} w;
		uint32_t d;
	} AF;
	union
	{
		struct
		{
			uint8_t C;
			uint8_t B;
			uint16_t x;
		} b;
		struct
		{
			uint16_t BC;
			uint16_t x;
		} w;
		uint32_t d;
	} BC;
	union
	{
		struct
		{
			uint8_t E;
			uint8_t D;
			uint16_t x;
		} b;
		struct
		{
			uint16_t DE;
			uint16_t x;
		} w;
		uint32_t d;
	} DE;
	union
	{
		struct
		{
			uint8_t L;
			uint8_t H;
			uint16_t x;
		} b;
		struct
		{
			uint16_t HL;
			uint16_t x;
		} w;
		uint32_t d;
	} HL;
	union
	{
		struct
		{
			uint8_t IXL;
			uint8_t IXH;
			uint16_t x;
		} b;
		struct
		{
			uint16_t IX;
			uint16_t x;
		} w;
		uint32_t d;
	} IX;
	union
	{
		struct
		{
			uint8_t IYL;
			uint8_t IYH;
			uint16_t x;
		} b;
		struct
		{
			uint16_t IY;
			uint16_t x;
		} w;
		uint32_t d;
	} IY;
	union
	{
		struct
		{
			uint8_t PCL;
			uint8_t PCH;
			uint16_t x;
		} b;
		struct
		{
			uint16_t PC;
			uint16_t x;
		} w;
		uint32_t d;
	} PC;	// PC == BasePC + Z80 PC
	union
	{
		struct
		{
			uint8_t SPL;
			uint8_t SPH;
			uint16_t x;
		} b;
		struct
		{
			uint16_t SP;
			uint16_t x;
		} w;
		uint32_t d;
	} SP;
	union
	{
		struct
		{
			uint8_t A2;
			uint8_t F2;
			uint8_t x;
			uint8_t FXY2;
		} b;
		struct
		{
			uint16_t AF2;
			uint16_t FXYW2;
		} w;
		uint32_t d;
	} AF2;
	union
	{
		struct
		{
			uint8_t C2;
			uint8_t B2;
			uint16_t x;
		} b;
		struct
		{
			uint16_t BC2;
			uint16_t x;
		} w;
		uint32_t d;
	} BC2;
	union
	{
		struct
		{
			uint8_t E2;
			uint8_t D2;
			uint16_t x;
		} b;
		struct
		{
			uint16_t DE2;
			uint16_t x;
		} w;
		uint32_t d;
	} DE2;
	union
	{
		struct
		{
			uint8_t L2;
			uint8_t H2;
			uint16_t x;
		} b;
		struct
		{
			uint16_t HL2;
			uint16_t x;
		} w;
		uint32_t d;
	} HL2;
	union
	{
		struct
		{
			uint8_t IFF1;
			uint8_t IFF2;
			uint16_t x;
		} b;
		struct
		{
			uint16_t IFF;
			uint16_t x;
		} w;
		uint32_t d;
	} IFF;
	union
	{
		struct
		{
			uint8_t R1;
			uint8_t R2;
			uint16_t x;
		} b;
		struct
		{
			uint16_t R;
			uint16_t x;
		} w;
		uint32_t d;
	} R;

	uint8_t I;
	uint8_t IM;
	uint8_t IntVect;
	uint8_t IntLine;

	uint32_t Status;
	uint32_t BasePC;	// Pointer to x86 memory location where Z80 RAM starts.
	uint32_t TmpSav0;
	uint32_t TmpSav1;

	uint32_t CycleCnt;
	uint32_t CycleTD;
	uint32_t CycleIO;
	uint32_t CycleSup;

	Z80_RB *ReadB[0x100];
	Z80_WB *WriteB[0x100];
	uint8_t *Fetch[0x100];
	Z80_RW *ReadW[0x100];
	Z80_WW *WriteW[0x100];

	Z80_RB *IN_C;
	Z80_WB *OUT_C;
	uint32_t RetIC;
	uint32_t IntAckC;
};

typedef struct _Z80_context Z80_CONTEXT;


/*************************/
/* Publics Z80 variables */
/*************************/


extern Z80_CONTEXT M_Z80;


/**
 * Z80 functions (asm)
 */


//uint32_t FASTCALL z80_Init(Z80_CONTEXT *z80);
//uint32_t FASTCALL z80_Reset(Z80_CONTEXT *z80);

uint32_t z80_Add_ReadB(Z80_CONTEXT *z80, uint32_t low_adr, uint32_t high_adr, Z80_RB *Func);
uint32_t z80_Add_ReadW(Z80_CONTEXT *z80, uint32_t low_adr, uint32_t high_adr, Z80_RW *Func);
uint32_t z80_Add_WriteB(Z80_CONTEXT *z80, uint32_t low_adr, uint32_t high_adr, Z80_WB *Func);
uint32_t z80_Add_WriteW(Z80_CONTEXT *z80, uint32_t low_adr, uint32_t high_adr, Z80_WW *Func);
uint32_t z80_Add_Fetch(Z80_CONTEXT *z80, uint32_t low_adr, uint32_t high_adr, uint8_t *Region);

//uint32_t FASTCALL z80_Read_Odo(Z80_CONTEXT *z80);
//void FASTCALL z80_Clear_Odo(Z80_CONTEXT *z80);
//void FASTCALL z80_Set_Odo(Z80_CONTEXT *z80, uint32_t Odo);
//void FASTCALL z80_Add_Cycles(Z80_CONTEXT *z80, uint32_t cycles);

uint32_t FASTCALL z80_Exec(Z80_CONTEXT *z80, int odo);


/**
 * Z80 functions (ported to C)
 */


void mdZ80_init(Z80_CONTEXT *z80);
void mdZ80_reset(Z80_CONTEXT *z80);


unsigned int mdZ80_get_PC(Z80_CONTEXT *z80);
unsigned int mdZ80_get_AF(Z80_CONTEXT *z80);
unsigned int mdZ80_get_AF2(Z80_CONTEXT *z80);
void mdZ80_set_PC(Z80_CONTEXT *z80, unsigned int PC);
void mdZ80_set_AF(Z80_CONTEXT *z80, uint32_t AF);
void mdZ80_set_AF2(Z80_CONTEXT *z80, uint32_t AF2);

unsigned int mdZ80_read_odo(Z80_CONTEXT *z80);
static inline void mdZ80_clear_odo(Z80_CONTEXT *z80)
{
	z80->CycleCnt = 0;
}
void mdZ80_set_odo(Z80_CONTEXT *z80, unsigned int odo);
void mdZ80_add_cycles(Z80_CONTEXT *z80, uint32_t cycles);

//uint32_t FASTCALL mdZ80_Exec(Z80_CONTEXT *z80, int odo);

void mdZ80_nmi(Z80_CONTEXT *z80);
void mdZ80_interrupt(Z80_CONTEXT *z80, unsigned char vector);


/**
 * Default Read/Write functions.
 * TODO: Port these to C.
 * This requires the Read/Write function prototype to be changed to non-fastcall.
 */
extern unsigned char mdZ80_def_mem[0x10000];
unsigned char FASTCALL mdZ80_def_ReadB(uint32_t address);
unsigned char FASTCALL mdZ80_def_In(uint32_t address);
unsigned short FASTCALL mdZ80_def_ReadW(uint32_t address);
void FASTCALL mdZ80_def_WriteB(uint32_t address, unsigned char data);
void FASTCALL mdZ80_def_Out(uint32_t address, unsigned char data);
void FASTCALL mdZ80_def_WriteW(uint32_t address, unsigned short data);


#ifdef __cplusplus
}
#endif

#endif /* __MDZ80_H */
