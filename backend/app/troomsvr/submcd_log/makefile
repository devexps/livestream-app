include ../make.include

INC += -I ../$(TFC)
LIB += ../$(PUBLIC_LIB)/libtfc.a -lpthread

CFLAGS += -DSTATISTIC_FLAG -fPIC

TARGET = $(SVR_NAME)_submcd_log_run.so

OBJ = troomsvr_submcd_log_proc.o \
	log_message_process.o \
	../cfg_mgr.o

$(TARGET):$(OBJ)
	$(CXX) $(CFLAGS) -o $@ $^ $(LIB) -shared

%.o: %.cpp
	$(CXX) $(CFLAGS) $(INC) -c -o $@ $<

%.o: %.c
	$(CXX) $(CFLAGS) $(INC) -c -o $@ $<

%.o: %.cc
	$(CXX) $(CFLAGS) $(INC) -c -o $@ $<

all: $(TARGET)

clean:
	rm -f $(OBJ)
	rm -f $(TARGET)

install:$(TARGET)
	install $(TARGET) ../$(SVR_BIN_DIR)/bin
