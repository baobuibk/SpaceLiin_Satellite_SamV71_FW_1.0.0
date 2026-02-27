

#ifndef _BOARD_H_
#define _BOARD_H_


#define BOARD_SAMV71_ISS
#if defined(BOARD_SAMV71_XULT)
	#include "board_v71_xult.h"
#elif defined(BOARD_SAMV71_ISS)
	#include "board_v71_iss.h"
#else
	#error "board definition not correct!"
#endif

#endif /* #ifndef _BOARD_H_ */

