	.text
	.align	2
	.global	mcount
	.type	mcount,function           
mcount:
	move.w  %sr,%d0
	move.w  #0x2000, %sr
	move.w  #0x9C00, 0xC00004
	move.w  %d0, %sr
	rts

	.align	2   
	.global	monstartup
	.type	monstartup,function        
monstartup:
	move.w  #0x9C01, 0xC00004
	rts

	.align 2
	.global moncontrol
	.type   moncontrol,function
moncontrol:
	move.w  #0x9C02, 0xC00004
	rts

	.align 2
	.global moncleanup
	.type   moncleanup,function    
moncleanup:
	move.w  #0x9C03, 0xC00004
	rts

	.align 2
	.global gens_log
	.type   gens_log,function 
gens_log:
	move.w  #0x9C04, 0xC00004
	rts
