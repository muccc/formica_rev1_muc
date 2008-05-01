/* Macros for configuring the IR receiver biasing */
#ifndef __IR_BIAS_H
#define __IR_BIAS_H
#include "device.h"

#define BIAS1 (1<<6)
#define BIAS2 (1<<7)

/* Must be called before ir-receive is initialised */
#define bias_init() do { P2DIR &= ~(BIAS1|BIAS2); P2SEL &= ~(BIAS1|BIAS2); } while (0)

/* Use bias resistor 1 */
#define bias_use1() do { P2DIR &= ~(BIAS1|BIAS2); P2OUT |= BIAS1; P2DIR |= BIAS1; } while (0)

/* Use bias resistor 2 */
#define bias_use2() do { P2DIR &= ~(BIAS1|BIAS2); P2OUT |= BIAS2; P2DIR |= BIAS2; } while (0)

#endif	/* __IR_BIAS_H */
