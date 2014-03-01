all:
	make -C Vcore clean
	make -C maya clean
	make -C Vcore
	make -C maya
	make -C prman
clean:
	make -C Vcore clean
	make -C maya clean
	make -C prman
install:
	cp maya/VmCityislands.bundle /Users/Shared/Autodesk/maya/plug-ins
