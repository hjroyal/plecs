/*
   Copyright (c) 2014-2022 by Plexim GmbH
   All rights reserved.

   A free license is granted to anyone to use this software for any legal
   non safety-critical purpose, including commercial applications, provided
   that:
   1) IT IS NOT USED TO DIRECTLY OR INDIRECTLY COMPETE WITH PLEXIM, and
   2) THIS COPYRIGHT NOTICE IS PRESERVED in its entirety.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
 */

MEMORY
{
   BOOT_RSVD         : origin = 0x000002, length = 0x0000A0     /* Part of M0, BOOT rom will use this for stack */
   RAMM0           	: origin = 0x0000A2, length = 0x00035E
   RAMM1             : origin = 0x000400, length = 0x000400     /* on-chip RAM block M1 */

   RAMLS	            : origin = 0x008000, length = 0x003F00
   //RAMLS0          : origin = 0x008000, length = 0x000800
   //RAMLS1          : origin = 0x008800, length = 0x000800
   //RAMLS2      	   : origin = 0x009000, length = 0x000800
   //RAMLS3      	   : origin = 0x009800, length = 0x000800
   //RAMLS4      	   : origin = 0x00A000, length = 0x000800
   //RAMLS5          : origin = 0x00A800, length = 0x000800
   //RAMD0           : origin = 0x00B000, length = 0x000800
   //RAMD1           : origin = 0x00B800, length = 0x000800
   RAMD1_RSVD        : origin = 0x00BF00, length = 0x000100  // JTAG communication buffer

   RAMGS_SCOPE     : origin = 0x00C000, length = 0x003000 // GS0-3 (see extmode.lua)

   CPU2TOCPU1RAM     : origin = 0x03F800, length = 0x00037F
   SCOPE_FLAG_CPU2   : origin = 0x03FB7F, length = 0x000001
   CPU2TOCPU1LINK    : origin = 0x03FB80, length = 0x000080

   CPU1TOCPU2RAM     : origin = 0x03FC00, length = 0x00037F
   SCOPE_FLAG_CPU1   : origin = 0x03FF7F, length = 0x000001
   CPU1TOCPU2LINK    : origin = 0x03FF80, length = 0x000080

   BEGIN           	: origin = 0x080000, length = 0x000002     // FLASH A
   /* Flash sectors */
   FLASH             : origin = 0x080002, length = 0x03FFFE	/* on-chip Flash */
   //FLASHA          : origin = 0x080002, length = 0x001FFE	/* on-chip Flash */
   //FLASHB          : origin = 0x082000, length = 0x002000	/* on-chip Flash */
   //FLASHC          : origin = 0x084000, length = 0x002000	/* on-chip Flash */
   //FLASHD          : origin = 0x086000, length = 0x002000	/* on-chip Flash */
   //FLASHE          : origin = 0x088000, length = 0x008000	/* on-chip Flash */
   //FLASHF          : origin = 0x090000, length = 0x008000	/* on-chip Flash */
   //FLASHG          : origin = 0x098000, length = 0x008000	/* on-chip Flash */
   //FLASHH          : origin = 0x0A0000, length = 0x008000	/* on-chip Flash */
   //FLASHI          : origin = 0x0A8000, length = 0x008000	/* on-chip Flash */
   //FLASHJ          : origin = 0x0B0000, length = 0x008000	/* on-chip Flash */
   //FLASHK          : origin = 0x0B8000, length = 0x002000	/* on-chip Flash */
   //FLASHL          : origin = 0x0BA000, length = 0x002000	/* on-chip Flash */
   //FLASHM          : origin = 0x0BC000, length = 0x002000	/* on-chip Flash */
   //FLASHN          : origin = 0x0BE000, length = 0x002000	/* on-chip Flash */

   RESET           	: origin = 0x3FFFC0, length = 0x000002
}

SECTIONS
{
   scope              : > RAMGS_SCOPE, ALIGN(2)
   scopeflag_remote   : > SCOPE_FLAG_CPU1
   scopeflag_local    : > SCOPE_FLAG_CPU2

   cpu1to2_link_mem : > CPU1TOCPU2LINK
   cpu2to1_link_mem : > CPU2TOCPU1LINK
   step              : > FLASH, ALIGN(4)
   dispatch          : > FLASH, ALIGN(4)

   codestart         : > BEGIN, ALIGN(4)
   .cinit            : > FLASH, ALIGN(4)
   .text             : > FLASH, ALIGN(4)
   .switch           : > FLASH, ALIGN(4)

   .stack            : > RAMM1 | RAMLS

#if defined(__TI_EABI__)
   .init_array      : > FLASH, ALIGN(4)
   .const           : > FLASH, ALIGN(4)
   .bss             : > RAMLS
   .bss:output      : > RAMLS
   .bss:cio         : > RAMLS
   .data            : > RAMLS
   .sysmem          : > RAMLS
#else
   .cio             : > FLASH, ALIGN(4)
   .econst          : > FLASH, ALIGN(4)
   .pinit           : > FLASH, ALIGN(4)
   .ebss            : > RAMLS
   .esysmem         : > RAMLS
#endif

   GROUP : LOAD = FLASH,
           RUN = RAMM0,
#if defined(__TI_EABI__)
           LOAD_START(RamfuncsLoadStart),
           LOAD_SIZE(RamfuncsLoadSize),
           LOAD_END(RamfuncsLoadEnd),
           RUN_START(RamfuncsRunStart),
           RUN_SIZE(RamfuncsRunSize),
           RUN_END(RamfuncsRunEnd),
#else
           LOAD_START(_RamfuncsLoadStart),
           LOAD_SIZE(_RamfuncsLoadSize),
           LOAD_END(_RamfuncsLoadEnd),
           RUN_START(_RamfuncsRunStart),
           RUN_SIZE(_RamfuncsRunSize),
           RUN_END(_RamfuncsRunEnd),
#endif
           ALIGN(4)
   {
      ramfuncs
      .TI.ramfunc
   }
   .reset              : > RESET, TYPE = DSECT

}
