#ifndef SIGNAL_TABLES_H_
#define SIGNAL_TABLES_H_
#include "main.h"
/********************** START of impulse_drive **********************/
/** In this project idt means impulse drive table **/
/** 				dsb - discrete-signals' byte **/
//typedef struct idt_error_codes
//{
//	uint8_t TXDI:1;
//	uint8_t TXDU:1;
//	uint8_t TXDVT:1;
//	uint8_t TXDDVT1:1;
//	uint8_t DVT:1;
//	uint8_t DPIT:1;
//	uint8_t UMAX2:1;
//	uint8_t DI:1;
//}idt_error_codes_t;
//
//typedef struct idt_dsb0
//{
//	uint8_t YDET:1;
//	uint8_t YRT1s:1;
//	uint8_t MT2:1;
//	uint8_t MT1:1;
//	uint8_t YK04:1;
//	uint8_t YYB:1;
//	uint8_t YKL3:1;
//	uint8_t YKL1:1;
//}idt_dsb0_t;
//typedef struct idt_dsb1
//{
//		uint8_t res:7;
//	uint8_t YRT2:1;
//}idt_dsb1_t;
//typedef struct idt_dsb2
//{
//		uint8_t res;
//}idt_dsb2_t;
//typedef struct idt_dsb3
//{
//	uint8_t res;
//}idt_dsb3_t;
//typedef struct idt_dsb4
//{
//	uint8_t XSQ5:1;
//		uint8_t res:1;
//	uint8_t XZ3:1;
//	uint8_t XZ2:1;
//	uint8_t XZ1:1;
//	uint8_t XH:1;
//	uint8_t XZ:1;
//	uint8_t XP:1;
//}idt_dsb4_t;
//typedef struct idt_dsb5
//{
//	uint8_t XKL3:1;
//	uint8_t XKL1:1;
//		uint8_t res1:2;
//	uint8_t XKYB:1;
//		uint8_t res2:1;
//	uint8_t YK01:1;
//	uint8_t XT:1;
//}idt_dsb5_t;
//typedef struct idt_dsb6
//{
//		uint8_t res1:2;
//	uint8_t XET3:1;
//		uint8_t res2:2;
//	uint8_t XMT:1;
//	uint8_t XET1:1;
//	uint8_t XAT:1;
//}idt_dsb6_t;
//typedef struct idt_dsb7
//{
//		uint8_t res;
//}idt_dsb7_t;
//typedef struct idt_dsb8
//{
//	uint8_t XDVT5:1;
//	uint8_t XDVT4:1;
//	uint8_t XDVT3:1;
//	uint8_t XDVT2:1;
//	uint8_t XDVT1:1;
//	uint8_t TZ:1;
//	uint8_t XVENT:1;
//	uint8_t XKT:1;
//}idt_dsb8_t;
//typedef struct idt_dsb9
//{
//	uint8_t LED_VD13:1;
//	uint8_t LED_VD12:1;
//	uint8_t YVENT:1;
//	uint8_t XDU:1;
//	uint8_t XDI:1;
//	uint8_t XDVT:1;
//	uint8_t XDVT7:1;
//	uint8_t XDVT6:1;
//}idt_dsb9_t;
//typedef struct idt_dsb10
//{
//		uint8_t res:7;
//	uint8_t XV0:1;
//}idt_dsb10_t;
//typedef struct idt_dsb11
//{
//		uint8_t res;
//}idt_dsb11_t;
//
//typedef struct impulse_drive_table
//{
//	idt_error_codes_t error_codes;
//	idt_dsb0_t	dsb0;
//	idt_dsb1_t	dsb1;
//	idt_dsb2_t	dsb2;
//	idt_dsb3_t	dsb3;
//	idt_dsb4_t	dsb4;
//	idt_dsb5_t	dsb5;
//	idt_dsb6_t	dsb6;
//	idt_dsb7_t	dsb7;
//	idt_dsb8_t	dsb8;
//	idt_dsb9_t	dsb9;
//	idt_dsb10_t	dsb10;
//	uint8_t res1;
//	idt_dsb11_t	dsb11;
//	uint8_t U[2];
//	uint8_t I[2];
//	uint8_t XTT[2];
//	uint8_t XTH[2];
//	uint8_t UY[2];
//	uint8_t CI[2];
//}impulse_drive_table_t;

