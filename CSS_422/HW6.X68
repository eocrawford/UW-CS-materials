*-----------------------------------------------------------
* Program    : Memory Tester with I/O (HW6.X68)
* Written by : Michael Grimm, Paula Haddad, Ethan Crawford
* Date       : 3-10-2009
* Description:
*   Program fills a given block of data with a memory pattern.
*   The memory is then tested for that pattern. If an error is
*   found, the location and bad data is stored. Once a memory
*   pattern is completed, it is changed, the memory is re-filled
*   and the test is repeated.
* Register Use:
*   D0: current data pattern
*   D6: count of patterns loaded
*   D7: count of memory errors
*   A0: start of memory range
*   A1: end of memory range
*   A2: current position in the memory range
*-----------------------------------------------------------
TEXT EQU 0
CODE EQU 1
	SECTION	TEXT
	ORG	$2000
	SECTION	CODE
	ORG	$400
	
stAddr	EQU	$00001000
endAddr	EQU	$0003FFFF
stack	EQU	$000A0000
maxErr	EQU	65534

*****************************************************************************
* Macros
*************************** print a line of text ****************************
PRINTLN	MACRO
	MOVEM.L	D0/A1,-(SP) * push
	LEA	\1,A1 * Load string pointer
	MOVE.B	#13,D0 * Print with newline
	TRAP	#15
	MOVEM.L	(SP)+,D0/A1 * pop
	ENDM
        
**************************** print a hex integer ****************************
PRINTHEX MACRO
	* Print integer
	MOVE.L \1,D1 * Number to print
	MOVE.L #16,D2 * Number base
	MOVE.L #15,D0 * Task
	TRAP	#15 * Print number
	ENDM

********************************* read CIN **********************************
CIN MACRO
	MOVE.B	#2,D0	*Load cin task into D0	
	LEA	usr_str,A1	*Load address of storage space
	TRAP	#15		*cin
	ENDM

**************************** call StringToLong ******************************
STRINGTOLONG MACRO
	* Call stringToLong
	MOVE.L	#usr_str,-(SP)  * Push input buffer
	MOVE.L	\1,-(SP)	* Push input buffer length
	BSR	_stl		* Call stringToLong
	LEA	8(SP),SP	* Pop stack parameters
	ENDM

*********************** main() ***********************
	LEA	stack,SP	*Load the SP
* Get the starting address
getStartAddr
	PRINTLN	st_str
	CIN
	
	* Call stringToLong
	STRINGTOLONG D1

	* Validate return value
	* A valid starting address is greater than $00001000 
	MOVE.L	D0,D5           * Save the starting address in D5
	CMP.L	#-1,D5		* If address is invalid, print an error and loop back
	BEQ	saErr
	
	CMP.L	#stAddr,D5
	BLE	saErr		* If address <= stAddr, print an error and loop back
	
	BRA	getEndAddr	* Else, get end address
saErr	
	PRINTLN err_str
	BRA	getStartAddr

* Get the ending address
getEndAddr	
	PRINTLN end_str
	CIN
	
	* Call stringToLong
	STRINGTOLONG D1

	* Check the ending address range for validity
	* A valid ending address is less than $0003FFFF
	* The ending address must be at least 2 bytes larger than the starting address.
	MOVE.L	D0,D6           * Save the ending address in D6
	CMP.L	#-1,D6		* If address is invalid, print an error and loop back
	BEQ	eaErr
	CMP.L	#endAddr,D6	* If address >=endAddr, print an error and loop back
	BGE	eaErr
	SUB.L	D5,D0		* endAddr must be at least 2 bytes larger than startAddr
	CMP.L	#2,D0
	BLT	eaErr
	BRA	getPattern	* Else, get pattern
eaErr	
	PRINTLN err_str
	BRA	getEndAddr

* Get the test pattern
getPattern
	PRINTLN test_str
	CIN
	
	* Call stringToLong
	STRINGTOLONG D1

	* Check the test pattern for validity (cannot be larger than 0xFFFF)
	MOVE.L	D0,D4           * Save the pattern in D4
	CMP.L	#-1,D4		* If address is invalid, print an error and loop back
	BEQ	gpErr
	CMP.L	#$FFFF,D4	* If pattern > 0xFFFF, print an error and loop back
	BGT	gpErr
	BRA	startTest	* Else, start the test
gpErr	
	PRINTLN err_str
	BRA	getPattern

* Print the summary heading
* Start the test
* Every time an error occurs,  print the address of the failed location, the data pattern that you wrote, and the data pattern that you read back.

