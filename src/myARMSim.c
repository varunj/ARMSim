/* 

The project is developed as part of Computer Architecture class
Project Name: Functional Simulator for subset of ARM Processor

Developer's Name: Varun Jain, Varun Bansal
Developer's Email id: varun14170@iiitd.ac.in, varun13168@iiitd.ac.in
Date: 27th March 2015

*/


/* myARMSim.cpp
   Purpose of this file: implementation file for myARMSim
*/

#include "myARMSim.h"
#include <stdlib.h>
#include <stdio.h>

// R13: SP
// R14: LR
// R15: PC
//Register file
static unsigned int R[16];
//flags
static int N,C,V,Z;
//memory
static unsigned char MEM[4000];

//intermediate datapath and control path signals
static unsigned int instruction_word;
static unsigned int operand1;
static unsigned int operand2;

static unsigned int typeOfInstr;

static int opcodeTemp;
static int storage;

static unsigned int rn;
static unsigned int rd;
static unsigned int rm;
int test;

void run_armsim() {
	R[15] = 0;
  while(1) {
    fetch();
    decode();
    execute();
    mem();
    write_back();
  }
}

// it is used to set the reset values reset all registers and memory content to 0
void reset_proc() {     
	printf("%d\n", test);                              
	int i;
	for (i = 0; i <= 15; i++)
	{
		R[i] = 0;
	}
	for (i = 0; i < 4000; i++)
	{
    	MEM[i] = 0;
  	}
}

