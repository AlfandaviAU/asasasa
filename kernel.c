/*
Kelompok AhjinGuild K-04 IF'19
Richard Rivaldo - 13519185
Muhammad Fawwaz Naabigh - 13519206
*/

// Definisi Fungsi dan Prosedur
// void putInMemory (int segment, int address, char character)
// int interrupt (int number, int AX, int BX, int CX, int DX)
// void makeInterrupt21()

#include "./library-header/fileIO.h"
#include "./library-header/constant-header/constant.h"
#include "./library-header/fileIO.h"
#include "./library-header/folderIO.h"
#include "./library-header/matematika.h"
#include "./library-header/teks.h"
#include "./library-header/utility.h"
#include "./library-header/render.h"

void handleInterrupt21 (int AX, int BX, int CX, int DX);
void executeProgram(char *filename, int segment, int *success, char parentIndex);
void clear(char *buffer, int length);

//syscall
void readSector(char *buffer, int sector);
void writeSector(char *buffer, int sector);

/*----------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------*/

// Main Function
int main(){
    char *a, *name;
    int *flag;
    
    // Make Interrupt Vector
    makeInterrupt21();
    

    // Go back to Text Mode
    // AH = 0x0, AL = 0x3
    interrupt(0x10, 0x3, 0, 0, 0);
    
    // printAt
    printAt(36, 12, 0xd, "Arise!");
    
    // Waiting for Enter to Continue
    interrupt(0x21, 0x1, a, 0, 0);
    while(a == '\n'){
    	interrupt(0x21, 0x1, a, 0, 0);
    }
    
    executeProgram("shell", 0x2000, &flag, 0xFF);

    
    // Infinite Loop                                        
    while(1);
}


/*----------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------*/

// Interrupt Handler
void handleInterrupt21 (int AX, int BX, int CX, int DX){
    // Switch on AX
    char AL, AH; 
    AL = (char) (AX); 
    AH = (char) (AX >> 8);

    switch (AX) {
    	//0 -> printString
    	case 0x00:
    		printString(BX); 
    		break;
    	// 1 -> readString
    	case 0x01: 
    		readString(BX); 
    		break;
        // 2 -> readSector
        case 0x02: 
            readSector(BX, CX); 
            break;
        // 3 -> writeSector
        case 0x03: 
            writeSector(BX, CX); 
            break;
        // 4 -> readFile
        case 0x04: 
            readFile(BX, CX, DX, AH); 
            break;
        // 5 -> writeFile
        case 0x05: 
            writeFile(BX, CX, DX, AH); 
            break;
        // case 0x06:
	    //     shell();
	    //     break;
        case 0x6:
            executeProgram(BX, CX, DX, AH);
            break;
    	// Default -> Invalid
    	default:
    		printString("Invalid interrupt");
    }
}

// Clear Buffer
void clear(char *buffer, int length){
    int j;

    // Iterate
    for(j = 0; j < length; ++j){
        buffer[j] = EMPTY;
    }
}

// Read Sector
void readSector(char *buffer, int sector){
    interrupt(0x13, 0x201, buffer, div(sector, 36) * 0x100 + mod(sector, 18) + 1, mod(div(sector, 18), 2) * 0x100);
}

// Write Sector
void writeSector(char *buffer, int sector){
    interrupt(0x13, 0x301, buffer, div(sector, 36) * 0x100 + mod(sector, 18) + 1, mod(div(sector, 18), 2) * 0x100);
}

void executeProgram(char *filename, int segment, int *success, char parentIndex) {
    // Buat buffer
    int isSuccess;
    char fileBuffer[512 * 16];
    // Buka file dengan readFile
    readFile(&fileBuffer, filename, &isSuccess, parentIndex);
    // If success, salin dengan putInMemory
    if (isSuccess) {
        // launchProgram
        int i = 0;
        for (i = 0; i < 512*16; i++) {
            putInMemory(segment, i, fileBuffer[i]);
        }
        launchProgram(segment);
    } else {
        interrupt(0x21, 0, "File not found!", 0,0);
    }
}
