#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    int x = (int) ALUControl;      //turns the ALUControl into an int

    // Checks the ALU control and does the operation based on it
    switch (x) {

    case 0:
        *ALUresult = B+A;       //if it is 0 then we add.
        break;
     case 1:
        *ALUresult = A - B;     //Z = A – B
        break;
    case 2:     
        if ((int) B > (int) A)          //if A < B, Z = 1; otherwise, Z = 0
            *ALUresult = 1;
        else
            *ALUresult = 0;
        break;
    case 3:
        if (A < B)               //if A < B, Z = 1; otherwise, Z = 0 (A and B are unsigned integers)
            *ALUresult = 1;
        else
            *ALUresult = 0;
        break;
    case 4:
        *ALUresult = A & B;
        break;
    case 5:
        *ALUresult = A | B; 
        break;
    case 6:
        *ALUresult = B << 16;       //shifting 16 bits to the left
        break;
    case 7:  
        *ALUresult = ~(A == 1);             
            //if A is true then we put NOT A, 1 -> 0
            //if A is false then we put NOT A, 0 -> 1
        break;

    }

    //....... 3. Assign Zero to 1 if the result is zero; otherwise, assign 0. <--instructions on assignment

    if (*ALUresult == 0)
        *Zero = 1;
    else
        *Zero = 0;
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    // Halt if the program counter is not word-aligned
    if (PC % 4 != 0) {
        return 1;
    }
    // Divide PC by 4 (bit shift by 2) since it is word-aligned. Load into instruction pointer
    *instruction = Mem[PC >> 2];
    return 0;
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    //bits range 31-26
    *(op)=(instruction>>26)&0b111111;
    //bits 25 to 21
    *(r1)=(instruction>>21)&0b11111;
    // bits range 20-16
    *(r2)=(instruction>>16)&0b11111;
    // bits range 15-11
    *(r3)=(instruction>>11)&0b11111;
    // bits range 5-0
    *(funct)=(instruction)&0b111111;
    // bits range 16-0
    *(offset)=(instruction)&0b1111111111111111;
    // bits range 26-0
    *(jsec)=(instruction)&0b11111111111111111111111111;
}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{

}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{   

    //use r1 as an index for array REG and store in in data 1 
    *data1 = Reg[r1];

    //same thing but for data 2 
    *data2 = Reg[r2];
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
    if ((offset >> 15) & 0b1) {
        // If the MSB of the 16 bit offset is 1, extend the 1's to the left
        *extended_value = offset | 0xFFFF0000;
    } else {
        // Otherwise, just extend the zeroes to the left
        *extended_value = offset & 0x0000FFFF;
    }
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{

}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

}

