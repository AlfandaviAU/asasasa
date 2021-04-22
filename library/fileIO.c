#include "../library-header/fileIO.h"
#include "../library-header/constant-header/constant.h"

int getUnusedSectCount(char *buffer, int sectors){
    int i;
    int count = 0;

    for (i = 0; i < sectors; i++) {
        if (buffer[i] == 0x00) {
            count++;
        }
    }

    return count;
}

int getFirstUnusedSect(char *buffer, int sectors){
    int i;

    for (i = 0; i < sectors; i++) {
        if (buffer[i] == 0x00) {
            return i;
        }
    }
}

char getPathIdx(char *path, char *files, char parentIndex){
    int i = 0;
    char curDir[MAX_CHAR], pathIdx;

    while (i < MAX_CHAR && path[i] != 0 && path[i] != '/'){
        curDir[i] = path[i];
        i++;
    }

    curDir[i] = '\0';

    // Get Current Idx
    pathIdx = getCurrentIdx(curDir, files, parentIndex);

    // Basis 1
    if (path[i] == '\0'){
        return pathIdx;
    }
    // Basis 2
    else if (pathIdx == NOT_FOUND_INDEX){
        // Not Found
        return NOT_FOUND_INDEX;
    }
    // Recurrents
    else{
        return getPathIdx(path + i + 1, files, pathIdx);
    }
}

char getCurrentIdx(char *target, char *files, char parentIndex){
    int i = 0;
    
    // Search for index
    if (target[0] == '.' && target[1] == 0){
        return parentIndex;
    }
    else if (target[0] == '.' && target[1] == '.' && target[2] == 0){
        if (parentIndex == 0xFF){ 
            // Root folder
            return parentIndex;
        }
        else{
            return files[parentIndex * 16];
        }
    }
    
    for (i; i < 64; i++){
        if (files[i * 16] == parentIndex){
            if (strcmp(target, files + (i * 16) + 2)){
                return i;
            }
        }
    }
    
    // Not Found
    return NOT_FOUND_INDEX;
}

void readFile(char *buffer, char *path, int *result, char parentIndex){
	char sectors[512], files[1024];
	int fileIdx, sect, sectPos, sectIdx;

    // Get File Path Idx
	// fileIdx = getPathIdx(parentIndex, path);

	fileIdx = getPathIdx(path, files, parentIndex);

    // Check if found
	if(fileIdx == -1){
		*result = -1;
        printString("File not found!");
		return;
	}

    // Check the folder
	else if((fileIdx >> 8) == 0x1){
		*result = -1;
        printString("Folder unreadable!");
		return;
	}
    
    // File Found
    // Read Sector
	readSector(files, 0x101);
	readSector(files + 512, 0x102);
	readSector(sectors, 0x103);

	// sectIdx
	sectIdx = files[fileIdx * 16 + 1];

	for (sect = 0; sect < 16; sect++){
		sectPos = sectIdx * 16 + sect;
		if (sectors[sectPos] == 0){
			// End of File
			break;
		}
		// readSector
		readSector(buffer + (sect * 512), sectors[sectPos]);
	}
	
	*result = 1;
}

void writeFile(char *buffer, char *path, int *sectorCount, char parentIndex) {
    char map[512], files[512 * 2], sectors[512];
    int fr, sr, yangMauDiubah, i, temp, cd;

    // Read map, files, sectors
    // 0x100, 0x101-0x102, 0x103
    readSector(map, 256);
    readSector(files, 0x101);
    readSector(files+512, 0x102);
    readSector(sectors, 0x103);
    
    if((parentIndex != 0xFF) && (files[(16*parentIndex)+1] != 0xFF)){
        *sectors = -4;
        return;
    }

    cd = parentIndex;

    for (fr = 0; fr < 64; fr++){
        if ((strcmp(files + (fr * 16) + 2, 14, path) == 1) && (files[fr * 16] == cd)){
            *sectors = -1;
            return;
        }
    }

    for (fr = 0; fr < 64; fr++){
        if (files[(fr << 4) + 2] == 0){
            break;
        }
    }

    if (fr == 64){
        *sectors = -2;
        return;
    }

    if (getUnusedSectCount(map, 256) < *sectorCount){
        *sectors = -3;
        return;
    }

    for(sr = 0; sr < 32; sr++){
        if((sectors[sr << 4]) == 0){
            break;
        }
    }

    if(sr == 32){
        *sectors = -3;
        return;
    }

    clear(files + (fr << 4), 16);
    files[fr<<4] = cd;
    files[(fr<<4) + 1] = sr;

    for (i = 0; i < 14; i++){
        if (path[i] == 0){
            break;
        }
        files[i + (fr<<4) + 2] = path[i];
    }

    for (i = 0; i < *sectorCount; i++){
        clear(files,512);

        yangMauDiubah = getFirstUnusedSect(map, 256);
        map[yangMauDiubah] = 0xFF;
        sectors[(sr<<4) + i] = yangMauDiubah;

        writeSector(buffer + (i<<9), yangMauDiubah);
    }

    writeSector(map, 0x100);
    writeSector(files, 0x101);
    writeSector(files+512, 0x102);
    writeSector(sectors, 0x103);
}