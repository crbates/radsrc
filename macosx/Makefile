#
# MacOX bundle executable and data
#
# copy everything into subdir named radsrc
# then right click and make archive
# be careful not to copy .svn info into the bundle
#
# Doug Wright

BASE = radsrc_v$(shell cat ../version)
ZIP  = $(BASE)_macosx.zip
DIR = ..

default: $(ZIP) push

#....prepare a directory containing what we want to zip
#    gets deleted after the zip
$(BASE):
        #....remove all zip files
	rm -rf *.zip 
	[ -e $@ ] && rm -rf $@; mkdir $@ $@/bin
        #....set bit so that the script looks like an executable to macosx
	/Developer/Tools/SetFile -a E *.command
	cp $(DIR)/bin/radsrc $@/bin    # executable

	cp Readme *.command $@       # test job        
	svn export $(DIR)/lib $@/lib	   # library (binning files)
	cp $(DIR)/lib/lib* $@/lib         # library itself
	svn export $(DIR)/data $@/data     # decay data
	svn export $(DIR)/doc $@/doc       # documentation
        #....reference output files
	svn export $(DIR)/test/test.du/reference $@/reference  
        #....example input files (need newer svn to export single file)
	cp $(DIR)/test/test.du/input $@/reference/input.txt

$(ZIP): $(BASE)
	ditto -ck --sequesterRsrc --keepParent $(BASE) $@
	/bin/rm -rf $(BASE)

push: $(ZIP)
	scp $< oban.llnl.gov:web.me

.PHONY: $(BASE) push
