.global mult
.global divide
.global rand_seed
.global rand
.global power

.data
rnd_state: .word 0, 0, 0, 0

.text

## Plagiarism alert
## Translated from ast.cpp (expt)
power:
    ori $t0, $zero, 1 # r = 1
.power_while:
    seq $t1, $zero, $a1 # is q == 0 -> $t1
    beq $t1, $zero, .power_end_while # if not false get out
    nop # account for branch delay
.power_while_if:
    sw $a0, 0($sp) # storing p
    sw $a1, 4($sp) # storing q
    sw $t0, 8($sp) # storing r
    move $a0, $a1 # setting up dividend as q
    li $a1, 2 # setting up divisor as 2
    addi $a2, $sp, 12 # poiting target to quotient
    addi  $a3, $sp, 16 # poiting target to remainder
    jal divide # calling integer division
    lw $a0, 0($sp) # acquiring p
    lw $a1, 4($sp) # acquiring q
    lw $t0, 8($sp) # acquiring r
    lw $t1, 16($sp) # acquiring the remainder
    addi $t2, $zero, 1 # setting up 1
    seq $t1, $t1, $t2 # comparing modulo to 1
    beq $t1, $zero, .power_while_endif
    sw $a0, 0($sp) # storing p
    sw $a1, 4($sp) # storing q
    sw $t0, 8($sp) # storing r
    move $a0, $t0 # setting up r as multiplicant
    move $a1, $a0 # setting up p as factor
    jal mult
    lw $a0, 0($sp) # acquiring p
    lw $a1, 4($sp) # acquiring q
    lw $t0, 8($sp) # acquiring r
    move $t0, $v0 # setting r = r times p
    addi $t2, $zero, 1 # setting up 1
    subu $a1, $a1, $t2 # substracting 1 from q

.power_while_endif:
    sw $a0, 0($sp) # storing p
    sw $a1, 4($sp) # storing q
    sw $t0, 8($sp) # storing r
    move $a0, $a0 # setting up p as multiplicant
    move $a1, $a0 # setting up p as factor
    jal mult
    lw $a0, 0($sp) # acquiring p
    lw $a1, 4($sp) # acquiring q
    lw $t0, 8($sp) # acquiring r
    move $a0, $v0 # setting r = r times p
    srl $a1, $a1, 1 # dividing q over 2
    j .power_while

.power_end_while:
    move $v0, $t0
    jr $ra

divide:
    move $t1, $zero
.while:
    slt $t0, $a0, $a1
    bnez $t0, .end_while
    sub $a0, $a0, $a1
    addi $t1, $t1, 1
    j .while

.end_while:
    sw $t1, 0($a2)
    sw $a0, 0($a3)
    jr $ra

mult:
    move $t0, $zero
    move $v0, $zero
.mult_loop:
    sltu $t1, $t0, $a0
    beqz $t1, .mult_end
    add $v0, $v0, $a1
    addi $t0, $t0, 1
    j .mult_loop
.mult_end:
	jr $ra

#$a0: seed
rand_seed:
	la $t0, rnd_state
	sw $a0, 0($t0) ##rnd_state[0]
	addiu $t1, $a0, 1
	sw $t1, 4($t0) #rnd_state[1]
	addiu $t1, $a0, 2
	sw $t1, 8($t0) #rnd_state[2]
	addiu $t1, $a0, 3
	sw $t1, 12($t0) #rnd_state[3]
	jr $ra

rand:
	la $t0, rnd_state
	lw $t1, 12($t0)
	sll $t2, $t1, 11
	xor $t1, $t1, $t2
	srl $t2, $t1, 8
	xor $t1, $t1, $t2

	lw $t2, 8($t0)
	sw $t2, 12($t0)

	lw $t2, 4($t0)
	sw $t2, 8($t0)

	lw $t2, 0($t0)
	sw $t2, 4($t0)

	xor $t1, $t1, $t2
	srl $t2, $t2, 19
	xor $t1, $t1, $t2

	sw $t1, 0($t0)
	li $t0, 0x000fffff
	and $v0, $t1, $t0
	# move $v0, $t1
	jr $ra
