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

Examples:
data.txt , model.bin

==============================================
Toelichting .sh files on cluster
modelscreate.sh : create the models for file R1 and R2
modelscombine.sh : combine the models for R1 and R2 and add reverse complement
