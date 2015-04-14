
/* 

The project is developed as part of Computer Architecture class
Project Name: Functional Simulator for subset of ARM Processor

Developer's Name:
Developer's Email id:
Date: 

*/


/* myARMSim.cpp
   Purpose of this file: implementation file for myARMSim
*/

#include "myARMSim.h"
#include <stdlib.h>
#include <stdio.h>

//Register file
static unsigned int R[16];
//flags
static int N,C,V,Z;
//memory
static unsigned char MEM[4000];

//intermediate datapath and control path signals
static unsigned int instruction_word;
static unsigned int operation_type;
static unsigned int operation;
static unsigned int rn;
static unsigned int rd;
static unsigned int rm;
static int operand1;
static int operand2;
static int result;
static int BITMASK = 0xff000000;

void run_armsim() {
  int i;
  while (1) {
    fetch();
    decode();
    execute();
    mem();
    write_back();
    printf("\n");
  }
}

//utility function to read msb to lsb bits from an int x
int read_bits(unsigned int x, int lsb, int msb) {
  int bits = (x >> lsb) & ~(~0 << (msb-lsb+1));
  return bits;
}

// it is used to set the reset values
//reset all registers and memory content to 0
void reset_proc() {
  int i;
  for (i=0; i<16; i++) {
    R[i] = 0;
  }
  for (i=0; i<4000; i++) {
    MEM[i] = 0;
  }
}

//load_program_memory reads the input memory, and pupulates the instruction 
// memory
void load_program_memory(char *file_name) {
  FILE *fp;
  unsigned int address, instruction;
  fp = fopen(file_name, "r");
  if(fp == NULL) {
    printf("Error opening input mem file\n");
    exit(1);
  }
  while(fscanf(fp, "%x %x", &address, &instruction) != EOF) {
    write_word(MEM, address, instruction);
  }
  fclose(fp);
}

//writes the data memory in "data_out.mem" file
void write_data_memory() {
  FILE *fp;
  unsigned int i;
  fp = fopen("data_out.mem", "w");
  if(fp == NULL) {
    printf("Error opening dataout.mem file for writing\n");
    return;
  }
  
  for(i=0; i < 4000; i = i+4){
    fprintf(fp, "0x%x 0x%x\n", i, read_word(MEM, i));
  }
  fclose(fp);
}

//should be called when instruction is swi_exit
void swi_exit() {
  write_data_memory();
  exit(0);
}


//reads from the instruction memory and updates the instruction register
void fetch() {
  instruction_word = read_word(MEM, R[15]);
  printf("FETCH: fetch instruction 0x%x from address 0x%x\n", instruction_word, R[15]);
}

