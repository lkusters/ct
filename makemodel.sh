#!/bin/bash
awk '{if(++count%4==2) print $1}' data.txt | ./ctmodel 2 > model.bin
