/* Deals with the network layer of things */
#ifndef __NET_TX_H
#define __NET_TX_H
#include <stdint.h>

/* Returns the next packet to be sent */
const uint8_t* net_tx_get_next_packet( uint8_t *len );

#endif	/* __NET_TX_H */
