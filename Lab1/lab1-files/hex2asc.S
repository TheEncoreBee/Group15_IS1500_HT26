  # hexmain.S
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

	.text
	# Declare hexasc global too so it can be called from another file
	# (e.g. timetemplate.S, or the auto-grader's test harness).
	.globl hex2asc, hexasc

hex2asc:
	li	a0, 15		# test number (from 0 to 15)
	
	addi    sp,sp,-4
	sw      ra,0(sp)
	
	jal	hexasc		# call hexasc
	
	li	a7, 11	# write a0 to stdout
	ecall

	lw      ra,0(sp)
	addi    sp,sp,4
	jr      ra	

  # You can write your own code for hexasc here
  #
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
  
  	
  
