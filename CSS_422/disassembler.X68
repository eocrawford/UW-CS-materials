*----------------------------------------------------------------------------
* Program    : 68k disassembler
* Written by : Ethan Crawford, Michael Grimm, Paula Haddad
* Date       : March 21, 2009
* Description: Given a starting and ending address, disassembles machine
*              language into readable 68000 code.
*----------------------------------------------------------------------------
START	ORG	$1000

*****************************************************************************
*                                equates                                    *
*****************************************************************************
stack	EQU	$000A0000
Dn	EQU	%000
An	EQU	%001
iAn	EQU	%010
iiAn	EQU	%011
idAn	EQU	%100
i16An	EQU	%101
i8An	EQU	%110
mode7	EQU	%111
absWord	EQU	%000
absLong	EQU	%001
immData	EQU	%100
d16PC	EQU	%010
d8PC	EQU	%011

*****************************************************************************
*                                macros                                     *
*****************************************************************************

***************** Prints a string from a string array ***********************
* PRINTINDEX(byte bOffset, word rgsz, word An)
PRINTINDEX MACRO
	ASL.W	#1,\1 * Multiply by two for ptr arithmetic
	MOVEA.W	\2,\3
	ADD.W   \1,\3 * Add offset to rgszSize to get string ptr
	MOVE.W	(\3),\3
	
	STRCPY \3,A0
	ENDM

****************************** strcpy ***************************************
* STRCPY(wpszSrc (An),wpszDst (An))
STRCPY MACRO
\@cpyChr
	CMP.B	#0,(\1) * If char is null, end the loop
	BEQ	\@scExit
	MOVE.B	(\1)+,(\2)+ * Copy char from src to dst
	BRA	\@cpyChr
\@scExit
	ENDM

**************************** mask word **************************************
* Masks the provided value by the provided mask and shifts the result 
* as far right as possible. 
MASK MACRO
	MOVE.W	\1,-(SP)  * Push wMask
	MOVE.W	\2,-(SP)  * Push wValue
	BSR	maskWord
	LEA	4(SP),SP	* Pop stack parameters
	ENDM

************************ add whitespace to a buffer *************************
ADDWS MACRO
	* Copy 4 spaces into buffer
	MOVEM.L	A0/D1,-(SP)
	MOVEA.L	\1,A0 * Pointer to output buffer
	MOVEQ.L	#3,D1 * Set up loop condition
\@as
	MOVE.B	bSpace,(A0)+ * Write space to buffer
	DBRA D1,\@as * Decrement counter and loop while > -1
	MOVEQ.L	#4,D0 * Set return value to number of characters written
	MOVEM.L (SP)+,A0/D1
	ENDM

*************************** print a line of text ****************************
PRINTLN	MACRO
	MOVEM.L	D0/A1,-(SP) * push
	LEA	\1,A1 * Load string pointer
	MOVE.B	#13,D0 * Print with newline
	TRAP	#15
	MOVEM.L	(SP)+,D0/A1 * pop
	ENDM
	
********************************* read CIN **********************************
CIN MACRO
	MOVEM.L A1,-(SP)
	MOVE.B	#2,D0 * Load cin task into D0	
	LEA	lpszInput,A1 * Load address of storage space
	TRAP	#15 * cin
	MOVE.L D1,D0 * Set the return value to the length of the input string
	MOVEM.L (SP)+,A1
	ENDM

**************************** call StringToLong ******************************
STRINGTOLONG MACRO
	* Call stringToLong
	MOVE.L	#lpszInput,-(SP)  * Push input buffer
	MOVE.L	\1,-(SP)	* Push input buffer length
	BSR	_stl		* Call stringToLong
	LEA	8(SP),SP	* Pop stack parameters
	ENDM

**************************** call LongToString ******************************
LONGTOSTRING MACRO
	* Call longToString
	MOVE.L	\1,-(SP)	* Push lValue
	MOVE.L	\2,-(SP) 	* Push lpszBuffer
	BSR	_lts		* Call longToString
	LEA	8(SP),SP	* Pop stack parameters	
	ENDM

****************************** call _decodeEA *******************************
* DECODEEA(byte bEA)
DECODEEA MACRO
	MOVE.W	\1,-(SP)  * Push bEA
	BSR	_decodeEA
	LEA	2(SP),SP	* Pop stack parameters
	ENDM

*************************** Calls _deXn2Way ********************************
* DEXN2WAY(word rgszXn, word wDir)
DEXN2WAY MACRO
	MOVEM.L D1,-(SP)
	
	MOVE.W	8(SP),D1 * Get wInst
	MOVE.W	D1,-(SP) * Push wInst
	MOVE.W	#\1,-(SP)  * Push rgszDn
	MOVE.W	\2,-(SP)  * Push wDir
	MOVE.W	\3,-(SP)  * Push wSize
	BSR	_deXn2Way * Call _deXn2Way
	LEA	8(SP),SP * Pop stack parameters
	
	MOVEM.L (SP)+,D1
	ENDM

*****************************************************************************
*                                main()                                     *
*****************************************************************************
	LEA	stack,SP	* Load the SP
	CLR	D6		* Clear the currentLines counter

* Print a greeting
Main
	PRINTLN lpszGreeting

** Get the number of rows on the screen. Subtract one for printing the "continue" text.
** Hard-code row height to 16
	MOVE.L	#33,D0
	CLR.L	D1
	CLR.L	D6
	TRAP	#15
	AND.L	#$FFFF,D1
 	DIVU.W	#16,D1
 	MOVE.W	D1,D7
	
** Get the starting address
getStartAddr
	PRINTLN	lpszEnterStart
	CIN

	CMP.L	#0,D0		* Make sure at least one character was entered
	BLE	saErr
	
	* Call stringToLong
	STRINGTOLONG D0

	* Validate return value
	MOVEA.L	D0,A0           * Save the starting address in A0
	CMP.L	#-1,A0		* If address is invalid, print an error and loop back
	BEQ	saErr
	
	* Validate alignment
	MOVE.L	A0,D0	
	BTST #0,D0 * Examine bit 0 of the address
	BNE	saErr * If bit 0 is a 1, error out (this implies an odd address)
	
	BRA	getEndAddr	* Else, get end address
saErr
	PRINTLN lpszError
	BRA	getStartAddr

* Get the ending address
getEndAddr
	PRINTLN lpszEnterEnd
	CIN
	
	CMP.L	#0,D0		* Make sure at least one character was entered
	BLE	saErr
	
	* Call stringToLong
	STRINGTOLONG D0

	* Check the ending address range for validity
	* The ending address must be at least 2 bytes larger than the starting address.
	MOVEA.L	D0,A1           * Save the ending address in A1

	CMP.L	#-1,A1		* If address is invalid, print an error and loop back
	BEQ	eaErr
	
	SUB.L	A0,D0		* endAddr must be at least 2 bytes larger than startAddr
	CMP.L	#2,D0
	BLT	eaErr

	MOVE.L	A1,D0
	BTST #0,D0 * Examine bit 0 of the address
	BNE	saErr * If bit 0 is a 1, error out (this implies an odd address)
		
	MOVE.L	A0,lCurrentAddress * Store the value in a global
	BRA	nextWord	* Else, start disassembly
eaErr	
	PRINTLN lpszError
	BRA	getEndAddr

* Begin the disassembly
nextWord
	MOVE.W	(A0),D1 * Read next word at current address
	
	* Call _decodeInstruction
	MOVE.W	D1,-(SP) 	* Push wValue
	BSR	_decodeInstruction * Call _formatData
	LEA	2(SP),SP	* Pop stack parameters
	
	* If decodeInstruction failed, print the data buffer
	CMP.L	#-1,D0
	BNE	printInst
printData
	* Format the data buffer
	MOVE.L	A0,-(SP)	* Push lAddress
	MOVE.W	D1,-(SP) 	* Push wValue
	BSR	_formatData	* Call _formatData
	LEA	6(SP),SP	* Pop stack parameters

	PRINTLN	lpszDataOutput
	ADDQ.L	#2,A0
	MOVE.L	A0,lCurrentAddress
	BRA	nextAddr
printInst
	MOVE.L	lCurrentAddress,A0 * Get current address
	PRINTLN	lpszInstOutput
nextAddr
	ADDI.W	#1,D6
	CMP.L	D7,D6 * Check if the number of printed lines is rowMax
	BLT go * If so, print the enter text and prompt for input
	PRINTLN lpszNextPage
	CIN
	CLR.L	D6
	BRA go
	
