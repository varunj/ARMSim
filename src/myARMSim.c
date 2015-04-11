
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

int nosInstr = 0;           //gloabl var for nos of instr (including exit)
static int opcodeTemp;
static int destRegAddr;
static int storage;
//new array of operand1, operand2, resultx


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

// it is used to set the reset values
//reset all registers and memory content to 0
void reset_proc() {                                   
	int i;
	for (i = 0; i < 16; i++)
	{
		R[i] = 0;
	}
}

//load_program_memory reads the input memory, and pupulates the instruction 
// memory
void load_program_memory(char *file_name) {
  FILE *fp;
  unsigned int address, instruction;
  fp = fopen("../test/simple_add.mem", "r");                             // changed
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
  fp = fopen("../test/simple_add_out.mem", "w");
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
  if(instruction_word == 0xEF000011)
  {
    swi_exit();
  } 
  printf("FETCH : Fetch instruction 0x%x from address 0x%x\n",instruction_word,R[15]);
  R[15] = R[15] + 4;
  printf("\n");
}

//reads the instruction register, reads operand1, operand2 fromo register file, decides the operation to be performed in execute stage
//http://i.stack.imgur.com/0cPOx.png
// http://infocenter.arm.com/help/topic/com.arm.doc.ddi0210c/graphics/ARM_instruction_set_formats.svg
void decode() {
  int operand1Temp, operand2Temp;
  operand1Temp = (instruction_word>>16)&15;                 //get operand1
  operand2Temp = instruction_word&4095;                     //get operand2
  opcodeTemp = (instruction_word>>21)&15;                   //get opcode
  destRegAddr = (instruction_word>>12)&15;                  //get destReg

  if ((instruction_word>>27)&1 == 1)                        //branch instr
  {
  	printf("DECODE (Branch): ");
    //DO BRANCH
  }
  else if ((instruction_word>>26)&1 == 1)                   // data transfer
  {
    printf("DECODE (Data Transfer): ");
	if((instruction_word>>25)&1 == 1)						//immediate
	{
		printf("Operand1/Read R[%d], Operand2/Imm #%d, DestReg R[%d]", operand1Temp, operand2Temp, destRegAddr);
	}
	else													//reg
	{
		printf("Operand1 R[%d], Operand1 R[%d], DestReg R[%d]\n\tReg R[%d] = %d, R[%d] = %d\n",operand1Temp, operand2Temp, destRegAddr, operand1Temp, R[operand1Temp], operand2Temp, R[operand2Temp]);
	} 
	
  }
  else														//data processing
  {
  	printf("DECODE (Data Processing): ");
  	operand1 = R[operand1Temp];
  	if((instruction_word>>25)&1 == 1)						//immediate
	{
		operand2 = operand2Temp;
	}
	else
	{
		operand2 = R[operand2Temp];
	}

	switch (opcodeTemp)
	{
		case 0: printf("AND "); break;
		case 1: printf("EOR "); break;
		case 2: printf("SUB "); break;
		case 4: printf("ADD "); break;
		case 5: printf("ADC "); break;			//with carry
		case 6: printf("SBC "); break;
		case 10: printf("CMP "); break;
		case 12: printf("ORR "); break;
		case 13: printf("MOV "); break;
		case 15: printf("MNV "); break;			//not
	}


  }
  	if(instruction_word>>26 == 1)
  	{
  		printf("Operand1: R[%d], Operand2: #%d, DestReg: R[%d]\n\t Reg R[%d] = %d, Immediate = %d\n", operand1Temp, operand2Temp, destRegAddr, operand1Temp, R[operand1Temp], operand2Temp);
  	} 
  	else
  	{
  		printf("Operand1 R[%d],Operand2 R[%d], DestReg R[%d]\n\t registers R[%d]= %d,R[%d] = %d\n", operand1Temp, operand2Temp, destRegAddr, operand1Temp, R[operand1Temp], operand2Temp, R[operand2Temp]);
  	} 
	printf("\n");
}


//executes the ALU operation based on ALUop
void execute() {
	printf("EXECUTE: ");
	int tmpVar;

	if((instruction_word>>26)&1 == 1)				//Data Transfer
	{
		if((opcodeTemp>>4)&1)
		{
			printf("Data Transfer from Mem at %d\n",R[operand1] + operand2);
			storage = R[operand1] + operand2;
		}
	}
	else if((instruction_word>>27)&1 == 1)			//Branch
	{
		//DO BRANCH
	}
	else											//Data Processing
	{
		switch(opcodeTemp)
		{
			case 0: printf("AND %d and %d\n",operand1,operand2); storage=operand1&operand2; break; //AND
			case 1: printf("EOR %d and %d\n",operand1,operand2);storage=operand1^operand2; break; //EOR
			case 2: printf("SUB %d and %d\n",operand1,operand2);storage=operand1-operand2; break; //SUB
			case 4: printf("ADD %d and %d\n",operand1,operand2);storage=operand1+operand2; break; //ADD
			case 5: printf("ADC %d and %d\n",operand1,operand2);storage=operand1+operand2; C=(operand1>>31)&(operand2>>31); break; //ADC
			case 10: printf("CMP %d and %d\n",operand1,operand2);tmpVar=operand1-operand2; Z=(!tmpVar); N=(tmpVar<0); break; //CMP
			case 12: printf("ORR %d and %d\n",operand1,operand2);storage=operand1|operand2; break; //ORR
			case 13: printf("Nothing\n");storage=operand2; break;
			case 15: printf("Nothing\n");storage=~operand2; printf("MNV, "); break;
		}
	}


}




//perform the memory operation
void mem() {
	if ((instruction_word>>26)&1 == 1)                   // data transfer
	{
		if ((opcodeTemp>>4)&1 == 1)
		{
			printf("MEMORY: Read %d into  R[%d]\n", storage, destRegAddr);
			storage=R[operand1] + operand2;
		}
		else
		{
			printf("No Opr\n");
		}
	}
	printf("\n");
}


//writes the results back to register file
void write_back() {
	R[destRegAddr] = storage;
	printf("WRITEBACK: Write %d to R[%d]\n", storage, destRegAddr);
	printf("\n");
	printf("----------------------------------------------\n");
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

  //un needed
  //printf("%x\n", *data_p);
  nosInstr++;
}
