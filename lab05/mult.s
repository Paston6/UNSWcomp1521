# COMP1521 18s1 Week 05 Lab

# XING XING z5142063

# void multMatrices(int n, int m, int p,
#                   int A[n][m], int B[m][p], int C[n][p])
# {
#    for (int r = 0; r < n; r++) {
#       for (int c = 0; c < p; c++) {
#          int sum = 0;
#          for (int i = 0; i < m; i++) {
#             sum += A[r][i] * B[i][c];
#          }
#          C[r][c] = sum;
#       }
#    }
# }
#$a0 $a1 $a2 $a4
#stack
   .text
   .globl multMatrices
multMatrices:
   # possible register usage:
   # n is $s0, m is $s1, p is $s2,
   # r is $s3, c is $s4, i is $s5, sum is $s6
   #=========================================================
   # set up stack frame for multMatrices()
   sw $fp, -4($sp)
   la $fp, -4($sp)
   sw $ra, -8($sp)
   sw $s0, -12($sp)
   sw $s1, -16($sp)
   sw $s2, -20($sp)
   sw $s3, -24($sp)
   sw $s4, -28($sp)
   sw $s5, -32($sp)
   sw $s6, -36($sp)
   addi $sp, $sp, -36
   #=========================================================

   #=========================================================
   # mulMatrices() function
   addi  $t0, $fp, 12       # $t0 = A[r][i]  nm
   addi  $t1, $fp, 8        # $t1 = B[i][c]  mp
   addi  $t2, $fp, 4        # $t2 = C[r][c]  np

   move  $s0, $a0           # $s0 = n
   move  $s1, $a1           # $s1 = m
   move  $s2, $a2           # $s2 = p

   li    $s3, 0             # $s3 = r
   li    $s4, 0             # $s4 = c
   li    $s5, 0             # $s5 = i
   li    $s6, 0             # $s6 = sum

   li    $t3, 4             # int
   
loop:                       # for (int r = 0; r < n; r++)
        bge $s3, $s0, exit
        li  $s4, 0
loop1:                      # for (int c = 0; c < p; c++)
        bge $s4, $s2, exit1
        li  $s5 , 0         # i = 0
        li  $s6 , 0         # sum = 0
loop2:                      # for (int i = 0; i < m; i++)
        bge $s5, $s1, exit2

        #get a
        lw  $t0, 12($fp)
        mul $t4, $s3, $s1   #temp = r * m
        add $t4, $t4, $s5   #temp = r * m + i
        mul $t4, $t4, $t3   #temp = ((r * n) + i) * 4
        add $t0, $t4, $t0   #&A = A + ((r * n) + i) * 4
        lw  $t0, ($t0)

        #get b
        lw  $t1, 8($fp)
        mul $t4, $s5, $s2   #temp = i * p
        add $t4, $t4, $s4   #temp = i * p + c
        mul $t4, $t4, $t3   #temp = ((i * p) + c) * 4
        add $t1, $t4, $t1   #&B = B + ((i * p) + i) * 4
        lw  $t1, ($t1)

        mul $t5, $t1, $t0   #$t5 = A * B
        add $s6, $s6, $t5   #sum = sum + $t5


        addi $s5, $s5, 1    # r++
        j loop2
exit2:

        lw  $t2, 4($fp)     # load C[][]
        mul $t4, $s3, $s2   #temp = r * p
        add $t4, $t4, $s4   #temp = r * p + c
        mul $t4, $t4, $t3   #temp = ((r * p) + c) * 4
        add $t2, $t4, $t2   #&C = C + ((r * p) + c) * 4
        sw  , $s6, ($t2)


        addi $s4, $s4, 1    # c++
        j loop1
exit1:

        addi $s3, $s3, 1    # i++
        j loop
   
exit:
   #=========================================================

   #=========================================================
   # clean up stack frame for multMatices()
    la $sp, 4($fp)
    lw $ra, -4($fp)
    lw $s0, -8($fp)
    lw $s1, -12($fp)
    lw $s2, -16($fp)
    lw $s3, -20($fp)
    lw $s4, -24($fp)
    lw $s5, -28($fp)
    lw $s6, -32($fp)
    lw $fp, ($fp)
    jr $ra
   #=========================================================