include make.include

INC += -I $(TFC)
LIB += $(PUBLIC_LIB)/libtfc.a -lpthread

CFLAGS += -DSTATISTIC_FLAG -fPIC

TARGET = $(SVR_NAME)_mcd_run.so

OBJ = troomsvr_mcd_proc.o \
	cfg_mgr.o

$(TARGET):$(OBJ)
	$(CXX) $(CFLAGS) -o $@ $^ $(LIB) -shared

%.o: %.cpp
	$(CXX) $(CFLAGS) $(INC) -c -o $@ $<

%.o: %.c
	$(CXX) $(CFLAGS) $(INC) -c -o $@ $<

%.o: %.cc
	$(CXX) $(CFLAGS) $(INC) -c -o $@ $<

SUBDIRS = submcd_log

define make_subdir
for subdir in $(SUBDIRS); do \
	(cd $$subdir && make $1) \
done
endef

clean:
	rm -f $(OBJ)
	rm -f $(TARGET)

cleanall:clean
	$(call make_subdir,clean)
	rm -f $(SVR_BIN_DIR)/bin/*.so
	rm -f $(SVR_BIN_DIR)/tools/op/showshm

buildVersion:cleanall
	@echo "buildVersion: " `git log -l 1 | grep -m 1 commit |cut -c8-15`
	@sed -i "s/.[0-9a-z]\{8\}\"/.`git log -l 1 | grep -m 1 commit |cut -c8-15`\"/1" $(SVR_NAME).h

all:buildVersion\
	$(TARGET)
	$(call make_subdir)

install:buildVersion\
	$(TARGET)
	install $(TARGET) $(SVR_BIN_DIR)/bin
	$(call make_subdir,install)
	install $(PUBLIC_LIB)/ss_tcp_complete.so $(SVR_BIN_DIR)/bin