# THIS FILE IS FOR LINUX! EDIT FOR OTHER PLATFORMS
# Soon this will be automated
EXE=
DLL=.so
LFLAGS.DLL=-Wl,-shared
DO.PLUGIN=$(LINK) $(LFLAGS.DLL) -o $@ $^
DO.EXEC=$(LINK) -o $@ $^