go	CMP.L	A1,A0
	BLE	nextWord * Loop while current addr is less than end addr
	
	* Ok, disassembly is done. Continue or quit.
	PRINTLN lpszContinue
	CIN

	* Read the buffer. 'd' means loop again,
	* anything else means exit.
	LEA	lpszInput,A0 * Load the input buffer
	MOVE.B	(A0)+,D1 * Read a character into D1
	CMP.B	#$64,D1 * Loop if 'd' was entered
	BEQ	Main	
mainExit
	MOVE.B	#9,D0
	TRAP	#15		Halt Simulator
stop	STOP	#$2700

*****************************************************************************
*                                globals                                    *
*****************************************************************************
lpszGreeting	DC.B	'Motorola 68000 disassembler',0
lpszEnterStart	DC.B	'Enter a starting address:',0
lpszEnterEnd	DC.B	'Enter an ending address:',0
lpszError	DC.B	'The address you have entered is invalid. Please try again.',0
lpszContinue	DC.B	'Enter "d" to disassemble another memory image, or any other value to quit.',0
lpszNextPage	DC.B	'Press Enter to continue...',0
lpszData        DC.B	'DATA',0
bSpace		DC.B	' '
lpszInput	DS.B	80 * Input buffer
lpszInstOutput	DS.B	80 * Instruction output buffer
lpszDataOutput 	DS.B	80 * Data output buffer
lCurrentAddress DS.L	1  * Current memory address being disassembled
lpszB		DC.B	'.B',0
lpszW		DC.B	'.W',0
lpszL		DC.B	'.L',0
rgsz2		DC.W	lpszB,lpszW,lpszL
rgsz1		DC.W	lpszW,lpszL
rgszMOVE	DC.W	0,lpszB,lpszL,lpszW
rgszMOVEA	DC.W	lpszL,lpszW
lpszADD		DC.B	'ADD',0
lpszADDA	DC.B	'ADDA',0
lpszADDI	DC.B	'ADDI',0
lpszAND		DC.B	'AND',0
lpszANDI	DC.B	'ANDI',0
lpszASL		DC.B	'ASL',0
lpszASR		DC.B	'ASR',0
lpszBSR		DC.B	'BSR',0
lpszCLR		DC.B	'CLR',0
lpszCMP		DC.B	'CMP',0
lpszCMPA	DC.B	'CMPA',0
lpszCMPI	DC.B	'CMPI',0
lpszEOR		DC.B	'EOR',0
lpszEORI	DC.B	'EORI',0
lpszEXG		DC.B	'EXG',0
lpszJMP		DC.B	'JMP',0
lpszJSR		DC.B	'JSR',0
lpszLEA		DC.B	'LEA',0
lpszLSL		DC.B	'LSL',0
lpszLSR		DC.B	'LSR',0
lpszMOVE	DC.B	'MOVE',0
lpszMOVEA	DC.B	'MOVEA',0
lpszMOVEM	DC.B	'MOVEM',0
lpszNEG		DC.B	'NEG',0
lpszNOP		DC.B	'NOP',0
lpszNOT		DC.B	'NOT',0
lpszOR		DC.B	'OR',0
lpszORI		DC.B	'ORI',0
lpszROL		DC.B	'ROL',0
lpszROR		DC.B	'ROR',0
lpszRTS		DC.B	'RTS',0
lpszSUB		DC.B	'SUB',0
lpszSUBA	DC.B	'SUBA',0
lpszSUBI	DC.B	'SUBI',0
lpszSWAP	DC.B	'SWAP',0
lpszD0		DC.B	'D0',0
lpszD1		DC.B	'D1',0
lpszD2		DC.B	'D2',0
lpszD3		DC.B	'D3',0
lpszD4		DC.B	'D4',0
lpszD5		DC.B	'D5',0
lpszD6		DC.B	'D6',0
lpszD7		DC.B	'D7',0
lpszA0		DC.B	'A0',0
lpszA1		DC.B	'A1',0
lpszA2		DC.B	'A2',0
lpszA3		DC.B	'A3',0
lpszA4		DC.B	'A4',0
lpszA5		DC.B	'A5',0
lpszA6		DC.B	'A6',0
lpszA7		DC.B	'A7',0
rgszDn		DC.W	lpszD0,lpszD1,lpszD2,lpszD3,lpszD4,lpszD5,lpszD6,lpszD7
rgszAn		DC.W	lpszA0,lpszA1,lpszA2,lpszA3,lpszA4,lpszA5,lpszA6,lpszA7
rgszMOVEM1	DC.W	lpszD0,lpszD1,lpszD2,lpszD3,lpszD4,lpszD5,lpszD6,lpszD7,lpszA0,lpszA1,lpszA2,lpszA3,lpszA4,lpszA5,lpszA6,lpszA7
rgszMOVEM2	DC.W	lpszA7,lpszA6,lpszA5,lpszA4,lpszA3,lpszA2,lpszA1,lpszA0,lpszD7,lpszD6,lpszD5,lpszD4,lpszD3,lpszD2,lpszD1,lpszD0
bDollar		DC.B	$24
bHash		DC.B	$23
bComma		DC.B	$2C
bSlash		DC.B	$2F
wEAMask		DC.W	$3F
bOpParen	DC.B	$28
bClParen	DC.B	$29
bPlus		DC.B	$2B
bMinus		DC.B	$2D
bP		DC.B	$50
bC		DC.B	$43

*struct InstructionInfo
*{
*    // Const mask (immutable part of instruction word)
*    word wConstMask;
*
*    // Pattern to match after const mask is applied.
*    word wConstMatch;
*
*    // Dynamic mask (changable part of instruction word)
*    // Set to 0 if no variable mask is required.
*    word wVariableMask;
*
*    // Range of valid values for variable mask. Each
*    // bit field represents the numbers 0-7.
*    // Ignored if wVariableMask is 0.
*    word wVariableRange;
*
*    // Pointer to zero-terminated opcode string
*    word wpszOpcode;
*
*    // Size mask (size bits)
*    // Used to retrieve the instruction size.
*    // Set to 0 if no size mask is required.
*    word wSizeMask;
*
*    // Array of string pointers that map to the size index for the instruction.
*    // This array is passed to a function that maps it to a size mask.
*    word* rgszSize;
*
*    // long DecodeOperands(word wpszBuffer, word wInst)
*    // Function pointer that decodes the operands.
*    // Set to 0 if no function is required to decode
*    // the operands.
*    word wpfnOperands;
*};

