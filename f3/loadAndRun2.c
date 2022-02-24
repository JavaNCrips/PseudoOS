/*
Brandon Hudson
CS 314
FOS
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 //#include "fos-kernel.h"
#include "computer.h"


void prompt();
void open_File(char* file);
int calc_ProcessSize();


FILE* progFile;
Process* pTableEntry;
int pid = 0;
int address = 0;
int runCnt = 0;
int processSize;
int pSize = 0;
int mainPages = 1;
int secPages = 0;
int page = 0;
int qt = 0;




int main(int argc, char* argv[]) {


	if (argc > 2) {
		fprintf(stderr, "Usage: %s fex-file\n", argv[0]);

		int mainMem = atoi(argv[1]);
		int secMem = atoi(argv[2]);

		createMainMem(mainMem);
		createSecMem(secMem);

		if (argv[3]) {
			pSize = atoi(argv[3]);

			mainPages = mainMem / pSize;
			secPages = secMem / pSize;
			page = mainPages;
		}

		if (argv[4]) {
			
			qt = atoi(argv[4]);
		}

		pTableEntry = (Process*)malloc(sizeof(Process) * (mainPages + secPages));

		initFOSKernel2(pSize, qt);
		initProcessTable(pTableEntry, (mainPages + secPages));
		prompt();

	}
	else {

		createMainMem(50);
		createSecMem(50);

		initFOSKernel1(50);
		initProcessTable(pTableEntry, (mainPages + secPages));

		char *file = argv[1];
		open_File(file);

		// calculating total memory size needed for process
		calc_ProcessSize();

		//    reads the rest of the fex file
		//    creates image of process in secondary memory at address	
		loadProgFile(progFile, &pTableEntry[0], address, &pid);

		// copy image of process from secondary to main memory		
		copySecToMain(address, 0, processSize);

		// this is not needed, but a way to see what is in memory
		// this dumps from 0 to the size of the process
		mainMemDump(0, processSize);

		// get cpu ready
		initCPU();

		// run the process
		while (startProcess(&pTableEntry[0]) == CLOCK_TICK) {
			printf(" **** process paused\n");
			saveProcessState(&pTableEntry[0]);
		}

		// Start of Shell command line
		prompt();

		printf(" #### process terminated\n");

		return 0;
	}
}

void prompt() {
	char command[10];
	//char str_Load[4] = "load";

	do {
		printf(" Enter a command: \n\n");
		if (scanf(" %s", command) != 1) { printf("\n Error \n"); }

		// load command loads process via fex file and places it in main memory
	if (strcmp(command, "load") == 0) {

			// If anything is in main memory we move it to secondary memory 
			copyMainToSec(address, 0, processSize);
			
			char *fileName = (char*)malloc(sizeof(char) * 50);

				printf("\nEnter file name: ");
				if (scanf(" %s", fileName) == 1) {

					// gets the new process (via fex file for us)
					open_File(fileName);
					printf("\nFile %s loaded.\n", fileName);
				}
			
				progFile = openProgFile(fileName, &pTableEntry[page]);



				loadProgFileToPage(progFile, &pTableEntry[page], mainPages, &pid);
				calc_ProcessSize();

				//puts process in main memory
				copySecToMain(address, 0, processSize);			

		}
	else if(strcmp(command, "runall") == 0){
	
		printf("Running all processes.\n");
	}
		// run command
		else if (strcmp(command, "run") == 0) {

			//CHANGE to check open main page!!
			if (pTableEntry[0].codeSize == 0) {
				printf("\nprocess terminated after %d quanta\n", runCnt); 
				runCnt++;

			}
			else {
				initCPU();

				while (startProcess(&pTableEntry[0]) == CLOCK_TICK) {
					printf(" **** process paused\n");
					saveProcessState(&pTableEntry[0]);
				}
				runCnt = 0;
			}
		}
		else if (strcmp(command, "dpt") == 0) {

		for (int i = 0; i < sizeof(pTableEntry); i++) {

		
			printf(pTableEntry[i].state);

		}

	}
		// OS-Noise MEM-Noise CPU-Noise toggles commands
		else if (strcmp(command, "osnoise") == 0) {
			toggleOSNoise();

		}
		else if (strcmp(command, "memnoise") == 0) {
			toggleMEMNoise();

		}
		else if (strcmp(command, "VMemNoise") == 0) {
			toggleVMEMNoise();

		}
		else if (strcmp(command, "cpunoise") == 0) {
			toggleCPUNoise();

		}
		// process state command
		else if (strcmp(command, "ps") == 0) {
			
			printf("PID\tcode\tpc \n %d \t  %d \t%d\n\n", pTableEntry[0].pid, pTableEntry[0].codeSize, cpu.pc);

		}
		else if (strcmp(command, "exit") == 0) {
			break;
		}
		else { printf("\nInvalid command\n"); }
			   
	} while (strcmp("exit", command) != 0);
}

void open_File(char* file) {

	progFile = openProgFile(file, &pTableEntry[0]);
	if (progFile == (FILE*)-1) {
		fprintf(stderr, "bad file name");
		exit(1);
		//page++;
	}
}

int calc_ProcessSize() {

	processSize = pTableEntry[0].codeSize
		+ pTableEntry[0].heapSize
		+ pTableEntry[0].stackSize;

	return processSize;
}
