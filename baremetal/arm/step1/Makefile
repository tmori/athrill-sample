ROOT_PATH 		:= ..
ATHRILL_ROOT	:= $(ROOT_PATH)/../../trunk/src
BUILD_PATH		:= .


TARGET	:= test_main.elf

CC		:= arm-none-eabi-gcc
LINK	:= arm-none-eabi-gcc

OBJS	:= start.o
OBJS	+= main.o


CFLAGS	:= -O0 
CFLAGS	+= -gdwarf-2 
CFLAGS  += -Wall -Wno-unused-label -Wpointer-arith    -fleading-underscore


LDSCRIPT = $(BUILD_PATH)/arm.ld
LDFLAGS	:=  -mcpu=cortex-a9 -mthumb 
LDFLAGS	+= -mthumb-interwork 
LDFLAGS	+= -marm -march=armv7-a 
LDFLAGS	+= -mfpu=vfpv3 -mfloat-abi=hard 
LDFLAGS	+= -mno-unaligned-access   -fleading-underscore
LDFLAGS	+= -Wl,--gc-sections 
LDFLAGS	+= -nostartfiles 
#LDFLAGS	+= --specs=nano.specs 
LDFLAGS	+= -u _printf_float 
LDFLAGS	+= -u _scanf_float -Wl,-Map=test.map,--cref 
LDFLAGS	+= -N -mcpu=cortex-a9  -L. 
LDFLAGS += -nostdlib 
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
	$(LINK) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) -Wl,-Map,$(TARGET).map $(LIBS)
	arm-none-eabi-objdump -d $(TARGET) > $(TARGET).dump



clean:
	rm -f $(OBJS) $(TARGET) *.map
