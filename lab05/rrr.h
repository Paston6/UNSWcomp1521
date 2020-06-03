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

#get a
lw  $t0, 12($fp)
li  $t4, 0          #load $t4 0
mul $t4, $s3, $s1   #temp = r * m
add $t4, $t4, $s5   #temp = r * m + i
mul $t4, $t4, $t3   #temp = ((r * n) + i) * 4
add $t0, $t4, $t0   #&A = A + ((r * n) + i) * 4
lw  $t0, ($t0)


# sum += A[r][i] * B[i][c];
li $t4, 0
mul $t4, $t0, $t1   #A[r][i] * B[i][c]
add $s6, $s6, $t4   #sum = sum + A[r][i] * B[i][c]






#get c

lw  $t2, 4($fp)
li  $t4, 0          #load $t4 0
mul $t4, $s3, $s2   #temp = r * p
add $t4, $t4, $s4   #temp = r * p + c
mul $t4, $t4, $t3   #temp = ((r * p) + c) * 4
add $t2, $t4, $t2   #&C = C + ((r * p) + c) * 4

sw  $s6,($t2)     #&C = sum