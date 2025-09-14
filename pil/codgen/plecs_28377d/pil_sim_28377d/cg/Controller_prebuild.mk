TOOLS_PATH=C:/Users/hjroyal/AppData/Local/Programs/PLECS C2000 TSP/cgt/ti-cgt-c2000_22.6.1.LTS

CGT_EXE_PATH=$(TOOLS_PATH)/bin

all:
	"$(CGT_EXE_PATH)/cl2000" -version > ./TI2837x_cpu1_cgt_version.txt