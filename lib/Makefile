SHAREDLIBSDIR = lib
SHAREDLIBS = $(wildcard $(SHAREDLIBSDIR)/*.c)
SHAREDLIBS_OBJ = $(SHAREDLIBS:$(SHAREDLIBSDIR)/%.c=$(BIN)/%.o)


SHAREDLIBS_STATIC = $(BIN)/sharedlib.a

LIBCFLAGS = -I$(SHAREDLIBSDIR)/include -I. $(IVARS)

$(SHAREDLIBS_OBJ): $(BIN)/%.o : $(SHAREDLIBSDIR)/%.c
	$(CC) $(CFLAGS) $(LIBCFLAGS) -c -o $@ $^

$(SHAREDLIBS_STATIC): $(SHAREDLIBS_OBJ)
	$(AR) -r $@ $^
	$(RANLIB) $@

