#ifndef __NET_H
#define __NET_H

enum
{
	/* Colour command */
	NET_CMD_COLOUR,

	/* Firmware blob */
	NET_CMD_FW_BLOB,

	/* Ask for the next firmware blob */
	NET_CMD_FW_NEXT,

	NET_CMD_LAST
};

extern uint16_t net_id;

#endif	/* __NET_H */
