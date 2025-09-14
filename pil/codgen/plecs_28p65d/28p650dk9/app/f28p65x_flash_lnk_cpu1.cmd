/*
   Copyright (c) 2023 by Plexim GmbH
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
   BEGIN            : origin = 0x080000, length = 0x000002

   BOOT_RSVD        : origin = 0x000002, length = 0x0001AF     /* Part of M0, BOOT rom will use this for stack */
   RAMM0            : origin = 0x0001B1, length = 0x00024F
   RAMM1            : origin = 0x000400, length = 0x000400

   // RAMD0            : origin = 0x00C000, length = 0x002000
   // RAMD1            : origin = 0x00E000, length = 0x002000
   // RAMD2            : origin = 0x01A000, length = 0x002000  // Can be mapped to either CPU1 or CPU2. When configured to CPU2, use the address 0x8000. User should comment/uncomment based on core selection
   // RAMD3            : origin = 0x01C000, length = 0x002000  // Can be mapped to either CPU1 or CPU2. When configured to CPU2, use the address 0xA000. User should comment/uncomment based on core selection
   // RAMD4            : origin = 0x01E000, length = 0x002000  // Can be mapped to either CPU1 or CPU2. When configured to CPU2, use the address 0xC000. User should comment/uncomment based on core selection
   // RAMD5            : origin = 0x020000, length = 0x002000  // Can be mapped to either CPU1 or CPU2. When configured to CPU2, use the address 0xE000. User should comment/uncomment based on core selection

   RAMLS              : origin = 0x008000, length = 0x003F00
   //RAMLS0           : origin = 0x008000, length = 0x000800
   //RAMLS1           : origin = 0x008800, length = 0x000800
   //RAMLS2           : origin = 0x009000, length = 0x000800
   //RAMLS3           : origin = 0x009800, length = 0x000800
   //RAMLS4           : origin = 0x00A000, length = 0x000800
   //RAMLS5           : origin = 0x00A800, length = 0x000800
   //RAMLS6           : origin = 0x00B000, length = 0x000800
   //RAMLS7           : origin = 0x00B800, length = 0x000800
   //RAMLS8           : origin = 0x022000, length = 0x002000  // When configured as CLA program use the address 0x4000
   //RAMLS9           : origin = 0x024000, length = 0x002000  // When configured as CLA program use the address 0x6000

   // RAMLS8_CLA    : origin = 0x004000, length = 0x002000  // Use only if configured as CLA program memory
   // RAMLS9_CLA    : origin = 0x006000, length = 0x002000  // Use only if configured as CLA program memory

   // we do not utilize any RAMGS RAM, as this memory is used for page 0
   // by the "ram_lnk" configuration
   RAMGS0           : origin = 0x010000, length = 0x002000
   RAMGS1           : origin = 0x012000, length = 0x002000
   RAMGS2           : origin = 0x014000, length = 0x002000
   RAMGS3           : origin = 0x016000, length = 0x002000
   RAMGS4           : origin = 0x018000, length = 0x002000


   CPU1TOCPU2RAM    : origin = 0x03A000, length = 0x00037F
   SCOPE_FLAG_CPU1   : origin = 0x03A37F, length = 0x000001
   CPU1TOCPU2LINK    : origin = 0x03A380, length = 0x000080

   CPU2TOCPU1RAM    : origin = 0x03B000, length = 0x00037F
   SCOPE_FLAG_CPU2   : origin = 0x03B37F, length = 0x000001
   CPU2TOCPU1LINK    : origin = 0x03B380, length = 0x000080

   CLATOCPURAM      : origin = 0x001480,   length = 0x000080
   CPUTOCLARAM      : origin = 0x001500,   length = 0x000080
   CLATODMARAM      : origin = 0x001680,   length = 0x000080
   DMATOCLARAM      : origin = 0x001700,   length = 0x000080

   CANA_MSG_RAM     : origin = 0x049000, length = 0x000800
   CANB_MSG_RAM     : origin = 0x04B000, length = 0x000800

   /* Flash Banks (128 sectors each) */
   //FLASH     : origin = 0x080002, length = 0x1FFFE  // Can be mapped to either CPU1 or CPU2. User should comment/uncomment based on core selection
   FLASH_BANK0     : origin = 0x080002, length = 0x1FFFE  // Can be mapped to either CPU1 or CPU2. User should comment/uncomment based on core selection
   FLASH_BANK1     : origin = 0x0A0000, length = 0x20000  // Can be mapped to either CPU1 or CPU2. User should comment/uncomment based on core selection
   FLASH_BANK2     : origin = 0x0C0000, length = 0x20000  // Can be mapped to either CPU1 or CPU2. User should comment/uncomment based on core selection
   // FLASH_BANK3     : origin = 0x0E0000, length = 0x20000  // Can be mapped to either CPU1 or CPU2. User should comment/uncomment based on core selection
   // FLASH_BANK4     : origin = 0x100000, length = 0x20000  // Can be mapped to either CPU1 or CPU2. User should comment/uncomment based on core selection

   RESET            : origin = 0x3FFFC0, length = 0x000002
}

SECTIONS
{
   scope            : > RAMLS, ALIGN(2)
   scopeflag_local  : > SCOPE_FLAG_CPU1
   scopeflag_remote : > SCOPE_FLAG_CPU2

   cpu1to2_link_mem : > CPU1TOCPU2LINK
   cpu2to1_link_mem : > CPU2TOCPU1LINK
   step             : > FLASH_BANK0, ALIGN(8)
   dispatch         : > FLASH_BANK0, ALIGN(8)

   codestart        : > BEGIN
   .cinit           : > FLASH_BANK0, ALIGN(8)
   .text            : > FLASH_BANK0, ALIGN(8)
   .switch          : > FLASH_BANK0, ALIGN(8)
   .reset              : > RESET, TYPE = DSECT

   .stack           : > RAMM1 | RAMLS

#if defined(__TI_EABI__)
   .init_array      : > FLASH_BANK0, ALIGN(8)
   .const           : > FLASH_BANK0, ALIGN(8)
   .bss             : > RAMLS
   .bss:output      : > RAMLS
   .data            : > RAMLS
   .sysmem          : > RAMLS
#else
   .pinit           : > FLASH_BANK0, ALIGN(8)
   .econst          : > FLASH_BANK0, ALIGN(8)
   .ebss            : > RAMLS
   .esysmem         : > RAMLS
#endif

   MSGRAM_CPU1_TO_CPU2 > CPU1TOCPU2RAM, type=NOINIT
   MSGRAM_CPU2_TO_CPU1 > CPU2TOCPU1RAM, type=NOINIT

   #if defined(__TI_EABI__)
       .TI.ramfunc : {} LOAD = FLASH_BANK0,
                        RUN = RAMM0,
                        LOAD_START(RamfuncsLoadStart),
                        LOAD_SIZE(RamfuncsLoadSize),
                        LOAD_END(RamfuncsLoadEnd),
                        RUN_START(RamfuncsRunStart),
                        RUN_SIZE(RamfuncsRunSize),
                        RUN_END(RamfuncsRunEnd),
                        ALIGN(8)
   #else
       .TI.ramfunc : {} LOAD = FLASH_BANK0,
                        RUN = RAMM0,
                        LOAD_START(_RamfuncsLoadStart),
                        LOAD_SIZE(_RamfuncsLoadSize),
                        LOAD_END(_RamfuncsLoadEnd),
                        RUN_START(_RamfuncsRunStart),
                        RUN_SIZE(_RamfuncsRunSize),
                        RUN_END(_RamfuncsRunEnd),
                        ALIGN(8)
   #endif

}

