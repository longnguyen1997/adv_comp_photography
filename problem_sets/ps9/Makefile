NUMBER := 9

BUILD_DIR  := _build
EXECUTABLE := a$(NUMBER)
OUTPUT     := Output

CXX        := g++

#
# Modify HALIDE_DIR to the appropriate path on your machine.
#
HALIDE_DIR ?= $(HOME)/dev/lib/halide
HALIDE_LIB := $(HALIDE_DIR)/lib/libHalide.a

_TUTORIALS := $(wildcard tutorial*.cpp)
TUTORIALS  := $(notdir $(_TUTORIALS:.cpp=))

INCLUDE    := $(wildcard *.h)
_OBJ       := a$(NUMBER).o timing.o
OBJ        := $(patsubst %, $(BUILD_DIR)/%,$(_OBJ))

# Special instructions for Mac users
# ==================================
# You need to or must have installed libpng through Macports or Homebrew.
# Assuming that the installation succeeded, you should be able to run
#
# The brew command for installing libpng is
# brew install libpng
#
# libpng-config --I_opts
# libpng-config --L_opts
#
# Please add the output of the above commands to the following variables:
PNG_INC := `libpng-config --I_opts`
PNG_LIB := `libpng-config --L_opts`

CXXFLAGS := $(PNG_INC) -I$(HALIDE_DIR)/include/ -I$(HALIDE_DIR)/tools/ -I. -Wall -std=c++11
LDFLAGS  := $(PNG_LIB) -L$(HALIDE_DIR)/bin/ -lz -lpthread -ldl -lncurses -lpng

all: $(EXECUTABLE) $(TUTORIALS)
.PHONY: all

run: $(EXECUTABLE)
	./$(EXECUTABLE)
.PHONY: run

clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE) $(TUTORIALS) $(OUTPUT) $(TESTS)
.PHONY: clean

prepare:
	mkdir -p asst
	cp *.cpp asst
	cp *.h asst
	cp Makefile asst
	cp -R Input asst
	zip -r a$(NUMBER)_submission.zip asst -x asst/tutorial*.cpp
	rm -rf asst
.PHONY: prepare

$(BUILD_DIR)/%.o: %.cpp $(INCLUDE)
	mkdir -p $(BUILD_DIR)
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(LDFLAGS)

$(EXECUTABLE): a$(NUMBER)_main.cpp $(OBJ)
	$(CXX) -o $@ $^ $(HALIDE_LIB) $(CXXFLAGS) $(LDFLAGS)
	mkdir -p $(OUTPUT)

%: %.cpp $(OBJ)
	$(CXX) -o $@ $^ $(HALIDE_LIB) $(CXXFLAGS) $(LDFLAGS)
