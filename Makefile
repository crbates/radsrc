#
# make       = make unix version, run test jobs, compare to reference output
# make mingw = on PC this generates version that can run without cygwin
# make win   = create zip file with PC binary and data
# make mac   = create zip file with MacOSX binary and data
#
# By default generate the setup file and then pass all
# targets to the Makefile in the src directory
#

default: setup
	@$(MAKE) -C src

$(MAKECMDGOALS): setup
	@$(MAKE) -C src $(MAKECMDGOALS)

win:
	$(MAKE) -C win

mac:
	$(MAKE) -C macosx

setup:
	@cat src/setup.template | sed s?LOCAL_DIR?$(CURDIR)?g > setup

.PHONY: test setup win mac

MAKEFLAGS += --no-print-directory
