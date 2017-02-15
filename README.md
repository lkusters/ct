ctmodel : make a model

awk '{if(++count%4==2) print $1}' data.txt | ./ctmodel 2 > model.bin

construct program from .cc:
gcc -DDEBUG -o ctmodel ctmodel.cc -lm

ctprint : print a model
./ctprint 2 model.bin

ctcombine : combine 2 models and add reverse complement
./ctcombine 2 model1.bin model2.bin > modelcombo.bin

ctapply : apply the model (calculate divergence)
awk '{if(++count%4==2) print $1}' data.txt | ./ctapply 2 model.bin > div.txt

ctdiv2hist : convert divergence values to histogram
{ cat divs.txt | awk '{s+=$1}END{print s/NR}' ; cat divs.txt; } | ./ctdiv2hist

Examples:
data.txt , model.bin
