#include "./library-header/shell_std.h"

void scrollScreen() {
    interrupt(0x21, 0x00, 0, 0x02, 0);
}

int getFullKey() {
    int key;
    interrupt(0x21, 0x01, &key, 0x01, 0);
    return key;
}

void print(char *string, char color) {
    // TODO : Extra, Maybe not safe (?)
    // TODO : Extra, Including black color
    if (0x0 < color && color <= 0xF)
        interrupt(0x21, 0x00, string, 0x00, color);
    else
        interrupt(0x21, 0x00, string, 0x00, 0x7);
}

void write(char *buffer, char *path, int *returncode, char parentIndex) {
    int AX = parentIndex << 8;
    AX |= 0x05;
    interrupt(0x21, AX, buffer, path, returncode);
}

void strcpybounded(char *dest, char *src, int n) {
    int i = 0;
    while (src[i] != '\0' && i < n) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

int getLastMatchedCharIdx(char c, char *string) {
    int i = 0, tp = -1;
    while (string[i] != 0x0) {
        if (string[i] == c)
            tp = i;
        i++;
    }
    return tp;
}

int getFirstMatchedCharIdx(char c, char *string) {
    int i = 0, tp = -1;
    while (string[i] != 0x0 && tp == -1) {
        if (string[i] == c)
            tp = i;
        i++;
    }
    return tp;
}

void putchar(char a) {
    int temp;
    char tempstring[2];
    tempstring[0] = a;
    tempstring[1] = '\0';
    if (a == 0x8)
        interrupt(0x21, 0x00, 0, 0x01, 0);
    else
        interrupt(0x21, 0x00, tempstring, 0x00, 0x7);

}
void strapp(char *string1, char *string2) {
    int i = strlen(string1), j = 0;
    while (string2[j] != 0x0) {
        string1[i] = string2[j];
        i++;
        j++;
    }
    string1[i] = 0x0;
}

void showKeyboardCursor() {
    interrupt(0x21, 0x0001, 0, 0x4, 0);
}

void hideKeyboardCursor() {
    interrupt(0x21, 0x0001, 0, 0x5, 0);
}

void setKeyboardCursor(char r, char c) {
    int BX = (r << 8);
    BX |= c;
    interrupt(0x21, 0x01, BX, 0x02, 0);
}

int getKeyboardCursor(bool isrow) {
    int pos;
    interrupt(0x21, 0x01, &pos, 0x3, isrow);
    return pos;
}

void clear(char *string, int length) {
    int i = 0;
    while (i < length) {
        string[i] = 0x0;
        i++;
    }
}

void setCursorPos(int r, int c) {
    int temp = 0x100*r + c;
    interrupt(0x10, 0x0200, 0x1, 0, temp);
}

int getCursorPos(bool isRow) {
    if (isRow)
        return getRawCursorPos() >> 8;
    else
        return getRawCursorPos() & 0xFF;
}

int strlen(char *string) {
    int i = 0;
    while (string[i] != '\0')
        i++;
    return i;
}

void strcpy(char *dest, char *src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

char strcmp(char *s1, char *s2) {
    int i = 0;
    if (strlen(s1) == strlen(s2)) {
        // If string length matches, check every char
        while (s1[i] != '\0') {
            if (s1[i] != s2[i])
                return 1;
            i++;
        }

        // If both string matches
        return 0;
    }

    return 1;
}