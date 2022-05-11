#ifndef SIGNAL_TABLES_H_
#define SIGNAL_TABLES_H_
#include "main.h"
/********************** START of impulse_drive **********************/
/** In this project idt means impulse drive table **/
/** 				dsb - discrete-signals' byte **/
typedef struct idt_error_codes
{
	uint8_t TXDI:1;
	uint8_t TXDU:1;
	uint8_t TXDVT:1;
	uint8_t TXDDVT1:1;
	uint8_t DVT:1;
	uint8_t DPIT:1;
	uint8_t UMAX2:1;
	uint8_t DI:1;
}idt_error_codes_t;

typedef struct idt_dsb0
{
	uint8_t YDET:1;
	uint8_t YRT1s:1;
	uint8_t MT2:1;
	uint8_t MT1:1;
	uint8_t YK04:1;
	uint8_t YYB:1;
	uint8_t YKL3:1;
	uint8_t YKL1:1;
}idt_dsb0_t;
typedef struct idt_dsb1
{
		uint8_t res:7;
	uint8_t YRT2:1;
}idt_dsb1_t;
typedef struct idt_dsb2
{
		uint8_t res;
}idt_dsb2_t;
typedef struct idt_dsb3
{
	uint8_t res;
}idt_dsb3_t;
typedef struct idt_dsb4
{
	uint8_t XSQ5:1;
		uint8_t res:1;
	uint8_t XZ3:1;
	uint8_t XZ2:1;
	uint8_t XZ1:1;
	uint8_t XH:1;
	uint8_t XZ:1;
	uint8_t XP:1;
}idt_dsb4_t;
typedef struct idt_dsb5
{
	uint8_t XKL3:1;
	uint8_t XKL1:1;
		uint8_t res1:2;
	uint8_t XKYB:1;
		uint8_t res2:1;
	uint8_t YK01:1;
	uint8_t XT:1;
}idt_dsb5_t;
typedef struct idt_dsb6
{
		uint8_t res1:2;
	uint8_t XET3:1;
		uint8_t res2:2;
	uint8_t XMT:1;
	uint8_t XET1:1;
	uint8_t XAT:1;
}idt_dsb6_t;
typedef struct idt_dsb7
{
		uint8_t res;
}idt_dsb7_t;
typedef struct idt_dsb8
{
	uint8_t XDVT5:1;
	uint8_t XDVT4:1;
	uint8_t XDVT3:1;
	uint8_t XDVT2:1;
	uint8_t XDVT1:1;
	uint8_t TZ:1;
	uint8_t XVENT:1;
	uint8_t XKT:1;
}idt_dsb8_t;
typedef struct idt_dsb9
{
	uint8_t LED_VD13:1;
	uint8_t LED_VD12:1;
	uint8_t YVENT:1;
	uint8_t XDU:1;
	uint8_t XDI:1;
	uint8_t XDVT:1;
	uint8_t XDVT7:1;
	uint8_t XDVT6:1;
}idt_dsb9_t;
typedef struct idt_dsb10
{
		uint8_t res:7;
	uint8_t XV0:1;
}idt_dsb10_t;
typedef struct idt_dsb11
{
		uint8_t res;
}idt_dsb11_t;

typedef struct impulse_drive_table
{
	idt_error_codes_t error_codes;
	idt_dsb0_t	dsb0;
	idt_dsb1_t	dsb1;
	idt_dsb2_t	dsb2;
	idt_dsb3_t	dsb3;
	idt_dsb4_t	dsb4;
	idt_dsb5_t	dsb5;
	idt_dsb6_t	dsb6;
	idt_dsb7_t	dsb7;
	idt_dsb8_t	dsb8;
	idt_dsb9_t	dsb9;
	idt_dsb10_t	dsb10;
	uint8_t res1;
	idt_dsb11_t	dsb11;
	uint8_t U[2];
	uint8_t I[2];
	uint8_t XTT[2];
	uint8_t XTH[2];
	uint8_t UY[2];
	uint8_t CI[2];
}impulse_drive_table_t;
/********************** END of impulse_drive **********************/

/********************** START of input-output board **********************/
/** In this project iobt means input-output board table **/
typedef struct iobt_K1_K5
{
	uint8_t K1:1;
	uint8_t K2:1;
	uint8_t K3:1;
	uint8_t K4:1;
	uint8_t K5:1;
	uint8_t res:3;
}iobt_K1_K5_t;

typedef struct iobt_KS1_KS8
{
	uint8_t KS1:1;
	uint8_t KS2:1;
	uint8_t KS3:1;
	uint8_t KS4:1;
	uint8_t KS5:1;
	uint8_t KS6:1;
	uint8_t KS7:1;
	uint8_t KS8:1;
}iobt_KS1_KS8_t;

typedef struct iobt_KS9_KS12
{
	uint8_t KS9:1;
	uint8_t KS10:1;
	uint8_t KS11:1;
	uint8_t KS12:1;
	uint8_t res:4;
}iobt_KS9_KS12_t;

typedef struct io_board_table
{
	iobt_K1_K5_t K;
	iobt_KS1_KS8_t KS1_8;
	uint8_t res3;
	iobt_KS9_KS12_t KS9_12;
	uint8_t CH2[2];
	uint8_t CH3[2];
	uint8_t CH4[2];
	uint8_t CH5[2];
	uint8_t CH7[2];
	uint8_t CH14[2];
	uint8_t CH15[2];
	uint8_t XA1[2];
	uint8_t XA2[2];
}io_board_table_t;
/********************** END of input-output board **********************/


#endif /* SIGNAL_TABLES_H_ */
