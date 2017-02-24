#!/bin/bash
K=2

echo "===================="
echo "make"
make
echo "===================="
echo "./ctmodel"
echo "===================="
echo "OUT"
cat dataAA.fastq | awk '{if(++count%4==2) print $1}' | ./ctmodel $K > model.bin
./ctprint $K model.bin
echo "TRUTH"
echo "AA:         0        35         0         0
AC:        34         0         0         0
AG:         0        35        35         0
AT:         0         0         0         0
CA:        34         0        35         0
CC:         0         0         0         0
CG:         0         0         0         0
CT:         0         0         0         0
GA:         0         0        36         0
GC:        36         0         0         0
GG:        35         0         0        36
GT:         0         0        35         0
TA:         0         0         0         0
TC:         0         0         0         0
TG:         0         0        35         0
TT:         0         0         0         0"
read -p "press enter to continue"
echo "===================="
echo "./ctrev"
echo "===================="
echo "OUT"
echo "CCTCCTCCTCCTCCT
AGGAGGAGGAGGAGG" | ./ctmodel $K > model.bin
./ctprint $K model.bin
echo "TRUTH"
echo "AA:         0         0         0         0
AC:         0         0         0         0
AG:         0         0         4         0
AT:         0         0         0         0
CA:         0         0         0         0
CC:         0         0         0         5
CG:         0         0         0         0
CT:         0         4         0         0
GA:         0         0         5         0
GC:         0         0         0         0
GG:         4         0         0         0
GT:         0         0         0         0
TA:         0         0         0         0
TC:         0         4         0         0
TG:         0         0         0         0
TT:         0         0         0         0"
read -p "press enter to continue"
echo "OUT"
./ctrev $K model.bin > modelrev.bin
./ctprint $K modelrev.bin
echo "TRUTH"
echo "AA:         0         0         0         0
AC:         0         0         0         0
AG:         0         0         8         0
AT:         0         0         0         0
CA:         0         0         0         0
CC:         0         0         0         10
CG:         0         0         0         0
CT:         0         8         0         0
GA:         0         0         10        0
GC:         0         0         0         0
GG:         8         0         0         0
GT:         0         0         0         0
TA:         0         0         0         0
TC:         0         8         0         0
TG:         0         0         0         0
TT:         0         0         0         0"
read -p "press enter to continue"
echo "===================="
echo "./ctapply"
echo "===================="
echo "OUT"
# gcc -DDEBUG -o ctapply ctapply.cc -lm
echo "GAG" | ./ctapply $K 0 modelrev.bin | xargs echo -e "\e[1m"
echo -e "\e[0m"
echo "GAG" | ./ctapply $K 1 modelrev.bin | xargs echo -e "\e[1m"
echo -e "\e[0mTRUTH"
echo -e "count : AG  0   0    1     0
p     : AG 1/6  1/6  1/2   1/6
q     : AG 1/20 1/20 17/20 1/20
SUM p*log2(p/q) = \e[1m0.4857\e[0m
SUM log2(p/q) = \e[1m-0.766\e[0m"
echo "OUT"
echo -e "AGG" | ./ctapply $K 0 modelrev.bin | xargs echo -e "\e[1m"
echo -e "\e[0m"
echo -e "AGG" | ./ctapply $K 1 modelrev.bin | xargs echo -e "\e[1m"
echo -e "\e[0mTRUTH"
echo -e "count : GA  0   0    1     0
p     : GA 1/6  1/6  1/2   1/6
q     : GA 1/24 1/24 21/24 1/24
SUM p*log2(p/q) = \e[1m0.5963\e[0m
SUM log2(p/q) = \e[1m-0.807\e[0m"
echo "OUT"
echo -e "CCT" | ./ctapply $K 0 modelrev.bin | xargs echo -e "\e[1m"
echo -e "\e[0m"
echo -e "CCT" | ./ctapply $K 1 modelrev.bin | xargs echo -e "\e[1m"
echo -e "\e[0mTRUTH"
echo -e "CCT : SUM p*log2(p/q) = \e[1m0.5963\e[0m"
echo -e "CCT : SUM log2(p/q) = \e[1m-0.807\e[0m"
echo "OUT"
echo -e "TCC" | ./ctapply $K 0 modelrev.bin | xargs echo -e "\e[1m"
echo -e "\e[0m"
echo -e "TCC" | ./ctapply $K 1 modelrev.bin | xargs echo -e "\e[1m"
echo -e "\e[0mTRUTH"
echo -e "TCC : SUM p*log2(p/q) = \e[1m0.4857\e[0m"
echo -e "TCC : SUM log2(p/q) = \e[1m-0.766\e[0m"
# gcc -DDEBUG -o ctapply ctapply.cc -lm
# FINAL CHECK
echo "Apply to self, so should be close to 0 div and both values the same"
echo "OUT"
echo "CCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCT
AGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGG" | ./ctmodel $K > model.bin
echo "CCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCT
AGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGG" | ./ctapply $K 0 model.bin
echo "CCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCT
AGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGG" | ./ctapply $K 1 model.bin
echo "CCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCT
AGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGG" | ./ctapply $K 2 model.bin
echo "CCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCTCCT
AGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGGAGG" | ./ctapply $K 3 model.bin

