/* CS240s14 Project B sampleColumnTester.c
 *
 * The following is a sample test driver for the column library.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "column.h"

#define SINGLEVALUETESTS "singleValueTests: validating has_single_value"
#define IMPOSSIBLESPLITTESTS "impossibleSplitTests: validating is_impossible_split"
#define ENTROPYTESTS "entropyTests: validating calc_entropy"
#define SPLITTESTS "splitTests: validating split functions"

#define FAIL "failed"

#define ERROR 0.0001

int singleValueTestDouble(double *fields, unsigned int *classes, unsigned int n, int expResult, char *testName)
{
	int fail = 0;
    
	Column *c = make_double_column(fields, classes, n);
	int result = has_single_value(c);
	if(result) result = 1;
	if(result != expResult)
	{
		printf("\t%s doubles: %s\n", testName, FAIL);
		fail = 1;
	}
	free_column(c);
    
	return fail;
}

int singleValueTestString(char **fields, unsigned int *classes, unsigned int n, int expResult, char *testName)
{
	int fail = 0;
    
	Column *c = make_string_column(fields, classes, n);
	int result = has_single_value(c);
	if(result) result = 1;
	if(result != expResult)
	{
		printf("\t%s strings: %s\n", testName, FAIL);
		fail = 1;
	}
	free_column(c);
    
	return fail;
}

int singleValueTests()
{
	int failCount = 0;
	printf("%s\n", SINGLEVALUETESTS);
    
	int expSingle = 1;
	int expNotSingle = 0;
    
	//single value, single row
	int sizeSS = 1;
	unsigned int classesSS[] = {42};
	double doublesSS[] = {42};
	char *stringsSS[] = {"hint:theAnswerIsInDoublesMM1"};
	failCount += singleValueTestDouble(doublesSS, classesSS, sizeSS, expSingle, "ss");
	failCount += singleValueTestString(stringsSS, classesSS, sizeSS, expSingle, "ss");
    
	//multiple values, multiple rows
	int sizeMM1 = 10;
	unsigned int classesMM1[] = {42, 42, 42, 42, 42, 42, 42, 42, 42, 42};
	double doublesMM1[] = {4, 8, 1, 5, 1, 6, 2, 3, 4, 2};
	char *stringsMM1[] = {"Where", "should", "Justin", "Bieber", "Retire?",
        "Canada,", "or", "The", "United", "States?"};
	failCount += singleValueTestDouble(doublesMM1, classesMM1, sizeMM1, expNotSingle, "mm1");
	failCount += singleValueTestString(stringsMM1, classesMM1, sizeMM1, expNotSingle, "mm1");
    
	return failCount;
}

int impossibleSplitTestDouble(double *fields, unsigned int *classes, unsigned int n, int expResult, char *testName)
{
	int fail = 0;
    
	Column *c = make_double_column(fields, classes, n);
	int result = is_impossible_split(c);
	if(result) result = 1;
	if(result != expResult)
	{
		printf("\t%s doubles: %s\n", testName, FAIL);
		fail = 1;
	}
	free_column(c);
    
	return fail;
}

int impossibleSplitTestString(char **fields, unsigned int *classes, unsigned int n, int expResult, char *testName)
{
	int fail = 0;
    
	Column *c = make_string_column(fields, classes, n);
	int result = is_impossible_split(c);
	if(result) result = 1;
	if(result != expResult)
	{
		printf("\t%s strings: %s\n", testName, FAIL);
		fail = 1;
	}
	free_column(c);
    
	return fail;
}

int impossibleSplitTests()
{
	int failCount = 0;
	printf("%s\n", IMPOSSIBLESPLITTESTS);
    
	int expImpossible = 1;
	int expPossible = 0;
    
	//single class, single row
	int sizeSS = 1;
	unsigned int classesSS[] = {42};
	double doublesSS[] = {42};
	char *stringsSS[] = {"I'd tell you a UDP joke, but you might not get it."};
	failCount += impossibleSplitTestDouble(doublesSS, classesSS, sizeSS, expImpossible, "ss");
	failCount += impossibleSplitTestString(stringsSS, classesSS, sizeSS, expImpossible, "ss");
    
	//multiple classes, multiple rows
	int sizeMM1 = 10;
	unsigned int classesMM1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
	double doublesMM1[] = {42, 42, 42, 42, 42, 42, 42, 42, 42, 42};
	char *stringsMM1[] = {"But", "I", "can", "keep telling you", "this", "TCP", "joke", "until", "you", "get it."};
	failCount += impossibleSplitTestDouble(doublesMM1, classesMM1, sizeMM1, expPossible, "mm1");
	failCount += impossibleSplitTestString(stringsMM1, classesMM1, sizeMM1, expPossible, "mm1");
    
	return failCount;
}

int entropyTest(double *fields, unsigned int *classes, unsigned int n, double expEntropy, char *testName)
{
	int fail = 0;
	
	Column *c = make_double_column(fields, classes, n);
	if(fabs(calc_entropy(c) - expEntropy) > ERROR)
	{
		printf("\t%s: %s %f\n", testName, FAIL, calc_entropy(c));
		fail = 1;
	}
	free_column(c);
    
	return fail;
}

int entropyTests()
{
	int failCount = 0;
	printf("%s\n", ENTROPYTESTS);
    
	double doubles[42] = {0};
    
	//single
	int sizeS = 1;
	unsigned int classesS[] = {42};
	double expEntropyS = 0.0;
	failCount += entropyTest(doubles, classesS, sizeS, expEntropyS, "single");
    
	//large few classes
	int sizeF = 42;
	unsigned int classesF[] = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
        0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
	double expEntropyF = 1.963840;
	failCount += entropyTest(doubles, classesF, sizeF, expEntropyF, "fewclasses");
    
	return failCount;
}

int splitTestDouble(double *fields, unsigned int *classes, unsigned int n, double expLo, double expHi, char *testName)
{
	int fail = 0;
    
	Column *c = make_double_column(fields, classes, n);
	double result = find_double_split_value(c);
	if(result < expLo - ERROR || result > expHi + ERROR)
	{
		printf("\t%s double: %s\n", testName, FAIL);
		fail = 1;
	}
	free_column(c);
    
	return fail;
}

int splitTestString(char **fields, unsigned int *classes, unsigned int n, char *expString, char *testName)
{
	int fail = 0;
    
	Column *c = make_string_column(fields, classes, n);
	if(strcmp(find_string_split_value(c), expString))
	{
		printf("\t%s string: %s\n", testName, FAIL);
		fail = 1;
	}
	free_column(c);
    
	return fail;
}

int splitTests()
{
	int failCount = 0;
	printf("%s\n", SPLITTESTS);
    
	int size = 30;
    
	//fields
	//large random
	double doublesR[] = {96, 67, 47, 4, 4, 48, 96, 3, 48, 48, 6, 37, 6, 96, 2, 57, 5, 37, 74,
        47, 64, 7, 46, 63, 37, 2, 4, 84, 26, 3};
	char* stringsR[] = {"q", "w", "u", "w", "a", "i", "q", "p", "c", "e", "w", "q", "c", "r",
        "g", "f", "q", "s", "w", "x", "q", "w", "y", "p", "i",
        "w", "x", "g", "e", "w"};
    
	//classes
	//large few classes
	unsigned int classesF[] = {3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 5,
        5, 5, 5, 5, 5, 5, 5, 5};
    
	//RF
	double expDoubleRFLo = 84;
	double expDoubleRFHi = 96;
	char *expStringRF = "q";
	failCount += splitTestDouble(doublesR, classesF, size, expDoubleRFLo, expDoubleRFHi, "RF");
	failCount += splitTestString(stringsR, classesF, size, expStringRF, "RF");
    
	return failCount;
}

int main(int argc, char **argv)
{
	printf("\tfailed %d test(s)\n", singleValueTests());
	printf("\tfailed %d test(s)\n", impossibleSplitTests());
	printf("\tfailed %d test(s)\n", entropyTests());
	printf("\tfailed %d test(s)\n", splitTests());
}