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
# If you want to compile plugins as well, edit:
#       PLUGIN for the name of the plugin
#       PLUGIN.OBJSRC for the location of the plugin sources
#       PLUGIN.LINKFLAGS for the libraries that the plugin needs
#
# If you installed Crystal Space (usually /usr/local/crystal) then
# this makefile should already work. If not you need to set the CRYSTAL
# environment variable to point to the Crystal Space directory.
#
# This makefile supports the following targets: all, clean, depend.
#======================================================================

#------
# Name of targets
#------
BLTEST=bltest$(DLL)
PFTEST=pftest$(DLL)
PFMESH=pfmesh$(DLL)
PFMOVE=pfmove$(DLL)
PFTOOLS=pftools$(DLL)
PFENG=pfengine$(DLL)
PFINV=pfinv$(DLL)
PFINPUT=pfinput$(DLL)
PLIMP=plimp$(DLL)
CPERSIST=cpersist$(DLL)
CELTEST=celtst$(EXE)

#------
# Location of sources and object files
#------
CPERSIST_SRC=$(wildcard persist/classic/*.cpp)
CPERSIST_OBJS=$(addsuffix .o, $(basename $(CPERSIST_SRC)))
PLIMP_SRC=$(wildcard plimp/*.cpp)
PLIMP_OBJS=$(addsuffix .o, $(basename $(PLIMP_SRC)))
BLTEST_SRC=$(wildcard bltest/*.cpp)
BLTEST_OBJS=$(addsuffix .o, $(basename $(BLTEST_SRC)))
PFTEST_SRC=$(wildcard pf/test/*.cpp pf/common/*.cpp)
PFTEST_OBJS=$(addsuffix .o, $(basename $(PFTEST_SRC)))
PFMESH_SRC=$(wildcard pf/mesh/*.cpp pf/common/*.cpp)
PFMESH_OBJS=$(addsuffix .o, $(basename $(PFMESH_SRC)))
PFMOVE_SRC=$(wildcard pf/move/*.cpp pf/common/*.cpp)
PFMOVE_OBJS=$(addsuffix .o, $(basename $(PFMOVE_SRC)))
PFTOOLS_SRC=$(wildcard pf/tools/*.cpp pf/common/*.cpp)
PFTOOLS_OBJS=$(addsuffix .o, $(basename $(PFTOOLS_SRC)))
PFENG_SRC=$(wildcard pf/engine/*.cpp pf/common/*.cpp)
PFENG_OBJS=$(addsuffix .o, $(basename $(PFENG_SRC)))
PFINV_SRC=$(wildcard pf/inv/*.cpp pf/common/*.cpp)
PFINV_OBJS=$(addsuffix .o, $(basename $(PFINV_SRC)))
PFINPUT_SRC=$(wildcard pf/input/*.cpp pf/common/*.cpp)
PFINPUT_OBJS=$(addsuffix .o, $(basename $(PFINPUT_SRC)))
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
CSCONFIG.MAK=csconfig.mak
-include $(CSCONFIG.MAK)
ifeq ($(LINK.PLUGIN),)
  LINK.PLUGIN=$(LINK)
endif
CEL_INCLUDES=-I. -Iinclude
CFLAGS := $(shell ./cs-config --cflags) $(CEL_INCLUDES)
CXXFLAGS := $(shell ./cs-config --cxxflags) $(CEL_INCLUDES)
CPERSIST_LINKFLAGS = $(shell ./cs-config --libs cstool csutil cssys csgfx csgeom)
PLIMP_LINKFLAGS := $(shell ./cs-config --libs cstool csutil cssys csgfx csgeom)
BLTEST_LINKFLAGS := $(shell ./cs-config --libs cstool csutil cssys csgfx csgeom)
PFTEST_LINKFLAGS := $(shell ./cs-config --libs cstool csutil cssys csgfx csgeom)
PFMESH_LINKFLAGS := $(shell ./cs-config --libs cstool csutil cssys csgfx csgeom)
PFMOVE_LINKFLAGS := $(shell ./cs-config --libs cstool csutil cssys csgfx csgeom)
PFTOOLS_LINKFLAGS := $(shell ./cs-config --libs cstool csutil cssys csgfx csgeom)
PFENG_LINKFLAGS := $(shell ./cs-config --libs cstool csutil cssys csgfx csgeom)
PFINV_LINKFLAGS := $(shell ./cs-config --libs cstool csutil cssys csgfx csgeom)
PFINPUT_LINKFLAGS :=  $(shell ./cs-config --libs cstool csutil cssys csgfx csgeom)
CELTEST_LINKFLAGS := $(shell ./cs-config --libs cstool csutil cssys csgfx csgeom)

#------
# Rules
#------
.PHONY: all depend clean
.SUFFIXES: .cpp
DO.SHARED.PLUGIN.CORE=$(LINK.PLUGIN) $(LFLAGS.DLL) -o $@ $^ $(PLUGIN.POSTFLAGS)
DO.PLUGIN = $(DO.SHARED.PLUGIN.PREAMBLE) $(DO.SHARED.PLUGIN.CORE) \
  $(DO.SHARED.PLUGIN.POSTAMBLE)
DO.EXEC = $(LINK) -o $@ $^ $(LFLAGS.EXE) $(LIBS.EXE.PLATFORM)

.cpp.o: $<
	$(CCC) $(CXXFLAGS) -o $@ -c $<

all: $(CSCONFIG.MAK) $(CPERSIST) $(PLIMP) $(CELTEST) $(BLTEST) $(PFTEST) $(PFMESH) \
	$(PFMOVE) $(PFTOOLS) $(PFENG) $(PFINV) $(PFINPUT)


$(CPERSIST): $(CPERSIST_OBJS)
	$(DO.PLUGIN) $(CPERSIST_LINKFLAGS)

$(PLIMP): $(PLIMP_OBJS)
	$(DO.PLUGIN) $(PLIMP_LINKFLAGS)

$(BLTEST): $(BLTEST_OBJS)
	$(DO.PLUGIN) $(BLTEST_LINKFLAGS)

$(PFTEST): $(PFTEST_OBJS)
	$(DO.PLUGIN) $(PFTEST_LINKFLAGS)

$(PFMESH): $(PFMESH_OBJS)
	$(DO.PLUGIN) $(PFMESH_LINKFLAGS)

$(PFMOVE): $(PFMOVE_OBJS)
	$(DO.PLUGIN) $(PFMOVE_LINKFLAGS)

$(PFTOOLS): $(PFTOOLS_OBJS)
	$(DO.PLUGIN) $(PFTOOLS_LINKFLAGS)

$(PFENG): $(PFENG_OBJS)
	$(DO.PLUGIN) $(PFENG_LINKFLAGS)

$(PFINV): $(PFINV_OBJS)
	$(DO.PLUGIN) $(PFINV_LINKFLAGS)

$(PFINPUT): $(PFINPUT_OBJS)
	$(DO.PLUGIN) $(PFINPUT_LINKFLAGS)

$(CELTEST): $(CELTEST_OBJS)
	$(DO.EXEC) $(CELTEST_LINKFLAGS)

clean:
	$(RM) $(CPERSIST_OBJS) $(PLIMP_OBJS) $(BLTEST_OBJS) $(PFTEST_OBJS) $(PFMESH_OBJS) \
		$(PFMOVE_OBJS) $(PFTOOLS_OBJS) $(PFINV_OBJS) $(CELTEST_OBJS) \
		$(PFENG_OBJS) $(CPERSIST) \
		$(PLIMP) $(BLTEST) $(PFTEST) $(PFENG) $(PFINPUT) \
		$(PFMESH) $(PFMOVE) $(PFINV) $(PFTOOLS) $(CELTEST)

#------
# Create dependencies
#------
depend: $(CSCONFIG.MAK)
	gcc -MM $(CXXFLAGS) $(CPERSIST_SRC) > makefile.dep
	gcc -MM $(CXXFLAGS) $(PLIMP_SRC) > makefile.dep
	gcc -MM $(CXXFLAGS) $(BLTEST_SRC) >> makefile.dep
	gcc -MM $(CXXFLAGS) $(PFTEST_SRC) >> makefile.dep
	gcc -MM $(CXXFLAGS) $(PFMESH_SRC) >> makefile.dep
	gcc -MM $(CXXFLAGS) $(PFMOVE_SRC) >> makefile.dep
	gcc -MM $(CXXFLAGS) $(PFTOOLS_SRC) >> makefile.dep
	gcc -MM $(CXXFLAGS) $(PFENG_SRC) >> makefile.dep
	gcc -MM $(CXXFLAGS) $(PFINV_SRC) >> makefile.dep
	gcc -MM $(CXXFLAGS) $(CELTEST_SRC) >> makefile.dep

#------
# Re-create the config flags include file
#------
$(CSCONFIG.MAK): ./cs-config
	./cs-config --makevars > $(CSCONFIG.MAK)

#------
# Include dependencies
#------
-include makefile.dep

