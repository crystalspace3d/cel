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
BLPYTHON=blpython$(DLL)
PFTEST=pftest$(DLL)
PFMESH=pfmesh$(DLL)
PFMOVE=pfmove$(DLL)
PFTOOLS=pftools$(DLL)
PFENG=pfengine$(DLL)
PFINV=pfinv$(DLL)
PFINPUT=pfinput$(DLL)
STDPL=stdphyslayer$(DLL)
CPERSIST=cpersist$(DLL)
CELTEST=celtst$(EXE)

#------
# Location of sources and object files
#------
CPERSIST_SRC=$(wildcard plugins/persist/classic/*.cpp)
CPERSIST_OBJS=$(addsuffix .o, $(basename $(CPERSIST_SRC)))
STDPL_SRC=$(wildcard plugins/stdphyslayer/*.cpp)
STDPL_OBJS=$(addsuffix .o, $(basename $(STDPL_SRC)))
BLTEST_SRC=$(wildcard plugins/behaviourlayer/test/*.cpp)
BLTEST_OBJS=$(addsuffix .o, $(basename $(BLTEST_SRC)))
BLPYTHON_SRC=$(wildcard plugins/behaviourlayer/python/*.cpp)
BLPYTHON_OBJS=$(addsuffix .o, $(basename $(BLPYTHON_SRC)))
PFTEST_SRC=$(wildcard plugins/propclass/test/*.cpp plugins/propclass/common/*.cpp)
PFTEST_OBJS=$(addsuffix .o, $(basename $(PFTEST_SRC)))
PFMESH_SRC=$(wildcard plugins/propclass/mesh/*.cpp plugins/propclass/common/*.cpp)
PFMESH_OBJS=$(addsuffix .o, $(basename $(PFMESH_SRC)))
PFMOVE_SRC=$(wildcard plugins/propclass/move/*.cpp plugins/propclass/common/*.cpp)
PFMOVE_OBJS=$(addsuffix .o, $(basename $(PFMOVE_SRC)))
PFTOOLS_SRC=$(wildcard plugins/propclass/tools/*.cpp plugins/propclass/common/*.cpp)
PFTOOLS_OBJS=$(addsuffix .o, $(basename $(PFTOOLS_SRC)))
PFENG_SRC=$(wildcard plugins/propclass/engine/*.cpp plugins/propclass/common/*.cpp)
PFENG_OBJS=$(addsuffix .o, $(basename $(PFENG_SRC)))
PFINV_SRC=$(wildcard plugins/propclass/inv/*.cpp plugins/propclass/common/*.cpp)
PFINV_OBJS=$(addsuffix .o, $(basename $(PFINV_SRC)))
PFINPUT_SRC=$(wildcard plugins/propclass/input/*.cpp plugins/propclass/common/*.cpp)
PFINPUT_OBJS=$(addsuffix .o, $(basename $(PFINPUT_SRC)))
CELTEST_SRC=$(wildcard celtest/*.cpp)
CELTEST_OBJS=$(addsuffix .o, $(basename $(CELTEST_SRC)))
OUT=.

#------
# Tools to use
#------
ifndef CXX
CXX=g++
endif
ifndef LINK
LINK=$(CXX)
endif
RM=rm -f

#------
# Abstractions
#------
LFLAGS.L = -L
LFLAGS.l = -l
CFLAGS.D = -D
CFLAGS.I = -I

#------
# Flags for compiler and linker.
# Make sure to update the required libraries for your own project
#------
CSCONFIG.MAK=csconfig.mak
-include $(CSCONFIG.MAK)
ifeq ($(LINK.PLUGIN),)
  LINK.PLUGIN=$(LINK)
endif
CSDIR := $(shell ./cs-config --prefix)
CEL_INCLUDES=-I. -Iinclude
CFLAGS := $(shell ./cs-config --cflags) $(CEL_INCLUDES) -DCEL_DEBUG
CXXFLAGS := $(shell ./cs-config --cxxflags) $(CEL_INCLUDES) -I$(PYTHON_INCDIR) -DCEL_DEBUG
CPERSIST_LINKFLAGS = $(shell ./cs-config --libs cstool csutil cssys csgfx csgeom)
STDPL_LINKFLAGS := $(shell ./cs-config --libs cstool csutil cssys csgfx csgeom)
BLTEST_LINKFLAGS := $(shell ./cs-config --libs cstool csutil cssys csgfx csgeom)
BLPYTHON_LINKFLAGS := $(shell ./cs-config --libs python cstool csutil cssys csgfx csgeom)
BLPYTHON_CXXFLAGS := $(shell ./cs-config --cxxflags python cstool csutil cssys csgfx csgeom) $(CEL_INCLUDES)
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
	$(CXX) $(CXXFLAGS) -o $@ -c $<

all: $(CSCONFIG.MAK) $(CPERSIST) $(STDPL) $(CELTEST) $(BLTEST) $(PFTEST) \
	$(PFMESH) $(PFMOVE) $(PFTOOLS) $(PFENG) $(PFINV) $(PFINPUT) \
	$(BLPYTHON)

$(CPERSIST): $(CPERSIST_OBJS)
	$(DO.PLUGIN) $(CPERSIST_LINKFLAGS)

$(STDPL): $(STDPL_OBJS)
	$(DO.PLUGIN) $(STDPL_LINKFLAGS)

$(BLTEST): $(BLTEST_OBJS)
	$(DO.PLUGIN) $(BLTEST_LINKFLAGS)

$(BLPYTHON_OBJS): %.o: %.cpp
	$(CXX) $(BLPYTHON_CXXFLAGS) -o $*.o -c $*.cpp
	
$(BLPYTHON): $(BLPYTHON_OBJS)
	$(DO.PLUGIN) $(BLPYTHON_LINKFLAGS)

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
	$(RM) $(CPERSIST_OBJS) $(STDPL_OBJS) $(BLTEST_OBJS) $(PFTEST_OBJS) \
		$(PFMESH_OBJS) $(PFMOVE_OBJS) $(PFTOOLS_OBJS) $(PFINV_OBJS) \
		$(CELTEST_OBJS) $(PFENG_OBJS) $(PFINPUT_OBJS) $(BLPYTHON_OBJS) \
		$(CPERSIST) $(STDPL) $(BLTEST) $(PFTEST) $(PFENG) $(PFINPUT) \
		$(PFMESH) $(PFMOVE) $(PFINV) $(PFTOOLS) $(CELTEST) \
		$(BLPYTHON) *.def

#------
# Run swig for the blpython plugin.
#------
blpythonswig:
	swig -python -I$(CSDIR)/include -I$(CSDIR)/include/ivaria -c++ -shadow \
		-o plugins/behaviourlayer/python/blcel.cpp plugins/behaviourlayer/python/blpython.i
	mv plugins/behaviourlayer/python/blcelc.py scripts

#------
# Create dependencies
#------
depend: $(CSCONFIG.MAK)
	gcc -M $(CPERSIST_SRC) $(CXXFLAGS) > makefile.dep
	gcc -M $(CXXFLAGS) $(STDPL_SRC) > makefile.dep
	gcc -M $(CXXFLAGS) $(BLTEST_SRC) >> makefile.dep
	gcc -M $(CXXFLAGS) $(BLPYTHON_SRC) >> makefile.dep
	gcc -M $(CXXFLAGS) $(PFTEST_SRC) >> makefile.dep
	gcc -M $(CXXFLAGS) $(PFMESH_SRC) >> makefile.dep
	gcc -M $(CXXFLAGS) $(PFMOVE_SRC) >> makefile.dep
	gcc -M $(CXXFLAGS) $(PFTOOLS_SRC) >> makefile.dep
	gcc -M $(CXXFLAGS) $(PFENG_SRC) >> makefile.dep
	gcc -M $(CXXFLAGS) $(PFINV_SRC) >> makefile.dep
	gcc -M $(CXXFLAGS) $(PFINPUT_SRC) >> makefile.dep
	gcc -M $(CXXFLAGS) $(CELTEST_SRC) >> makefile.dep

#------
# Re-create the config flags include file
#------
$(CSCONFIG.MAK): ./cs-config
	./cs-config --makevars > $(CSCONFIG.MAK)

#------
# Include dependencies
#------
-include makefile.dep
