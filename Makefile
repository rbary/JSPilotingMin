EXEC_NAME=JSPilotingMin #output filename

SDK_DIR=/home/rbary/Parrot_Dev/ARDroneSDK

IDIR=./
CFLAGS=-I$(IDIR) -I$(SDK_DIR)/out/Unix-base/staging/usr/include

OBJDIR=obj
LDIR = $(SDK_DIR)/out/Unix-base/staging/usr/lib

EXTERNAL_LIB=-lncurses

LIBS=-L$(LDIR) -larsal -larcontroller -lardiscovery $(EXTERNAL_LIB) -Wl,-rpath=$(LDIR)
LIBS_DBG=-L$(LDIR) -larsal_dbg -larcontroller_dbg -lardiscovery_dbg $(EXTERNAL_LIB) -Wl,-rpath=$(LDIR)

_DEPS = JSPilotingMinCommands.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = JSPilotingMinCommands.o main.o
OBJ = $(patsubst %,$(OBJDIR)/%,$(_OBJ))

$(OBJDIR)/%.o: %.c $(DEPS)
	@ [ -d $(OBJDIR) ] || mkdir $(OBJDIR)
	@ $(CC) -c -o $@ $< $(CFLAGS)

$(EXEC_NAME): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	@ rm -f $(OBJDIR)/*.o *~ core $(INCDIR)/*~
	@ rm -rf $(OBJDIR)
	@ rm -r $(EXEC_NAME)

run:
	LD_LIBRARY_PATH=$(LDIR) ./$(EXEC_NAME)

valgrind:
	LD_LIBRARY_PATH=$(LDIR) valgrind ./$(EXEC_NAME)
