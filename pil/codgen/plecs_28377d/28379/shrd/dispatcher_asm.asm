;  Copyright (c) 2014 by Plexim GmbH
;  All rights reserved.

;  NOTICE: This software embodies unpublished proprietary
;  and trade secret information of Plexim GmbH.  Any copying,
;  disclosure or transmission not expressly authorized by
;  the owner is expressly prohibited. Plexim GmbH retains exclusive
;  ownership of all intellectual property rights in this software,
;  including all copyright, trade secret and patent rights.

   .if __TI_EABI__
   .asg DISPR_disableInt, _DISPR_disableInt
   .asg DISPR_restoreInt, _DISPR_restoreInt
   .endif
   .def _DISPR_disableInt
   .def _DISPR_restoreInt

_DISPR_disableInt:
    PUSH  ST1
    SETC  INTM,DBGM
    MOV   AL, *--SP
    LRETR

_DISPR_restoreInt:
    MOV   *SP++, AL
    POP   ST1
    LRETR
