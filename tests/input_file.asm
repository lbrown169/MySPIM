21080001	# addi $8, $8, 1		r8 = 1      			$t0 = 1 
21090002	# addi $9, $8, 2		r9 = r8 + 2 = 3			$t1 = 3
01095020	# add $10, $8, $9		r10 = r8 + r9 = 4		$t2 = 4
afaa0000	# sw $10, 0($29)        	Mem[sp+0] = r10			
8fab0000	# lw $11, 0($29) 	      	r11= Mem[sp+0] = 4		$t3 = 4
016a602a	# slt $12, $11, $10    		if(r11<r10) then r12 = 1	$t4 = 0
012a682b	# sltu $13, $9, $10  		if(r9<r10) then r13 = 1		$t5 = 1
3c0e0020	# lui $14, 32							$t6 = 0x00200000
0800100b	# j end		      		jump over the bad instructions
deadbeaf	# label: bad instr
badabeaf	# bad instr
114bfffd	# end: beq $10, $11, label   	(r10==r11) will branch back and halt 
