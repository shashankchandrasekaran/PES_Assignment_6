/*
 *  Filename: test_cbfifo.h
 *  Created on: Nov 11, 2022
 *  Author: Shashank Chandrasekaran
 *  Description: This H file contains the declaration of CBFIFO test function
 *  Reference: None
 */

#ifndef TEST_CBFIFO_H_
#define TEST_CBFIFO_H_

/*
 * Function for test case 1: When nbytes enqueued for first time and is less than
 * capacity of array
 * Parameters Passed: None
 * Parameters returned: 1 on success and 0 on failure
 */
int cbfifo_test1(void);

/*
 * Function for test case 2: When a null element is tried to enqueued in the array
 * capacity of array
 * Parameters Passed: None
 * Parameters returned: 1 on success and 0 on failure
 */
int cbfifo_test2(void);

/*
 * Function for test case 3: When dequeued element is tried to be stored in NULL
 * pointer
 * Parameters Passed: None
 * Parameters returned: 1 on success and 0 on failure
 */
int cbfifo_test3(void);

/*
 * Function for test case 4: When dequeued more than available length in array
 * Parameters Passed: None
 * Parameters returned: 1 on success and 0 on failure
 */
int cbfifo_test4(void);

/*
 * Function for test case 5: When trying to enqueue more than capacity of array
 * i.e. nbytes>n
 * Parameters Passed: None
 * Parameters returned: 1 on success and 0 on failure
 */
int cbfifo_test5(void);

/*
 * Function for test case 6: Combined operation of enqueue and dequeue to check
 * the circular buffer condition
 * Parameters Passed: None
 * Parameters returned: 1 on success and 0 on failure
 */
int cbfifo_test6(void);

/*
 * Function for test case 7: When nbyte is zero
 * Parameters Passed: None
 * Parameters returned: 1 on success and 0 on failure
 */
int cbfifo_test7(void);

/*
 * Function for test case 8: Checking length function after performing dequeue operation
 * Parameters Passed: None
 * Parameters returned: 1 on success and 0 on failure
 */
int cbfifo_test8(void);

/*
 * Function for test case 9:When integer is passed as input
 * Parameters Passed: None
 * Parameters returned: 1 on success and 0 on failure
 */
int cbfifo_test9(void);

/*
 * Overall test function: This test function calls all the above test case functions
 * and returns 1 on success and 0 on faiure
 * Parameters Passed: None
 * Parameters returned: 1 on success and 0 on failure
 */
void test_cbfifo(void);

#endif /* TEST_CBFIFO_H_ */
