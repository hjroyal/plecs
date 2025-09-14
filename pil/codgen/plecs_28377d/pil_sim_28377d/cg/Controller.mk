#   Copyright (c) 2019 by Plexim GmbH
#   All rights reserved.
#
#   A free license is granted to anyone to use this software for any legal
#   non safety-critical purpose, including commercial applications, provided
#   that:
#   1) IT IS NOT USED TO DIRECTLY OR INDIRECTLY COMPETE WITH PLEXIM, and
#   2) THIS COPYRIGHT NOTICE IS PRESERVED in its entirety.
#
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
#   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#   SOFTWARE.

include Controller_sources.mk

TSP_VERSION=1.10.1
TARGET_ROOT=C:/app/modeling/plecs/C2000TSP/tsp_ti_c2000/ccs/2837x
TOOLS_PATH=C:/Users/hjroyal/AppData/Local/Programs/PLECS C2000 TSP/cgt/ti-cgt-c2000_22.6.1.LTS
BIN_DIR=./output_TI2837x_cpu1
OUT_NAME=Controller
MAKEFILE=Controller.mk
LINKFILE=Controller.lkf
INSTALL_DIR=./
BASE_NAME=Controller
CPU_POSTFIX=
CCXML_FILE=C:/Users/hjroyal/Desktop/pebsa/4_PlecsModel/plecs_28377d/28379/xds100v3.ccxml
FLASH_EXE=C:/ti/uniflash_9.0.0/dslite.bat
CORE_POST_FIX=_cpu1
CPU_NUM=1
CORE_FLASH_OPT=

##############################################################

C_SOURCE_FILES=\
$(BASE_NAME)$(CPU_POSTFIX)_hal.c \
$(BASE_NAME)$(CPU_POSTFIX)_main.c\
dispatcher.c \
power.c \
F2837xD_Adc.c\
F2837xD_DefaultISR.c\
F2837xD_DevInit.c\
F2837xD_GlobalVariableDefs.c\
F2837xD_Gpio.c\
dio_2837x.c \
sci_2837x.c \
epwm_type4.c \
ain_type4.c \
dac_type1.c \
qep.c \
cap_2837x.c \
dcan_type0.c \
spi_type2.c \
plx_ipc.c \
csv.c \
sdfm.c \
stack_monitor.c \

GENERATED_HEADERS=

CLA_SOURCE_FILES=\
$(BASE_NAME)$(CPU_POSTFIX)_cla.cla

ASM_SOURCE_FILES=\
F2837xD_CodeStartBranch.asm\
F2837xD_usDelay.asm\
dispatcher_asm.asm

##############################################################
space:=
space+=
# for MacOS - NOTE: not tolerant to leading spaces or already escaped spaces '\ '
EscapeSpaces=$(subst $(space),\$(space),$(1))
FlipSlashesBack=$(subst /,\,$(1))

ifeq ($(OS),Windows_NT)
# Windows
SHELL := cmd.exe
FixPath=$(call FlipSlashesBack,$(1))
ClearFile=del /F /Q "$(call FlipSlashesBack,$(1))"
ClearDir=del /F /Q "$(call FlipSlashesBack,$(1))\*.*"
MoveFile=move /Y "$(call FlipSlashesBack,$(1))" "$(call FlipSlashesBack,$(2))"
CopyFile=copy /Y "$(call FlipSlashesBack,$(1))" "$(call FlipSlashesBack,$(2))"
TouchFile=type nul > "$(call FlipSlashesBack,$(1))"

