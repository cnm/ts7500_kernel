#ifndef __strcop_defs_h
#define __strcop_defs_h

/*
 * This file is autogenerated from
 *   file:           ../../inst/strcop/rtl/strcop_regs.r
 *     id:           strcop_regs.r,v 1.5 2003/10/15 12:09:45 kriskn Exp
 *     last modfied: Mon Apr 11 16:09:38 2005
 *
 *   by /n/asic/design/tools/rdesc/src/rdes2c --outfile strcop_defs.h ../../inst/strcop/rtl/strcop_regs.r
 *      id: $Id: strcop_defs.h,v 1.1.1.1 2010/03/11 21:10:23 kris Exp $
 * Any changes here will be lost.
 *
 * -*- buffer-read-only: t -*-
 */
/* Main access macros */
#ifndef REG_RD
#define REG_RD( scope, inst, reg ) \
  REG_READ( reg_##scope##_##reg, \
            (inst) + REG_RD_ADDR_##scope##_##reg )
#endif

#ifndef REG_WR
#define REG_WR( scope, inst, reg, val ) \
  REG_WRITE( reg_##scope##_##reg, \
             (inst) + REG_WR_ADDR_##scope##_##reg, (val) )
#endif

#ifndef REG_RD_VECT
#define REG_RD_VECT( scope, inst, reg, index ) \
  REG_READ( reg_##scope##_##reg, \
            (inst) + REG_RD_ADDR_##scope##_##reg + \
	    (index) * STRIDE_##scope##_##reg )
#endif

#ifndef REG_WR_VECT
#define REG_WR_VECT( scope, inst, reg, index, val ) \
  REG_WRITE( reg_##scope##_##reg, \
             (inst) + REG_WR_ADDR_##scope##_##reg + \
	     (index) * STRIDE_##scope##_##reg, (val) )
#endif

#ifndef REG_RD_INT
#define REG_RD_INT( scope, inst, reg ) \
  REG_READ( int, (inst) + REG_RD_ADDR_##scope##_##reg )
#endif

#ifndef REG_WR_INT
#define REG_WR_INT( scope, inst, reg, val ) \
  REG_WRITE( int, (inst) + REG_WR_ADDR_##scope##_##reg, (val) )
#endif

#ifndef REG_RD_INT_VECT
#define REG_RD_INT_VECT( scope, inst, reg, index ) \
  REG_READ( int, (inst) + REG_RD_ADDR_##scope##_##reg + \
	    (index) * STRIDE_##scope##_##reg )
#endif

#ifndef REG_WR_INT_VECT
#define REG_WR_INT_VECT( scope, inst, reg, index, val ) \
  REG_WRITE( int, (inst) + REG_WR_ADDR_##scope##_##reg + \
	     (index) * STRIDE_##scope##_##reg, (val) )
#endif

#ifndef REG_TYPE_CONV
#define REG_TYPE_CONV( type, orgtype, val ) \
  ( { union { orgtype o; type n; } r; r.o = val; r.n; } )
#endif

#ifndef reg_page_size
#define reg_page_size 8192
#endif

#ifndef REG_ADDR
#define REG_ADDR( scope, inst, reg ) \
  ( (inst) + REG_RD_ADDR_##scope##_##reg )
#endif

#ifndef REG_ADDR_VECT
#define REG_ADDR_VECT( scope, inst, reg, index ) \
  ( (inst) + REG_RD_ADDR_##scope##_##reg + \
    (index) * STRIDE_##scope##_##reg )
#endif

/* C-code for register scope strcop */

/* Register rw_cfg, scope strcop, type rw */
typedef struct {
  unsigned int td3         : 1;
  unsigned int td2         : 1;
  unsigned int td1         : 1;
  unsigned int ipend       : 1;
  unsigned int ignore_sync : 1;
  unsigned int en          : 1;
  unsigned int dummy1      : 26;
} reg_strcop_rw_cfg;
#define REG_RD_ADDR_strcop_rw_cfg 0
#define REG_WR_ADDR_strcop_rw_cfg 0


/* Constants */
enum {
  regk_strcop_big                          = 0x00000001,
  regk_strcop_d                            = 0x00000001,
  regk_strcop_e                            = 0x00000000,
  regk_strcop_little                       = 0x00000000,
  regk_strcop_rw_cfg_default               = 0x00000002
};
#endif /* __strcop_defs_h */