* Array of InstructionInfo structs that stores information about each instruction.
* Note that struct members are words, not longs, and that some of these words are
* function pointers. This means that the layout of the table below and the
* associated table parsing instructions would need to be modified if required functions
* were to extend past 0xFFFF in memory.
* Fields:
*               wConstMask wConstMatch wVariableMask wVariableRange wpszOpcode wSizeMask rgszSize wpfnOperands
ADDi	DC.W	$F000,     $D000,      $C0,          $7,            lpszADD,   $C0,      rgsz2,   _deDn2Way
ADDAi	DC.W	$F0C0,     $D0C0,      0,            0,             lpszADDA,  $100,     rgsz1,   _deAn1Way
ADDIi	DC.W	$FF00,     $0600,      0,            0,             lpszADDI,  $C0,      rgsz2,   _deOpI
ANDi	DC.W	$F000,     $C000,      0,            0,             lpszAND,   $C0,      rgsz2,   _deDn2Way
ANDIi	DC.W	$FF00,     $0200,      0,            0,             lpszANDI,  $C0,      rgsz2,   _deOpI
ASLi1	DC.W	$F118,     $E100,      0,            0,             lpszASL,   $C0,      rgsz2,   _deShiftRot
ASLi2	DC.W	$FFC0,     $E1C0,      0,            0,             lpszASL,   0,        0,       _deShiftRot
ASRi1	DC.W	$F118,     $E000,      0,            0,             lpszASR,   $C0,      rgsz2,   _deShiftRot
ASRi2	DC.W	$FFC0,     $E0C0,      0,            0,             lpszASR,   0,        0,       _deShiftRot
BSRi	DC.W	$FF00,     $6100,      0,            0,             lpszBSR,   0,        0,       _deBSR
CLRi	DC.W	$FF00,     $4200,      0,            0,             lpszCLR,   $C0,      rgsz2,   _deEA
CMPi	DC.W	$F100,     $B000,      $C0,         $7,             lpszCMP,   $C0,      rgsz2,   _deDn1Way
CMPAi	DC.W	$F0C0,     $B0C0,      0,            0,             lpszCMPA,  $100,     rgsz1,   _deAn1Way
CMPIi	DC.W	$FF00,     $0C00,      0,            0,             lpszCMPI,  $C0,      rgsz2,   _deOpI
EORi	DC.W	$F100,     $B100,      $C0,         $7,             lpszEOR,   $C0,      rgsz2,   _deDnEA1Way
EORIi	DC.W	$FF00,     $0A00,      0,            0,             lpszEORI,  $C0,      rgsz2,   _deOpI
EXGi	DC.W	$F130,     $C100,      0,            0,             lpszEXG,   0,        0,       _deEXG
JMPi	DC.W	$FFC0,     $4EC0,      0,            0,             lpszJMP,   0,        0,       _deEA
JSRi	DC.W	$FFC0,     $4E80,      0,            0,             lpszJSR,   0,        0,       _deEA
LEAi	DC.W	$F1C0,     $41C0,      0,            0,             lpszLEA,   0,        0,       _deAn1Way
LSLi1	DC.W	$F118,     $E108,      0,            0,             lpszLSL,   $C0,      rgsz2,   _deShiftRot
LSLi2	DC.W	$FFC0,     $E3C0,      0,            0,             lpszLSL,   0,        0,       _deShiftRot
LSRi1	DC.W	$F118,     $E008,      0,            0,             lpszLSR,   $C0,      rgsz2,   _deShiftRot
LSRi2	DC.W	$FFC0,     $E2C0,      0,            0,             lpszLSR,   0,        0,       _deShiftRot
MOVEi	DC.W	$C000,     0,          $3000,        $E,            lpszMOVE,  $3000,    rgszMOVE,_deMOVE
MOVEAi	DC.W	$E1C0,     $2040,      0,            0,             lpszMOVEA, $1000,    rgszMOVEA,_deAn1Way
MOVEMi	DC.W	$FB80,     $4880,      0,            0,             lpszMOVEM, $40,      rgsz1,   _deMOVEM
NEGi	DC.W	$FF00,     $4400,      0,            0,             lpszNEG,   $C0,      rgsz2,   _deEA
NOPi	DC.W	$FFFF,     $4E71,      0,            0,             lpszNOP,   0,        0,       0
NOTi	DC.W	$FF00,     $4600,      0,            0,             lpszNOT,   $C0,      rgsz2,   _deEA
ORi	DC.W	$F000,     $8000,      0,            0,             lpszOR,    0,        0,       _deDn2Way
ORIi	DC.W	$FF00,     $0000,      0,            0,             lpszORI,   $C0,      rgsz2,   _deOpI
ROLi1	DC.W	$F118,     $E118,      0,            0,             lpszROL,   $C0,      rgsz2,   _deShiftRot
ROLi2	DC.W	$FFC0,     $E7C0,      0,            0,             lpszROL,   0,        0,       _deShiftRot
RORi1	DC.W	$F118,     $E018,      0,            0,             lpszROR,   $C0,      rgsz2,   _deShiftRot
RORi2	DC.W	$FFC0,     $E6C0,      0,            0,             lpszROR,   0,        0,       _deShiftRot
RTSi	DC.W	$FFFF,     $4E75,      0,            0,             lpszRTS,   0,        0,       0
SUBi	DC.W	$F000,     $9000,      $C0,          $7,            lpszSUB,   $C0,      rgsz2,   _deDn2Way
SUBAi	DC.W	$F0C0,     $90C0,      0,            0,             lpszSUBA,  $100,     rgsz1,   _deAn1Way
SUBIi	DC.W	$FF00,     $0800,      0,            0,             lpszSUBI,  $C0,      rgsz2,   _deOpI
SWAPi	DC.W	$FFF8,     $4840,      0,            0,             lpszSWAP,  0,        0,       _deSWAP

rgInstructions	DC.W	ADDi,ADDAi,ADDIi,ANDi,ANDIi,ASLi1,ASLi2,ASRi1,ASRi2,BSRi,CLRi,CMPi,CMPAi,CMPIi,EORi,EORIi,EXGi,JMPi,JSRi,LEAi,LSLi1,LSLi2,LSRi1,LSRi2,MOVEi,MOVEAi,MOVEMi,NEGi,NOPi,NOTi,ORi,ORIi,ROLi1,ROLi2,RORi1,RORi2,RTSi,SUBi,SUBAi,SUBIi,SWAPi
cInstructions	DC.W	(((cInstructions-rgInstructions))/2)

*****************************************************************************
*                                subroutines                                *
*****************************************************************************
	ORG	$2000

*****************************************************************************
* long _deEXG(word wInst)
** Description: decodes the EXG instruction, writing the 
*               result to output buffer
** Parameters: wInst: Instruction
** Return value: offset to add to lCurrentInstruction
*****************************************************************************
_deEXG
	MOVEM.L A1-A3/D1,-(SP)
	* A0: global output buffer
	* A1: rgszXn1
	* A2: rgszXn2
	* A3: temp space
	*
	* D0: return value
	* D1: wInst
	
	MOVE.W	20(SP),D1 * Get wInst
	
	MASK	#$F8,D1 * Get opmode
	CMPI.L	#8,D0 * data registers
	BEQ	deexgDn
	CMPI.L	#9,D0 * address registers
	BEQ	deexgAn
	BRA	deexgXn * data and address register
deexgDn
	LEA rgszDn,A1
	LEA rgszDn,A2
	BRA	deexgDoIt
deexgAn
	LEA rgszAn,A1
	LEA rgszAn,A2
	BRA	deexgDoIt
deexgXn
	LEA rgszDn,A1
	LEA rgszAn,A2
deexgDoIt
	MASK	#$E00,D1 * Get rgx
	PRINTINDEX D0,A1,A3 * Write rgx

	MOVE.B	bComma,(A0)+ * Write comma
	MASK	#7,D1 * Get rgy
	PRINTINDEX D0,A2,A3 * Write rgy

	CLR.L	D0 * Set return value
	
	MOVEM.L (SP)+,A1-A3/D1
	RTS

*****************************************************************************
* long _deBSR(word wInst)
** Description: decodes the BSR instruction, writing the 
*               result to output buffer
** Parameters: wInst: Instruction
** Return value: offset to add to lCurrentInstruction
*****************************************************************************
_deBSR
	MOVEM.L A1/D1-D3,-(SP)
	* A0: global output buffer
	* A1: lCurrentAddress
	*
	* D0: return value
	* D1: wInst
	* D2: address
	* D3: Cumulative offset

	CLR.L	D2 * Clear address
	CLR.L	D3 * Clear cumulative offset
	MOVE.W	20(SP),D1 * Get wInst
	MOVE.L	lCurrentAddress,A1 * Get current address

	MASK	#$FF,D1 * Get address byte
	CMP.B	#0,D0 * 0 means the address is in the first word after the instruction
	BEQ	_debW
	
	CMPI.B	#$FF,D0 * $FF means that the address is in the first long word after the instruction
	BEQ	_debL
	
	MOVE.B	D0,D2 * Move the masked address byte to D2
	MOVE.L	A1,D0 * Make a temporary copy
	ADD.B	D2,D0 * Calculate branch address
	MOVE.L	D0,A1
	BRA	_debDoIt	
_debW
	MOVE.W	(A1),D2 * Move the address word to D2
	ADDQ.B	#2,D3
	MOVE.L	A1,D0 * Make a temporary copy
	ADD.W	D2,D0 * Calculate branch address
	MOVE.L	D0,A1
	BRA	_debDoIt
_debL
	MOVE.L	(A1),D2 * Move the address long word to D2
	ADDQ.B	#4,D3
	MOVE.L	A1,D0 * Make a temporary copy
	ADD.L	D2,D0 * Calculate branch address
	MOVE.L	D0,A1
_debDoIt
	MOVE.B	bDollar,(A0)+ * Write $
	
	LONGTOSTRING	A1,A0 * Write int
	ADD.L	D0,A0

	MOVE.L	D3,D0 * Set return value
	
	MOVEM.L (SP)+,A1/D1-D3
	RTS
	
