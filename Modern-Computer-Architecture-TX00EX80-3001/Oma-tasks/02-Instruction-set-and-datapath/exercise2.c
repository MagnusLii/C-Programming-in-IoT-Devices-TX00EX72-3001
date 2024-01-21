/*
===============================================================================
 Name        : asm_test.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
 */

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>
#include <stdio.h>

// TODO: insert other include files here

// TODO: insert other definitions and declarations here
__attribute__(( naked )) int asm_test(int v0, int v1, int v2, int v3)
{
	// write your code between push and pop instructions
	asm volatile
	(
			"push {r4, r5, r6, r7} \n" // do not remove
			// do not add any code before this comment
			"adds r3, r2, r1 \n" // example assembly code replace with your own code
			"mov r0, r3 \n" // example assembly code replace with your own code
			// add more code here

			// do not add any code after this comment
			"pop {r4, r5, r6, r7} \n" // do not remove
			"bx lr \n" // do not remove
	);
}

void print(const char *text)
{
	printf(text);
}

void fail() {
	print("Failed\n"); // set a break point here
	while(1);
}

void ok() {
	print("All ok\n"); // set a break point here
	while(1);
}


int main(void) {

#if defined (__USE_LPCOPEN)
	// Read clock settings and update SystemCoreClock variable
	SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
	// Set up and initialize all required blocks and
	// functions related to the board hardware
	Board_Init();
	// Set the LED to the state of "On"
	Board_LED_Set(0, true);
#endif
#endif

	// TODO: insert code here
	int m0;
	int m1;
	int m2;
	int m3;

	m0 = 1; m1 = 2; m2 = 3; m3 = 4;
	m0 = asm_test(m0, m1, m2, m3);
	if(m0 != 43) fail();

	m0 = 8; m1 = 5; m2 = 6; m3 = 21;
	m0 = asm_test(m0, m1, m2, m3);
	if(m0 != 1524) fail();

	m0 = 3; m1 = 4; m2 = 5; m3 = 1;
	m0 = asm_test(m0, m1, m2, m3);
	if(m0 != 328) fail();

	m0 = 3; m1 = 5; m2 = 7; m3 = 8;
	m0 = asm_test(m0, m1, m2, m3);
	if(m0 != 931) fail();

	m0 = 33; m1 = 22; m2 = 11; m3 = 0;
	m0 = asm_test(m0, m1, m2, m3);
	if(m0 != 250239) fail();

	m0 = 42; m1 = 55; m2 = 12; m3 = 1;
	m0 = asm_test(m0, m1, m2, m3);
	if(m0 != 9280754) fail();

	ok();

	// Force the counter to be placed into memory
	volatile static int i = 0 ;
	// Enter an infinite loop, just incrementing a counter
	while(1) {
		i++ ;
	}
	return 0 ;
}
