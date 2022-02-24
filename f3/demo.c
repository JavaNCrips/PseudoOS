/*
 * demo.c
 * example of vmm code working
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "computer.h"
#include "vmm.h"

// no cmd this long, room for typos
// this is a security flaw, commands longer thatn CMD_LEN could do damage
#define CMD_LEN 20
#define MAX_PROCESS 10

#define TRUE 1
#define FALSE 0

int initSystem(int mainMemSize, int secMemSize, int pageSize){
   createMainMem(mainMemSize);
   createSecMem(secMemSize);
   initCPU();
   initFOSKernel1(pageSize);
   initVMM();

   return 0;
}

/*
 *
 * return
 *   0 on success
 *
 */
int loadProcess(char *progFileName, Process *pTable, int *pid){

   FILE* progFile;
   Process *ptEntry = NULL;

   // fprintf(stderr, "find free pTable entry\n");
   // ps(pTable, MAX_PROCESS); // here for debugging

   for (int ptEntryIndex = 0; ptEntryIndex < MAX_PROCESS &&
                           ptEntry == NULL; ptEntryIndex++){
      if(pTable[ptEntryIndex].valid == FALSE){
         ptEntry = &pTable[ptEntryIndex];
      }
   }

   progFile = openProgFile(progFileName, ptEntry);
   // TODO: check for success
   if(progFile == NULL) {
      printf("failed to load\n");
      return 1;
   }

   // F1 did this...
   // int processSize = ptEntry->codeSize + ptEntry->heapSize + ptEntry->stackSize;
   // loadProgFile(progFile, ptEntry, 0, pid);
   // copySecToMain(0, 0, processSize);

   // loop till process loaded
      FILE* notFullyLoaded = progFile;
      int vPage = 0;
   do {
      int pageFrame;

      // find free secondary page frame
      pageFrame = pageTableGetFreeSecPage();
      fprintf(stderr, "going to load in sec page frame %d\n",pageFrame);

      if(pageFrame == -1){
         fprintf(stderr, "TODO: clean up on failure - load program\n");
      }

      // load a page
      pageTableLoadProcessToSecFrame(pageFrame, (*pid)+1);
      pageTable[pageFrame].vPage = vPage++;
      notFullyLoaded = loadProgFileToPage(progFile, ptEntry, pageFrame, pid);
      // secMemDump(pageFrame * getPageSize(), (pageFrame +1)*getPageSize());
   } while (notFullyLoaded != NULL);

   return 0; 
}

int main(int argc, char* argv[]){

    Process pTable[MAX_PROCESS];
    int nextPID = 0;

    if(4 > argc){
        fprintf(stderr, "need three command line parameters to \n");
        fprintf(stderr, "specify the size of main and secondary memory; and ");
        fprintf(stderr, "specify the page size\n");
        exit(1);
    }

    int mainMemSize = atoi(argv[1]);
    int secMemSize = atoi(argv[2]);
    int pageSize = atoi(argv[3]);

    initSystem(mainMemSize, secMemSize, pageSize);

    initProcessTable(pTable, MAX_PROCESS);

    char* command; 
    command = (char *) malloc(CMD_LEN); 

    do {
        int commandRecognized = FALSE;

        printf("enter a command: ");
        if (scanf (" %20s", command) != -1) {
          // didn’t read a command 
        }

        if(0 == strcmp("load", command)){
            commandRecognized = TRUE;
            char filename[100];
            printf("enter name of file to load: ");
            scanf(" %s", filename);
            loadProcess(filename, pTable, &nextPID);
        }

        if(0 == strcmp("run", command)){
            commandRecognized = TRUE;
            int runPID; 
            printf("enter PID to run: ");
            scanf(" %d", &runPID);
            // F1 did this: runToCompletion(&pTable[0]);
            // F2 solution deleted 
        }

        if(0 == strcmp("ps", command)){
            commandRecognized = TRUE;
            // F2 solution deleted 
        }

        if(0 == strcmp("dpt", command)){
            commandRecognized = TRUE;
            // F2 solution deleted 
        }

        if(0 == strcmp("help", command)){
            commandRecognized = TRUE;
            printf("there is no help for you ;-)\n", command);
        }

        if(0 == strcmp("memnoise", command)){
            commandRecognized = TRUE;
            toggleMEMNoise();
        }

        if(0 == strcmp("cpunoise", command)){
            commandRecognized = TRUE;
            toggleCPUNoise();
        }

        if(0 == strcmp("osnoise", command)){
            commandRecognized = TRUE;
            toggleOSNoise();
        }

        if(0 == strcmp("vmemnoise", command)){
            commandRecognized = TRUE;
            toggleVMEMNoise();
        }

        if(0 == strcmp("exit", command)){
            commandRecognized = TRUE;
        }

        if(!commandRecognized){
            printf("invalid command: %s\n", command);
        }

    } while (strcmp("exit", command));

   return 0;
}
