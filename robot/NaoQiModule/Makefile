VPATH = ../
ROBOT_DIR=../

EXTENSION=.cpp

NO_COLOR=\033[0m
GREEN_COLOR=\033[32m
BLUE_COLOR=\033[34m
RED_COLOR=\033[31m
YELLOW_COLOR=\033[33m
MAGENTA_COLOR=\033[35m
CYAN_COLOR=\033[36m

SOURCES?=$(shell find ./src -type f \( -name *.cc -or -name *.cpp \) )  
SOURCES+=$(shell find $(ROBOT_DIR)/shared -type f \( -name *.cc -or -name *.cpp \) )  
SOURCES+=$(shell find $(ROBOT_DIR)/Agent -type f \( -name *.cc -or -name *.cpp \) )  
SOURCES+=$(shell find $(ROBOT_DIR)/Factory -type f \( -name *.cc -or -name *.cpp \) )  
SOURCES+=$(shell find $(ROBOT_DIR)/Behaviors -type f \( -name *.cc -or -name *.cpp \) )  
SOURCES+=$(shell find $(ROBOT_DIR)/Comm -type f \( -name *.cc -or -name *.cpp \) )  
SOURCES+=$(shell find $(ROBOT_DIR)/GameController -type f \( -name *.cc -or -name *.cpp \) )  
SOURCES+=$(shell find $(ROBOT_DIR)/Log -type f \( -name *.cc -or -name *.cpp \) )  
SOURCES+=$(shell find $(ROBOT_DIR)/Localization -type f \( -name *.cc -or -name *.cpp \) )  
SOURCES+=$(shell find $(ROBOT_DIR)/Motion -type f \( -name *.cc -or -name *.cpp \) )  
SOURCES+=$(shell find $(ROBOT_DIR)/Vision -type f \( -name *.cc -or -name *.cpp \) )  
SOURCES+=$(shell find $(ROBOT_DIR)/WorldModel -type f \( -name *.cc -or -name *.cpp \) )  

SRCS=$(patsubst $(ROBOT_DIR)/%, %, $(SOURCES) )

HEADERS?=$(shell find ./src -type f -name *.h )
HEADERS+=$(shell find ../ -type f -name *.h )

CFLAGS= --sysroot $(CTC_DIR)/staging/geode-linux/ -Wall -g -fPIC -march=geode

LFLAGS=--sysroot $(CTC_DIR)/staging/geode-linux/ \
	-lgcc -L$(CTC_DIR)/cross/geode/i586-linux/lib/ -lc -lstdc++ -ldl -lrt -lpthread -lz \
	-Wl,--sysroot $(CTC_DIR)/staging/geode-linux/


INCLUDES=  \
	-I$(CTC_DIR)/staging/geode-linux/usr/include/ \
	-I$(CTC_DIR)/staging/geode-linux/usr/include/libthread/ \
	-I$(CTC_DIR)/staging/geode-linux/usr/include/alvalue/ \
	-I$(CTC_DIR)/staging/geode-linux/usr/include/alfactory/ \
	-I$(CTC_DIR)/cross/geode/lib/gcc/i586-linux/4.3.3/include/ \
	-I$(CTC_DIR)/cross/geode/i586-linux/include/c++/ \
	-I$(CTC_DIR)/cross/geode/i586-linux/include/c++/i586-linux/ \
	-I$(CTC_DIR)/staging/geode-linux/usr/include/c++/ \
	-I$(CTC_DIR)/staging/geode-linux/usr/include/c++/i486-linux/ \
	-I$(CTC_DIR)/staging/geode-linux/usr/include/python2.5/ \
	-I$(AL_DIR)/include/alcommon \
	-I$(AL_DIR)/include/altools \
	-I$(AL_DIR)/include/alcore \
	-I$(AL_DIR)/include/alproxies \
	-I$(ROBOT_DIR)

LIBS= \
	-rdynamic -L$(AL_DIR)/lib \
	-L$(CTC_DIR)/staging/geode-linux/usr/lib \
	-L$(AL_DIR)/lib \
	-L$(AL_DIR)/lib/static \
	-Wl,-Bstatic -ltinyxml -Wl,-Bdynamic -llibcore -lalcommon -lalvalue -lpthread -llibthread -Wl,-Bstatic -Wl,-Bdynamic -lboost_signals-mt -lboost_serialization-mt -lpython2.6 \
	-Wl,-rpath,$(AL_DIR)/lib:$(CTC_DIR)/staging/geode-linux/usr/lib:$(AL_DIR)/lib

CXX=${CTC_DIR}/cross/geode/bin/i586-linux-g++

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

LIB=bin/libcmurfs.so

$(LIB): $(OBJS_OUT) $(DEPENDS)
	@rm -rf ./bin
	@mkdir ./bin
	@echo "$(BLUE_COLOR)Linking ...$(NO_COLOR)"
	@$(CXX) -shared -Wl,--no-undefined $(LFLAGS) -o $(LIB) $(OBJS_OUT) $(LIBS)
	@echo "$(CYAN_COLOR)=========================================$(NO_COLOR)"
	@echo -n "$(CYAN_COLOR)====== $(NO_COLOR)"
	@echo -n "$(RED_COLOR)CMurfs NaoQi Module Created $(NO_COLOR)"
	@echo    "$(CYAN_COLOR)====== $(NO_COLOR)"
	@echo "$(CYAN_COLOR)=========================================$(NO_COLOR)"

all: $(LIB)

clean: 
	@echo "$(BLUE_COLOR)Cleaning up build/ and bin/ ...$(NO_COLOR)"
	@rm -rf ./bin
	@rm -rf ./build

localinstall: $(LIB)
	cp $(LIB) $(AL_DIR)/lib/naoqi/

install: $(LIB)
	@echo "Copying $(BLUE_COLOR)$(LIB) $(NO_COLOR)to $(RED_COLOR)$(NAO_IP)$(NO_COLOR)" 
	@scp $(LIB) nao@$(NAO_IP):/home/nao/naoqi/lib/naoqi/

artemis: $(LIB)
	@echo "Copying $(BLUE_COLOR)$(LIB) $(NO_COLOR)to $(RED_COLOR)artemis$(NO_COLOR)" 
	@scp $(LIB) nao@artemis:/home/nao/naoqi/lib/naoqi/

apollo: $(LIB)
	@echo "Copying $(BLUE_COLOR)$(LIB) $(NO_COLOR)to $(RED_COLOR)apollo$(NO_COLOR)" 
	@scp $(LIB) nao@apollo:/home/nao/naoqi/lib/naoqi/

ares: $(LIB)
	@echo "Copying $(BLUE_COLOR)$(LIB) $(NO_COLOR)to $(RED_COLOR)ares$(NO_COLOR)" 
	@scp $(LIB) nao@ares:/home/nao/naoqi/lib/naoqi/

athena: $(LIB)
	@echo "Copying $(BLUE_COLOR)$(LIB) $(NO_COLOR)to $(RED_COLOR)athena$(NO_COLOR)" 
	@scp $(LIB) nao@athena:/home/nao/naoqi/lib/naoqi/

-include $(DEPS_OUT)