typedef struct new_idt
{
	struct
	{
		uint8_t TXDI:1;
		uint8_t TXDU:1;
		uint8_t TXDVT:1;
		uint8_t TXDVT1:1;
		uint8_t DVT:1;
		uint8_t DPIT:1;
		uint8_t UMAX2:1;
		uint8_t DI:1;
	}byte_1;
	struct
	{
		uint8_t YDET:1;
		uint8_t YRT1:1;
		uint8_t MT2:1;
		uint8_t MT1:1;
		uint8_t YK04:1;
		uint8_t YYB:1;
		uint8_t YKL3:1;
		uint8_t YKL1:1;
	}byte_2;
	struct
	{
		uint8_t YRT2:1;
		uint8_t XSQ5:1;
		uint8_t XZ3:1;
		uint8_t XZ2:1;
		uint8_t XZ1:1;
		uint8_t XH:1;
		uint8_t XZ:1;
		uint8_t XP:1;
	}byte_3;
	struct
	{
		uint8_t XKL3:1;
		uint8_t XKL1:1;
		uint8_t XKYB:1;
		uint8_t XK01:1;
		uint8_t XT:1;
		uint8_t XET3:1;
		uint8_t XMT:1;
		uint8_t XET1:1;
	}byte_4;
	struct
	{
		uint8_t XAT:1;
		uint8_t XDVT5:1;
		uint8_t XDVT4:1;
		uint8_t XDVT3:1;
		uint8_t XDVT2:1;
		uint8_t XDVT1:1;
		uint8_t TZ:1;
		uint8_t XVENT:1;
	}byte_5;
	struct
	{
		uint8_t XKT:1;
		uint8_t LED_VD13:1;
		uint8_t LED_VD12:1;
		uint8_t YVENT:1;
		uint8_t XDU:1;
		uint8_t XDI:1;
		uint8_t XDVT:1;
		uint8_t XDVT7:1;
	}byte_6;
	struct
	{
		uint8_t XDVT6:1;
		uint8_t XV0:1;
		uint8_t res0 : 6;
	}byte_7;
	uint8_t res0;
	uint8_t U[2];
	uint8_t I[2];
	uint8_t XTT[2];
	uint8_t XTH[2];
	uint8_t UY[2];
	uint8_t CI[2];
}new_idt_t;
/********************** END of impulse_drive **********************/

