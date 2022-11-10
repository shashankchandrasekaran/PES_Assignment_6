/*
 * Filename: test_cbfifo.c
 * Edited by: Shashank Chandrasekaran
 * Created on: Nov 11, 2022
 * Changes made: Changes made to pass the structure in the CBFIFO functions
 * Reference:  * assignment2_tester.c - automated test cases for grading the
 * student's llfifo and cbfifo implementations
 *
 * Author: Howdy Pierce, howdy.pierce@colorado.edu
 *
 * To use: You'll need to modify the student's assignment to isolate
 * out the student's llfifo and cbfifo test cases; hopefully this is
 * as simple as just isolating their llfifo.c and cbfifo.c files. Then
 * do:
 *
 * gcc -Wall -Werror assignment2_tester.c cbfifo.c llfifo.c -o runme
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "cbfifo.h"

Q_T RX; //CBFIFO instance

#define test_assert(value) {                                            \
  tests_total++;                                                      \
  if (value) {                                                          \
    tests_passed++;                                                   \
  }                                                                     \
}

#define test_equal(value1, value2) {                                    \
  tests_total++;                                                      \
  long res1 = (long)(value1);                                           \
  long res2 = (long)(value2);                                           \
  if (res1 == res2) {                                                   \
    tests_passed++;                                                   \
  }                                                                     \
}

/**********************************CBFIFO Test Function***************************************/
void test_cbfifo(void)
{
	int tests_passed = 0;
	int tests_total = 0;

	cbfifo_init(&RX);

	char *str ="To be, or not to be: that is the question:\n"
			"Whether 'tis nobler in the mind to suffer\n"
			"The slings and arrows of outrageous fortune,\n"
			"Or to take arms against a sea of troubles,\n"
			"And by opposing end them? To die, to sleep--\n"
			"No more--and by a sleep to say we end\n"
			"The heart-ache and the thousand natural shocks\n"
			"That flesh is heir to, 'tis a consummation\n"
			"Devoutly to be wish'd. To die, to sleep;\n"
			"To sleep: perchance to dream: ay, there's the rub;\n"
			"For in that sleep of death what dreams may come\n"
			"When we have shuffled off this mortal coil,\n"
			"Must give us pause.";

	char buf[1024];
	const int cap = cbfifo_capacity();
	size_t ret = 0;

	// the asserts in the following 2 lines are there because they're
	// not testing the student, they're validating that the test is
	// correct
	assert(strlen(str) >= cap*2);
	assert(sizeof(buf) > cap);
	test_assert(cap == 256);

	test_equal(cbfifo_length(&RX), 0);
	test_equal(cbfifo_dequeue(buf, cap, &RX), 0);
	test_equal(cbfifo_dequeue(buf, 2, &RX), 0);

	// enqueue 10 bytes, then dequeue same amt
	test_equal(cbfifo_enqueue(str, 10, &RX), 10);
	test_equal(cbfifo_length(&RX), 10);
	test_equal(cbfifo_dequeue(buf, 10, &RX), 10);
	test_equal(strncmp(buf, str, 10), 0);
	test_equal(cbfifo_length(&RX), 0);

	// enqueue 20 bytes;  dequeue 5, then another 20
	test_equal(cbfifo_enqueue(str, 20, &RX), 20);
	test_equal(cbfifo_length(&RX), 20);
	test_equal(cbfifo_dequeue(buf, 5, &RX), 5);
	test_equal(cbfifo_length(&RX), 15);
	test_equal(cbfifo_dequeue(buf+5, 20, &RX), 15);
	test_equal(cbfifo_length(&RX), 0);
	test_equal(strncmp(buf, str, 20), 0);

	// fill buffer and then read it back out
	test_equal(cbfifo_enqueue(str, cap, &RX), cap);
	test_equal(cbfifo_length(&RX), cap);
	ret = cbfifo_enqueue(str, 1, &RX);
	test_assert(ret == 0 || (int)ret == -1);
	test_equal(cbfifo_dequeue(buf, cap, &RX), cap);
	test_equal(cbfifo_length(&RX), 0);
	test_equal(strncmp(buf, str, cap), 0);

	// Add 20 bytes and pull out 18
	test_equal(cbfifo_enqueue(str, 20, &RX), 20);
	test_equal(cbfifo_length(&RX), 20);
	test_equal(cbfifo_dequeue(buf, 18, &RX), 18);
	test_equal(cbfifo_length(&RX), 2);
	test_equal(strncmp(buf, str, 18), 0);

	// Now add a bunch of data in 4 chunks
	int chunk_size = (cap-2) / 4;
	for (int i=0; i<4; i++) {
		test_equal(cbfifo_enqueue(str+i*chunk_size, chunk_size, &RX), chunk_size);
		test_equal(cbfifo_length(&RX), (i+1)*chunk_size + 2);
	}
	test_equal(cbfifo_length(&RX), 4*chunk_size + 2);

	// Take out the 2 remaining bytes from above
	test_equal(cbfifo_dequeue(buf, 2, &RX), 2);
	test_equal(strncmp(buf, str+18, 2), 0);

	// now read those chunks out a byte at a time
	for (int i=0; i<chunk_size*4; i++) {
		test_equal(cbfifo_dequeue(buf+i, 1, &RX), 1);
		test_equal(cbfifo_length(&RX), chunk_size*4 - i - 1);
	}
	test_equal(strncmp(buf, str, chunk_size*4), 0);

	// write more than capacity
	test_equal(cbfifo_enqueue(str, 65, &RX), 65);
	test_equal(cbfifo_enqueue(str+65, cap, &RX), cap-65);
	test_equal(cbfifo_length(&RX), cap);
	test_equal(cbfifo_dequeue(buf, cap, &RX), cap);
	test_equal(cbfifo_length(&RX), 0);
	test_equal(strncmp(buf, str, cap), 0);

	// write zero bytes
	test_equal(cbfifo_enqueue(str, 0, &RX), 0);
	test_equal(cbfifo_length(&RX), 0);

	// Exercise the following conditions:
	//    enqueue when read < write:
	//        bytes < CAP-write  (1)
	//        bytes exactly CAP-write  (2)
	//        bytes > CAP-write but < space available (3)
	//        bytes exactly the space available (4)
	//        bytes > space available (5)
	test_equal(cbfifo_enqueue(str, 32, &RX), 32);  // advance so that read < write
	test_equal(cbfifo_length(&RX), 32);
	test_equal(cbfifo_dequeue(buf, 16, &RX), 16);
	test_equal(cbfifo_length(&RX), 16);
	test_equal(strncmp(buf, str, 16), 0);

	test_equal(cbfifo_enqueue(str+32, 32, &RX), 32);  // (1)
	test_equal(cbfifo_length(&RX), 48);
	test_equal(cbfifo_enqueue(str+64, cap-64, &RX), cap-64);  // (2)
	test_equal(cbfifo_length(&RX), cap-16);
	test_equal(cbfifo_dequeue(buf+16, cap-16, &RX), cap-16);
	test_equal(strncmp(buf, str, cap), 0);

	test_equal(cbfifo_enqueue(str, 32, &RX), 32);  // advance so that read < write
	test_equal(cbfifo_length(&RX), 32);
	test_equal(cbfifo_dequeue(buf, 16, &RX), 16);
	test_equal(cbfifo_length(&RX), 16);
	test_equal(strncmp(buf, str, 16), 0);

	test_equal(cbfifo_enqueue(str+32, cap-20, &RX), cap-20);  // (3)
	test_equal(cbfifo_length(&RX), cap-4);
	test_equal(cbfifo_dequeue(buf, cap-8, &RX), cap-8);
	test_equal(strncmp(buf, str+16, cap-8), 0);
	test_equal(cbfifo_length(&RX), 4);
	test_equal(cbfifo_dequeue(buf, 8, &RX), 4);
	test_equal(strncmp(buf, str+16+cap-8, 4), 0);
	test_equal(cbfifo_length(&RX), 0);

	test_equal(cbfifo_enqueue(str, 49, &RX), 49);  // advance so that read < write
	test_equal(cbfifo_length( &RX), 49);
	test_equal(cbfifo_dequeue(buf, 16,  &RX), 16);
	test_equal(cbfifo_length( &RX), 33);
	test_equal(strncmp(buf, str, 16), 0);

	test_equal(cbfifo_enqueue(str+49, cap-33,  &RX), cap-33);  // (4)
	test_equal(cbfifo_length(&RX), cap);
	test_equal(cbfifo_dequeue(buf, cap, &RX), cap);
	test_equal(cbfifo_length(&RX), 0);
	test_equal(strncmp(buf, str+16, cap), 0);

	test_equal(cbfifo_enqueue(str, 32, &RX), 32);  // advance so that read < write
	test_equal(cbfifo_length(&RX), 32);
	test_equal(cbfifo_dequeue(buf, 16, &RX), 16);
	test_equal(cbfifo_length(&RX), 16);
	test_equal(strncmp(buf, str, 16), 0);

	test_equal(cbfifo_enqueue(str+32, cap, &RX), cap-16);  // (5)
	test_equal(cbfifo_dequeue(buf, 1, &RX), 1);
	test_equal(cbfifo_length(&RX), cap-1);
	test_equal(cbfifo_dequeue(buf+1, cap-1, &RX), cap-1);
	test_equal(cbfifo_length(&RX), 0);
	test_equal(strncmp(buf, str+16, cap), 0);

	//    enqueue when write < read:
	//        bytes < read-write (6)
	//        bytes exactly read-write (= the space available) (7)
	//        bytes > space available (8)
	int wpos=0, rpos=0;
	test_equal(cbfifo_enqueue(str, cap-4, &RX), cap-4);
	wpos += cap-4;
	test_equal(cbfifo_length(&RX), cap-4);
	test_equal(cbfifo_dequeue(buf, 32, &RX), 32);
	rpos += 32;
	test_equal(cbfifo_length(&RX), cap-36);
	test_equal(strncmp(buf, str, 32), 0);
	test_equal(cbfifo_enqueue(str+wpos, 12, &RX), 12);
	wpos += 12;
	test_equal(cbfifo_length(&RX), cap-24);

	test_equal(cbfifo_enqueue(str+wpos, 16, &RX), 16);  // (6)
	test_equal(cbfifo_length(&RX), cap-8);
	test_equal(cbfifo_dequeue(buf, cap, &RX), cap-8);
	test_equal(cbfifo_length(&RX), 0);
	test_equal(strncmp(buf, str+rpos, cap-8), 0);

	// reset
	wpos=0;
	rpos=0;
	test_equal(cbfifo_enqueue(str, cap-4, &RX), cap-4);
	wpos += cap-4;
	test_equal(cbfifo_length(&RX), cap-4);
	test_equal(cbfifo_dequeue(buf, 32, &RX), 32);
	rpos += 32;
	test_equal(cbfifo_length(&RX), cap-36);
	test_equal(strncmp(buf, str, 32), 0);
	test_equal(cbfifo_enqueue(str+wpos, 12, &RX), 12);
	wpos += 12;
	test_equal(cbfifo_length(&RX), cap-24);

	test_equal(cbfifo_enqueue(str+wpos, 24, &RX), 24);  // (7)
	test_equal(cbfifo_length(&RX), cap);
	test_equal(cbfifo_dequeue(buf, cap, &RX), cap);
	test_equal(cbfifo_length(&RX), 0);
	test_equal(strncmp(buf, str+rpos, cap), 0);

	// reset
	wpos=0;
	rpos=0;
	test_equal(cbfifo_enqueue(str, cap-4, &RX), cap-4);
	wpos += cap-4;
	test_equal(cbfifo_length(&RX), cap-4);
	test_equal(cbfifo_dequeue(buf, 32, &RX), 32);
	rpos += 32;
	test_equal(cbfifo_length(&RX), cap-36);
	test_equal(strncmp(buf, str, 32), 0);
	test_equal(cbfifo_enqueue(str+wpos, 12, &RX), 12);
	wpos += 12;
	test_equal(cbfifo_length(&RX), cap-24);

	test_equal(cbfifo_enqueue(str+wpos, 64, &RX), 24);  // (8)
	test_equal(cbfifo_length(&RX), cap);
	test_equal(cbfifo_dequeue(buf, cap, &RX), cap);
	test_equal(cbfifo_length(&RX), 0);
	test_equal(strncmp(buf, str+rpos, cap), 0);

	printf("Testing CBFIFO: passed %d/%d test cases\r\n",
			tests_passed, tests_total);
	if(tests_passed==tests_total)
		printf("Success: All CBFIFO test cases have passed\r\n");
	else
		printf("Failure: CBFIFO test failed\r\n");
}


