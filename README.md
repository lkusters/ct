ctmodel : make a model

awk '{if(++count%4==2) print $1}' data.txt | ./ctmodel 2 > model.bin

construct program from .cc:
gcc -DDEBUG -o ctmodel ctmodel.cc -lm
see also makefile

ctprint : print a model
./ctprint 2 model.bin

<removed> ctcombine : combine 2 models and add reverse complement
<removed>./ctcombine 2 model1.bin model2.bin > modelcombo.bin

ctrev : load model and add reverse complement
./ctrev 2 model1.bin > model1rev.bin
can test it with data1.txt which has strand and reverse strand, therefore
model1rev.bin will have double counts in comparison to model1.bin

ctapply : apply the model (calculate divergence)
awk '{if(++count%4==2) print $1}' data.txt | ./ctapply K DIV model.bin > div.txt
K: model depth
DIV: Divergence estimation method
0: Divergence D(seq||model) -> 1/N SUM n(c) SUM P(a|c) log( P(a|c) / M(a|c) )
1: Divergence estimate D(seq||model) -> 1/N SUM n(a|c) log( P(a|c) / M(a|c) )
2: Divergence D(model||seq)
3: Divergence estimate D(model||seq)\n", argv[0]);

# not used: ctdiv2hist : convert divergence values to histogram
{ cat divs.txt | awk '{s+=$1}END{print s/NR}' ; cat divs.txt; } | ./ctdiv2hist

Testfile
./cttest.sh : for testing the functions (output model.bin , modelrev.bin )
example data: dataAA.fastq , example.fastq

==============================================
Toelichting .sh files on cluster
modelscreate.sh : create the models for file R1 and R2
modelscombine.sh : combine the models for R1 and R2 and add reverse complement

==============================================
1902 verification
./ctmodel : tested in -DDEBUG mode on dataAA.fastq > dataAA.txt and gives correct result
./ctprint : tested on model.bin (constructed for previous exp) and gives correct result
./ctcombine : dont need it anymore -> replace with ./ctrev
