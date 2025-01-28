
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif

#include <crtdbg.h>
#include <String.h>
#include "String.h"
#include <iostream>
#include <cassert>
using std::cout;

// Asserts that two strings are equal. Compares each character in the lhs String with the rhs C-string.
void AssertStrEqual(const String& lhs, const char* rhs) {
    for (size_t i = 0; i < lhs.str_size && *rhs != '\0'; ++i) {
        assert(lhs[i] == rhs[i]);// Ensure each character matches.
        assert(rhs[i] != 0); // Ensure no null character is encountered 
    }
    assert(rhs[lhs.str_size] == 0);// Ensure rhs is terminated properly after lhs's size.

}
// Tests that the capacity setting of a String object works correctly.
void TestCapacitySetting() {
    {
        // Test case 1: Ensures String can handle a larger capacity after push_back operations.
        String s, r;
        int str_size = s.str_size;
        for (int i = 0; i < str_size * 2 + 16; ++i)
            r.push_back('a' + i % 26);// Fill `r` with a pattern of characters.
        s = r;// Assign `r` to `s`.
        assert(s.str_size <= s.str_capacity);// Verify `s`'s size is within its capacity.
    }
    {
        // Test case 2: Ensures a larger string assignment respects capacity limits.
        String r;
        for (int i = 0; i < 65; ++i)
            r.push_back('a' + i % 26);// Fill `r` with 65 characters.
        String s(r);// Copy-construct `s` from `r`.
        assert(s.str_size <= s.str_capacity);// Verify `s`'s size is within its capacity.
    }
}
// Tests the push_back functionality of the String class.
void TestPushBack() {
    {
        // Test case 1: Appending a single character to a string.
        String str = "bar";
        str.push_back('a'); // Add 'a' to the string.
        assert(str == "bara");// Ensure the resulting string matches the expected value.
    }
    {
        // Test case 2: Appending multiple characters and checking size/capacity.
        String r;
        int str_size = 65;
        for (int i = 0; i < str_size; ++i)
            r.push_back('a' + i % 26); // Fill `r` with 65 characters.
        assert(r.str_size <= r.str_capacity); // Ensure capacity is not exceeded.
        assert(r.str_size == str_size);// Ensure size matches the expected value.
    }
    // Test case 3: Testing push_back with reallocations to grow capacity.
   
    {
        String r;
        int str_size = 65;
        for (str_size = 0; str_size < 65; ++str_size)
            r.push_back('a' + str_size % 26);// Add characters until size reaches 65.

        // Push characters until the string requires reallocation.
        while (r.str_size >= r.str_capacity) {
            r.push_back('a' + str_size % 26);
            ++str_size;
        }
        const char* ptr = r.data();// Store pointer to current buffer.
        int cap = r.str_capacity; // Store current capacity.
        while (ptr == r.data())// Keep pushing characters until buffer reallocates.
        {
            r.push_back('x');
        }

        (r.str_size == cap + 1);// Check if size is updated correctly after reallocation.
    }
}
// Tests the reallocation behavior of push_back and ensures correct copying.
void TestPushBackReallocation() {
    String str("hej");
    assert(str.str_size <= str.str_capacity); // Ensure initial size is within capacity.
    assert(str.str_size == 3);// Verify the size matches the initialized value.
    auto hej = "hej";
    AssertStrEqual(str, hej); // Ensure the string matches the expected value.
    AssertStrEqual(str, "hej");


    auto internalBuf = &str[0]; // Store the pointer to the internal buffer.
    auto cap = str.str_capacity;
    auto siz = str.str_size;
    size_t i;
    for (i = siz + 1; i <= cap; ++i) {
        str.push_back(char(i) + 'a');// Add characters up to capacity.
        assert(internalBuf == &str[0]); // Ensure no reallocation occurs.
        assert(cap == str.str_capacity); // Ensure capacity remains unchanged.

        assert(i == str.str_size);// Verify size increments correctly.
    }
    str.push_back(char(i)); // Push one more character to force reallocation.
    assert(internalBuf != &str[0]);// Ensure reallocation occurred.
    assert(cap < str.str_capacity); // Ensure capacity increased.
    assert(i == str.str_size);// Verify size matches the expected value.
}
// Comprehensive test for passing and manipulating String objects.
void TestForPassingString() {
    String str0;
    AssertStrEqual(str0, "");// Verify an empty string

    String s1("foo");
    assert(s1 == "foo");// Verify initialization with a C-string.
    String str(s1);
    assert(str == "foo"); // Test copy constructor.
    String s3("bar");
    assert(s3 == "bar"); // Verify initialization with another string.

    delete new String("hej");// Test dynamic allocation and deletion.


    str = "hej";
    assert((str = s3) == s3);// Test assignment operator.
    assert((str = str) == s3);// Self-assignment.


    str = "heja";
    s3 = str;
    str[0] = 'x'; assert(s3[0] == 'h');// Test character access and modifications.
    s3[1] = 'y'; assert(str[1] == 'e');// Verify that `s3` and `str` are independent.



    String str1("foo"), str2("bar"), str3("hej");
    str3 = str = str1;
    assert(str3 == str);
    assert(str1 == str);

    TestCapacitySetting(); // Test capacity functionality.

    AssertStrEqual(str1, "foo");// Verify string contents.
    auto xxx = str1.data();
    str1 = String("huj");
    assert(xxx == str1.data());

    {
        String s, r;
        s.push_back(0);
        s.push_back(0);
        s.push_back(0);
        r.push_back(0);
        assert(s != r && !(r == s));
    }


    assert(str1 != str);
    assert(!(str1 != str1));

    str = "bar";
    str[-1];
    str[1000];
    assert(str[1] == 'a');
    str[1] = 'y';
    assert(str[1] == 'y');

    const String sc(str);
    assert(sc[1] == 'y');
    assert(std::is_const<std::remove_reference< decltype(sc[1])>::type>::value);

    TestPushBack();// Test push_back functionality.

    const char* temp = str.data();
    assert(temp == &str[0]);

    TestPushBackReallocation(); // Test push_back with reallocations.

    cout << String("TestForPassingString completed.") << std::endl;
}

