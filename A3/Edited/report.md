#Assignment 3
Authors:
Atharva Gangal
Benedikt Heuser

##Part 1
####Before Deoptimization

When we run our ```execute_numa.sh``` script on the original ```numa.c``` code we observe only very small differences in the runtimes between the three policies. The runtime for the remote policy is consistantly between 0.05 and 0.1 seconds slower than the others. For 8589934592 accesses the runtimes are as follows:

- Local: 2.798 seconds (3.258e-10 s/a)
- Interleaved: 2.778 seconds (3.234e-10 s/a)
- Remote: 2.866 seconds (3.336e-10 s/a)

####After Deoptimization
Deoptimizations in ```numa.c```:
1. Next_addr: We changed the ```next_addr()``` function to ```return 64 * (arr[i] + 1)```. By basing the calculation of the next index on an entry in the array, we can ensure that the next index can't be computed before the last one because the processor needs to know where to look in the array. It also introduces randomness in the access pattern since the array entries are random numbers. We add one to make sure that we don't access the same index twice and we multiply by 64 to prevent the make the increments larger to avoid the benefit from caching. This change had the largest impact on the access time.
2. Init_array: We canged the initialization of the array such that the entries in the array are randomly generated on line 18 so we can use this randomness in the next_addr function. We also increment variable i by a random amount based on the value generated to prevent precalculation. We found this increased the access time by a fair amount.

With these deoptimizations we recieved the following runtimes for 67119765 accesses:

- Local: 1.428 seconds (2.127e-08 seconds per access)
- Interleaved: 1.77 seconds (2.637e-08 seconds per access)
- Remote: 2.182 seconds (3.251e-08 seconds per access)

We can clearly see now that the effect of NUMA. The remote policy has the longes access times and local the shortest while interleaved is a mix of the two.

