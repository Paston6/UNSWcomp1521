# COMP1521 18s2 Week 04 Lab
# Compute factorials, no functions


### Global data

    .data
nrows:
    .word 3
ncols:
    .word 4
M:  
    .word 1, 2, 3, 4
    .word 3, 4, 5, 6
    .word 5, 6, 7, 8
factor:
    .word 2
eol:
    .asciiz "\n"
    .align 2
space:
    .asciiz " "
    .align 2
### main() function
   .text
   .globl main

main:
   # set up stack frame
   sw    $fp, -4($sp)       # push $fp onto stack
   la    $fp, -4($sp)       # set up $fp for this function
   sw    $ra, -4($fp)       # save return address
   sw    $s0, -8($fp)       # save $s0 to use as ... ;
   sw    $s1, -12($fp)      # save $s1 to use as ... ;
   sw    $s2, -16($fp)      # save $s2 to use as ... ;
   addi  $sp, $sp, -20      # reset $sp to last pushed item

   # code for main()
   lw $a0, nrows
   lw $a1, ncols
   la $a2, M
   lw $a3, factor

   jal change

   # Print the new matrix
   
   lw   $s2, nrows
   lw   $s3, ncols
   li   $s0, 0         # row = 0
   li   $t4, 4
loop1:                 
   bge  $s0, $s2, end_loop1
   li   $s1, 0         # col = 0
loop2:
   bge  $s1, $s3, end_loop2
   mul  $t0, $s0, $s3  # t0 = row*ncols
   mul  $t0, $t0, $t4  # t0 = row*ncols*4
   mul  $t2, $t4, $s1  # t2 = 4*col  
   add  $t0, $t0, $t2  # convert [row][col] to byte offset
   lw   $a0, M($t0)
   li   $v0, 1        # printf("%d", M[row][col])
   syscall
   la   $a0, space
   li   $v0, 4
   syscall
   addi $s1, $s1, 1    # col++
   j    loop2
end_loop2:
   la   $a0, eol
   li   $v0, 4        # printf("\n")
   syscall
   addi $s0, $s0, 1    # row++
   j    loop1
end_loop1:


   # clean up stack frame
   lw    $s2, -16($fp)      # restore $s2 value
   lw    $s1, -12($fp)      # restore $s1 value
   lw    $s0, -8($fp)       # restore $s0 value
   lw    $ra, -4($fp)       # restore $ra for return
   la    $sp, 4($fp)        # restore $sp (remove stack frame)
   lw    $fp, ($fp)         # restore $fp (remove stack frame)

   li    $v0, 0
   jr    $ra                # return 0



change:

   # params:    nrows is $a0, ncols is $a1, &M is $a2, factor is $a3
   # registers: row is $s0, col is $s1

   # set up stack frame
   sw   $fp, -4($sp)
   la   $fp, -4($sp)
   sw   $ra, -4($fp)
   sw   $s0, -8($fp)
   sw   $s1, -12($fp)
   addi $sp, $sp, -16

   li   $t4, 4           # sizeof(int)
   li   $s0, 0
while1:                   # for (int row = 0; row < nrows; row++)
   bge  $s0, $a0, end1
   li   $s1, 0
while2:                   # for (int col = 0; col < ncols; col++)
   bge  $s1, $a1, end2

   # get &M[row][col]
   mul  $t0, $s0, $a1
   mul  $t0, $t0, $t4    # offset of M[row]  (#bytes)
   mul  $t1, $s1, $t4    # offset within M[row]  (#bytes)
   add  $t0, $t0, $t1    # offset of M[row][col] (#bytes)
   add  $t0, $t0, $a2    # &M[row][col]

   lw   $t1, ($t0)
   mul  $t1, $t1, $a3
   sw   $t1, ($t0)       # M[row][col] = factor * M[row][col]

   addi $s1, $s1, 1
   j    while2
end2:
   addi $s0, $s0, 1
   j    while1
end1:

   # clean up stack frame
   lw   $ra, -4($fp)
   lw   $s0, -8($fp)
   lw   $s1, -12($fp)
   la   $sp, 4($fp)
   lw   $fp, ($fp)

   jr   $ra