startTest
	PRINTLN header_str
	
	MOVE.L	D5,A1		*Load starting address
	MOVE.L	D6,A2		*Load ending address
	BSR	fillMem		*Fill the memory range with the pattern
	BSR	testMem		*Test the memory with the pattern
stop	STOP	#$2700

errAddr	DS.L	1		*Variable for address of last error
errData	DS.W	1		*Variable for storing the data of the last error

whitespace DC.B '            ',0
header_str DC.B 'ADDRESS     DATA WRITTEN      DATA READ',0
st_str		DC.B	'Enter a starting address > 00001000',0
st_len		DC.W	st_len-st_str		* length of Start String
end_str		DC.B	'Enter an ending address < 0003FFFF',0
end_len		DC.W	end_len-end_str		* length of End String
test_str 	DC.B	'Enter a test pattern <= FFFF',0
test_len 	DC.W	test_len-test_str	* length of string
err_str		DC.B	'The number you have entered is invalid. Please try again.',0
usr_str		DC.B	80			*Space for user typed starting address input
space		DC.B	'        '
space_len	DC.W	space_len-space
	
*------- fillMem ---------------
* Description
*   Fills a block of memory with a given data pattern.
* Parameters
*  -- D4 Data pattern to put into memory
*  -- A1 Starting address of memory to fill
*  -- A2 Ending address of memory to fill
*-------------------------------
	ORG	$800
fillMem	MOVE.L	A1,A3
fillNxt	MOVE.W	D4,(A3)+	*Move data pattern to the current address
	CMP.L	A2,A3		*Compare currAddr to endAddr
	BLE	fillNxt		*IF currAddr <= endAddr THEN fillMem 
	RTS			*   ELSE finished with fillMem

*------- testMem ---------------
* Description
*   Tests if a block of memory contains a given data pattern.
* Parameters
*  -- D4 Data pattern to test for
*  -- A1 Starting address of memory to test
*  -- A2 Ending address of memory to test
*  -- A3 Current address of memory to be tested next
*-------------------------------
testMem	MOVEA.L	A1,A3		*Setup current address in A3
nextTest
	MOVE.W 	(A3),D5
	CMP.W	D5,D4	*Compare the data at currAddr to the pattern, increment currAddr
	BNE	branchErr	*IF data is NOT EQUAL THEN setErr
	BRA	testContinue	*continue the test
branchErr
	* Call _prtErr
	MOVE.L	A3,-(SP)	* Push lAddress
	MOVE.W	D4,-(SP)	* Push wWritten
	MOVE.W	D5,-(SP)	* Push wRead
	BSR	_prtErr		* Call _prtErr
	LEA	8(SP),SP	* Pop stack parameters
testContinue
	ADDQ.L	#2,A3
	CMP.L	A2,A3		*Compare currAddr to endAddr
	BLE	nextTest	*IF currAddr < endAddr THEN test currAddr
	RTS			*  ELSE finished with testMem

*****************************************************************************
* void printErr(long lAddress, word wWritten, word wRead)
** Description: Prints an error string
** Parameters: lAddress: Bad memory address
**             wWritten: Memory written
**             wRead: Memory read
** Return value: void
*****************************************************************************
_prtErr
	MOVEM.L D1-D5,-(A7)
	* Load parameters from the stack
	MOVE.W 24(SP),D5 * Get wRead
	MOVE.W 26(SP),D4 * Get wWritten
	MOVE.L 28(SP),D3 * Get lAddress
	
	PRINTHEX D3 * Print address
	LEA	whitespace,A1 * Print whitespace
	MOVE.B	#14,D0 * Print without newline
	TRAP	#15
	
	PRINTHEX D4 * Print data written
	LEA	whitespace,A1 * Print whitespace
	MOVE.B	#14,D0 * Print without newline
	TRAP	#15
	
	PRINTHEX D5 * Print data read
	LEA	whitespace,A1 * Print whitespace
	MOVE.B	#13,D0 * Print with newline
	TRAP	#15
	
	MOVEM.L (A7)+,D1-D5
	RTS

*****************************************************************************
* long stringToLong(long lpsz, long cch)
** Description: Converts a string (lpsz) of length cch into a long
** Parameters: lpsz: pointer to ASCII string containing adddress
**             cch: count of characters in lpsz
** Return value: the long value represented by the string or -1 on error
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
	MOVE.B (A0)+,D1 * Read a character into D6

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

	END	$400






*~Font name~Courier New~
*~Font size~10~
*~Tab type~1~
*~Tab size~8~
