	"addi x29, x0, 16\n\t" 
	"vsetvli x5, x29, e16\n\t"
	
	"vle16.v v0, (%[p_x])\n\t"

	"vle16.v v1, (%[p_x])\n\t"
	
	"vle16.v v2, (%[p_x])\n\t"

	
	"addi %[flag],x0,0\n\t"
	
	"addi x29,x0,8\n\t"

	"addi x6, x0, 0\n\t" 
	
	"loop:\n\t"
	"vadd.vv v3, v0, v2\n\t"
	"vmseq.vx v4, v3, %[target] \n\t"
	"vfirst.m x7, v4\n\t"
	"bge x7,x0,setflag\n\t"
	
	"vadd.vv v3, v1, v2\n\t"
	"vmseq.vx v4, v3, %[target] \n\t"
	"vfirst.m x7, v4\n\t"
	"bge x7,x0,setflag\n\t"
	
	"addi x6, x6,1\n\t"
	"vslidedown.vi v0,v0,1\n\t"
	"addi %[p_x], %[p_x], -2\n\t"
	"vle16.v v1, (%[p_x])\n\t"		
	"blt x6,x29,loop\n\t"
	"beq x0,x0,exit\n\t"
	
	"setflag:\n\t"
	"addi %[flag],x0,1\n\t"
	
	"exit:\n\t"


