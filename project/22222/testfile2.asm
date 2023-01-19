# text = 56 B
# static = 0 B
.data
.text
double: 
addi $t0, $zero, 2
mult $s0, $t0
mflo $t1
add $s0, $zero, $t1

add $a0, $zero, $s0
addi $v0, $zero, 1
syscall

jr $ra

add: 
add $t1, $s0, $s1
add $s0, $zero, $t1

add $a0, $zero, $s0
addi $v0, $zero, 1
syscall

jr $ra

