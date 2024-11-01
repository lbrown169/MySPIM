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
        *ALUresult = ~A;             
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
    // bits range 15-0
    *(offset)=(instruction)&0b1111111111111111;
    // bits range 25-0
    *(jsec)=(instruction)&0b11111111111111111111111111;
}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{
    switch(op)
    {
        // r-type instructions 000
        case 0:
            controls->RegDst = 1;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            //111 is thew ALUop for R-type instructions
            controls->ALUOp=7;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 1;
            return 0;
            break;


        // jump 010
        case 2:
            controls->RegDst = 0;
            controls->Jump = 1;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            // 0 means don't care for ALUOp
            controls->ALUOp=0;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 0;
            return 0;
            break;
        
        // branch equal 100
        case 4:
            controls->RegDst = 2;
            controls->Jump = 0;
            controls->Branch = 1;
            controls->MemRead = 0;
            controls->MemtoReg = 2;
            controls->ALUOp=1;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 0;
            return 0;
            break;
        
        // add immediate 1000
        case 8:
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp=0;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            return 0;
            break;
        
        // Set less than Immediate 001 010
        case 10:
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp=2;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 1;
            return 0;
            break;

        // Set less than Immediate unsigned 001 011
        case 11:
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp=3;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 1;
            return 0;
            break;

        

        // load word 10 0011
        case 35:
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 1;
            controls->MemtoReg = 1;
            controls->ALUOp=0;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            return 0;
            break;
            

        // store word 10 1011
        case 43:
            controls->RegDst = 2;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 2;
            controls->ALUOp=0;
            controls->MemWrite = 1;
            controls->ALUSrc = 1;
            controls->RegWrite = 0;
            return 0;
            break;

        default:
            return 1;

    }
    return 1;
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
    // There is already an ALU operation defined from instruction_decode. We'll pass that
    if (ALUOp != 0) {
        if (ALUSrc == 1) {
            ALU(data1, extended_value, ALUOp, ALUresult, Zero);
        } else {
            ALU(data1, data2, ALUOp, ALUresult, Zero);
        }
    } else {
        switch (funct) {
            // add
            case 0b100000:
                ALU(data1, data2, 000, ALUresult, Zero);
                break;
            // subtract
            case 0b100010:
                ALU(data1, data2, 001, ALUresult, Zero);
                break;
            // and
            case 0b100100:
                ALU(data1, data2, 100, ALUresult, Zero);
                break;
            // or
            case 0b100101:
                ALU(data1, data2, 101, ALUresult, Zero);
                break;
            // slt
            case 0b101010:
                ALU(data1, data2, 010, ALUresult, Zero);
                break;
            // sltu
            case 0b101011:
                ALU(data1, data2, 011, ALUresult, Zero);
                break;
            // No r-type instruction matched. Halt
            default:
                return 1;
                break;
        }
    }
    // Reached end. No halt occurred
    return 0;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
    if (MemRead == 1) {
        // Halt if not word aligned
        if (ALUresult % 4 != 0) return 1;
        // Bit shift ALUresult to what we need it for Mem, read into memdata
        *memdata = Mem[ALUresult >> 2];
    } else if (MemWrite == 1) {
        // Halt if not word aligned
        if (ALUresult % 4 != 0) return 1;
        // Bit shift ALUresult like above, but now write to Mem
        Mem[ALUresult >> 2] = data2;
    }
    return 0;
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
    if(RegWrite == 1 && MemtoReg == 1){
        //data is coming from memory
         if (RegDst == 1) { 
            Reg[r3] = memdata;
        }  
        else
            Reg[r2] = memdata;
    }
    else if(RegWrite == 1 && MemtoReg == 0){
        //data is coming from ALU result
        if (RegDst == 1) { 
            Reg[r3] = ALUresult;
        } 
        else 
            Reg[r2] = ALUresult;
    }
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
    *PC = *PC + 4;
    if (Branch = 1 && Zero == 1) {
        *PC = *PC + (extended_value << 2);
    } else if (Jump == 1) {
        // Create PC using the jsec bit shifted twice, and grabbing the upper 4 buts of PC
        *PC = (jsec << 2) | (PC & 0xF0000000);
    }
}

