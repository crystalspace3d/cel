# THIS FILE IS FOR MINGW! EDIT FOR OTHER PLATFORMS
# Soon this will be automated
EXE=.exe
DLL=.dll
LFLAGS.DLL=-q -s --no-export-all-symbols --dllname $(patsubst %.dll,%,$@)
DO.PLUGIN=dllwrap $(LFLAGS.DLL) -o $@ $^ -mwindows -mconsole
DO.EXEC=$(LINK) -o $(patsubst %.exe,%,$@) $^

# THIS FILE IS FOR LINUX! EDIT FOR OTHER PLATFORMS
# Soon this will be automated
# EXE=
# DLL=.so
# LFLAGS.DLL=-Wl,-shared
# DO.PLUGIN=$(LINK) $(LFLAGS.DLL) -o $@ $^
# DO.EXEC=$(LINK) -o $@ $^

