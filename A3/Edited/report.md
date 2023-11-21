#Assignment 3
Authors:
Atharva Gangal
Benedikt Heuser

##Part 1
###(Answers 1 and 2)
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

##Part 2
#### Memory-Reorderings (Answer 3)

On producing a dump of the assembly code, we identify the following instructions:

In thread1Func,
movl	$1, X(%rip)   (X=1)  (S0)
movl	Y(%rip), %eax (r1=Y) (L0)
movl	%eax, r1(%rip)

In thread2Func,
movl	$1, Y(%rip)   (Y=1)  (S1)
movl	X(%rip), %eax (r2=Y) (L1)
movl	%eax, r2(%rip)

Reorderings happen when the out-of-order processor runs L0 AND L1 BEFORE S0 AND S1. As a result, the load into r1 and r2 get old values of the variables (0), leading to both r1 and r2 printing out 0.

We modified them by adding a memory fence between the two instructions using the command ```asm volatile("mfence" ::: "memory")```, so that S0 happens before L0 and S1 happens before L1.

New thread1Func,
movl	$1, X(%rip)
#APP
# 18 "order.c" 1
mfence
# 0 "" 2
#NO_APP
movl	Y(%rip), %eax
movl	%eax, r1(%rip)

New thread2Func,
movl	$1, Y(%rip)
#APP
# 32 "order.c" 1
mfence
# 0 "" 2
#NO_APP
movl	X(%rip), %eax
movl	%eax, r2(%rip)

In the hardware, these cause the changes to be visible before the loads happen, through various mechanisms like flushing the entire store buffer before the fence instructions completes. As a result, the latest values are available and reorderings are not detected. We re-run ```./order``` with the fence and verify that there are 0 reorderings.

#### NUMA (Answer 4)
