#
# make a zip file for Windows XP distribution
#
#BASE = $(shell basename `(cd ..; pwd)`)
BASE = radsrc_v1.3
DIR = ..

default: $(BASE)_win.zip

#....prepare a directory containing what we want to zip
#    gets deleted after the zip
$(BASE): check
	[ -e $@ ] && rm -rf $@; mkdir $@ $@/bin
	cp $(DIR)/bin/radsrc.exe $@/bin    # executable
	cp Readme test_radsrc.bat run_radsrc.bat $@       # test job        
	svn export $(DIR)/lib $@/lib	   # library (binning files)
	svn export $(DIR)/data $@/data     # decay data
	svn export $(DIR)/doc $@/doc       # documentation
        #....reference output files
	svn export $(DIR)/test/test.du/reference.mingw $@/reference  
        #....example input file
	cp $(DIR)/test/test.du/input $@/reference/input.txt

#....Make sure that the executable is the mingw version, 
#    stop the zip if it is not
check:
	@objdump -p $(DIR)/bin/radsrc.exe | grep -q msvcrt.dll || (echo This is not mingw version, no zip made; false)

#....zip up the directory (delete it if zip was succesful)
$(BASE)_win.zip: $(BASE)
	zip -qr $@ $< && /bin/rm -rf $(BASE)

push: $(BASE)_win.zip
	scp $(BASE)_win.zip oban.llnl.gov:web.me

clean:
	/bin/rm -r $(BASE)*

.PHONY: check clean push