String* MakeString(size_t str_size) {
    String* str = new String();
    int i;
    for (i = 0; i < str_size; ++i)
        str->push_back('a' + i % 26);
    return str;
}

void TestDelete() {
    void* sSave = malloc(sizeof(String));
    void* sPtr;
    {
        String s("hejsan");
        sPtr = &s;
        memcpy(sSave, &s, sizeof(String));
    }
    if (memcmp(sSave, sPtr, sizeof(String)))
        cout << "You are doing uneccessary things in you deconstructor ~String()\n";
    else {
        {
            String s;
            sPtr = &s;
            memcpy(sSave, &s, sizeof(String));
        }
        if (memcmp(sSave, sPtr, sizeof(String)))
            cout << "You are doing uneccessary things in you deconstructor ~String()\n";
    }
    free(sSave);
}

void TestNoExtraAlloc() {
    String r;
    int str_size;
    for (str_size = 0; str_size < 33; ++str_size)
        r.push_back('a' + str_size % 26);
    while (r.str_size >= r.str_capacity) {
        r.push_back('a' + str_size % 26);
        ++str_size;
    }

    String* sPtr;
    sPtr = MakeString(r.str_size - 1);
    sPtr->reserve(sPtr->str_size + 1);
    const char* dPtr;
    dPtr = sPtr->data();
    *sPtr = r;
    assert(dPtr == sPtr->data());
    delete sPtr;

    sPtr = MakeString(r.str_size - 1);
    dPtr = sPtr->data();
    *sPtr = r;
    assert(dPtr != sPtr->data());
    if (dPtr == sPtr->data())
        
    delete sPtr;
}

int main() {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    TestForPassingString();
    //TestForWellPassingString();

    return 0;
}