# text = 260 B
# static = 12 B
.data
num1: .word 0
num2: .word 0
num3: .word 0
.text
main: 
lw $t0, 0($gp)
addi $t1, $zero, 3
addi $t2, $zero, 3
mult $t1, $t2
mflo $t3
sw $t3, 0($gp)

lw $t3, 4($gp)
addi $t0, $zero, 12
addi $t2, $zero, 4
div $t0, $t2
mflo $t1
sw $t1, 4($gp)

lw $t1, 0($gp)
add $s0, $zero, $t1
jal ass

lw $t1, 0($gp)
add $s0, $zero, $t1
jal co

lw $t1, 4($gp)
lw $t3, 0($gp)
add $s0, $zero, $t1
add $s1, $zero, $t3
jal dv

lw $t3, 8($gp)
addi $t1, $zero, 2
addi $t2, $zero, 3
add $t0, $t1, $t2
addi $t2, $zero, 6
mult $t0, $t2
mflo $t1
addi $t2, $zero, 12
addi $t0, $zero, 8
addi $t4, $zero, 4
sub $t5, $t0, $t4
div $t2, $t5
mflo $t4
sub $t5, $t1, $t4
sw $t5, 8($gp)

lw $t5, 4($gp)
lw $a0, 4($gp)
addi $v0, $zero, 1
syscall

addi $v0, $zero, 10
syscall

ass: 
addi $t5, $zero, 3
add $t3, $s0, $t5
add $s0, $zero, $t3

add $a0, $zero, $s0
addi $v0, $zero, 1
syscall

jr $ra

co: 
addi $t3, $zero, 3
mult $s0, $t3
mflo $t5
add $s0, $zero, $t5

add $a0, $zero, $s0
addi $v0, $zero, 1
syscall

jr $ra

dv: 
add $t5, $s0, $s1
add $s0, $zero, $t5

add $a0, $zero, $s0
addi $v0, $zero, 1
syscall

jr $ra

