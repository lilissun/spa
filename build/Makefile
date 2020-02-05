VERSION=v0.1.4

CXXFLAGS=-g -Os -std=gnu++11 -Wall
CVVFLAGS=-g -Os -std=gnu99 
LDFLAGS=-Os -std=gnu++11

ifeq ($(OS),Windows_NT)
    ARCH=$(PROCESSOR_ARCHITECTURE)
    CXX=g++
    CVV=gcc
    LDFLAGS+=-static
else
    OS=$(shell uname -s)
    ARCH=$(shell uname -m)
    ifeq ($(strip $(OS)),Linux)
        CXX=clang++
        CVV=clang
    else
        # Applies for Darwin
        CXX=g++
        CVV=gcc
    endif
endif

ROOT = ..
TARGET_DIR = bin/$(VERSION)/$(OS)-$(ARCH)-$(VERSION)
EXTERNAL = $(ROOT)/../external/$(OS)-$(ARCH)
EXTERNAL_HEADER_DIR = $(EXTERNAL)/include
EXTERNAL_LIB_DIR = $(EXTERNAL)/lib
LIBS = -lgmpxx -lgmp -lppl -ldl 

COM_TARGET=libcommon.dylib
COM_SRCCXX+=$(wildcard $(ROOT)/common/*.cpp)
COM_OBJECT=$(COM_SRCCVV:%.c=%.o) $(COM_SRCCXX:%.cpp=%.o)

EXP_TARGET=libexplicit.dylib
EXP_SRCCXX+=$(wildcard $(ROOT)/explicit/*.cpp) 
EXP_OBJECT=$(EXP_SRCCVV:%.c=%.o) $(EXP_SRCCXX:%.cpp=%.o)

SYM_TARGET=libsymbolic.dylib
SYM_SRCCXX+=$(wildcard $(ROOT)/symbolic/*.cpp)
SYM_OBJECT=$(SYM_SRCCVV:%.c=%.o) $(SYM_SRCCXX:%.cpp=%.o)

PPL_TARGET=libppl.dylib
PPL_SRCCXX+=$(wildcard $(ROOT)/ppl/*.cpp)
PPL_OBJECT=$(PPL_SRCCVV:%.c=%.o) $(PPL_SRCCXX:%.cpp=%.o)

TIM_TARGET=libtime.dylib
TIM_SRCCXX+=$(wildcard $(ROOT)/time/*.cpp) 
TIM_OBJECT=$(TIM_SRCCVV:%.c=%.o) $(TIM_SRCCXX:%.cpp=%.o)

CAL_TARGET=libcalculus.dylib
CAL_SRCCXX+=$(wildcard $(ROOT)/calculus/*.cpp) 
CAL_OBJECT=$(CAL_SRCCVV:%.c=%.o) $(CAL_SRCCXX:%.cpp=%.o)

SPA_TARGET=spa
SPA_SRCCXX+=$(wildcard $(ROOT)/spa/*.cpp)
SPA_OBJECT=$(SPA_SRCCVV:%.c=%.o) $(SPA_SRCCXX:%.cpp=%.o)

all: makedir $(SPA_TARGET) movelib

makedir:
	@mkdir -p $(TARGET_DIR)

movelib: 
	@cp $(EXTERNAL_LIB_DIR)/libgmpxx.4.dylib $(TARGET_DIR)
	@cp $(EXTERNAL_LIB_DIR)/libgmp.10.dylib $(TARGET_DIR)
	@cp $(EXTERNAL_LIB_DIR)/libppl.13.dylib $(TARGET_DIR)
	@mv $(COM_TARGET) $(TARGET_DIR)
	@mv $(EXP_TARGET) $(TARGET_DIR)
	@mv $(SYM_TARGET) $(TARGET_DIR)
	@mv $(PPL_TARGET) $(TARGET_DIR)
	@mv $(TIM_TARGET) $(TARGET_DIR)
	@mv $(CAL_TARGET) $(TARGET_DIR)
	@mv $(SPA_TARGET) $(TARGET_DIR)
	
removelib: 
	$(RM) $(TARGET_DIR)/libgmpxx.4.dylib
	$(RM) $(TARGET_DIR)/libgmp.10.dylib
	$(RM) $(TARGET_DIR)/libppl.13.dylib
	$(RM) $(TARGET_DIR)/$(COM_TARGET)
	$(RM) $(TARGET_DIR)/$(EXP_TARGET)
	$(RM) $(TARGET_DIR)/$(SYM_TARGET)
	$(RM) $(TARGET_DIR)/$(PPL_TARGET)
	$(RM) $(TARGET_DIR)/$(TIM_TARGET)
	$(RM) $(TARGET_DIR)/$(CAL_TARGET)
	$(RM) $(TARGET_DIR)/$(SPA_TARGET)
	
$(COM_TARGET): $(COM_OBJECT)
	$(CXX) -dynamiclib $(LDFLAGS) -L$(EXTERNAL_LIB_DIR) $^ -o $@ $(LIBS)
	
$(EXP_TARGET): $(COM_TARGET) $(EXP_OBJECT)
	$(CXX) -dynamiclib $(LDFLAGS) -L$(EXTERNAL_LIB_DIR) $^ -o $@ $(LIBS) 

$(SYM_TARGET): $(EXP_TARGET) $(SYM_OBJECT)
	$(CXX) -dynamiclib $(LDFLAGS) -L$(EXTERNAL_LIB_DIR) $^ -o $@ $(LIBS)

$(PPL_TARGET): $(EXP_TARGET) $(SYM_TARGET) $(PPL_OBJECT) 
	$(CXX) -dynamiclib $(LDFLAGS) -L$(EXTERNAL_LIB_DIR) $^ -o $@ $(LIBS)

$(TIM_TARGET): $(COM_TARGET) $(EXP_TARGET) $(SYM_TARGET) $(PPL_TARGET) $(TIM_OBJECT)
	$(CXX) -dynamiclib $(LDFLAGS) -L$(EXTERNAL_LIB_DIR) $^ -o $@ $(LIBS)

$(CAL_TARGET): $(COM_TARGET) $(EXP_TARGET) $(SYM_TARGET) $(PPL_TARGET) $(TIM_TARGET) $(CAL_OBJECT)
	$(CXX) -dynamiclib $(LDFLAGS) -L$(EXTERNAL_LIB_DIR) $^ -o $@ $(LIBS)

$(SPA_TARGET): $(COM_TARGET) $(EXP_TARGET) $(SYM_TARGET) $(PPL_TARGET) $(TIM_TARGET) $(CAL_TARGET) $(SPA_OBJECT)
	$(CXX) $(LDFLAGS) -L$(EXTERNAL_LIB_DIR) $^ -o $@ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I$(ROOT) -I$(EXTERNAL_HEADER_DIR) -c $< -o $@ -fpic
	
%.o: %.c
	$(CVV) $(CVVFLAGS) -I$(ROOT) -I$(EXTERNAL_HEADER_DIR) -c $< -o $@ -fpic

clean: 
	$(RM) $(COM_OBJECT)
	$(RM) $(EXP_OBJECT)
	$(RM) $(SYM_OBJECT)
	$(RM) $(PPL_OBJECT)
	$(RM) $(TIM_OBJECT)
	$(RM) $(CAL_OBJECT)
	$(RM) $(SPA_OBJECT)
	$(RM) $(COM_TARGET)
	$(RM) $(EXP_TARGET)
	$(RM) $(SYM_TARGET)
	$(RM) $(PPL_TARGET)
	$(RM) $(TIM_TARGET)
	$(RM) $(CAL_TARGET)
	$(RM) $(SPA_TARGET)
