*----------------------------------------------------------------------------
* Program    : Homework 4, problem 1
* Written by : Ethan Crawford
* Course     : CSS 422
* Date       : February 10, 2009
* Description: write a program that fills all of memory between $0000A000 to 
*              $0000A2FF, inclusive, with the longword pattern $5555AAAA.
*----------------------------------------------------------------------------

*****************************************************************************
*
* EQUates section
*
*****************************************************************************
startAddr EQU $0000A000
endAddr   EQU $0000A2FF
pattern   EQU $5555AAAA

*****************************************************************************
*
* Beginning of code segment.
*
*****************************************************************************
	OPT	CRE
START	ORG	$400

	* Set up registers
	MOVE.L #pattern,D0    * Move fill value to D0
	MOVEA.L #startAddr,A0 * Move starting addr to A0
	MOVEA.L #endAddr,A1   * Move ending addr to A1
	
LOOP	MOVE.L D0,(A0)+       * Write fill value and increment startAddr
	CMPA.L A1,A0          * Subtract endAddr from startAddr
	BLE    LOOP           * Do it again while startAddr <= endAddr

	MOVE.B	#9,D0
	TRAP	#15		Halt Simulator

	END	START


*~Font name~Courier New~
*~Font size~10~
*~Tab type~1~
*~Tab size~8~
