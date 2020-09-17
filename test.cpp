#include "del-bf.h"

#include <cassert>

#define ADD_UINT32(INT) {x = INT; dbf.add((char*) &x, 4);}
#define TEST_UINT32_SUCCESS(INT) {x = INT; assert(dbf.test((char*) &x, 4));}
#define TEST_UINT32_FAILURE(INT) {x = INT; assert(!dbf.test((char*) &x, 4));}
#define TEST_AND_REMOVE_UINT32_SUCCESS(INT) {x = INT; assert(dbf.testAndRemove((char*) &x, 4));}
#define TEST_AND_REMOVE_UINT32_FAILURE(INT) {x = INT; assert(!dbf.testAndRemove((char*) &x, 4));}

int main(int argc, char** argv){
    DeletableBloomFilter dbf(128, 128, 0.1);
    uint32_t x;
    ADD_UINT32(2);
    ADD_UINT32(4);
    ADD_UINT32(6);

    assert(dbf.getCount() == 3);
    TEST_UINT32_SUCCESS(2);
    TEST_UINT32_SUCCESS(4);
    TEST_UINT32_SUCCESS(6);
    TEST_UINT32_FAILURE(3);

    TEST_AND_REMOVE_UINT32_SUCCESS(2);
    TEST_AND_REMOVE_UINT32_SUCCESS(4);
    TEST_AND_REMOVE_UINT32_SUCCESS(6);
    TEST_AND_REMOVE_UINT32_FAILURE(3);
}