else
# Linux style
FixPath = $(1)
ClearFile=rm -Rf $(call EscapeSpaces,$(1))
ClearDir=rm -Rf $(call EscapeSpaces,$(1))/*
MoveFile=mv $(call EscapeSpaces,$(1)) $(call EscapeSpaces,$(2))
CopyFile=cp $(call EscapeSpaces,$(1)) $(call EscapeSpaces,$(2))
TouchFile=touch $(call EscapeSpaces,$(1))

endif 

CGT_EXE_PATH=$(TOOLS_PATH)/bin
CGT_LIB_PATH=$(TOOLS_PATH)/lib
CGT_INC_PATH=$(TOOLS_PATH)/include

BIN_DIR_OS=$(call FixPath,$(BIN_DIR))

# compiler / assembler
C_OPTIONS=\
-D_PLEXIM_\
-DCPU$(CPU_NUM)\
-DBASE_NAME=$(BASE_NAME)\
-DEXTERNAL_MODE=1\
-fr$(BIN_DIR)\
-fs$(BIN_DIR)\
-v28 \
-ml \
-mt \
--float_support=fpu32 \
--fp_mode=relaxed \
--float_operations_allowed=all \
-O0 \
--include_path="$(TARGET_ROOT)/app$(CORE_POST_FIX)" \
--include_path="$(TARGET_ROOT)/../pil" \
--include_path="$(TARGET_ROOT)/../shrd" \
--include_path="$(TARGET_ROOT)/../inc" \
--include_path="$(TARGET_ROOT)/inc_impl" \
--include_path="$(TARGET_ROOT)/tiinc" \
--include_path="$(TARGET_ROOT)/tiinc/driverlib" \
--include_path="$(TOOLS_PATH)/include" \
--include_path="./" \
-g \
--symdebug:dwarf_version=3\
--abi=eabi \
--cla_support=cla1 \
--tmu_support=tmu0 \
--c11 \
--printf_support=full \
\

L_OPTIONS=$(LINKFILE)

C_OBJFILES=$(patsubst %.c, $(BIN_DIR)/%.obj, $(C_SOURCE_FILES)) \
  $(patsubst %.c, $(BIN_DIR)/%.obj, $(SOURCE_FILES)) \
  $(patsubst %.cla, $(BIN_DIR)/%.obj, $(CLA_SOURCE_FILES))
  
ASM_OBJFILES=$(patsubst %.asm, $(BIN_DIR)/%.obj, $(ASM_SOURCE_FILES))

OBJFILES=$(C_OBJFILES) $(ASM_OBJFILES)

DEPENDFILES=$(patsubst %.c, $(BIN_DIR)/%.d, $(C_SOURCE_FILES)) $(patsubst %.cla, $(BIN_DIR)/%.d, $(CLA_SOURCE_FILES))

L_DEPENDENCIES=\
	$(LINKFILE) \
	$(call EscapeSpaces,$(TARGET_ROOT))/../pil/pil_c28_fpu32_eabi.lib \
	$(call EscapeSpaces,$(TARGET_ROOT))/tisrc/driverlib/plx_driverlib_fpu32_eabi.lib

# make all variables available to sub-makes
export

# Top level 
##########################################################################
all: $(MAKEFILE).uptodate
ifneq ($(wildcard $(BIN_DIR_OS)),  $(BIN_DIR_OS))
	"$(MAKE)" -f $(MAKEFILE) clean
endif
	"$(MAKE)" -f $(MAKEFILE) $(BIN_DIR)/$(OUT_NAME).elf
	
# Download
##########################################################################rm C
$(BIN_DIR)/$(BASE_NAME).ccxml:	$(call EscapeSpaces,$(CCXML_FILE)) $(MAKEFILE)
							$(call CopyFile,$(CCXML_FILE),$(BIN_DIR)/$(BASE_NAME).ccxml)

ifneq ($(and $(FLASH_EXE),$(CCXML_FILE)),)
download: $(BIN_DIR)/$(OUT_NAME).out $(BIN_DIR)/$(BASE_NAME).ccxml
	"$(FLASH_EXE)" --flash $(CORE_FLASH_OPT) --config=$(call FixPath,$(BIN_DIR)/$(BASE_NAME).ccxml) $(call FixPath,$(BIN_DIR)/$(OUT_NAME).out) -u
else
download:
	@echo "Download not configured."
endif 
 
# Linker
##########################################################################
ifeq ($(CPU_POSTFIX), )
$(BIN_DIR)/$(OUT_NAME).elf:  $(BIN_DIR)/$(OUT_NAME).out
						$(call CopyFile,$(BIN_DIR)/$(OUT_NAME).out,$(INSTALL_DIR)/$(OUT_NAME).elf)
else
$(BIN_DIR)/$(OUT_NAME).elf:  $(BIN_DIR)/$(OUT_NAME).out
						$(call CopyFile,$(BIN_DIR)/$(OUT_NAME).out,$(INSTALL_DIR)/$(OUT_NAME)$(CPU_POSTFIX).elf)
endif

$(BIN_DIR)/$(OUT_NAME).out:  $(OBJFILES) $(L_DEPENDENCIES)
						"$(CGT_EXE_PATH)"/cl2000 -z -i"$(CGT_LIB_PATH)" $(OBJFILES) $(L_OPTIONS)

# Generated dependencies
##########################################################################
#-include ($(DEPENDFILES)) -- disabled due to CGT bug: https://sir.ext.ti.com/jira/browse/EXT_EP-9771

# Force clean when makefile changes
##########################################################################
$(MAKEFILE).uptodate: $(MAKEFILE)
	"$(MAKE)" -f $(MAKEFILE) clean
	$(call TouchFile,$(MAKEFILE).uptodate)

# Explicit rules (we must use explicit rules to allow spaces in $(TARGET_ROOT))
##########################################################################
$(BIN_DIR)/dispatcher.obj:	$(call EscapeSpaces,$(TARGET_ROOT))/../shrd/dispatcher.c
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) --preproc_dependency=$(patsubst %.obj,%.d,$@) "$<"
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) "$<"

$(BIN_DIR)/dispatcher_asm.obj:	$(call EscapeSpaces,$(TARGET_ROOT))/../shrd/dispatcher_asm.asm
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) --preproc_dependency=$(patsubst %.obj,%.d,$@) "$<"
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) "$<"

$(BIN_DIR)/power.obj:	$(call EscapeSpaces,$(TARGET_ROOT))/../shrd/power.c
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) --preproc_dependency=$(patsubst %.obj,%.d,$@) "$<"
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) "$<"

$(BIN_DIR)/F2837xD_Adc.obj:	$(call EscapeSpaces,$(TARGET_ROOT))/tisrc/F2837xD_Adc.c
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) --preproc_dependency=$(patsubst %.obj,%.d,$@) "$<"
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) "$<"

$(BIN_DIR)/F2837xD_DefaultISR.obj:	$(call EscapeSpaces,$(TARGET_ROOT))/tisrc/F2837xD_DefaultISR.c
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) --preproc_dependency=$(patsubst %.obj,%.d,$@) "$<"
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) "$<"

$(BIN_DIR)/F2837xD_GlobalVariableDefs.obj:	$(call EscapeSpaces,$(TARGET_ROOT))/tisrc/F2837xD_GlobalVariableDefs.c
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) --preproc_dependency=$(patsubst %.obj,%.d,$@) "$<"
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) "$<"

$(BIN_DIR)/F2837xD_DevInit.obj:	$(call EscapeSpaces,$(TARGET_ROOT))/tisrc/F2837xD_DevInit.c
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) --preproc_dependency=$(patsubst %.obj,%.d,$@) "$<"
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) "$<"

$(BIN_DIR)/F2837xD_Gpio.obj:	$(call EscapeSpaces,$(TARGET_ROOT))/tisrc/F2837xD_Gpio.c
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) --preproc_dependency=$(patsubst %.obj,%.d,$@) "$<"
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) "$<"

$(BIN_DIR)/F2837xD_CodeStartBranch.obj:	$(call EscapeSpaces,$(TARGET_ROOT))/tisrc/F2837xD_CodeStartBranch.asm
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) --preproc_dependency=$(patsubst %.obj,%.d,$@) "$<"
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) "$<"

$(BIN_DIR)/F2837xD_usDelay.obj:	$(call EscapeSpaces,$(TARGET_ROOT))/tisrc/F2837xD_usDelay.asm
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) --preproc_dependency=$(patsubst %.obj,%.d,$@) "$<"
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) "$<"

$(BIN_DIR)/epwm_type4.obj:	$(call EscapeSpaces,$(TARGET_ROOT))/../shrd/epwm_type4.c
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) --preproc_dependency=$(patsubst %.obj,%.d,$@) "$<"
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) "$<"

$(BIN_DIR)/ain_type4.obj:	$(call EscapeSpaces,$(TARGET_ROOT))/../shrd/ain_type4.c
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) --preproc_dependency=$(patsubst %.obj,%.d,$@) "$<"
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) "$<"

$(BIN_DIR)/dio_2837x.obj:	$(call EscapeSpaces,$(TARGET_ROOT))/src/dio_2837x.c
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) --preproc_dependency=$(patsubst %.obj,%.d,$@) "$<"
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) "$<"

$(BIN_DIR)/qep.obj:	$(call EscapeSpaces,$(TARGET_ROOT))/../shrd/qep.c
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) --preproc_dependency=$(patsubst %.obj,%.d,$@) "$<"
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) "$<"

$(BIN_DIR)/sci_2837x.obj:	$(call EscapeSpaces,$(TARGET_ROOT))/src/sci_2837x.c
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) --preproc_dependency=$(patsubst %.obj,%.d,$@) "$<"
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) "$<"

$(BIN_DIR)/spi_type2.obj:	$(call EscapeSpaces,$(TARGET_ROOT))/../shrd/spi_type2.c
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) --preproc_dependency=$(patsubst %.obj,%.d,$@) "$<"
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) "$<"

$(BIN_DIR)/dac_type1.obj:	$(call EscapeSpaces,$(TARGET_ROOT))/../shrd/dac_type1.c
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) --preproc_dependency=$(patsubst %.obj,%.d,$@) "$<"
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) "$<"
						
$(BIN_DIR)/cap_2837x.obj:	$(call EscapeSpaces,$(TARGET_ROOT))/src/cap_2837x.c
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) --preproc_dependency=$(patsubst %.obj,%.d,$@) "$<"
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) "$<"
						
$(BIN_DIR)/dcan_type0.obj:	$(call EscapeSpaces,$(TARGET_ROOT))/../shrd/dcan_type0.c
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) --preproc_dependency=$(patsubst %.obj,%.d,$@) "$<"
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) "$<"

$(BIN_DIR)/plx_ipc.obj:	$(call EscapeSpaces,$(TARGET_ROOT))/../shrd/plx_ipc.c
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) --preproc_dependency=$(patsubst %.obj,%.d,$@) "$<"
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) "$<"

$(BIN_DIR)/csv.obj:	$(call EscapeSpaces,$(TARGET_ROOT))/../shrd/csv.c
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) --preproc_dependency=$(patsubst %.obj,%.d,$@) "$<"
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) "$<"

$(BIN_DIR)/sdfm.obj:	$(call EscapeSpaces,$(TARGET_ROOT))/../shrd/sdfm.c
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) --preproc_dependency=$(patsubst %.obj,%.d,$@) "$<"
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) "$<"

$(BIN_DIR)/stack_monitor.obj:	$(call EscapeSpaces,$(TARGET_ROOT))/../shrd/stack_monitor.c
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) --preproc_dependency=$(patsubst %.obj,%.d,$@) "$<"
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) "$<"

# Implicit Rules for generated files
##########################################################################
$(BIN_DIR)/%.obj:		%.c	$(GENERATED_HEADERS) $(MAKEFILE)
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) --preproc_dependency=$(patsubst %.obj,%.d,$@) "$<"
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) $<

$(BIN_DIR)/%.obj:		%.cla	$(MAKEFILE)
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) --preproc_dependency=$(patsubst %.obj,%.d,$@) "$<"
						"$(CGT_EXE_PATH)"/cl2000 $(C_OPTIONS) $<

##########################################################################

clean:
ifeq ($(wildcard $(BIN_DIR_OS)),  $(BIN_DIR_OS))
		$(call ClearDir,$(BIN_DIR_OS))
else
		mkdir $(BIN_DIR_OS)
endif
