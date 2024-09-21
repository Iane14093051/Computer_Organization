
    "addi x28, x0, 0\n\t" 
    "addi x29, x0, 16\n\t" 
    "addi %[arith_cnt], %[arith_cnt], 2\n\t"
    "loop:\n\t"
    "lw x5, (%[h])\n\t"
    "lw x6, (%[x])\n\t"
    "add x7, x6, x5\n\t"
    "sw x7, (%[y])\n\t"
    "addi %[h], %[h], 2\n\t"
    "addi %[x], %[x], 2\n\t"
    "addi %[y], %[y], 2\n\t"
    "addi %[arith_cnt], %[arith_cnt], 5\n\t"
    "addi %[lw_cnt], %[lw_cnt], 2\n\t"
    "addi %[sw_cnt], %[sw_cnt], 1\n\t"
    "addi %[others_cnt], %[others_cnt], 1\n\t"
    "addi x28, x28,1\n\t"
    "blt x28,x29,loop\n\t"
    

   
