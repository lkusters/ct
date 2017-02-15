args <- commandArgs(trailingOnly=TRUE)
##print("binning!")
##file <- "/exports/lgtc/projects/kusters-Eexome/ct/out/div/divee1_1.txt.gz"
file <- args[1]
data <- read.table(file) 
## use scan function ipv read, hoping that it will solve large file size issue
##data <- scan(file, what=double())
##print(dim(data))
h <- hist(data[,1])
cat(h$breaks, "\n")
cat(h$counts, "\n")



