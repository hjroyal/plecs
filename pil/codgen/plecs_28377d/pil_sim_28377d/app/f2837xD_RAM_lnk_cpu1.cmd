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
   BEGIN           	: origin = 0x000000, length = 0x000002  // MO RAM
   BOOT_RSVD         : origin = 0x000002, length = 0x000121     /* Part of M0, BOOT rom will use this for stack */
   RAMM0           	: origin = 0x000124, length = 0x0002DC
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

   PRAMGS      		: origin = 0x00C000, length = 0x008000

   CPU2TOCPU1RAM     : origin = 0x03F800, length = 0x00037F
   SCOPE_FLAG_CPU2   : origin = 0x03FB7F, length = 0x000001
   CPU2TOCPU1LINK    : origin = 0x03FB80, length = 0x000080

   CPU1TOCPU2RAM     : origin = 0x03FC00, length = 0x00037F
   SCOPE_FLAG_CPU1   : origin = 0x03FF7F, length = 0x000001
   CPU1TOCPU2LINK    : origin = 0x03FF80, length = 0x000080

   RESET           	: origin = 0x3FFFC0, length = 0x000002
}

SECTIONS
{
   scope             : > RAMLS, ALIGN(2)
   scopeflag_local   : > SCOPE_FLAG_CPU1
   scopeflag_remote  : > SCOPE_FLAG_CPU2

   cpu1to2_link_mem : > CPU1TOCPU2LINK
   cpu2to1_link_mem : > CPU2TOCPU1LINK
   step              : > PRAMGS
   dispatch          : > PRAMGS

   codestart         : > BEGIN
   .cinit            : > PRAMGS
   .text             : > PRAMGS
   .switch           : > PRAMGS

   .stack            : > RAMM1 | RAMLS

#if defined(__TI_EABI__)
   .init_array      : > PRAMGS
   .const           : > PRAMGS
   .bss             : > RAMLS
   .bss:output      : > RAMLS
   .bss:cio         : > RAMLS
   .data            : > RAMLS
   .sysmem          : > RAMLS
#else
   .cio             : > PRAMGS
   .econst          : > PRAMGS
   .pinit           : > PRAMGS
   .ebss            : > RAMLS
   .esysmem         : > RAMLS
#endif

   GROUP: > PRAMGS
   {
      ramfuncs
      .TI.ramfunc
   }
   .reset              : > RESET, TYPE = DSECT
   
}