*****************************************************************************
* long _deMOVEM(word wInst)
** Description: decodes the MOVEM instruction, writing the 
*               result to output buffer
** Parameters: wInst: Instruction
** Return value: offset to add to lCurrentInstruction
*****************************************************************************
_deMOVEM
	MOVEM.L A1-A2/D1-D6,-(SP)
	* A0: global output buffer
	* A1: rgszXn
	* A2: lCurrentAddress
	*
	* D0: return value
	* D1: wInst
	* D2: direction: register to memory or memory to register
	* D3: 1 if mask order is D0-D7/A0-A7 (opmode is 100); 0 if mask order is A7-A0/D7-D0
	* D4: Cumulative offset
	* D5: register mask
	* D6: EA
	
	CLR.L	D4 * Clear cumulative offset

	MOVE.W	36(SP),D1 * Get wInst
	MOVE.L	lCurrentAddress,A2 * Get current address
	
	MASK	#$38,D1 * Get register mask order
	CMPI.W	#4,D0 * If match is %100, mask order is D0-D7/A0-A7
	BNE	demm7to0 * Otherwise, mask order is A7-A0/D7-D0
	
demm0to7
	LEA	rgszMOVEM2,A1 * Load array of string pointers
	BRA	demmDir
demm7to0
	LEA	rgszMOVEM1,A1 * Load array of string pointers
demmDir
	MASK	wEAMask,D1 * Get EA
	MOVE.W	D0,D6 * Store EA
	
	MASK	#$400,D1 * Get dr
	MOVE.W	D0,D2 * Store dr
	CMPI.W	#1,D2
	BEQ	demmMtoR  * Go in the right direction
demmRtoM * Print reglist,<ea>
	BRA	demmRegList
demmRtoMEA
	MOVE.B	bComma,(A0)+ * Write comma

	DECODEEA D6
	CMP.L   #-1,D0
	BEQ demmError

	ADD.W	D0,D4
	BRA	demmSuccess

demmMtoR * Print <ea>,reglist	
	DECODEEA D6
	CMP.L   #-1,D0
	BEQ demmError

	ADD.W	D0,D4
	MOVE.B	bComma,(A0)+ * Write comma
demmRegList
	* D0: return value
	* D1: reg mask
	* D2: direction: register to memory or memory to register
	* D3: Max index
	* D4: Cumulative offset
	* D5: Current index
	
	CLR.L	D5 * Clear current index
	MOVE.W	#15,D3 * Init max index

	MOVE.W	(A2),D1 * Get the reg mask
	ADDI.W	#2,D4 * Increment the cumulative address offset
	
demmNextBit
	BTST #0,D1 * Test position zero in reg mask
	BEQ	demmASR * If bit is 0, Move to next bit
	MOVE.W	D5,D0 * Copy counter
	PRINTINDEX D0,A1,A2 * Else, print the reg string
	ASR.L	#1,D1
	CMPI.W	#0,D1 * Test reg mask to determine if a slash is required
	BEQ	demmIncr * If mask is 0, Move to next bit
	MOVE.B	bSlash,(A0)+ * Else, print a slash
	BRA	demmIncr
demmASR
	* Shift reg mask one position right and increment counter
	ASR.L	#1,D1
demmIncr	
	ADDI.W	#1,D5
	CMP.W	D3,D5
	BLE	demmNextBit * Loop while current index <= maxIndex
	
	CMPI.W	#0,D2 * Check dr flag
	BEQ demmRtoMEA * If there's still an EA to print, do it.
demmSuccess
	MOVE.L	D4,D0 * Set return value
	BRA demmExit
demmError
	MOVE.L  #-1,D0
demmExit	
	MOVEM.L (SP)+,A1-A2/D1-D6
	RTS	

*****************************************************************************
* long _deMOVE(word wInst)
** Description: decodes the MOVE instruction, writing the 
*               result to output buffer
** Parameters: wInst: Instruction
** Return value: offset to add to lCurrentInstruction
*****************************************************************************
_deMOVE
	MOVEM.L A1-A2/D1-D4,-(SP)
	
	* A0: global output buffer
	* A1: rgszAn
	* A2: lCurrentAddress
	*
	* D0: return value
	* D1: wInst
	* D2: EA
	* D3: return value aggregator
	* D4: temp space
	
	MOVE.W	28(SP),D1 * Get wInst
	LEA	rgszAn,A1  * Load rgszAn
	MOVE.L	lCurrentAddress,A2 * Get current address	

	MASK	wEAMask,D1 * Get src EA
	MOVE	D0,D2	
	
	* Add size to highest two bits of EA byte
	MASK	#$3000,D1 * Get size
	* Convert custom size to standard decodeEA parameters
	CMPI.B	#1,D0
	BNE	demWord
	MOVEQ #0,D0
	BRA demOR
demWord
	CMPI.B	#3,D0
	BNE	demLong
	MOVEQ #1,D0
	BRA demOR
demLong
	MOVEQ #2,D0
demOR
	ASL.L	#6,D0
	OR.L	D0,D2

	DECODEEA D2
	CMP.L   #-1,D0
	BEQ demExit

	ADDA.L	D0,A2 * Increment lCurrentAddress
	MOVE.L	A2,lCurrentAddress * Set current address	
	MOVE	D0,D3 * Save offset
	
	MOVE.B	bComma,(A0)+ * Write comma

	* Reverse dest EA octals
	MASK	#$FC0,D1 * Get dest EA
	MOVE.B	D0,D2
	MOVE.B	D2,D4 * Make a copy
	ASR.B	#3,D2 * Swap ear and eam
	ASL.B	#3,D4
	OR.B	D4,D2
	AND.B	#$3F,D2

	DECODEEA D2
	CMP.L   #-1,D0
	BEQ demExit
	
	* Add up cumulative offset
	ADD.L	D0,D3
	MOVE.L	D3,D0
demExit
	MOVEM.L (SP)+,A1-A2/D1-D4
	RTS
	
*****************************************************************************
* long _deDnEA1Way(word wInst)
** Description: decodes instruction into Dn,<ea>, writing the 
*               result to output buffer
** Parameters: wInst: Instruction
** Return value: offset to add to lCurrentInstruction
*****************************************************************************
_deDnEA1Way
	DEXN2WAY rgszDn,#0,#1
	RTS
	
*****************************************************************************
* long _deDn1Way(word wInst)
** Description: decodes instruction into <ea>,Dn, writing the 
*               result to output buffer
** Parameters: wInst: Instruction
** Return value: offset to add to lCurrentInstruction
*****************************************************************************
_deDn1Way
	DEXN2WAY rgszDn,#1,#1
	RTS
	
*****************************************************************************
* long _deAn1Way(word wInst)
** Description: decodes instruction into <ea>,An, writing the 
*               result to output buffer
** Parameters: wInst: Instruction
** Return value: offset to add to lCurrentInstruction
*****************************************************************************
_deAn1Way
	DEXN2WAY rgszAn,#1,#0
	RTS
	
*****************************************************************************
* long _deDn2Way(word wInst)
** Description: decodes instruction into Dn,<ea> or <ea>,Dn, writing the 
*               result to output buffer
** Parameters: wInst: Instruction
** Return value: offset to add to lCurrentInstruction
*****************************************************************************
_deDn2Way
	DEXN2WAY rgszDn,#2,#1
	RTS
	
*****************************************************************************
* long _deXn2Way(word wInst, word* rgszXn, word wDir)
** Description: decodes instruction into Xn,<ea> or <ea>,Xn, writing the 
*               result to output buffer
** Parameters: wInst: Instruction
**             rgszXn: Pointer to an array of register strings
**             wDir: 0 if only Xn,<ea>. 1 if only <ea>,Xn. 2 if either.
**             wSize: 0 if W/L; 1 if B/W/L
** Return value: offset to add to lCurrentInstruction
*****************************************************************************
_deXn2Way
	MOVEM.L A1-A2/D1-D4,-(SP)
	* A0: global output buffer
	* A1: rgszDn
	* A2: temp space
	*
	* D0: return value
	* D1: wInst
	* D2: reg
	* D3: EA
	* D4: wDir
	
	MOVE.W	28(SP),D5 * Get wSize
	MOVE.W	30(SP),D4 * Get wDir
	MOVE.W	32(SP),A1 * Get rgszXn
	MOVE.W	34(SP),D1 * Get wInst
	
	MASK	#$E00,D1 * Get reg
	MOVE	D0,D2
	
	MASK	wEAMask,D1 * Get EA
	MOVE	D0,D3
	
* If wSize is zero, mask for W/L
	CMPI.W	#0,D5
	BNE	_deXn2BWL
	MASK	#$100,D1 * Get size
	
	* Convert W/L size to B/W/L format
	CMPI.B	#0,D0
	BNE	_deXn2Long
	MOVEQ #1,D0
	BRA _deXn2OR
