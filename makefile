all: ctmodel.cc ctprint.cc ctrev.cc ctapply.cc ctfunctions.h
	gcc -o ctmodel ctmodel.cc
	gcc -o ctprint ctprint.cc -lm
	gcc -o ctrev ctrev.cc
	gcc -o ctapply ctapply.cc -lm
	gcc -o ctrates ctrates.cc -lm
