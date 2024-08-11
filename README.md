# beaver_c
this branch takes in the existing code I had lying around and tries diffrent compilers on it. 
notice how most the time is spend in user space? this means we are cpu bound.

# results 
tcc: 0m1.9s
cproc: 0m1.4s

gcc-14: 0m0.93s

clang-18 0m0.72s
zig cc: same as clang

# notes
seems like arch=native libc versions and similar things make absolutly no effect.
results are fairly consistent maybe a few nanoseconds here or there.

# bugs?
valgrind seems to be angry on static code. seems to be a false alarm.

