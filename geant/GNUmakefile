# $Id: GNUmakefile,v 1.1 1999/01/07 16:05:40 gunter Exp $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := exampleN01
G4TARGET := $(name)
G4EXLIB := true
EXTRALIBS += -L$(RADSRC_HOME)/lib/ -lradsrc
CPPFLAGS += -I$(RADSRC_HOME)/src/libradsrc/

.PHONY: all
all: lib bin

ifdef G4INSTALL
  include $(G4INSTALL)/config/binmake.gmk
else
  lib:
	@echo G4INSTALL variable not defined, please setup GEANT environment.
endif
