/*
 * vmm.c 
 * virtual memory management for fos os
 *
 * version 0.91
 */

#include "vmm.h"
#include "computer.h"
#include <stdlib.h>
// #include <stdio.h>

/*
 * the functions in this file fall under two categories:
 *   1. functions that access / update the pageTable
 *   2. functions that use the pageTable to do address translation
 *
 * functions in the first category all all named pageTable*
 * (that is, the names of the functions all begin with "pageTable")
 * there is nothing 'magic' about this naming, except that it should
 * could help the programmer keep straight the intention of the functions
 *
 */


/*
 * initVMM
 * make the necessary data structures for virtual memory
 *
 * return
 *    0 if success
 *    non-zero value for failure
 */

int initVMM(){
   if(VMEM_NOISE) printf("VMEM: init\n");

   // create memory for and initialize page table(s)
   pageTable = calloc(getNumSecPages(), sizeof(PageTableRec));
   if(pageTable == 0){
      fprintf(stderr, "failed to create pageTable data structure\n");
      return 2;
   }
   for(int page = 0; page < getNumSecPages(); page++){
      pageTable[page].free = TRUE;
      pageTable[page].vPage = -1;
      pageTable[page].mainPageFrame = -1;
   }
   
   return 0;
}

/*
 * pageTableGetFreeSecPage
 *    search the pageTable for a free secondary page
 *    find the first entry where free == TRUE
 *    the index of the entry is also the secondary page number
 *
 *    return
 *       the secondary page number of a currently free frame
 *       -1 on failure (no secondary page available)
 */
int pageTableGetFreeSecPage(){
   if(VMEM_NOISE) printf("VMEM: \n");

   return -1;
}

/*
 * pageTableLoadProcessToSecFrame
 *    this does not do the loading (done in kernel)
 *    this records in the page frame that a page of a process
 *    has been loaded in secondary memory
 *
 * return
 *    0 success
 *    -1 failure (secondary page is already occupied)
 */
int pageTableLoadProcessToSecFrame(int sPageFrame, int pid){
   if(VMEM_NOISE) printf("VMEM: \n");

   return 0;
}

/*
 * pageTableAccessPageFrame
 *    update the pageTable with last access time
 *    used for (some) page replacement algorithms
 *
 *    parameters
 *       mPageFrame - the main memory page frame accessed
 *       write (boolean) - 0 if reading, non-zero for write
 *
 *    return (int)
 *       0 success - page table updated appropriately
 *       -1 failure - page referenced is unoccupied
 */
int pageTableAccessPageFrame(int mPageFrame, int write){
   if(VMEM_NOISE) printf("VMEM: \n");

   return -1;
}
/*
 * pageTableProcessTerm
 *    update the pageTable to reflect that a process has terminated
 *
 *    return
 *       void
 */

void pageTableProcessTerm(int pid){
   if(VMEM_NOISE) printf("VMEM: \n");

}

/*
 * pageTableCopyToPageFrame
 *    update the pageTable to reflect that a page has been (or will be)
 *    copied to main memory
 *
 *    parameters
 *       sPageFrame - secondary memory page frame copied
 *       mPageFrame - main memory page frame where copy was placed
 *
 *    return
 *       0 success
 *       -1 failure 
 */
int pageTableCopyToPageFrame(int sPageFrame, int mPageFrame){
   if(VMEM_NOISE) printf("VMEM: \n");

   return -1;
}

/*
 * pageTablePageEvicted
 *    update pageTable to reflect that a page was (or will be) evicted
 *
 *    parameters
 *       pid - the pid of the process
 *       mPageFrame - the main memory page frame evicted
 *
 *    return
 *       void 
 */
void pageTablePageEvicted(int pid, int mPageFrame){
   if(VMEM_NOISE) printf("VMEM: \n");

}

/*
 * pageTableFindFreeMainPageFrame
 *    find a free page frame in main memory
 *
 *    return
 *       the main page number of a currently free frame
 *       -1 on failure (no secondary page available)
 */
int pageTableFindFreeMainPageFrame(){
   if(VMEM_NOISE) printf("VMEM: searching for free main page frame\n");

   int pageFrame[getNumMainPages()];

   for(int page=0; page < getNumMainPages(); page++){
      pageFrame[page] = -1;
   }

   for(int page=0; page < getNumSecPages(); page++){
      if(pageTable[page].free == FALSE && pageTable[page].mainPageFrame != -1){
         pageFrame[pageTable[page].mainPageFrame] = pageTable[page].pid;
      }
   }

   for(int page=0; page < getNumMainPages(); page++){
      if (pageFrame[page] == -1){
         return page;
      }
   }

   if(VMEM_NOISE) printf("VMEM: no free main page frame\n");
   return -1;
}

/*
 * pageTableFindLRUFrame
 *    find the main memory page frame used least recently
 *
 *    return
 *       a secondary page frame number that corresponds to LRU main page frame
 */
int pageTableFindLRUFrame(){
   if(VMEM_NOISE) printf("VMEM: \n");

   return -1;
}

/*
 * readWordFromMainMem
 *    vAddr - a virtual memory address
 *    return - value read from memory
 * 
 * the CPU will call this to read a value from a virtual address
 * this function converts the virtual address to a physical address
 * and reads the value from main memory at the virutal address
 * 
 * this is the only function that reads directly from mainMem[]
 */
WORD readWordFromMainMem(WORD vAddr){

   if(VMEM_NOISE) printf("vmemnoise: reading vAddr: %ld\n", vAddr);
   WORD pAddr;

   // TODO: convert from physical to virtual address
         if(getPageSize() != 1) fprintf(stderr, "vmm TODO: address translation read\n");
   pAddr = vAddr;

   // TODO: handle page faults 
         if(getPageSize() != 1) fprintf(stderr, "vmm TODO: page faults read\n");

         if(getPageSize() != 1) fprintf(stderr, "vmm TODO: update page table on access read\n");


   if(pAddr < 0 || pAddr >= getMainMemSize()){
      fprintf(stderr, "seg fault in readWordFromMainMem\n");
      exit(1);
   }
   return mainMem[pAddr];
}

/*
 * writeWordToMainMem(WORD addr, WORD value)
 *
 */

int writeWordToMainMem(WORD vAddr, WORD value){
   WORD pAddr;

   // TODO: convert virtual address to physical address
         if(getPageSize() != 1) fprintf(stderr, "vmm TODO: address translation write\n");
   pAddr = vAddr;

   // TODO: handle page faults 
         if(getPageSize() != 1) fprintf(stderr, "vmm TODO: handle page faults write\n");
         if(getPageSize() != 1) fprintf(stderr, "vmm TODO: update page table on access write\n");

   if(pAddr < 0 || pAddr >= getMainMemSize()){
      fprintf(stderr, "seg fault in writeWordToMainMem\n");
      exit(1);
   }
   mainMem[pAddr] = value;

   return 0;
}