//reads the instruction register, reads operand1, operand2 from register file, decides the operation to be performed in execute stage
void decode() {
  operation_type = read_bits(instruction_word, 26, 27);

  //dp instructions
  if (operation_type==0) {
    operation = read_bits(instruction_word, 21, 24);

    //non-immediate operations
    if (read_bits(instruction_word, 25, 25)==0) {
      //register numbers
      rm = read_bits(instruction_word, 0, 3);
      rn = read_bits(instruction_word, 16, 19);
      rd = read_bits(instruction_word, 12, 15);
      //operand values
      operand2 = R[rm];
      if ((operation!=13)&&(operation!=15)) {
        operand1 = R[rn];
      }
      else {
        operand1 = 0;
      }
     
     //what operation 
     if (operation==0) {
      printf("DECODE: Operation is AND, first operand R%d, second operand R%d, destination R%d\n", rn, rm, rd);
      printf("DECODE: Read Registers: R%d = %d, R%d = %d\n", rn, operand1, rm, operand2);
     }
     else if (operation==1) {
      printf("DECODE: Operation is EOR, first operand R%d, second operand R%d, destination R%d\n", rn, rm, rd);
      printf("DECODE: Read Registers: R%d = %d, R%d = %d\n", rn, operand1, rm, operand2);
     }
     else if (operation==2) {
      printf("DECODE: Operation is SUBTRACT, first operand R%d, second operand R%d, destination R%d\n", rn, rm, rd);
      printf("DECODE: Read Registers: R%d = %d, R%d = %d\n", rn, operand1, rm, operand2);
     }
     else if (operation==4) {
      printf("DECODE: Operation is ADD, first operand R%d, second operand R%d, destination R%d\n", rn, rm, rd);
      printf("DECODE: Read Registers: R%d = %d, R%d = %d\n", rn, operand1, rm, operand2);
     }
     else if (operation==5) {
      printf("DECODE: Operation is ADC, first operand R%d, second operand R%d, destination R%d\n", rn, rm, rd);
      printf("DECODE: Read Registers: R%d = %d, R%d = %d\n", rn, operand1, rm, operand2);
     }
     else if (operation==10) {
      printf("DECODE: Operation is CMP, first operand R%d, second operand R%d\n", rn, rm);
      printf("DECODE: Read Registers: R%d = %d, R%d = %d\n", rn, operand1, rm, operand2);
     }
     else if (operation==12) {
      printf("DECODE: Operation is ORR, first operand R%d, second operand R%d, destination R%d\n", rn, rm, rd);
      printf("DECODE: Read Registers: R%d = %d, R%d = %d\n", rn, operand1, rm, operand2);
     }
     else if (operation==13) {
      printf("DECODE: Operation is MOV, operand R%d, destination R%d\n", rm, rd);
      printf("DECODE: Read Registers: R%d = %d\n", rm, operand2);
     }
     else if (operation==15) {
      printf("DECODE: Operation is MVN operand R%d, destination R%d\n", rm, rd);
      printf("DECODE: Read Registers: R%d = %d", rm, operand2);
     }
    }

    //immediate operations
    else if (read_bits(instruction_word, 25, 25)==1) {
      //register numbers
      rn = read_bits(instruction_word, 16, 19);
      rd = read_bits(instruction_word, 12, 15);
      //operand values
      operand2 = read_bits(instruction_word, 0, 11);
      if ((operation!=13)&&(operation!=15)) {
        operand1 = R[rn];
      }
      else {
        operand1 = 0;
      }
     
     //what operation 
     if (operation==0) {
      printf("DECODE: Operation is AND, first operand R%d, second operand immediate, destination R%d\n", rn, rd);
      printf("DECODE: Read Registers: R%d = %d, immediate = %d\n", rn, operand1, operand2);
     }
     else if (operation==1) {
      printf("DECODE: Operation is EOR, first operand R%d, second operand immediate, destination R%d\n", rn, rd);
      printf("DECODE: Read Registers: R%d = %d, immediate = %d\n", rn, operand1, operand2);
     }
     else if (operation==2) {
      printf("DECODE: Operation is SUBTRACT, first operand R%d, second operand immediate, destination R%d\n", rn, rd);
      printf("DECODE: Read Registers: R%d = %d, immediate = %d\n", rn, operand1, operand2);
     }
     else if (operation==4) {
      printf("DECODE: Operation is ADD, first operand R%d, second operand immediate, destination R%d\n", rn, rd);
      printf("DECODE: Read Registers: R%d = %d, immediate = %d\n", rn, operand1, operand2);
     }
     else if (operation==5) {
      printf("DECODE: Operation is ADC, first operand R%d, second operand immediate, destination R%d\n", rn, rd);
      printf("DECODE: Read Registers: R%d = %d, immediate = %d\n", rn, operand1, operand2);
     }
     else if (operation==10) {
      printf("DECODE: Operation is CMP, first operand R%d, second operand immediate\n", rn);
      printf("DECODE: Read Registers: R%d = %d, immediate = %d\n", rn, operand1, operand2);
     }
     else if (operation==12) {
      printf("DECODE: Operation is ORR, first operand R%d, second operand immediate, destination R%d\n", rn, rd);
      printf("DECODE: Read Registers: R%d = %d, immediate = %d\n", rn, operand1, operand2);
     }
     else if (operation==13) {
      printf("DECODE: Operation is MOV, operand is immediate, destination R%d\n", rd);
      printf("DECODE: immediate = %d\n", operand2);
     }
     else if (operation==15) {
      printf("DECODE: Operation is MVN, operand is immediate, destination R%d\n", rd);
      printf("DECODE: immediate = %d\n", operand2);
     }
    }
  }

  //dt instructions
  else if (operation_type==1) {
    operation = read_bits(instruction_word, 20, 25);
    //set up register numbers
    rn = read_bits(instruction_word, 16, 19);
    rd = read_bits(instruction_word, 12, 15);
    //set up operand values
    operand1 = R[rn];
    operand2 = read_bits(instruction_word, 0, 11);

    //ldr
    if (operation==25) {
      printf("DECODE: Operation is LDR, base address is in R%d, offset is immediate, destination register is R%d\n", rn, rd);
      printf("DECODE: R%d = 0x%x, Offset = %d bytes\n", rn, operand1, operand2);      
    }
    //str
    if (operation==24) {
      printf("DECODE: Operation is STR, base address is in R%d, offset is immediate, register to be stores in memory is R%d\n", rn, rd);
      printf("DECODE: R%d = 0x%x, Offset = %d bytes\n", rn, operand1, operand2);
    }
  }

  //branch instructions
  else if (operation_type==2) {
    operation = read_bits(instruction_word, 24, 25);
    //branch
    if (operation==2) {
      int condition = read_bits(instruction_word, 28, 31);
      if (condition==0) {
        printf("DECODE: Operation is BEQ\n");
      }
      else if (condition==1) {
        printf("DECODE: Operation is BNE\n");
      }
      else if (condition==11) {
        printf("DECODE: Operation is BLT\n");
      }
      else if (condition==12) {
        printf("DECODE: Operation is BGT\n");
      }
      else if (condition==13) {
        printf("DECODE: Operation is BLE\n");
      }
      else if (condition==10) {
        printf("DECODE: Operation is BGE\n");
      }
      else if (condition==14) {
        printf("DECODE: Operation is Unconditional Branch\n");
      }
    }
    //exception
    else {

    }
  }

  //exit
  else if (operation_type==3) {
    printf("Reached end. Now exiting.\n");
  }

  //exception
  else {

  }
}