_deXn2Long
	MOVEQ #2,D0
	BRA _deXn2OR
* Else, mask for B/W/L
_deXn2BWL
	MASK	#$C0,D1 * Get size	
_deXn2OR
	LSL.L	#6,D0
	OR.L	D0,D3

	CMPI.L	#0,D4
	BEQ	twoWayXnEA * Check user flags
	CMPI.L	#1,D4
	BEQ	twoWayEAXn
	
	MASK	#$100,D1 * If we can go both ways, check the operand order bit
	CMPI.L	#1,D0
	BEQ	twoWayXnEA
	BRA	twoWayEAXn
	
twoWayXnEA	
	PRINTINDEX D2,A1,A2
	MOVE.B	bComma,(A0)+ * Write comma
	DECODEEA D3
	BRA twoWayExit
twoWayEAXn
	DECODEEA D3
	MOVE.B	bComma,(A0)+ * Write comma
	PRINTINDEX D2,A1,A2
twoWayExit
	MOVEM.L (A7)+,A1-A2/D1-D4
	RTS

*****************************************************************************
* long _deSWAP(word wInst)
** Description: decodes SWAP, writing 
*               the result to output buffer
** Parameters:
** Return value: offset to add to lCurrentInstruction
*****************************************************************************
_deSWAP
	MOVEM.L A1-A2/D1,-(A7)
	* A0: global output buffer
	* A1: rgszDn
	* A2: temp space
	*
	* D0: return value
	* D1: wInst
	
	MOVE.W	16(SP),D1 * Get wInst
	LEA rgszDn,A1 Load data register string array
	
	* Get Dn
	MASK	#7,D1
	
	PRINTINDEX D0,A1,A2
	CLR.L	D0
	
	MOVEM.L (A7)+,A1-A2/D1
	RTS
*****************************************************************************
* long _deShiftRot(word wInst)
** Description: decodes operands for ASL,ASR,LSL,LSR,ROL,ROR, writing 
*               the result to output buffer
** Parameters:
** Return value: offset to add to lCurrentInstruction
*****************************************************************************
_deShiftRot
	MOVEM.L A1-A2/D1-D3,-(A7)
	* A0: global output buffer
	* A1: rgszDn
	* A2: temp space
	*
	* D0: return value
	* D1: wInst
	* D2: count/Dx for register mode
	* D3: Dy
	
	MOVE.W	24(SP),D1 * Get wInst
	
	LEA rgszDn,A1 Load data register string array
	
	* Determine register or memory shift
	MASK	#$C0,D1
	CMPI.W	#3,D0
	BEQ	deSRMem
deSRReg
	* Get Dx/immediate value
	MASK	#$E00,D1
	MOVE.W	D0,D2 * Store Dx/immediate value
	
	* Get Dy
	MASK	#7,D1
	MOVE.W	D0,D3 * Store Dy
	
	* Determine immediate or register mode
	MASK	#$20,D1
	CMPI.B	#1,D0
	BEQ	deSRDx * 1 is register mode
	
	* Immediate mode - 0 actually means 8
	CMPI.B	#0,D2
	BNE	deSRImmed
	MOVE.B	#8,D2	
	
deSRImmed
	* Write immediate value
	MOVE.B	bHash,(A0)+ * Write #
	
	LONGTOSTRING	D2,A0 * Write int
	ADD.L	D0,A0
	MOVE.B	bComma,(A0)+ * Write comma
	
	BRA	deSRDy
deSRDx
	PRINTINDEX D2,A1,A2
	MOVE.B	bComma,(A0)+ * Write comma
deSRDy
	PRINTINDEX D3,A1,A2
	CLR.L	D0 * Return zero if no EA
	BRA	deSRExit
deSRMem
	MASK	wEAMask,D1
	DECODEEA D0
deSRExit
	MOVEM.L (A7)+,A1-A2/D1-D3
	RTS

*****************************************************************************
* long _deOpI(word wInst)
** Description: decodes operands for ADDI,ORI,ANDI,SUBI,EORI,CMPI, writing 
*               the result to output buffer
** Parameters: 
** Return value: offset to add to lCurrentInstruction
*****************************************************************************
_deOpI
	MOVEM.L A1-A2/D1-D3,-(A7)
	* A0: global output buffer
	* A1: Address of current instruction (retrieved from global)
	* A2: Address of instruction after immediate data retrieval
	* D0: return value
	* D1: wInst
	* D2: temp data
	* D3: Size
	
	CLR.L	D0
	MOVE.W	24(SP),D1 * Get wInst
	MOVE.L	lCurrentAddress,A1 * Get current address
	MOVEA.L A1,A2 * Copy ptr
	
	MASK	#$C0,D1 * Get size
	MOVE.L	D0,D3 * Save it for later use

	* Check size to read the correct amount of immediate data	
	CMPI.W	#1,D0
	BGT	deopiL
	MOVE.W	(A2)+,D2 * Read one word of immediate data
	BRA	deopiEA
deopiL
	MOVE.L	(A2)+,D2 * Read a long word of immediate data
deopiEA	
	* Write immediate value
	MOVE.B	bHash,(A0)+ * Write # * Write #<data>
	MOVE.B	bDollar,(A0)+ * Write $
	
	MOVE.L	A2,lCurrentAddress * Set current address	

	LONGTOSTRING	D2,A0 * Write int
	ADD.L	D0,A0
	MOVE.B	bComma,(A0)+ * Write comma

	 * Get EA
	MASK	wEAMask,D1
	ASL.L	#6,D3 * Attach size bits to EA
	OR.L	D3,D0
	
	DECODEEA D0 * decodeEA returns the number of bytes by which to increment lCurrentAddress
	CMP.L   #-1,D0
	BEQ deopiExit

	ADD.W	D0,A2

	SUB.L	A1,A2 * Calculate return value
	MOVE.L	A2,D0

deopiExit	
	MOVEM.L (A7)+,A1-A2/D1-D3
	RTS

*****************************************************************************
* long _deEA(word wInst)
** Description: decodes the effective address operand for CLR,JMP,JSR,LEA,NEG,NOT, writing 
*               the result to output buffer.
** Parameters: 
** Return value: offset to add to lCurrentInstruction
*****************************************************************************
_deEA
	MOVEM.L D1,-(A7)
	* A0: global output buffer
	* D0: return value
	* D1: wInst
	* D2: temp data
	MOVE.W	8(SP),D1 * Get wInst
	
	MASK	wEAMask,D1 * Write EA
	DECODEEA D0
	
	MOVEM.L (A7)+,D1
	RTS	
	
*****************************************************************************
* long decodeInstruction(word wInstruction)
** Description: Decodes an instruction
** Parameters: wInstruction: instruction to decode
** Return value: The number of bytes by which to increment lCurrentAddress on success, -1 on failure
** Remarks: This function consumes globals lCurrentAddress and lpszInstOutput
** Author: Ethan Crawford
*****************************************************************************
_decodeInstruction
	MOVEM.L A0-A2/D1-D3,-(A7)
	
	* A0: lpszInstOutput
	* A1: lCurrentAddress
	* A2: rgInstructions
	* A3: Pointer to current InstructionInfo struct member
	* A4: temp space
	*
	* D0: Return value
	* D1: wInstruction
	* D2: loop counter
	* D3: temp space
	* D4: temp space

	CLR.L	D0
	MOVE.W	28(SP),D1 * Get wInstruction
	LEA	lpszInstOutput,A0 * Get instruction output buffer
	MOVEA.L	lCurrentAddress,A1 * Get current address
	LEA	rgInstructions,A2 * Load the struct array
	MOVE.W	cInstructions,D2 * Init struct table counter
	
	* Write instruction address and whitespace
	ADDWS A0	* Add whitespace
	ADDA.L D0,A0	* Advance pointer

	LONGTOSTRING A1,A0 * Write address
	ADD.L	D0,A0	* Advance pointer
	
	ADDWS A0	* Add whitespace
	ADDA.L D0,A0	* Advance pointer
	
