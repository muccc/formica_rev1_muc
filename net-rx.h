/* The receive side of the net stuff */
#ifndef __NET_RX_H
#define __NET_RX_H
#include <stdint.h>

/* A new frame has been received */
void net_rx_proc_incoming( uint8_t* frame, uint8_t len );

#endif	/* __NET_RX_H */