//executes the ALU operations based on ALUop
void execute() {

  //dp instructions
  if (operation_type==0) {
    if (operation==0) {
      result = operand1 & operand2;
      printf("EXECUTE: Bitwise AND %d and %d\n", operand1, operand2);
    }
    if (operation==1) {
      result = operand1 ^ operand2;
      printf("EXECUTE: EOR %d and %d\n", operand1, operand2);
    }
    if (operation==2) {
      result = operand1 - operand2;
      printf("EXECUTE: SUBRACT %d from %d\n", operand2, operand1);
    }
    if (operation==4) {
      result = operand1 + operand2;
      printf("EXECUTE: ADD %d and %d\n", operand1, operand2);
    }
    if (operation==5) {
      result = operand1 + operand2;
      printf("EXECUTE: ADD with Carry %d and %d\n", operand1, operand2);
    }
    if (operation==10) {
      result = operand1 - operand2;
      if (result==0) {
        Z=1;
      }
      else {
        Z=0;
      }
      if (result<0) {
        N=1;
      }
      else {
        N=0;
      }
      printf("EXECUTE: CMP %d and %d. Updating Flags.\n", operand1, operand2);
      printf("N=%d, Z=%d\n", N, Z);
    }
    if (operation==12) {
      result = operand1 | operand2;
      printf("EXECUTE: ORR %d and %d\n", operand1, operand2);
    }
    if (operation==13) {
      result = operand2;
      printf("EXECUTE: MOV %d\n", operand2);
    }
    if (operation==15) {
      result = ~operand2;
      printf("EXECUTE: MVN %d\n", operand2);
    }
    R[15] = R[15] + 4;
  }

  //dt instructions
  else if (operation_type==1) {
    //ldr or str
    if ((operation==25) || (operation==24)) {
      result = operand1 + operand2;
      printf("EXECUTE: Calculate address 0x%x + 0x%x\n", operand1, operand2);
    }
    else {

    }
    R[15] = R[15] + 4;
  }

  //branch instructions
  else if (operation_type==2) {
    int temp = read_bits(instruction_word, 0, 23);
    result = (temp>>23==0) ? temp:(temp|BITMASK);
    result = result*4;
    int condition = read_bits(instruction_word, 28, 31);
    if (condition==0) {
      if (Z==1) {
        R[15] = R[15] + result;
        return;
      }
    }
    else if (condition==1) {
      if (Z!=1) {
        printf("EXECUTE: Branch taken\n");
        R[15] = R[15] + result;
        return;
      }
    }
    else if (condition==11) {
      if (N==1) {
        printf("EXECUTE: Branch taken\n");
        R[15] = R[15] + result;
        return;
      }
    }
    else if (condition==13) {
      if ((N==1)||(Z==1)) {
        printf("EXECUTE: Branch taken\n");
        R[15] = R[15] + result;
        return;
      }
    }
    else if (condition==12) {
      if ((N!=1)&&(Z!=1)) {
        printf("EXECUTE: Branch taken\n");
        R[15] = R[15] + result;
        return;
      }
    }
    else if (condition==10) {
      if (N!=1) {
        printf("EXECUTE: Branch taken\n");
        R[15] = R[15] + result;
        return;
      }
    }
    else if (condition==14) {
      printf("EXECUTE: Branch taken\n");
      R[15] = R[15] + result;
      return;
    }
    printf("EXECUTE: Branch not taken\n");
    R[15] = R[15] + 4;
  }

  //swi exit
  else if (operation_type==3){
    swi_exit();
  }

}

//performs the memory operations
void mem() {

  //dt
  if (operation_type==1) {
    if (operation==25) {
      result == MEM[result];
      printf("MEMORY: Read %d from memory\n", result);
    }
    else if (operation==24) {
      MEM[result] = R[rd];
      printf("MEMORY: Write %d to to memory\n", R[rd]);
    }
  }

  //dp, branch
  else {
    printf("MEMORY: No memory operation\n");
  }

}

//writes the results back to register file
void write_back() {

  //dp
  if (operation_type==0) {
    //non-cmp
    if (operation!=10) {
      printf("WRITEBACK: write %d to R%d\n", result, rd);
      R[rd] = result;
    }
    //cmp
    else if (operation==10) {
      printf("WRITEBACK: No writeback\n");
    }
  }

  //dt
  else if (operation_type==1) {
    //ldr
    if (operation==25) {
      printf("WRITEBACK: write %d to R%d\n", result, rd);
    }
    //str
    else if (operation==24) {
      printf("WRITEBACK: No writeback operation\n");
    }
  }

  //branch
  else if (operation_type==2) {
    printf("WRITEBACK: No writeback operation\n");
  }
}


int read_word(char *mem, unsigned int address) {
  int *data;
  data =  (int*) (mem + address);
  return *data;
}

void write_word(char *mem, unsigned int address, unsigned int data) {
  int *data_p;
  data_p = (int*) (mem + address);
  *data_p = data;
}
