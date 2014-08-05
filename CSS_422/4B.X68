*----------------------------------------------------------------------------
* Program    : Homework 4, problem 2
* Written by : Ethan Crawford
* Course     : CSS 422
* Date       : February 10, 2009
* Description: Reads the memory between the addresses $000A0000 and $000C0000, 
* searching for the 16-bit word $ABC5. If found, it stores the 
* address in a memory location located just after the program code and 
* terminates. If it fails to find the pattern with the specified memory range, 
* it writes $FFFF to the memory location and terminates. 
*----------------------------------------------------------------------------

*****************************************************************************
*
* EQUates section
*
*****************************************************************************
startAddr EQU $000A0000
endAddr   EQU $000C0000
pattern   EQU $ABC5
wFail     EQU $FFFF
lRet	  DS.L	1             * Allocate one long for return address

*****************************************************************************
*
* Beginning of code segment.
*
*****************************************************************************
	OPT	CRE

START	ORG	$400

	* Set up registers and reserve memory for found word
	MOVE.L #pattern,D0    * Move search value to D0
	MOVEA.L #startAddr,A0 * Move starting addr to A0
	MOVEA.L #endAddr,A1   * Move ending addr to A1
	CLR.L	lRet          * Clear return value

LOOP	MOVE.W (A0)+,D1       * load the word and increment startAddr
	CMP.W D0,D1           * compare it to pattern 
	BNE ADDR              * If pattern does not match, check addresses
	MOVE.L A0,lRet        * else, store the word's address in a local
	BRA EXIT              * and exit

ADDR	CMPA.L A1,A0          * If startAddr <= endAddr, 
	BLE LOOP              * loop to the beginning.
	MOVE.L #wFail,lRet    * else, store 0xFFFF in a local and exit

EXIT	MOVE.B	#9,D0
	TRAP	#15		Halt Simulator
	END	START



*~Font name~Courier New~
*~Font size~10~
*~Tab type~1~
*~Tab size~8~
