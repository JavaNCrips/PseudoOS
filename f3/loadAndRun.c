/*
 * loadAndRun.c
 *
 * demonstrates some of the features of the fos-kernel
 *
 */

#include <stdio.h>
#include <stdlib.h>

//#include "fos-kernel.h"
#include "computer.h"

int main(int argc, char* argv[]){

   // this is needed to read a fex file
   FILE* progFile;

   // the user must type a fex file to execute as CLP
   // if no CLP, show USAGE and exit
   if(argc != 2) {
     fprintf(stderr, "Usage: %s fex-file\n", argv[0]);
     exit(1);
   }

   // this is the process table (for one process)
   Process pTableEntry[1];

   // creating main memory of 50 words (why 50? why not)
   createMainMem(50);

   // creating secondary memory of 50 words
   createSecMem(50);

   // prepare to use the FOS kernel
   initFOSKernel();

   // set values in the process table
   // second parameter is size of table
   initProcessTable(pTableEntry, 1);

   // pid = process ID
   // not needed for this program, but needed to use fos-kernel
   int pid = 0;
  
   // openProgFile 
   //    reads the header of the fex file and
   //    sets the pTableEntry values based on the fex file
   //    returns the file handle for the fex file (needed below)
   //    this just opens the file; does not load anything
   progFile = openProgFile(argv[1], &pTableEntry[0]);
   if (progFile == (FILE*)-1){
      fprintf(stderr, "bad file name");
      exit(1);
   }

   // calculating total memory size needed for process
   // needed to know how much memory to copy from sec to main
   int processSize = pTableEntry[0].codeSize 
                   + pTableEntry[0].heapSize 
                   + pTableEntry[0].stackSize;

   // going to load at this address
   int address = 0;

   // loadProgFile
   //    reads the rest of the fex file
   //    creates image of process in secondary memory at address
   //    (F2 will need a different function that loads a page)
   loadProgFile(progFile, &pTableEntry[0], address, &pid);

   // can't run process in secondary memory
   // copy image of process from secondary to main memory
   // the first two parameters are 1. sec mem address, 2. main mem address
   copySecToMain(address, 0, processSize);

   // this is not needed, but a way to see what is in memory
   // this dumps from 0 to the size of the process
   mainMemDump(0, processSize);

   // this is not needed, but turns on verbose output to see 
   // what is going on inside cpu
   // ignoring the return value from toggleCPUNoise
   toggleCPUNoise();

   // get cpu ready
   initCPU();

   // run the process
   while(startProcess(&pTableEntry[0]) == CLOCK_TICK) {
      printf(" **** process paused\n");
      saveProcessState(&pTableEntry[0]);
   }
   printf(" #### process terminated\n");

   return 0;
}
