#include "battery.h"
#include "device.h"
#include "stdint.h"
#include "types.h"

#define PG (1<<3)
#define STAT1 (1<<1)
#define STAT2 (1<<2)

#define get_pg() ( P3IN & PG )
#define get_stat1() (P3IN & STAT1)
#define get_stat2() (P3IN & STAT2)

uint16_t battval = 65000;

bool battery_low( void )
{
	/* Low = 3.5V. 1.75 out of divider, 1.75/2.5*1023 = 716 */
	return battval < 716 ? TRUE : FALSE;
}

bool battery_critical( void )
{
	/* Low = 3.3V. 1.65 out of divider, 1.65/2.5*1023 = 675 */
	return battval < 675 ? TRUE : FALSE;
}

void battery_init( void )
{
	P3DIR &= ~(PG | STAT1 | STAT2);
	P3REN |= (PG | STAT1 | STAT2);
	P3OUT |= (PG | STAT1 | STAT2);
}

bool battery_charge_complete( void )
{
	/* stat1 = high, stat2 = low, pg = low */
	if( get_stat1() && (!get_stat2()) && (!get_pg()) )
		return TRUE;
	else
		return FALSE;		       
}

bool battery_charge_in_progress( void )
{
	/* stat1 = low, stat2 = high, pg = low */
	if( (!get_stat1()) && get_stat2() && (!get_pg()) )
		return TRUE;
	else
		return FALSE;
}

bool battery_power_good( void )
{
	if( !get_pg() )
		return TRUE;

	return FALSE;
}
