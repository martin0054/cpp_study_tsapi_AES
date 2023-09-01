##################################################
#  ARIBO CTI Make File
#  Written : Shin Won Sub
#  Date    : 2017.09.25
##################################################

default:
	@echo "Usage : make moption"
	@echo ""
	@echo "Linux Options"
	@echo "linux       : Linux Release Mode, So Library Compile"
	@echo "linux_d     : Linux Debug Mode, So Library  Compile"

linux32:
	make BIT=32 release -f makefile.linux 
	
linux32_d:
	make BIT=32 debug -f makefile.linux

linux64:
	make BIT=64 release -f makefile.linux 
	
linux64_d:
	make BIT=64 debug -f makefile.linux

clean :
	make clean -f makefile.linux

install :
	make install -f makefile.linux

uninstall :
	make uninstall -f makefile.linux

test :
	make BIT=32 test -f makefile.linux
	make BIT=64 test -f makefile.linux