* For each entry in the struct:
diNextInstr
	* Get a struct from the struct pointer array
	SUBI.W	#1,D2 * Get the next element
	
	CMPI.W	#-1,D2 * If there are no more struct elements to check, exit
	BLE	diErr
	
	MOVE.L	D2,D3 * Calculate struct address offset
	MULU.W	#2,D3 * Struct is word-aligned, so multiply by two
	MOVE.L	A2,A3 * Copy rgInstructions
	ADDA.L	D3,A3 * Add offset to rgInstructions to get current struct pointer
	MOVEA.W	(A3),A3 * Load the current struct (pointer to a pointer, cool)

	* Check const mask
	MOVE.W	(A3)+,D3 * Get wConstMask
	MOVE.W	D1,D4 * Apply wConstMask to word
	AND.W	D3,D4
	MOVE.W	(A3)+,D3 * Get wConstMatch
	CMP.W	D4,D3 * Compare wConstMatch to word
	BNE	diNextInstr * If not a match, search again
	
	* Check variable mask
	MOVE.W	(A3)+,D3 * Get wVariableMask
	CMP.W	#0,D3 * If it's zero, skip to next section
	BEQ	diNoVarMask
	MASK	D3,D1
	MOVE.W	D0,D4
	
	MOVE.W	(A3)+,D3 * Get wVariableRange
	* Test the bit indicated by the variable value against the variable range
	BTST	D4,D3 * Check if this range is supported
	* If FALSE, that is, if BTST did not set the Z bit because the tested bitfield is a 
	* 1, we have a match. Print the string to the buffer.
	BNE	diWriteOpcode
	BRA	diNextInstr * Else, loop again
	
diNoVarMask
	LEA 2(A3),A3 * Skip over wVariableRange
* Write opcode string
diWriteOpcode
	MOVE.W	(A3)+,A4 * Get string pointer
	CMP.W	#0,A4 * Check for null pointer
	BEQ	diSize
	
	STRCPY A4,A0
	
* Write size suffix
diSize
	MOVE.W	(A3)+,D3 * Get size mask
	CMPI.W	#0,D3
	BEQ	diNoSize * If it's zero, skip this section

	MOVEA.W	(A3)+,A4 * Get size array
	CMP.W	#0,A4
	BEQ	diWS * If it's zero, skip this section

	MASK	D3,D1
	
    * Call decodeSize
	MOVE.W	D0,-(SP) * Push wSize
	MOVE.W	A4,-(SP) * Push rgszSize
	MOVE.W	A0,-(SP) * Push wpszBuffer
    BSR     _decodeSize
	LEA	6(SP),SP * Clean up stack
	ADDQ.W	#2,A0 * Add 2 to string pointer
	BRA	diWS

* Jump here if no size parameter is specified
diNoSize
	LEA 2(A3),A3 * Skip over wpfnSize
* Write whitespace
diWS
	ADDWS	A0	* Add whitespace
	ADDA.L	D0,A0	* Advance pointer

* Write operands
	ADDQ.L	#2,A1 * Opcode word is written, skip past it
	MOVE.L	A1,lCurrentAddress * Set current address	
	
	MOVEA.W	(A3)+,A3 * Get operands function pointer
	CMP.W	#0,A3
	BEQ	diExit * If it's zero, skip this section	
	
	MOVE.W	D1,-(SP) * Push wInstruction
	JSR	(A3) * call operands function
	LEA	2(SP),SP * Clean up stack
	
	CMPI.W	#-1,D0 * Check return value for success
	BEQ	diErr
	
	ADD.W	D0,A1 * Add return value to lCurrentAddress
	BRA	diExit * All is well, goto exit
	
diErr	
	MOVE.L #-1,D0 * Return error
	BRA	diCleanup
diExit
	MOVE.L	A1,lCurrentAddress * Set current address	
	MOVE.B	#0,(A0)	* Null-terminate the string
diCleanup
	MOVEM.L (A7)+,A0-A2/D1-D3
	RTS

*****************************************************************************
* void _decodeSize(word wSize, word* rgszSize, word wpszBuffer)
** Description: Decodes wSize using the indices of string pointers in rgszSize,
**              writing the result to wpszBuffer
** Parameters: wpszBuffer: output buffer
**             wSize: word containing the size
** Return value: void
*****************************************************************************
_decodeSize
	MOVEM.L A0-A1/D1,-(A7)
	* A0: wpszBuffer
	* A1: rgszSize and size string buffer
	*
	* D0: (not used)
	* D1: wSize

	CLR.L	D0
	MOVE.W	16(SP),A0 * Get wpszBuffer
	MOVE.W	18(SP),A1 * Get rgszSize
	MOVE.W	20(SP),D1 * Get wSize
	ASL.W	#1,D1	  * Multiply by two for ptr arithmetic
    
	ADD.W   D1,A1 * Add offset to rgszSize to get string ptr
	MOVEA.W	(A1),A1 * Load ptr
	
	STRCPY A1,A0
	
	MOVEM.L (A7)+,A0-A1/D1
	RTS
	
*****************************************************************************
* long decodeEA(byte bEA)
** Description: Decodes an effective address
** Parameters: 
**             bEA:        effective address
** Return value: number of bytes by which to increment the current address
** Remarks: This function consumes globals lCurrentAddress and lpszInstOutput
** Author: Michael Grimm
*****************************************************************************
_decodeEA
	MOVEM.L A1-A3/D1-D4,-(A7)	*Save these registers
	* D1 = Effective Address -> Mode bits
	* D2 = Effective Address -> Register bits
	* D3 = For operations
	* D4 = EA -> Size bits
	* 	     00 = .B; 01 = .W; 10 = .L
	* A1 = pointer for register array
	* A2 = lCurrentAddress
	* A3 = alternate lCurrentAddress
	CLR.L	D0			*Clear D0 for the return
	CLR.L	D1
	CLR.L	D2
	CLR.L	D3
	CLR.L	D4
	MOVE.W	32(SP),D1 		*Load bEA into D1 (Mode bits)
	MOVE.B	D1,D2			*Copy bEA into D2 (Register bits)
	MOVE.B	D1,D4			*copy bEA into D4 (Size bits)
	
	ANDI.B	#%00111000,D1		*Mask off the Mode bits
	LSR.B	#3,D1			*Shift the Mode bits right by 3
	ANDI.B	#$07,D2			*Mask off the Register bits
	LSR.B	#6,D4			*Shift size bits 6 to the right
	*Ready to decode the EA
	
	CMPI.B	#Dn,D1			*Is this data register direct?
	BEQ	_Dn
	CMPI.B	#An,D1			*Is this address register direct?
	BEQ	_An
	CMPI.B	#iAn,D1			*Is this address register indirect?
	BEQ	_iAn
	CMPI.B	#iiAn,D1		*Is this address register indirect, post-increment?
	BEQ	_iiAn
	CMPI.B	#idAn,D1		*Is this address register indirect, pre-decrement?
	BEQ	_idAn
	CMPI.B	#i16An,D1		*Is this address register indirect, 16-bit displacement?
	BEQ	_i16An
	CMPI.B	#i8An,D1		*Is this address register indirect, 8-bit displacement?
	BEQ	_i8An
	CMPI.B	#mode7,D1		*Is this another addressing mode?
	BEQ	_mode7
	BRA	_modeError
	
_Dn	
	LEA	rgszDn,A1	*Load data register string pointer into A1
	MULU.W	#2,D2		*Multiply register bits by 2 to account for word size
	ADD.W	D2,A1		*Set the array offset to grab the right string
	MOVEA.W	(A1),A1		*Get the right Dx string out of the array
				*Add 'Dx' to lpszInstOutput
	STRCPY	A1, A0
	BRA	_decodeEAend
_An
	BSR	_printAddrReg
	BRA	_decodeEAend
_iAn
	MOVE.B	bOpParen,(A0)+	* Write (
	BSR	_printAddrReg
	MOVE.B	bClParen,(A0)+	* Write )
	BRA	_decodeEAend
_iiAn
	MOVE.B	bOpParen,(A0)+	* Write (
	BSR	_printAddrReg
	MOVE.B	bClParen,(A0)+	* Write )
	MOVE.B	bPlus,(A0)+	* Write +
	BRA	_decodeEAend
_idAn
	MOVE.B	bMinus,(A0)+	* Write -
	MOVE.B	bOpParen,(A0)+	* Write (
	BSR	_printAddrReg
	MOVE.B	bClParen,(A0)+	* Write )
	BRA	_decodeEAend
_i16An
	MOVE.B	bDollar,(A0)+	* Write $
	MOVEA.L	lCurrentAddress,A2	*Load the current address
	MOVE.W	(A2),D3
	CMPI.W	#0,D3
	BGE	_i16AnNext
	NEG.W	D3
	MOVE.B	bMinus,(A0)+		* Write -
