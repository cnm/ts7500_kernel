/* $Id: byteorder.h,v 1.1.1.1 2010/03/11 21:10:20 kris Exp $ */
#ifndef _SPARC_BYTEORDER_H
#define _SPARC_BYTEORDER_H

#include <asm/types.h>

#if defined(__GNUC__) && !defined(__STRICT_ANSI__) || defined(__KERNEL__)
#  define __BYTEORDER_HAS_U64__
#  define __SWAB_64_THRU_32__
#endif

#include <linux/byteorder/big_endian.h>

#endif /* _SPARC_BYTEORDER_H */
