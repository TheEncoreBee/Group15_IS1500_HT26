# timetemplate.S
# Written 2015 by F Lundevall
# Ported 2024/06 by W Szczerek (from MIPS to RISC-V)
# Copyright abandonded - this file is in the public domain.

#############################################################
# PUSH / POP macros.                                        #
#                                                           #
# RARS and the GNU assembler (used for the DTEK-V board)    #
# use slightly different macro syntax. The file ships with  #
# the RARS-style block active, because Assignments 1-4 are  #
# done in RARS. When you port to the board in Assignment 5, #
# comment out the RARS block below and uncomment the DTEK   #
# block beneath it.                                         #
#############################################################
###################
# Macros for RARS #
###################
#.macro	PUSH (%reg)
#	addi	sp,sp,-4
#	sw	%reg,0(sp)
#.end_macro

#.macro	POP (%reg)
#	lw	%reg,0(sp)
#	addi	sp,sp,4
#.end_macro
###################
# Macros for DTEK #
###################
.macro	PUSH reg
	addi sp,sp,-4
	sw \reg,0(sp)
.endm

.macro	POP reg
	lw	\reg,0(sp)
	addi	sp,sp,4
.endm
#############################################################

	.data
	.align 2
mytime:	.word 	0x5957
timstr:	.asciz 	"text more text lots of text\0"
	.text
	# delay is declared global so the auto-grader's test harness (which
	# calls delay directly from a small labmain.S) can resolve the symbol.
	.globl timetemplate, tick, time2string, delay

# Function for displaying a string with a newline at the end	
display_string:	
	li a7,4
	ecall
	li a0, 10
	li a7,11
	ecall
	jr ra
	
timetemplate:
	la	a0, timstr
	jal     display_string
	
	# wait a little
	li	a0, 1000		# ms
	jal	delay
	
	# call tick
	la 	a0, mytime
	jal	tick
	
	# call your function time2string
	la	a0, timstr
	la	t0, mytime
	lw	a1, 0(t0)
	jal	time2string
	
	# go back and do it all again
	j	timetemplate
	

	

	
# tick: update time pointed to by $a0
tick:	lw	t0, 0(a0)	# get time
	addi	t0, t0, 1	# increase
	andi	t1, t0, 0xf	# check lowest digit
	sltiu	t2, t1, 0xa	# if digit < a, okay
	bnez	t2, tiend
	addi	t0, t0, 0x6	# adjust lowest digit
	
	andi	t1, t0, 0xf0	# check next digit
	sltiu	t2, t1, 0x60	# if digit < 6, okay
	bnez	t2, tiend
	addi	t0, t0, 0xa0	# adjust digit
	
	li	t3, 0xF
	slli	t3, t3, 0x8
	and	t1, t0, t3	# check minute digit
	addi	t3, x0, 0xA
	slli	t3, t3, 0x8
	slt	t2, t1, t3	# if digit < a, okay
	bnez	t2, tiend
	addi	t0, t0, 0x600	# adjust digit - this one's okay, it's lower than 0x7FF 
	
	li	t3, 0xF
	slli	t3, t3, 0xC
	and	t1, t0, t3	# check last digit
	addi	t3, x0, 0x6
	slli	t3, t3, 0xC
	slt	t2, t1, t3	# if digit < 6, okay
	bnez	t2, tiend
	
	li	t3, 0xA
	slli	t3, t3, 0xC
	add	t0, t0, t3	# adjust last digit
tiend:	sw	t0,0(a0)	# save updated result
	jr	ra		# return
delay: #delay subroutine
	PUSH a0
	li t0, 0 #int i = 0, starting condition in the for loop
	li t1, 0 #the constant 0 in the while condition
	li t2, 1 #the incrementing value in both the while- and for loops
	li t3, 10250
	j while
	
while:		
	ble a0, t1, while_done
	sub a0, a0, t2
	j for
for:
	bge t0, t3, for_done
	add t0, t0, t2
	j for
	
	
while_done:
	POP a0
	jr ra
	
for_done:
	seqz t0, t0
	j while 
#########################################################
# Place for your functions: time2string, hexasc, delay. #
#########################################################

hexasc:
  	slli t0, a0, 28 #delete all but the 4 most insignificant bits
  	srli t1, t0, 28
  	addi t2, zero, 10 #initialize value 10
  	bge t1, t2, letter # if the argument is 10 or greater, the output will be A to F 
  	addi a0, t1, 48
  	
  	jr ra
  letter: 
  	addi a0, t1, 55
  	jr ra

#time2string takes an adress a0 and the timebytes in a1. 
time2string:
	PUSH ra #pushing this instance of return adress and a0 to the stack so we can restore them after the hexasc call.
	
	PUSH a0
	srli a0, a1, 12
	jal hexasc
	mv t1, a0
	POP a0 #restoring a0 to load the characters from hexasc.
	sb t1, 0(a0)
	
	PUSH a0
	srli a0, a1, 8
	jal hexasc
	mv t1, a0
	POP a0
	sb t1, 1(a0)
	
	li t1, 0x3A
	sb t1, 2(a0) #store the semicolonn
	
	PUSH a0
	srli a0, a1, 4
	jal hexasc
	mv t1, a0
	POP a0
	sb t1, 3(a0)
	
	PUSH a0
	mv a0, a1
	jal hexasc
	mv t1, a0
	POP a0
	sb t1, 4(a0)
	
	li t1, 0x00 
	sb t1, 5(a0) #store the null byte
	
	POP ra #restore the return address and return
	jr ra
	

	
	
	
