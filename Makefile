#======================================================================
# This is a generic makefile that has to be used together with the
# 'cs-config' utility that can be generated by Crystal Space.
#
# To use this you first have to generate a config file by doing:
#       make csconfig
# in the Crystal Space directory. Then copy the generated cs-config
# script (which will be in the CS root initially) to your project
# directory.
#
# You also have to edit this makefile so it is suitable for your project.
# You probably want to change at least:
#       EXECUTABLE for the name of the executable
#       OBJSRC for the location of the sources
#       LINKFLAGS for all the libraries that your application needs
#
# If you installed Crystal Space (usually /usr/local/crystal) then
# this makefile should already work. If not you need to set the CRYSTAL
# environment variable to point to the Crystal Space directory.
#
# This makefile supports the following targets: all, clean, depend.
#======================================================================

include csconfig.mak

#------
# Name of targets
#------
BLTEST=bltest${DLL}
PFTEST=pftest${DLL}
PFMESH=pfmesh${DLL}
PFINV=pfinv${DLL}
PLIMP=plimp${DLL}
CELTEST=celtst${EXE}

#------
# Location of sources and object files
#------
PLIMP_SRC=$(wildcard plimp/*.cpp)
PLIMP_OBJS=$(addsuffix .o, $(basename $(PLIMP_SRC)))
BLTEST_SRC=$(wildcard bltest/*.cpp)
BLTEST_OBJS=$(addsuffix .o, $(basename $(BLTEST_SRC)))
PFTEST_SRC=$(wildcard pf/test/*.cpp)
PFTEST_OBJS=$(addsuffix .o, $(basename $(PFTEST_SRC)))
PFMESH_SRC=$(wildcard pf/mesh/*.cpp)
PFMESH_OBJS=$(addsuffix .o, $(basename $(PFMESH_SRC)))
PFINV_SRC=$(wildcard pf/inv/*.cpp)
PFINV_OBJS=$(addsuffix .o, $(basename $(PFINV_SRC)))
CELTEST_SRC=$(wildcard celtest/*.cpp)
CELTEST_OBJS=$(addsuffix .o, $(basename $(CELTEST_SRC)))

#------
# Tools to use
#------
CCC=g++ 
LINK=g++
RM=rm

#------
# Flags for compiler and linker.
# Make sure to update the required libraries for your own project
#------
CEL_INCLUDES=-I. -Iinclude
CFLAGS = $(shell ./cs-config --cflags) $(CEL_INCLUDES)
CXXFLAGS = $(shell ./cs-config --cxxflags) $(CEL_INCLUDES)
PLIMP_LINKFLAGS = $(shell ./cs-config --libs cstool csutil cssys csgfx csgeom)
BLTEST_LINKFLAGS = $(shell ./cs-config --libs cstool csutil cssys csgfx csgeom)
PFTEST_LINKFLAGS = $(shell ./cs-config --libs cstool csutil cssys csgfx csgeom)
PFMESH_LINKFLAGS = $(shell ./cs-config --libs cstool csutil cssys csgfx csgeom)
PFINV_LINKFLAGS = $(shell ./cs-config --libs cstool csutil cssys csgfx csgeom)
CELTEST_LINKFLAGS = $(shell ./cs-config --libs cstool csutil cssys csgfx csgeom)

#------
# Rules
#------
.SUFFIXES: .cpp

.cpp.o: $<
	$(CCC) $(CXXFLAGS) -o $@ -c $<

all: $(PLIMP) $(CELTEST) $(BLTEST) $(PFTEST) $(PFMESH) $(PFINV)


$(PLIMP): $(PLIMP_OBJS)
	$(DO.PLUGIN) $(PLIMP_LINKFLAGS)

$(BLTEST): $(BLTEST_OBJS)
	$(DO.PLUGIN) $(BLTEST_LINKFLAGS)

$(PFTEST): $(PFTEST_OBJS)
	$(DO.PLUGIN) $(PFTEST_LINKFLAGS)

$(PFMESH): $(PFMESH_OBJS)
	$(DO.PLUGIN) $(PFMESH_LINKFLAGS)

$(PFINV): $(PFINV_OBJS)
	$(DO.PLUGIN) $(PFINV_LINKFLAGS)

$(CELTEST): $(CELTEST_OBJS)
	$(DO.EXEC) $(CELTEST_LINKFLAGS)

clean:
	$(RM) $(PLIMP_OBJS) $(PLIMP) $(BLTEST) $(PFTEST) $(PFMESH) $(PFINV) $(CELTEST)

#------
# Create dependencies
#------
depend:
	gcc -MM $(CXXFLAGS) $(PLIMP_SRC) > makefile.dep
	gcc -MM $(CXXFLAGS) $(BLTEST_SRC) >> makefile.dep
	gcc -MM $(CXXFLAGS) $(PFTEST_SRC) >> makefile.dep
	gcc -MM $(CXXFLAGS) $(PFMESH_SRC) >> makefile.dep
	gcc -MM $(CXXFLAGS) $(PFINV_SRC) >> makefile.dep
	gcc -MM $(CXXFLAGS) $(CELTEST_SRC) >> makefile.dep

#------
# Include dependencies
#------
-include makefile.dep

