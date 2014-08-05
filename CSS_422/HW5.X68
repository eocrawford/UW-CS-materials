*----------------------------------------------------------------------------
* Program    : Homework 5
* Written by : Ethan Crawford
* Course     : CSS 422
* Date       : February 24, 2009
* Description: Implements a memory test program.
*----------------------------------------------------------------------------

*****************************************************************************
*
* Beginning of code segment.
*
*****************************************************************************
	OPT	CRE

START	ORG	$400
MAIN	MOVEA.L #lStackPtr,A7 * Init stack pointer
	CLR.L lBadAddr
	CLR.W wBadCount
	CLR.W wDataWritten
	CLR.W wDataRead

* fill and test memory with rgPatterns
	MOVE.L #rgPatterns,-(A7) * Push rgPatterns onto the stack
	JSR testPatternArray
	ADDQ.L #4,A7 * Deallocate parameter
	
* Complement rgPatterns and fill and test memory with rgPatterns
	MOVEA.W #rgPatterns,A0 * Initialize patterns array pointer
NOTP	MOVE.W (A0),D1
	NOT.W D1 * Complement the word
	MOVE.W D1,(A0) * Store the complemented word
	ADDQ.L #2,A0 * Move to the next word
	ADDQ.B #1,D2 * Increment counter
	CMP.B #cPatterns,D2 * Check counter
	BLT NOTP * Loop again if patterns remain
	
	MOVE.L #rgPatterns,-(A7) * Push rgPatterns onto the stack
	JSR testPatternArray
	ADDQ.L #4,A7 * Deallocate parameter
	
* Bit-shift wBitShift 16 times and fill and test memory
	MOVE.L #1,D1
	MOVE.L D1,-(A7) * Push wShiftValue onto the stack
	JSR bitShiftTest
	ADDQ.L #4,A7 * Deallocate parameter
	
	* Complement bitshift pattern and fill and test memory
	NOT.W D1
	MOVE.L D1,-(A7) * Push wShiftValue onto the stack
	JSR bitShiftTest
	ADDQ.L #4,A7 * Deallocate parameter
	
	JMP EXIT * Exit the program

*****************************************************************************
*
* EQUates section
*
*****************************************************************************
lStartAddr   EQU $00001000 * starting memory address
lEndAddr     EQU $00001008 * ending memory address
lStackPtr    EQU $000A0000 * stack pointer address
bWordSize    EQU 16 * Number of bits in a word
cPatterns    EQU 2 * Patterns to test and complement
rgPatterns   DC.W $FFFF,$AAAA * Patterns to test and complement
lBadAddr     DS.L 1 * Address of latest bad memory location
wBadCount    DS.W 1 * Number of memory test errors found
wDataWritten DS.W 1 * Data written to latest bad memory location
wDataRead    DS.W 1 * Data read from latest bad memory location

* void bitShift(word wShiftValue)	
bitShiftTest
	MOVEM.L D1-D2,-(A7)
	CLR.L D1 * Clear loop counter
	MOVE.L 12(A7),D2 * Load bitshift value
BSHIFT	MOVE.L D2,-(A7) * Push bitshifted value onto the stack

	JSR fillAndTestMemory
	ADDQ.L #4,A7 * Deallocate parameter
	ADD.W D0,wBadCount * Add the return value to wBadCount
	
	ROL.W #1,D2 * Bitshift D0
	ADDQ.B #1,D1 * Increment counter
	CMP.B #bWordSize,D1 * Check counter
	BLT BSHIFT * Loop again if patterns remain
	MOVEM.L (A7)+,D1-D2
	RTS
	
* void testPatternArray(word* rgPatterns)
testPatternArray
	MOVEM.L A0/D0-D1,-(A7)
	CLR.L D1 * Clear loop counter
	MOVEA.L 16(A7),A0 * Get rgPatterns
PL	MOVE.W (A0)+,D1 * Put the nth pattern into D0

	MOVE.L D1,-(A7) * Push wPattern onto the stack
	JSR fillAndTestMemory * Call the subroutine
	ADDQ.L #4,A7 * Deallocate parameter
	ADD.W D0,wBadCount * Add the return value to wBadCount
	
	ADDQ.B #1,D1 * Increment counter
	CMP.B #cPatterns,D1 * Check counter
	BLT PL * Loop again if patterns remain
	MOVEM.L (A7)+,A0/D0-D1
	RTS

* int fillAndTestMemory(word wPattern)
fillAndTestMemory
	MOVEM.L A1/A2/D1/D2,-(A7)
	MOVE.L 20(A7),D1 * Get wPattern
	CLR.L D0 * Clear return value
	
	* Fill memory block
	MOVE.L #lStartAddr,A1 * Init start
	MOVE.L #lEndAddr,A2 * Init end
FMB	MOVE.W D1,(A1) * Fill word
	ADDQ.L #2,A1 * Advance address another word
	CMPA.L A2,A1 * Loop while lStartAddr <= lEndAddr
	BLE FMB
	
	* Test memory block
	MOVE.L #lStartAddr,A1 * Init start again
	
TMB	MOVE.W (A1),D2 * Get memory value
	CMP.W D1,D2 * Compare retrieved value to wPattern
	BEQ TMBEQ * If the values are equal, go to the next word
	
	ADDQ.L #1,D0 * Else, increment the return value and store interesting data
	MOVE.L A1,lBadAddr * Store bad address
	MOVE.W D1,wDataWritten * Store data written
	MOVE.W D2,wDataRead * Store data read
	
TMBEQ	ADDQ.L #2,A1 * Advance address another word
	CMPA.L A2,A1 * Loop while lStartAddr <= lEndAddr
	BLE TMB
	
	* Epilogue
	MOVEM.L (A7)+,A1/A2/D1/D2
	RTS
	
EXIT	MOVE.B	#9,D0
	TRAP	#15		Halt Simulator
	END	START


*~Font name~Courier New~
*~Font size~10~
*~Tab type~1~
*~Tab size~8~