_i16AnNext

	LONGTOSTRING	D3,A0
	ADD.L	D0,A0
	MOVE.B	bOpParen,(A0)+	* Write (
	BSR	_printAddrReg
	MOVE.B	bClParen,(A0)+	* Write )
	MOVEQ.L	#2,D0
	BRA	_decodeEAend
_i8An
	MOVE.B	bOpParen,(A0)+		* Write (
	MOVE.B	bDollar,(A0)+		* Write $
	
	MOVEA.L	lCurrentAddress,A2	*Load the current address

	*Find the displacement
	MOVE.W (A2),D3			*Get the word again
	ANDI.L	#$000000FF,D3		*Mask off the register
	CMPI.B	#0,D3
	BGE	_i8AnNext
	NEG.B	D3
	MOVE.B	bMinus,(A0)+		* Write -
_i8AnNext
					*Print the displacement
	LONGTOSTRING	D3,A0
	ADD.L	D0,A0
	CMPI.L	#15,D3
	BLE	_i8An1Space

_i8An1Space
	MOVE.B	bComma,(A0)+		* Write ,
	BSR	_printAddrReg
	MOVE.B	bComma,(A0)+		* Write ,

	*Print the 'Xn' Register	
	MOVE.W	(A2),D3			*Get the word after the instruction
	ROL	#4,D3			*Start with the Register
	ANDI.W	#$000F,D3		*Mask off the register
	CMPI.W	#7,D3			*Compare it to 7
	BGT	_i8AnAddrReg		*If it is an Address register, print that
	
	*'Xn' is a 'Dn' so print it
	MOVE.W	D3,D2			*Otherwise, print as a data register
	LEA	rgszDn,A1		*Load data register string pointer into A1
	MULU.W	#2,D2			*Multiply register bits by 2 to account for word size
	ADD.W	D2,A1			*Set the array offset to grab the right string
	MOVEA.W	(A1),A1			*Get the right Dx string out of the array
					*Add 'Dx' to lpszInstOutput
	STRCPY	A1, A0
	BRA	_i8AnEnd
	
_i8AnAddrReg	*'Xn' is an 'An' so print it
	SUBI.W	#8,D3			*Subtract 8 to get the correct offset
	MOVE.W	D3,D2			*Set up D2 for print call
	BSR	_printAddrReg		*Print 'An'
	
_i8AnEnd				*If 'Xn' is an 'An'
	MOVE.B	bClParen,(A0)+		* Write )
	MOVEQ.L	#2,D0			*Set up a return of 2
	BRA	_decodeEAend

_printAddrReg
	LEA	rgszAn,A1	*Load data register string pointer into A1
	MULU.W	#2,D2		*Multiply register bits by 2 to account for word size
	ADD.W	D2,A1		*Set the array offset to grab the right string
	MOVEA.W	(A1),A1		*Get the right Dx string out of the array
				*Add 'Ax' to lpszInstOutput
	STRCPY	A1, A0		
	RTS

*mode7 is for all modes that are %000 and use the register bits to differ
_mode7
	CMPI.B	#absWord,D2		*Is this absolute word addressing?
	BEQ	_absWord
	CMPI.B	#absLong,D2		*Is this absolute long addressing?
	BEQ	_absLong
	CMPI.B	#immData,D2		*Is this immediate data?
	BEQ	_immData
	CMPI.B	#d16PC,D2		*Is this displaced program counter 16-bit?
	BEQ	_d16PC
	CMPI.B	#d8PC,D2		*Is this displaced program counter 8-bit?
	BEQ	_d8PC
	BRA	_modeError

_absWord
	MOVE.B	bOpParen,(A0)+		* Write (
	MOVE.B	bDollar,(A0)+		* Write $
	MOVEA.L	lCurrentAddress,A2	*Get the current address
	MOVE.W	(A2),D3			*Load the contents of the next block
	LONGTOSTRING	D3,A0
	ADD.L	D0,A0			*Move the buffer forward
	MOVE.B	bClParen,(A0)+		* Write )
	MOVEQ.L	#2,D0			*Set up the return of 2
	BRA	_decodeEAend
_absLong
	MOVE.B	bOpParen,(A0)+		* Write (
	MOVE.B	bDollar,(A0)+		* Write $
	MOVEA.L	lCurrentAddress,A2	*Get the current address
	MOVE.L	(A2),D3			*Load the contents of the next block
	LONGTOSTRING	D3,A0
	ADD.L	D0,A0			*Move the buffer forward
	MOVE.B	bClParen,(A0)+		* Write )
	MOVEQ.L	#4,D0			*Set up the return of 4
	BRA	_decodeEAend
_immData
	MOVE.B	bHash,(A0)+		* Write #
	MOVE.B	bDollar,(A0)+		* Write $

_immDataLong
	CMPI.B	#2,D4
	BNE	_immDataWord
	MOVEA.L	lCurrentAddress,A2	*Get the current address
	MOVE.L	(A2),D3			*Load the contents of the next block
	LONGTOSTRING	D3,A0
	ADD.L	D0,A0
	BRA	_immDataEnd
	
_immDataWord
	CMPI.B	#1,D4
	BNE	_immDataByte
	MOVEA.L	lCurrentAddress,A2	*Get the current address
	MOVE.W	(A2),D3			*Load the contents of the next block
	LONGTOSTRING	D3,A0
	ADD.L	D0,A0
	BRA	_immDataEnd
	
_immDataByte
	CMPI.B	#0,D4
	BNE	_immDataError
	MOVEA.L	lCurrentAddress,A2	*Get the current address
	MOVE.W	(A2),D3			*Load the contents of the next block
	LONGTOSTRING	D3,A0
	ADD.L	D0,A0
	ADDQ.L	#1,D4			*Add 1 to byte to prep for return
	BRA	_immDataEnd
	
_immDataEnd
	ASL	#1,D4			*Multiply D4 *2 for return
	MOVE.W	D4,D0			*return how far to move
	BRA	_decodeEAend
_immDataError
	MOVE.L	#-1,D0
	BRA	_decodeEAend
_d16PC
	MOVE.B	bDollar,(A0)+		* Write $
	MOVEA.L	lCurrentAddress,A2	*Load the current address
	MOVE.W	(A2),D3
	MOVE.L	A2,A3
	ADD.W	D3,A3
	ANDI.L	#$FFFF0000,D3
	ADD.L	A3,D3
	LONGTOSTRING	D3,A0
	ADD.L	D0,A0

	MOVE.B	bOpParen,(A0)+		* Write (
	MOVE.B	bP,(A0)+		* Write P
	MOVE.B	bC,(A0)+		* Write C
	MOVE.B	bClParen,(A0)+		* Write )
	MOVE.L	#2,D0			*Set up the return of 2
	BRA	_decodeEAend
_d8PC
	MOVE.B	bOpParen,(A0)+		* Write (
	MOVE.B	bDollar,(A0)+		* Write $
	MOVEA.L	lCurrentAddress,A2	*Load the current address
	MOVE.W 	(A2),D3			*Get the word again
	MOVE.L	A2,A3
	ANDI.W	#$00FF,D3
	CMPI.B	#0,D3
	BGE	_d8PCNext
	NEG.B	D3
	SUB.L	D3,A3
	MOVE.L	A3,D3
	BRA	_d8PCPrint
_d8PCNext
	ADD.L	A3,D3
_d8PCPrint
					*Print the displacement
	LONGTOSTRING	D3,A0
	ADD.L	D0,A0
	CMPI.L	#15,D3
	BLE	_d8PC1Space

_d8PC1Space
	MOVE.B	bComma,(A0)+		* Write ,
	MOVE.B	bP,(A0)+		* Write P
	MOVE.B	bC,(A0)+		* Write C
	MOVE.B	bComma,(A0)+		* Write ,

	*Print the 'Xn' Register	
	MOVE.W	(A2),D3			*Get the word after the instruction
	ROL	#4,D3			*Start with the Register
	ANDI.W	#$000F,D3		*Mask off the register
	CMPI.W	#7,D3			*Compare it to 7
	BGT	_d8PCAddrReg		*If it is an Address register, print that
	
	*'Xn' is a 'Dn' so print it
	MOVE.W	D3,D2			*Otherwise, print as a data register
	LEA	rgszDn,A1		*Load data register string pointer into A1
	MULU.W	#2,D2			*Multiply register bits by 2 to account for word size
	ADD.W	D2,A1			*Set the array offset to grab the right string
	MOVEA.W	(A1),A1			*Get the right Dx string out of the array
					*Add 'Dx' to lpszInstOutput
	STRCPY	A1, A0
	BRA	_d8PCEnd
	
_d8PCAddrReg	*'Xn' is an 'An' so print it
	SUBI.W	#8,D3			*Subtract 8 to get the correct offset
	MOVE.W	D3,D2			*Set up D2 for print call
	BSR	_printAddrReg		*Print 'An'
	
_d8PCEnd				*If 'Xn' is an 'An'
	MOVE.B	bClParen,(A0)+		* Write )
	MOVE.L	#2,D0			*Set up a return of 2
	BRA	_decodeEAend
	
