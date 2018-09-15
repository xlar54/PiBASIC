#ifndef types_h
#define types_h

#include <stdint.h>
#include <stddef.h>
#include <uspi/types.h>

/*typedef char				uint8_t;
typedef signed char			int8_t;
typedef unsigned short		uint16_t;
typedef short				int16_t;
typedef unsigned int		uint32_t;
typedef int					int32_t;*/
typedef unsigned long long	uint64_t;
typedef signed long long	int64_t;

/* These types MUST be 16-bit or 32-bit */
typedef int				INT;
typedef unsigned int	UINT;

/* This type MUST be 8-bit */
typedef unsigned char	BYTE;

/* These types MUST be 16 bit */
typedef short			SHORT;
typedef unsigned short	WORD;
typedef unsigned short	WCHAR;

/* These types MUST be 32-bit */
typedef long			LONG;
typedef unsigned long	DWORD;

/* This type MUST be 64-bit (Remove this for C89 compatibility) */
typedef unsigned long long QWORD;

typedef enum {
	EXIT_UNKNOWN,
	EXIT_RESET,
	EXIT_CD,
	EXIT_KEYBOARD,
	EXIT_AUTOLOAD
} EXIT_TYPE;


typedef void(*func_ptr)();

#endif