/********************** START of input-output board **********************/
/** In this project iobt means input-output board table **/
//typedef struct iobt_K1_K5
//{
//	uint8_t K1:1;
//	uint8_t K2:1;
//	uint8_t K3:1;
//	uint8_t K4:1;
//	uint8_t K5:1;
//	uint8_t res:3;
//}iobt_K1_K5_t;
//typedef struct iobt_KS1_KS8
//{
//	uint8_t KS1:1;
//	uint8_t KS2:1;
//	uint8_t KS3:1;
//	uint8_t KS4:1;
//	uint8_t KS5:1;
//	uint8_t KS6:1;
//	uint8_t KS7:1;
//	uint8_t KS8:1;
//}iobt_KS1_KS8_t;
//typedef struct iobt_KS9_KS12
//{
//	uint8_t KS9:1;
//	uint8_t KS10:1;
//	uint8_t KS11:1;
//	uint8_t KS12:1;
//	uint8_t res:4;
//}iobt_KS9_KS12_t;
//typedef struct io_board_table
//{
//	iobt_K1_K5_t K;
//	iobt_KS1_KS8_t KS1_8;
//	uint8_t res3;
//	iobt_KS9_KS12_t KS9_12;
//	uint8_t CH2[2];
//	uint8_t CH3[2];
//	uint8_t CH4[2];
//	uint8_t CH5[2];
//	uint8_t CH7[2];
//	uint8_t CH14[2];
//	uint8_t CH15[2];
//	uint8_t XA1[2];
//	uint8_t XA2[2];
//}io_board_table_t;
typedef struct iobt2
{
	struct
	{
		uint8_t signal_for_driver_passanger_out1 : 1;
		uint8_t signal_for_driver_passanger_out2 : 1;
		uint8_t signal_for_driver_passanger_out3 : 1;
		uint8_t signal_for_apparel : 1;
		uint8_t signal_for_invalid_near_door : 1;
		uint8_t signal_for_invalid_cabin : 1;
		uint8_t signal_for_door1_opened : 1;
		uint8_t signal_for_door2_opened : 1;

		uint8_t signal_for_door3_opened : 1;
		uint8_t signal_for_hv_circuit_on : 1;
		uint8_t signal_for_washing : 1;
		uint8_t signal_for_arrow_under_current : 1;
		uint8_t signal_for_arrow_without_current : 1;
		uint8_t signal_for_master_cabin : 1;
		uint8_t signal_for_slave_cabin : 1;
		uint8_t signal_for_tram_disconect_1 : 1;

		uint8_t signal_for_static_KKB_power_on : 1;
		uint8_t signal_for_static_KC_power_on : 1;
		uint8_t signal_for_tram_disconect_2 : 1;
		uint8_t res0 : 5;
	}DIN;
	struct
	{
		uint8_t indication_apparel : 1;
		uint8_t indication_signal_PT_on : 1;
		uint8_t indication_signal_hv_circuit : 1;
		uint8_t indication_signal_from_cabin : 1;
		uint8_t indication_control_on : 1;
		uint8_t indication_tram_disconect : 1;
		uint8_t indication_work_with_PZ : 1;
		uint8_t res0 : 1;
	}DOUT;
	struct
	{
		uint8_t battery_voltage[2];
	}analog;

}iobt2_t;
typedef struct iobt1
{
	struct
	{
		uint8_t signal_for_close_lights : 1;
		uint8_t signal_for_far_lights : 1;
		uint8_t signal_for_PT_lights : 1;
		uint8_t signal_for_half_heating : 1;
		uint8_t signal_for_full_heating : 1;
		uint8_t signal_for_left_indication : 1;
		uint8_t signal_for_right_indication : 1;
		uint8_t signal_for_work_with_PZ : 1;

		uint8_t signal_for_air_conditioning_for_KB : 1;
		uint8_t signal_for_air_conditioning_for_KC : 1;
		uint8_t res0 : 6;
	}DIN;
	struct
	{
		uint8_t indication_close_lights : 1;
		uint8_t indication_far_lights : 1;
		uint8_t indication_air_conditioning_for_KB : 1;
		uint8_t indication_half_heating : 1;
		uint8_t indication_full_heating : 1;
		uint8_t indication_air_conditioning_for_KC : 1;
		uint8_t indication_left : 1;
		uint8_t indication_right : 1;

		uint8_t indication_cabin_fan : 1;
		uint8_t indication_warning_doors : 1;
		uint8_t res0 : 6;
	}DOUT;
	struct
	{
		uint8_t tachogenerator_phase1[2];
		uint8_t tachogenerator_phase2[2];
	}analog;
}iobt1_t;
typedef struct iobt3
{
	struct
	{
		uint8_t signal_for_dimension_lights : 1;
		uint8_t signal_for_turn_on_half_lighting : 1;
		uint8_t signal_for_turn_on_full_lighting : 1;
		uint8_t signal_for_turn_on_control : 1;
		uint8_t signal_for_unauthorized_PZ_access : 1;
		uint8_t signal_for_fire : 1;
		uint8_t signal_for_LS_state : 1;
		uint8_t res0 : 1;
	}DIN;
	struct
	{
		uint8_t indication_dimension_lights : 1;
		uint8_t indication_half_lighting : 1;
		uint8_t indication_full_lighting : 1;
		uint8_t indication_emergency_lighting : 1;
		uint8_t indication_control_startup : 1;
		uint8_t indication_fire : 1;
		uint8_t indication_control_working : 1;
		uint8_t indication_AT_cabin12_power_on : 1;

		uint8_t indication_E1_cabin12_power_on : 1;
		uint8_t indication_PZ_power_up : 1;
		uint8_t res0 : 6;
	}DOUT;
	uint8_t res0;
}iobt3_t;
typedef struct idtA
{
	struct
	{
		uint8_t signal_for_brake : 1;
		uint8_t signal_for_running_out_mode : 1;	// Выбег
		uint8_t signal_for_move : 1;
		uint8_t signal_for_forward : 1;
		uint8_t signal_for_backward : 1;
		uint8_t signal_for_setting_PZ1 : 1;
		uint8_t signal_for_setting_PZ2 : 1;
		uint8_t signal_for_setting_PZ3 : 1;
		uint8_t signal_for_YUZ_power_off : 1;
		uint8_t signal_for_KL1_state : 1;
		uint8_t signal_for_KL3_state : 1;
		uint8_t signal_for_MT1_state : 1;
		uint8_t signal_for_PWM_move : 1;
		uint8_t signal_for_PWM_break : 1;
		uint8_t signal_for_E1 : 1;
		uint8_t signal_for_E3 : 1;
		uint8_t signal_for_AT : 1;
		uint8_t signal_for_emergency_start : 1;
	}DIN;
	struct
	{
		uint8_t indication_KL1_power_on : 1;
		uint8_t indication_KL3_power_on : 1;
		uint8_t indication_breaking_of_MT1 : 1;
		uint8_t indication_full_break_of_MT1 : 1;
		uint8_t indication_sandboxes : 1;
		uint8_t indication_rail_breaks : 1;
		uint8_t indication_YUZ_signal : 1;
		uint8_t indication_speed_equals_zero : 1;
		uint8_t res0 : 6;
	}DOUT;
}idtA_t;
typedef struct idtB
{
	struct
	{
		uint8_t signal_for_brake : 1;
		uint8_t signal_for_running_out_mode : 1;	// Выбег
		uint8_t signal_for_move : 1;
		uint8_t signal_for_forward : 1;
		uint8_t signal_for_backward : 1;
		uint8_t signal_for_setting_PZ1 : 1;
		uint8_t signal_for_setting_PZ2 : 1;
		uint8_t signal_for_setting_PZ3 : 1;
		uint8_t signal_for_YUZ_power_off : 1;
		uint8_t signal_for_KL2_state : 1;
		uint8_t signal_for_KL4_state : 1;
		uint8_t signal_for_MT2_state : 1;
		uint8_t signal_for_PWM_move : 1;
		uint8_t signal_for_PWM_break : 1;
		uint8_t signal_for_E1 : 1;
		uint8_t signal_for_E3 : 1;
		uint8_t signal_for_AT : 1;
		uint8_t signal_for_emergency_start : 1;
	}DIN;
	struct
	{
		uint8_t indication_KL2_power_on : 1;
		uint8_t indication_KL4_power_on : 1;
		uint8_t indication_breaking_of_MT2 : 1;
		uint8_t indication_full_break_of_MT2 : 1;
		uint8_t indication_sandboxes : 1;
		uint8_t indication_rail_breaks : 1;
		uint8_t indication_YUZ_signal : 1;
		uint8_t indication_speed_equals_zero : 1;
		uint8_t res0 : 6;
	}DOUT;
}idtB_t;
typedef struct new_MODBUSTable_struct
{
	new_idt_t idtA_struct;
	new_idt_t idtB_struct;
	iobt2_t iobt2_struct;
	iobt1_t iobt1_struct;
	iobt3_t iobt3_struct;
}TableStructure_t;
typedef union new_MODBUSTable_uni
{
	TableStructure_t bit_table;
	uint8_t byte_table[sizeof(TableStructure_t)];
}ModbusTable_t;
/********************** END of input-output board **********************/


#endif /* SIGNAL_TABLES_H_ */
