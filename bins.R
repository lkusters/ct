f <- file("stdin")
##args <- commandArgs(trailingOnly=TRUE)
##print("binning!")
##file <- "/exports/lgtc/projects/kusters-Eexome/ct/out/div/divee1_1.txt.gz"
##file <- args[1]
open(f)
data <- read.table(f) 
## use scan function ipv read, hoping that it will solve large file size issue
##data <- scan(file, what=double())
##print(dim(data))
h <- hist(data[,1],breaks=seq(-1,4,by=0.01))
cat(h$breaks, "\n")
cat(h$counts, "\n")



