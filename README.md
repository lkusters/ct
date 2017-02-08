ctmodel : make a model

awk '{if(++count%4==2) print $1}' data.txt | ./ctmodel 2 > result.txt

construct program from .cc:
gcc -DDEBUG -o ctmodel ctmodel.cc
