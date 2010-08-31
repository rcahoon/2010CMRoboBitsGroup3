VPATH = ../../robot/
ROBOT_DIR=../../robot/

EXTENSION=.cpp

NO_COLOR=\033[0m
GREEN_COLOR=\033[32m
BLUE_COLOR=\033[34m
RED_COLOR=\033[31m
YELLOW_COLOR=\033[33m
MAGENTA_COLOR=\033[35m
CYAN_COLOR=\033[36m

#NO_COLOR='\e[0m'
#GREEN_COLOR='\e[1;32m'
#BLUE_COLOR='\e[1;34m'
#RED_COLOR='\e[1;31m'
#YELLOW_COLOR='\e[1;33m'
#MAGENTA_COLOR='\e[1;35m'
#CYAN_COLOR='\e[1;36m'

SOURCES?=$(shell find ./src -type f \( -name *.cc -or -name *.cpp \) )  
SOURCES+=$(shell find $(ROBOT_DIR)/shared -type f \( -name *.cc -or -name *.cpp \) )  
SOURCES+=$(shell find $(ROBOT_DIR)/Agent -type f \( -name *.cc -or -name *.cpp \) )  
SOURCES+=$(shell find $(ROBOT_DIR)/Factory -type f \( -name *.cc -or -name *.cpp \) )  
SOURCES+=$(shell find $(ROBOT_DIR)/Behaviors -type f \( -name *.cc -or -name *.cpp \) )  
SOURCES+=$(shell find $(ROBOT_DIR)/Comm -type f \( -name *.cc -or -name *.cpp \) )  
SOURCES+=$(shell find $(ROBOT_DIR)/GameController -type f \( -name *.cc -or -name *.cpp \) )  
SOURCES+=$(shell find $(ROBOT_DIR)/Log -type f \( -name *.cc -or -name *.cpp \) )  
SOURCES+=$(shell find $(ROBOT_DIR)/Localization -type f \( -name *.cc -or -name *.cpp \) )  
SOURCES+=$(shell find $(ROBOT_DIR)/Motion/MotionLib -type f \( -name *.cc -or -name *.cpp \) )  
SOURCES+=$(shell find $(ROBOT_DIR)/Vision -type f \( -name *.cc -or -name *.cpp \) )  
SOURCES+=$(shell find $(ROBOT_DIR)/WorldModel -type f \( -name *.cc -or -name *.cpp \) )  

SRCS=$(patsubst $(ROBOT_DIR)/%, %, $(SOURCES) )

HEADERS?=$(shell find ./src -type f -name *.h )
HEADERS+=$(shell find ../ -type f -name *.h )

CFLAGS=-Wall -g -O3
#CFLAGS= --sysroot $(CTC_DIR)/staging/geode-linux/ -Wall -g -fPIC -march=geode -O3

LFLAGS=


INCLUDES=-I$(ROBOT_DIR)

LIBS= \
	-Wl,-Bstatic -lz -Wl,-Bdynamic -lpthread -Wl,-Bstatic -Wl,-Bdynamic -lboost_serialization-mt -lpython2.6 \
	-Wl,-rpath

CXX=g++

OBJS = $(patsubst %$(EXTENSION),%.o,$(filter %$(EXTENSION),$(SRCS)))
OBJS_OUT = $(addprefix build/, $(OBJS))

DEPS = $(patsubst %$(EXTENSION),%.d,$(filter %$(EXTENSION),$(SRCS)))
DEPS_OUT = $(addprefix build/, $(DEPS))

build/%.o: %$(EXTENSION) $(DEPENDS)
	@mkdir -p $(dir $@)
	@echo "$(BLUE_COLOR)Compiling: $(GREEN_COLOR)$(patsubst build/%.o,%$(EXTENSION),$@)$(NO_COLOR)"
	@$(CXX) -MD $(CFLAGS) $(INCLUDES) -c -o $@ $< $(DEFINES)
	@sed -i 's,\(build/$*\.o\)[ :]*\(.*\),$@ : $$\(wildcard \2\)\n\1 : \2,g' build/$*.d

DEPENDS=

TARGET=./logRunner

$(TARGET): $(OBJS_OUT) $(DEPENDS)
	@rm -rf ./bin
	@mkdir ./bin
	@echo "$(BLUE_COLOR)Linking ...$(NO_COLOR)"
	@$(CXX) -o $@ $^ $(LFLAGS) $(LIBS)
	@echo "$(CYAN_COLOR)=========================================$(NO_COLOR)"
	@echo -n "$(CYAN_COLOR)====== $(NO_COLOR)"
	@echo -n "$(RED_COLOR)CMurfs LogRunner is Created $(NO_COLOR)"
	@echo "$(CYAN_COLOR)====== $(NO_COLOR)"
	@echo "$(CYAN_COLOR)=========================================$(NO_COLOR)"

all: $(TARGET)

clean:
	@echo "$(BLUE_COLOR)Cleaning up build/ and ./logRunner ...$(NO_COLOR)"
	@rm -rf ./logRunner
	@rm -rf ./build

-include $(DEPS_OUT)
