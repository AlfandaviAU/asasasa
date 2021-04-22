;lib.asm
;Michael Black, 2007
;Modified by Asisten Sister, 2021

;lib.asm contains assembly functions that you can use in the shell

global _interrupt
global _getRawCursorPos

;int interrupt (int number, int AX, int BX, int CX, int DX)
_interrupt:
	push bp
	mov bp,sp
	mov ax,[bp+4]	;get the interrupt number in AL
	push ds		;use self-modifying code to call the right interrupt
	mov bx,cs
	mov ds,bx
	mov si,intr
	mov [si+1],al	;change the 00 below to the contents of AL
	pop ds
	mov ax,[bp+6]	;get the other parameters AX, BX, CX, and DX
	mov bx,[bp+8]
	mov cx,[bp+10]
	mov dx,[bp+12]

intr:	int 0x00	;call the interrupt (00 will be changed above)

	mov ah,0	;we only want AL returned
	pop bp
	ret


;int getRawCursorPos()
;returning integer with higher 8 bit as row, lower 8 bit as column
_getRawCursorPos:
	push   bx
	push   cx
	push   dx
	mov    ax,0x0300 ; using INT 10H with AH = 03H
	mov    bx,0x0    ; page number = 0, BH = 0x00H
	int    0x10      ; direct int call
	mov    ax,dx ; return %dx;
	pop    dx
	pop    cx
	pop    bx
	ret
