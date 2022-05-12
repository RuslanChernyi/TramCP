#include "Algo.h"
#include "main.h"


/** In this project iobt means input-output board table **/
typedef struct iobt_input2
{
	struct Operation_mode
	{
		uint8_t move;
		uint8_t brake;
		uint8_t stopway;
	}Operation_mode_t;

	enum
	{
		DOOR_CLOSED,
		DOOR_OPENED
	}door_state_enum;
	struct door_state
	{
		uint8_t door1;
		uint8_t door2;
		uint8_t door3;
	}door_state_t;

	uint8_t high_voltage_net;

	uint8_t washing_mode;

	uint8_t arrow_under_current;
	uint8_t arrow_without_current;

	uint8_t master_carriage;
	uint8_t slave_carriage;

	uint8_t tram_breakup1;
	uint8_t tram_breakup2;

	// HV - high voltage
	uint8_t HV_KKB_power;// Питание статика ККВ
	uint8_t HV_KC_power;// Питание статика KC

	uint32_t accumulator_battery1;	// Заряд батареи
	uint32_t accumulator_battery2;	// Заряд батареи
}iobt_input2_t;

typedef struct
{
	uint8_t apparel;
	uint8_t PT_on;		// Сигнал П/Т Включены

	uint8_t master_carriage_high_voltage_net;
	uint8_t slave_carriage_high_voltage_net;

	uint8_t signal_from_cabin;
	uint8_t control_on;
	uint8_t tram_breakup;

	uint8_t working_with_PZ;	// Работа с ПЗ?
}iobt_output2_t;

typedef struct iobt_input2
{

}iobt_input2_t;




