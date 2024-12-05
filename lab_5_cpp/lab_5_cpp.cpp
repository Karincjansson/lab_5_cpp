
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

void AssertStrEqual(const String& lhs, const char* rhs) {
    for (size_t i = 0; i < lhs.str_size && *rhs != '\0'; ++i) {
        assert(lhs[i] == rhs[i]);
        assert(rhs[i] != 0);
    }
    assert(rhs[lhs.str_size] == 0);
}

void TestCapacitySetting() {
    {
        String s, r;
        int str_size = s.str_size;
        for (int i = 0; i < str_size * 2 + 16; ++i)
            r.push_back('a' + i % 26);
        s = r;
        assert(s.str_size <= s.str_capacity);
    }
    {
        String r;
        for (int i = 0; i < 65; ++i)
            r.push_back('a' + i % 26);
        String s(r);
        assert(s.str_size <= s.str_capacity);
    }
}

void TestPushBack() {
    {
        String str = "bar";
        str.push_back('a');
        assert(str == "bara");
    }
    {
        String r;
        int str_size = 65;
        for (int i = 0; i < str_size; ++i)
            r.push_back('a' + i % 26);
        assert(r.str_size <= r.str_capacity);
        assert(r.str_size == str_size);
    }

    //Test No Extra Alloc
    {
        String r;
        int str_size = 65;
        for (str_size = 0; str_size < 65; ++str_size)
            r.push_back('a' + str_size % 26);
        while (r.str_size >= r.str_capacity) {
            r.push_back('a' + str_size % 26);
            ++str_size;
        }
        const char* ptr = r.data();
        int cap = r.str_capacity;
        while (ptr == r.data())
        {
            r.push_back('x');
        }

        (r.str_size == cap + 1);
    }
}

void TestPushBackReallocation() {
    String str("hej");
    assert(str.str_size <= str.str_capacity);
    assert(str.str_size == 3);
    auto hej = "hej";
    AssertStrEqual(str, hej);
    AssertStrEqual(str, "hej");
#ifdef VG
    //If VG we try to take 20 empty places: (str_size+1 < str_capacity)
    while (str.str_size() + 20 >= str.str_capacity() && str.str_size() < 1000)
        str.push_back('A' + rand() % 32);
    assert(str.str_size() < 1000);	//If this fail it prbably the case that str_capacity is increased with a constant.
#endif //VG

    auto internalBuf = &str[0];
    auto cap = str.str_capacity;
    auto siz = str.str_size;
    size_t i;
    for (i = siz + 1; i <= cap; ++i) {
        str.push_back(char(i) + 'a');
        assert(internalBuf == &str[0]);
        assert(cap == str.str_capacity);
        assert(i == str.str_size);
    }
    str.push_back(char(i));
    assert(internalBuf != &str[0]);
    assert(cap < str.str_capacity);
    assert(i == str.str_size);
}

void TestForPassingString() {
    String str0;
    AssertStrEqual(str0, "");

    String s1("foo");
    assert(s1 == "foo");
    String str(s1);
    assert(str == "foo"); //current bug is that s1 is not copied correctly.
    String s3("bar");
    assert(s3 == "bar");

    delete new String("hej");

    str = "hej";
    assert((str = s3) == s3);
    assert((str = str) == s3);


    str = "heja";
    s3 = str;
    str[0] = 'x'; assert(s3[0] == 'h');
    s3[1] = 'y'; assert(str[1] == 'e');


    String str1("foo"), str2("bar"), str3("hej");
    str3 = str = str1;
    assert(str3 == str);
    assert(str1 == str);

    TestCapacitySetting();

    AssertStrEqual(str1, "foo");
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
    assert(std::is_const<std::remove_reference< decltype(sc[1])>::type>::value); //Kolla att det blir en const resultat av indexering

    TestPushBack();

    const char* temp = str.data();
    assert(temp == &str[0]);

    TestPushBackReallocation();

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
        cout << "********It is possible that you don't allocate more memory in the operator= even if it is needed!!!******";
    delete sPtr;
}

void TestForWellPassingString() {
#ifdef VG
    TestDelete();
    TestNoExtraAlloc();

    String str("bar");

    //-	at(size_t i) som indexerar med range check
    try {
        str.at(-1);
        assert(false);
    }
    catch (std::out_of_range&) {};
    try {
        str.at(4);
        assert(false);
    }
    catch (std::out_of_range&) {};

    //- at indexerar

    //-	at(size_t i) 
    str = "bar";
    assert(str.at(1) == 'a');
    str.at(1) = 'y';
    assert(str.at(1) == 'y');

    const String strC(str);
    assert(strC.at(1) == 'y');
    assert(std::is_const<std::remove_reference< decltype(strC.at(1))>::type>::value);


    //	reserve()
    auto internalBuf = &str[0];
    auto cap = str.str_capacity();
    auto siz = str.str_size();

    str.reserve(cap);
    assert(internalBuf == &str[0]);
    assert(cap == str.str_capacity());
    assert(siz == str.str_size());

    str.reserve(cap + 1);
    assert(internalBuf != &str[0]);
    assert(cap < str.str_capacity());
    assert(siz == str.str_size());

    // shrink_to_fit
    str = "hej";
    str.reserve(10);
    internalBuf = &str[0];
    cap = str.str_capacity();
    siz = str.str_size();

    str.shrink_to_fit();
    assert(internalBuf != &str[0]);
    assert(str.str_capacity() == str.str_size());
    AssertStrEqual(str, "hej");

    ///////////////////
    //-	operator+=(Strong strong) som tolkas som konkatenering.
    //foo, bar, hej
    String str1("foo"), str2("bar"), str3("hej");
    ((str = "xyz") += str1) += (str3 += str1);
    assert(str3 == "hejfoo" && str == "xyzfoohejfoo" && str1 == "foo");

    //+= som for plats;
    str = "bar"; str.reserve(10);
    str += "foo";
    assert(str == "barfoo");

    //+= som inte for plats;
    str.reserve(10);
    str = "";
    size_t i;
    for (i = 0; str.str_size() < str.str_capacity(); ++i)
        str.push_back(char('0' + i));
    str1 = "bar";
    str += str1;
    for (size_t k = 0; k < i; ++k)
        assert(str[k] == '0' + k);
    assert(str[i] == 'b');
    str = "foo";
    str += str;
    assert(str == "foofoo");

    //-	operator+
    str = "bar";
    assert(str + "foo" == "barfoo");
    AssertStrEqual(str, "bar");

    cout << "TestForWellPassingString completed." << std::endl;
#endif //VG
}

int main() {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    TestForPassingString();
    //TestForWellPassingString();

    return 0;
}