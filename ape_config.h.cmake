#cmakedefine HAVE_STDINT_H
#ifndef HAVE_STDINT_H
	#ifdef _WIN32
		#define WIN32_LEAN_AND_MEAN
		#include <windows.h>
		#define uint8_t BYTE
		#define uint16_t WORD
		#define uint32_t DWORD
	#endif
#endif

#cmakedefine HAVE_STDBOOL_H
#ifndef __bool_true_false_are_defined
	#define bool int
	#define true 1
	#define false 0
#endif
