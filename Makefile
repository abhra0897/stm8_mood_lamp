TARGET=main

# toolchain
TOOLCHAIN_PREFIX=sdcc

CC=$(TOOLCHAIN_PREFIX)

PROGRAMMER = stlinkv2
DEVICE = stm8s003f3
F_CPU = 16000000
MCU = stm8
MCFLAGS = -m$(MCU)


################## User Sources ####################
SRCS = src/main.c
SRCS += src/ws2812_driver.c
SRCS += src/buttons.c
SRCS += src/timer.c
SRCS += src/color_utils.c
SRCS += src/flash.c

################## Objects ####################
OBJS = build/main.rel
OBJS += build/ws2812_driver.rel
OBJS += build/buttons.rel
OBJS += build/timer.rel
OBJS += build/color_utils.rel
OBJS += build/flash.rel

################## Includes ########################
INCLS = -I. -Iinc

################## Libs ########################
LDFLAGS =	-lstm8 --out-fmt-ihx
LDFLAGS += $(MCFLAGS)

################ Compiler Flags ######################
CFLAGS = --std-sdcc11
CFLAGS += $(MCFLAGS)
CFLAGS += -DF_CPU=$(F_CPU)UL
CFLAGS += --stack-auto --noinduction --use-non-free 

############# CFLAGS for Optimization ##################



################### Recipe to make all (build and burn) ####################
.PHONY: all
all: build

################### Recipe to build ####################
.PHONY: build
build: build/$(TARGET).hex


################### Recipe to burn ####################
.PHONY: burn
burn:
	@echo "[Flashing] build/$(TARGET).hex"
	@stm8flash -c $(PROGRAMMER) -p $(DEVICE) -w build/$(TARGET).hex
		

################### Recipe to make .hex ####################
build/$(TARGET).hex: $(OBJS)
	@echo "[Linking] $@"
	@$(CC) $(LDFLAGS)  $^ -o $@


################### Recipe to make .rel ####################
build/%.rel: src/%.c
	@echo "[Building] $@"
	@$(CC) $(INCLS) $(CFLAGS) -c -o $@ $<


################### Recipe to get size ####################
size:
	@$(OBJCOPY) -I ihex --output-target=binary build/$(TARGET).hex
	@echo "----------"
	@echo "Image size:"
	@stat -L -c %s build/$(TARGET).hex


################### Recipe to clean all ####################
.PHONY: clean
clean:
	@echo "[Cleaning]"
	@rm -rf build/*