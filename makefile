VERSION = 0.0.1
COMPILER = g++
# Options passed to COMPILER
CFLAGS = -Wall 
ifdef DEBUG
CFLAGS += -g
endif

PLATFORM = standard

/:= $(strip \)
SRC_DIR = src
PLATFORMS_DIR = platforms
TEST_DIR = test
HEADER_DIR = include
LIB_DIR = libs
PB_DIR = $(LIB_DIR)$/nanopb
ONNX_DIR = $(LIB_DIR)$/onnx


LIB_OBJECTS = $(PB_DIR)$/pb_common.o $(PB_DIR)$/pb_decode.o $(ONNX_DIR)$/onnx.pb.o 
TEST_OBJECTS = $(TEST_DIR)$/decode.o
PLATFORMS_OBJECTS = $(PLATFORMS_DIR)$/$(PLATFORM)$/cm.o
SRC_OBJECTS =
OBJECTS = $(LIB_OBJECTS) $(SRC_OBJECTS) $(PLATFORMS_OBJECTS) $(TEST_OBJECTS)

OUTPUT = $(TEST_DIR)$/decode.exe

$(OUTPUT) : $(OBJECTS) 
	$(COMPILER) $(CFLAGS) $(OBJECTS) -o $(OUTPUT)

$(PB_DIR)$/pb_common.o : $(PB_DIR)$/pb_common.c $(PB_DIR)$/pb_common.h $(PB_DIR)$/pb.h
	$(COMPILER) $(CFLAGS) -I $(PB_DIR) -c $(PB_DIR)$/pb_common.c -o $(PB_DIR)$/pb_common.o 

$(PB_DIR)$/pb_decode.o : $(PB_DIR)$/pb_decode.c $(PB_DIR)$/pb_decode.h $(PB_DIR)$/pb.h
	$(COMPILER) $(CFLAGS) -I $(PB_DIR) -c $(PB_DIR)$/pb_decode.c -o $(PB_DIR)$/pb_decode.o

$(ONNX_DIR)$/onnx.pb.o : $(ONNX_DIR)$/onnx.pb.c $(ONNX_DIR)$/onnx.pb.h $(PB_DIR)$/pb.h
	$(COMPILER) $(CFLAGS) -I $(ONNX_DIR) -I $(PB_DIR) -c $(ONNX_DIR)$/onnx.pb.c -o $(ONNX_DIR)$/onnx.pb.o

$(PLATFORMS_DIR)$/$(PLATFORM)$/cm.o : $(PLATFORMS_DIR)$/$(PLATFORM)$/cm.c $(HEADER_DIR)$/cm.h
	$(COMPILER) $(CFLAGS) -I $(HEADER_DIR) -c $(PLATFORMS_DIR)$/$(PLATFORM)$/cm.c -o $(PLATFORMS_DIR)$/$(PLATFORM)$/cm.o

$(TEST_DIR)$/decode.o : $(TEST_DIR)$/decode.cpp $(PB_DIR)$/pb_decode.h $(ONNX_DIR)$/onnx.pb.h $(HEADER_DIR)$/cm.h
	$(COMPILER) $(CFLAGS) -I $(PB_DIR) -I $(ONNX_DIR) -I $(HEADER_DIR) -c $(TEST_DIR)$/decode.cpp -o $(TEST_DIR)$/decode.o


RM = del

.PHONY: cleanobj clean

clean: cleanobj
	-$(RM) .$/$(OUTPUT)

cleanobj: 
	-$(RM) .$/$(PLATFORMS_DIR)$/$(PLATFORM)$/*.o
	-$(RM) .$/$(TEST_DIR)$/*.o
	-$(RM) .$/$(PB_DIR)$/*.o
	-$(RM) .$/$(ONNX_DIR)$/*.o
	-$(RM) .$/$(SRC_DIR)$/*.o