_modeError
	MOVE.L	#-1,D0			*Set a negative return for error
	BRA	_decodeEAend

_decodeEAend
	MOVEM.L (A7)+,A1-A3/D1-D4
	RTS			*return back to decodeInstruction

*****************************************************************************
* long maskWord(word wMask, word wValue)
** Description: Masks the provided value 
** Parameters: wMask: mask
**             wValue: value
** Return value: Masked int
*****************************************************************************
maskWord
	MOVEM.L	D1,-(SP)
	MOVE.W	8(SP),D0 * Get wValue
	MOVE.W	10(SP),D1 * Get wMask

	AND.W	D1,D0 * Apply mask
	
* Extract value
mwLoop
	BTST #0,D1 * Examine bit 0 of the mask
	BNE	mwExit * If bit 0 is a 1, exit - we've shifted this word as far as it needs to go.
	ASR	#1,D1 * Otherwise, shift mask by one,
	ASR	#1,D0 * shift word by one,
	BRA	mwLoop * and loop back.
mwExit
	MOVEM.L (SP)+,D1
	RTS
	
*****************************************************************************
* void formatData(long lAddress, word wValue)
** Description: Formats lpszDataOutput with the provided address and value
** Parameters: 
**             lAddress: address of data
**             wValue: value of data
** Return value: void
** Remarks: This function consumes globals lpszDataOutput. The format is
**          '    lAddress    DATA    $wValue',0
** Author: Paula Haddad
*****************************************************************************
_formatData
	MOVEM.L A0-A1/D1-D2,-(SP)
	
	* A0: lpszDataOutput
	* D1: wValue
	* D2: lAddress
	
	* Load lpsz and cch from the stack
	CLR.L	D1 * Zero out the high word
	MOVE.W	20(SP),D1 * Get wValue
	MOVE.L	22(SP),D2 * Get lAddress
	LEA	lpszDataOutput,A0 * Load output buffer

	ADDWS A0	* Add whitespace
	ADDA.L D0,A0	* Advance pointer

	LONGTOSTRING D2,A0 * Write address
	ADD.L	D0,A0	* Advance pointer
	
	ADDWS A0	* Add whitespace
	ADDA.L D0,A0	* Advance pointer
	
	* Add "DATA"
	LEA	lpszData,A1	* Load string to be copied
	
	STRCPY A1,A0
	
	ADDWS A0	* Add whitespace
	ADDA.L D0,A0	* Advance pointer
	
	MOVE.B	bDollar,(A0)+ * Write '$' sigil
	
	LONGTOSTRING D1,A0 * Write value
	ADD.L	D0,A0 * Advance pointer
	
	* Write terminating null
	MOVE.B	#0,(A0)
	
	MOVEM.L (SP)+,A0-A1/D1-D2
	RTS
	
*****************************************************************************
* long longToString(long lValue, long lpszBuffer)
** Description: Converts a long (lValue) to a string and writes the output to
**              lpszBuffer
** Parameters: lValue: long value to covert to string
**             lpszBuffer: pointer to an output buffer
** Return value: Number of characters written to buffer
** Author: Paula Haddad
*****************************************************************************
_lts
	MOVEM.L A0/D1-D4,-(A7)
	
	* D0: return value
	* D1: lValue
	* D2: nibble counter
	* D3: non-zero value found flag
	* D4: masked nibble
	
	* Load lpsz and cch from the stack
	MOVE.L 24(SP),A0 * Get lpszBuffer
	MOVE.L 28(SP),D1 * Get lValue
	CLR.L	D0 * return value
	CLR.L	D3 * non-zero value found flag
	CLR.L	D4 * Clear nibble register
	MOVEQ.L	#7,D2 * nibble counter
	
	* Special-case: lValue is zero
	CMPI.L	#0,D1
	BNE	ltsNextNibble
	MOVE.L	#0,D2 * Iterate only once
	BRA	ltsAddChar * Write the zero

* Skip leading zeros
ltsNextNibble
	ROL.L	#4,D1 * Rotate left 4 to move most sig. to least sig. position
	MOVE.L	D1,D4 * Make a copy for ANDing
	ANDI.L	#$F,D4 * Mask off the least sig. nibble
	CMPI.L	#0,D3 * Check non-zero flag - don't want to print leading zeros
	BNE	ltsAddChar * Flag is set; ok to print zeros now
	CMPI.L	#0,D4 * Check nibble value
	DBNE	D2,ltsNextNibble * If value is zero, check the next nibble
	MOVEQ.L	#1,D3 * Set the non-zero found flag to 1
ltsAddChar
	CMPI.L	#10,D4 * Check nibble value
	BGE	ltsABC * Go to A-F section if >= 10
	ADDI.L	#$30,D4 * Calculate ASCII value
	BRA	ltsAppend
ltsABC
	ADDI.L	#$37,D4 * Calculate ASCII value	
ltsAppend
	MOVE.B	D4,(A0)+ * Write char to string
	ADDQ.L #1,D0 * Increment the return value
	DBRA D2,ltsNextNibble * Decrement counter and loop while > -1
	
	MOVEM.L (A7)+,A0/D1-D4
	RTS
	
*****************************************************************************
* long stringToLong(long lpsz, long cch)
** Description: Converts a string (lpsz) of length cch into a long
** Parameters: lpsz: pointer to ASCII string containing adddress
**             cch: count of characters in lpsz
** Return value: the long value represented by the string or -1 on error
** Author: Paula Haddad
*****************************************************************************
_stl
	MOVEM.L A0/D1,-(A7)
	* Load lpsz and cch from the stack
	MOVE.L 12(SP),D1 * Get cch
	MOVE.L 16(SP),A0 * Get lpsz
	
	CMPI.L #8,D1 * If cch > 8, return error
	BGT gaERROR
	
	CLR.L D0 * Clear return value
	CLR.L D1 * Clear register for reuse

* Build up a long, one nibble at a time
gaLOOP
	MOVE.B (A0)+,D1 * Read a character into D1

	* Check for null terminator - exit if found
	CMPI.B	#0,D1
	BEQ	gaEXIT

	* Check character for validity
  	ASL.L	#4, D0		* move 4 bits to the left 
	CMPI.B	#$30, D1	* compare the input character to 0 in ASCII
  	BLT	gaERROR         * return if input is less than 0 in ASCII
        CMPI.B	#$39, D1	* compare input to the number 9 in ASCII
        BGT	CapLetterAF     * if greater than 9 go to CapLetterAF subroutine
        SUBI.B 	#$30, D1	* convert ASCII to hex
        BRA gaOR
	
CapLetterAF
	CMPI.B	#$41, D1	* compare the input character to Letter A in ASCII
	BLT	gaERROR		* return if input is >39 & <41 in ASCII
	CMPI.B	#$46, D1	* compare input to F in ASCII
	BGT	lowerCaseaf	* if greater than F go to lowerCaseaf subroutine
	SUBI.B	#$37, D1	* convert ASCII to hex
	BRA gaOR
	  	
lowerCaseaf
	CMPI.B 	#$61, D1	* compare the input character to the Letter a in ASCII
	BLT	gaERROR		* return if input is < 61
	CMPI.B	#$66, D1	* compare the input to f in ASCII
	BGT	gaERROR		* if greater than 66 - go to ERROR
	SUBI.B	#$57, D1	* convert ASCII to hex

gaOR	
	OR.L	D1, D0		* OR bits into D0
	BRA gaLOOP * Loop again until the null terminator is found		
gaERROR
	MOVE.L #-1,D0 * Return an error
gaEXIT
	MOVEM.L (A7)+,A0/D1
	RTS

	END	START
*~Font name~Courier New~
*~Font size~10~
*~Tab type~1~
*~Tab size~8~
