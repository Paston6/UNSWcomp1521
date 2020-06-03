# COMP1521 18s2 Week 04 Lab
# Compute factorials, no functions


### Global data

    .data
nrows:
    .word 6
ncols:
    .word 12
flag:
    .byte '#', '#', '#', '#', '#', '.', '.', '#', '#', '#', '#', '#'
    .byte '#', '#', '#', '#', '#', '.', '.', '#', '#', '#', '#', '#'
    .byte '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'
    .byte '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'
    .byte '#', '#', '#', '#', '#', '.', '.', '#', '#', '#', '#', '#'
    .byte '#', '#', '#', '#', '#', '.', '.', '#', '#', '#', '#', '#'
eol:
    .asciiz "\n"
    
### main() function
   .text
   .globl main

main:
   # set up stack frame
   sw    $fp, -4($sp)       # push $fp onto stack
   la    $fp, -4($sp)       # set up $fp for this function
   sw    $ra, -4($fp)       # save return address
   sw    $s0, -8($fp)       # save $s0 to use as ... int n;
   sw    $s1, -12($fp)      # save $s1 to use as ... int i;
   sw    $s2, -16($fp)      # save $s2 to use as ... int fac;
   sw    $s3, -20($fp)
   addi  $sp, $sp, -24      # reset $sp to last pushed item

   # code for main()
   lw    $s1, nrows
   lw    $s0, ncols
   li    $s2, 0
   li    $s3, 0

loop1:
   beq   $s2, $s1, exit1    # if i == nrows, exit1
   li    $s3, 0             # row/j = 0
loop2:
   beq   $s3, $s0, exit2    # if j == ncols, exit2

   # printf("%d", flag[i][j])
   # i*ncols+j+flag
   mul $t0, $s2, $s0        # t0 = i*ncols
   add $t0, $t0, $s3        # $t0 = i*ncols+j
   lb  $a0, flag($t0)       # a0 contains flag[i][j]
   li  $v0, 11
   syscall
   add $s3, $s3, 1
   j loop2

exit2:
   #print a \n
   la   $a0, eol
   li   $v0, 4
   syscall
   add $s2, $s2, 1
   j loop1
exit1:
    




   # clean up stack frame
   lw    $s3, -20($fp)
   lw    $s2, -16($fp)      # restore $s2 value
   lw    $s1, -12($fp)      # restore $s1 value
   lw    $s0, -8($fp)       # restore $s0 value
   lw    $ra, -4($fp)       # restore $ra for return
   la    $sp, 4($fp)        # restore $sp (remove stack frame)
   lw    $fp, ($fp)         # restore $fp (remove stack frame)

   li    $v0, 0
   jr    $ra                # return 0