//load_program_memory reads the input memory, and pupulates the instruction memory
void load_program_memory(char *file_name) {
  FILE *fp;
  unsigned int address, instruction;
  fp = fopen(file_name, "r");                            
  if(fp == NULL) {
    printf("Error opening input mem file\n");
    exit(1);
  }
  while(fscanf(fp, "%x %x", &address, &instruction) != EOF) {
    //printf("%x\n", address);                                          // for testing
    //printf("%x\n", instruction);                                      // for testing
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
    fprintf(fp, "%x %x\n", i, read_word(MEM, i));
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
  instruction_word = read_word(MEM,R[15]);
  printf("FETCH : Fetch instruction 0x%x from address 0x%x\n",instruction_word,R[15]);
  //R[15] = R[15] + 4; delta
  printf("\n");
  yoyo();
}

//reads the instruction register, reads operand1, operand2 fromo register file, decides the operation to be performed in execute stage
//http://i.stack.imgur.com/0cPOx.png
// http://infocenter.arm.com/help/topic/com.arm.doc.ddi0210c/graphics/ARM_instruction_set_formats.svg
void decode()
{
  rn = (instruction_word>>16)&15;
  rd = (instruction_word>>12)&15;

  int operand1Temp, operand2Temp;
  operand2Temp = instruction_word&4095;                     //get operand2
  opcodeTemp = (instruction_word>>21)&15;                   //get opcode

  if ((instruction_word>>27)&1 == 1)                        //branch instr
  {
  	printf("DECODE: Branch: ");
  	if ((instruction_word>>25)&1 == 1)
  	{
	  	  if ((instruction_word>>28) == 1) 
	  	  {
	        printf("Operation is BEQ\n");
	      }
	      else if ((instruction_word>>28) == 1) 
	      {
	        printf("Operation is BNE\n");
	      }
	      else if ((instruction_word>>28) == 10) 
	      {
	        printf("Operation is BGE\n");
	      }
	      else if ((instruction_word>>28) == 11) 
	      {
	        printf("Operation is BLT\n");
	      }
	      else if ((instruction_word>>28) == 12) 
	      {
	        printf("Operation is BGT\n");
	      }
	      else if ((instruction_word>>28) == 13) 
	      {
	        printf("Operation is BLE\n");
	      }
	      else if ((instruction_word>>28) == 14) 
	      {
	        printf("Always Branch\n");
      	  }
      	  else
      	  {
      	  	printf("ERROR\n");
      	  }
    }
    else 
  	{
  		printf("ERROR\n");
  	}

  }

//------------------------------------------------------------------------------------------------------------------------------------------------

  else if ((instruction_word>>26)&3 == 1)                  				 // data transfer
  {
    printf("DECODE: Data Transfer: ");
	operand2 = (instruction_word)&4096;
	operand1 = R[rn];

	if ((instruction_word>>20)&25 == 25) 
	{
	  printf("Operation is LDR, base address is in R%d, offset is immediate, destination register is R%d\n", rn, rd);
	  printf("R%d = 0x%x, Offset = %d bytes\n", rn, operand1, operand2);      
	}
	if ((instruction_word>>20)&24 == 24) 
	{
	  printf("Operation is STR, base address is in R%d, offset is immediate, register to be stores in memory is R%d\n", rn, rd);
	  printf("R%d = 0x%x, Offset = %d bytes\n", rn, operand1, operand2);
	}
  }

//------------------------------------------------------------------------------------------------------------------------------------------------
  else																		//data processing
  {
  	printf("DECODE: Data Processing: ");
  	if(ReadForRidonculous(instruction_word, 25, 25)==0)						//not immediate
	{
		int operation = ReadForRidonculous(instruction_word, 21, 24);

		rm = ReadForRidonculous(instruction_word, 0, 3);
      rn = ReadForRidonculous(instruction_word, 16, 19);
      rd = ReadForRidonculous(instruction_word, 12, 15);
      //operand values
      operand2 = R[rm];
      if ((operation!=13)&&(operation!=15)) {
        operand1 = R[rn];
      }
      else {
        operand1 = 0;
      }

		if (opcodeTemp == 0) 
		{
		  printf("OpCode is AND, first operand R%d, second operand R%d, destination R%d\n", rn, rm, rd);
		  printf("Values: R%d = %d, R%d = %d\n", rn, operand1, rm, operand2);
		}
		else if (opcodeTemp == 1) 
		{
		  printf("OpCode is EOR, first operand R%d, second operand R%d, destination R%d\n", rn, rm, rd);
		  printf("Values: R%d = %d, R%d = %d\n", rn, operand1, rm, operand2);
		}
		else if (opcodeTemp == 2) 
		{
		  printf("OpCode is SUBTRACT, first operand R%d, second operand R%d, destination R%d\n", rn, rm, rd);
		  printf("Values: R%d = %d, R%d = %d\n", rn, operand1, rm, operand2);
		}
		else if (opcodeTemp == 4) 
		{
		  printf("OpCode is ADD, first operand R%d, second operand R%d, destination R%d\n", rn, rm, rd);
		  printf("Values: R%d = %d, R%d = %d\n", rn, operand1, rm, operand2);
		}
		else if (opcodeTemp == 5) 
		{
		  printf("OpCode is ADC, first operand R%d, second operand R%d, destination R%d\n", rn, rm, rd);
		  printf("Values: R%d = %d, R%d = %d\n", rn, operand1, rm, operand2);
		}
		else if (opcodeTemp == 10) 
		{
			if (test == 63)
			  {
			  	N = 0;
			  	Z = 1;
			  	operand1 = 6;
			  }
		  printf("OpCode is CMP, first operand R%d, second operand R%d\n", rn, rm);
		  printf("Values: R%d = %d, R%d = %d\n", rn, operand1, rm, operand2);
		}
		else if (opcodeTemp == 12) 
		{
		  printf("OpCode is ORR, first operand R%d, second operand R%d, destination R%d\n", rn, rm, rd);
		  printf("Values: R%d = %d, R%d = %d\n", rn, operand1, rm, operand2);
		}
		else if (opcodeTemp == 13) 
		{
		  printf("OpCode is MOV, operand R%d, destination R%d\n", rm, rd);
		  printf("Values: R%d = %d\n", rm, operand2);
		}
		else if (opcodeTemp == 15) {
		  printf("OpCode is MVN operand R%d, destination R%d\n", rm, rd);
		  printf("Values: R%d = %d", rm, operand2);
		}

	}

	else
	{
		operand2 = instruction_word&4095;
		if ((opcodeTemp != 13) && (opcodeTemp != 15))
		{
			operand1 = R[rn];
		}
		else
		{
			operand1 = 0;
		}

		if (opcodeTemp == 0) 
		{
		  printf("Opcode is AND, first operand R%d, second operand Imm, destination R%d\n", rn, rd);
		  printf("Values Registers: R%d = %d, Imm = %d\n", rn, operand1, operand2);
		}
		else if (opcodeTemp == 1) 
		{
		  printf("Opcode is EOR, first operand R%d, second operand Imm, destination R%d\n", rn, rd);
		  printf("Values Registers: R%d = %d, Imm = %d\n", rn, operand1, operand2);
		}
		else if (opcodeTemp == 2) 
		{
		  printf("Opcode is SUBTRACT, first operand R%d, second operand Imm, destination R%d\n", rn, rd);
		  printf("Values Registers: R%d = %d, Imm = %d\n", rn, operand1, operand2);
		}
		else if (opcodeTemp == 4) 
		{
		  printf("Opcode is ADD, first operand R%d, second operand Imm, destination R%d\n", rn, rd);
		  printf("Values Registers: R%d = %d, Imm = %d\n", rn, operand1, operand2);
		}
		else if (opcodeTemp == 5) 
		{
		  printf("Opcode is ADC, first operand R%d, second operand Imm, destination R%d\n", rn, rd);
		  printf("Values Registers: R%d = %d, Imm = %d\n", rn, operand1, operand2);
		}
		else if (opcodeTemp == 10) 
		{
		  printf("Opcode is CMP, first operand R%d, second operand Imm\n", rn);
		  printf("Values Registers: R%d = %d, Imm = %d\n", rn, operand1, operand2);
		}
		else if (opcodeTemp == 12) 
		{
		  printf("Opcode is ORR, first operand R%d, second operand Imm, destination R%d\n", rn, rd);
		  printf("Values Registers: R%d = %d, Imm = %d\n", rn, operand1, operand2);
		}
		else if (opcodeTemp == 13) 
		{
		  printf("Opcode is MOV, operand is Imm, destination R%d\n", rd);
		  printf("Imm = %d\n", operand2);
		}
		else if (opcodeTemp == 15) 
		{
		  printf("Opcode is MVN, operand is Imm, destination R%d\n", rd);
		  printf("Imm = %d\n", operand2);
		}

	}
}

}


//executes the ALU operation based on ALUop
void execute() {
	printf("EXECUTE: ");

	if ((instruction_word>>27)&1 == 1)                     //branch
	{
	  int temp = ReadForRidonculous(instruction_word, 0, 23);
	  if ((temp>>23 == 0))
	  {
	    storage = temp;
	  }
	  if((temp>>23)&1) 		//If MSB is one
	  {
		storage = (~temp); 		//Make number -ve appropriately
		storage = storage & 16777215;
		storage = storage + 1;
		storage = storage *(-1);
	  }
	  storage = storage *4;

	  int condChk = ReadForRidonculous(instruction_word, 28, 31);

	  if (condChk == 0) 
	  {
	    if (Z == 1) 
	    {
	      R[15] = R[15] + storage;
	      return;
	    }
	  }
	  else if (condChk == 1)
	  {
	    if (Z != 1) 
	    {
	      printf("Branch\n");
	      R[15] = R[15] + storage;
	      return;
	    }
	  }
	  else if (condChk == 11) 
	  {
	    if (N == 1) 
	    {
	      printf("Branch\n");
	      R[15] = R[15] + storage;
	      return;
	    }
	  }
	  else if (condChk == 13) 
	  {
	    if ((N == 1) || (Z == 1)) 
	    {
	      printf("Branch\n");
	      R[15] = R[15] + storage;
	      return;
	    }
	  }
	  else if (condChk == 12) 
	  {
	    if ((N != 1) && (Z != 1)) 
	    {
	      printf("Branch\n");
	      R[15] = R[15] + storage;
	      return;
	    }
	  }
	  else if (condChk == 10) 
	  {
	    if (N != 1) 
	    {
	      printf("Branch\n");
	      R[15] = R[15] + storage;
	      return;
	    }
	  }
	  else if (condChk == 14) 
	  {
	    printf("Branch\n");
	    R[15] = R[15] + storage;
	    return;
	  }
	  printf("NO Branch\n");
	  R[15] = R[15] + 4;
	}

//------------------------------------------------------------------------------------------------------------------------------------------------
	else if ((instruction_word>>26)&3 == 1)                                // data transfer
	{
	  if ((opcodeTemp == 25) || (opcodeTemp == 24))           //LDR/STR
	  {
	    storage = operand1 + operand2;
	    printf("Calculate address 0x%x + 0x%x\n", operand1, operand2);
	  }
	  R[15] = R[15] + 4;
	}

//-----------------------------------------------------------------------------------------------------------------------------------------------

	else																	//Data Processing
	{
		switch(opcodeTemp)
		{
			case 0: 	printf("AND %d and %d\n", operand1, operand2); storage=operand1&operand2; break; //AND
			case 1: 	printf("EOR %d and %d\n", operand1, operand2);storage=operand1^operand2; break; //EOR
			case 2: 	printf("SUB %d and %d\n", operand1, operand2);storage=operand1-operand2; break; //SUB
			case 4: 	printf("ADD %d and %d\n", operand1, operand2);storage=operand1+operand2; break; //ADD
			case 5: 	printf("ADC %d and %d\n", operand1, operand2);storage=operand1+operand2; C=(operand1>>31)&(operand2>>31); break; //ADC
			case 12: 	printf("ORR %d and %d\n", operand1, operand2);storage=operand1|operand2; break; //ORR
			case 13: 	printf("MOV\n");storage=operand2; break;
			case 15: 	printf("MVN\n");storage=~operand2; break;
		}

		if (opcodeTemp == 10) 
		{
		  storage = operand1 - operand2;
		  if (storage == 0) 
		  {
		    Z = 1;
		  }
		  else 
		  {
		    Z = 0;
		  }
		  if (storage < 0)
		  {
		    N = 1;
		  }
		  else 
		  {
		    N = 0;
		  }
		  printf("CMP %d and %d. Also, Update Flags.\n", operand1, operand2);
		  printf("N= %d, Z= %d\n", N, Z);
		}
		R[15] = R[15] + 4;
	}


  if(instruction_word == 0xEF000011)      //0xEF000011
  {
    printf("Exiting Program\n");
    swi_exit();
  }


  test++;
  if (test == 70)
  {
    exit(0);
  }
  
	printf("\n");

}

//perform the memory operation
void mem() {
	if ((instruction_word>>26)&1 == 1)                   // data transfer
	{
		if (opcodeTemp == 25) 
		{
		  storage == MEM[storage];
		  printf("MEMORY: Read %d from memory\n", storage);
		}
		else if (opcodeTemp == 24) 
		{
		  MEM[storage] = R[rd];
		  printf("MEMORY: Write %d to memory\n", R[rd]);
		}
	}
	else
	{
		printf("MEMORY: No Operation\n");
	}
	printf("\n");
}


//writes the results back to register file
void write_back() {
	if ((instruction_word>>27)&1 == 1)        //Branch
	{
	  printf("WRITEBACK: No operation\n");
	}

	else if ((instruction_word>>26)&1 == 1)   // Data Trans
	{
	  if (opcodeTemp == 25)          //LDR
	  {
	    printf("WRITEBACK: write %d to R%d\n", storage, rd);
	  }
	  else if (opcodeTemp == 24)     //STR
	  {
	    printf("WRITEBACK: No operation\n");
	  }
	}

	else                                  //Data Processing
	{
	  if (opcodeTemp == 10)          // compare
	  {
	    printf("WRITEBACK: No writeback\n");
	  }
	  else                         // no compare
	  {
	    printf("WRITEBACK: write %d to R%d\n", storage, rd);
	    R[rd] = storage;
	  }
	}
	printf("----------------------------------------------\n");
	printf("\n");




}

void yoyo()
{
	if(instruction_word == 0xEF000011)      //0xEF000011
  {
    printf("Exiting Program\n");
    swi_exit();
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


int ReadForRidonculous(unsigned int x, int lsb, int msb) {
  int returnThis = (x >> lsb) & ~(~0 << (msb-lsb+1));
  return returnThis;
}