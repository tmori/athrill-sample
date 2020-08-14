ROOT_PATH 		:= ..
ATHRILL_ROOT	:= $(ROOT_PATH)/../../trunk/src
BUILD_PATH		:= .


TARGET	:= test_main.elf

CC		:= arm-none-eabi-gcc
LINK	:= arm-none-eabi-gcc

OBJS	:= start.o
OBJS	+= main.o


CFLAGS	:= -O2
CFLAGS	+= -gdwarf-2 
#CFLAGS  += -Wall -Wno-unused-label -Wpointer-arith    -fleading-underscore
CFLAGS  += -Wall -Wno-unused-label -Wpointer-arith   
CFLAGS  += -mcpu=cortex-a9 -marm   
CFLAGS  += -mno-thumb-interwork   
CFLAGS  += -marm -march=armv7-a  
CFLAGS  += -mfpu=neon -mfloat-abi=hard 


LDSCRIPT = $(BUILD_PATH)/arm.ld
LDFLAGS	:=  -mcpu=cortex-a9 -marm
LDFLAGS	+= -mno-thumb-interwork 
LDFLAGS	+= -marm -march=armv7-a 
LDFLAGS	+= -mfpu=neon -mfloat-abi=hard 
LDFLAGS	+= -mno-unaligned-access   -fleading-underscore
LDFLAGS	+= -Wl,--gc-sections 
LDFLAGS	+= -nostartfiles 
#LDFLAGS	+= --specs=nano.specs 
LDFLAGS	+= -u _printf_float 
LDFLAGS	+= -u _scanf_float -Wl,-Map=test.map,--cref 
LDFLAGS	+= -N -mcpu=cortex-a9
LDFLAGS += -nostdlib 
#LDFLAGS += -L/usr/lib/gcc/arm-none-eabi/4.9.3/fpu/
#LDFLAGS += -L/usr/lib/arm-none-eabi/newlib/fpu/

LDFLAGS :=   -mcpu=cortex-a9 -marm -mno-thumb-interwork 
LDFLAGS += -marm -march=armv7-a -mfpu=vfp3 -mfloat-abi=hard -mno-unaligned-access  
LDFLAGS += -L/usr/lib/gcc/arm-none-eabi/4.9.3/fpu -L/usr/lib/arm-none-eabi/newlib/fpu 
LDFLAGS += -Wl,--gc-sections -nostartfiles  -u _printf_float -u _scanf_float 
LDFLAGS += -Wl,-Map=GR-PEACH_mbed.map,--cref -mlittle-endian 
LDFLAGS += -nostartfiles
#LDFLAGS += --specs=nano.specs 
#LDFLAGS += -e start 
LDFLAGS += -L /usr/lib/arm-none-eabi/newlib/fpu/ -N -mcpu=cortex-a9  -L. 
LDFLAGS += -lm -lc -lgcc -lnosys
LDFLAGS += -T $(LDSCRIPT)

TEST_PATH := .

IFLAGS	:= -I.
IFLAGS	+= -I$(ROOT_PATH)/common
IFLAGS	+= -I$(CPU_CONFIG_PATH)

VPATH	:= $(ROOT_PATH)/common
VPATH	+= $(TEST_PATH)

LIBS := 


%.o: %.S
	$(CC) -c $(IFLAGS) $(CFLAGS) $<

%.o: %.c
	$(CC) -c $(IFLAGS) $(CFLAGS) $<

all: $(OBJS)
	$(LINK) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) -lc -lgcc -lnosys -Wl,-Map,$(TARGET).map $(LIBS)
	arm-none-eabi-objdump -d $(TARGET) > $(TARGET).dump



clean:
	rm -f $(OBJS) $(TARGET) *.map
