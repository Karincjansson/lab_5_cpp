// lab_5_cpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif // _DEBUG
//#include <stdlib.h>
#include <crtdbg.h>

#include "String.h"
#include <string>
#include <iostream>
#include <cassert>
#include <utility>
using namespace std;

void AssertStrEqual(const String& lhs, const char* rhs) {
	for (size_t i = 0; i < lhs.size() && *rhs != '\0'; ++i) {
		assert(lhs[i] == rhs[i]);
		assert(rhs[i] != 0);
	}
	assert(rhs[lhs.size()] == 0);
}
void TestPushBackReallocation() {
	String str("hej");
	assert(str.size() <= str.capacity());
	assert(str.size() == 3);
	auto hej = "hej";
	//AssertStrEqual(str, hej);
	AssertStrEqual(str, "hej");

		auto internalBuf = &str[0];
	auto cap = str.capacity();
	auto siz = str.size();
	size_t i;
	for (i = siz + 1; i <= cap; ++i) {
		str.push_back(char(i) + 'a');
		assert(internalBuf == &str[0]);
		assert(cap == str.capacity());
		assert(i == str.size());
	}
	str.push_back(char(i));
	assert(internalBuf != &str[0]);
	assert(cap < str.capacity());
	assert(i == str.size());
}
#pragma region DeleteCheckFel
//#pragma push_macro("new")
//#undef new
//
//void* memoryPtr;
//void* operator new(size_t size) {
// memoryPtr = ::malloc(size);
// return memoryPtr;
//}
//
//void operator delete(void* ptr) {
// if (ptr != memoryPtr) {
// cout << "You are deleting the wrong string!?\n";
// }
// ::free(ptr);
//}
//
//void TestDelete() {
// void* sSave = malloc(sizeof(String));
// void* sPtr;
// {
// String s("huj");
// sPtr = &s;
// memcpy(sSave, &s, sizeof(String));
// s.~String();
//#pragma pop_macro("new")
// }
// if (memcmp(sSave, sPtr, sizeof(String)))
// cout << "You are doing uneccessary things in you deconstructor ~String()\
n";
//}
#pragma endregion DeleteCheckFel
void TestDelete() {
	
	void* sSave = malloc(sizeof(String));
	void* sPtr;
	{
		String s("hejsan");
		sPtr = &s;
		memcpy(sSave, &s, sizeof(String));
	}
	if (memcmp(sSave, sPtr, sizeof(String)))
	{
		cout<< "You are doing uneccessary things in you deconstructor ~String()\n";
	}
		
	else { //F r att f nga "null" version, tveksamt om den hittar n got!? ? ?
		{
			String s;
			sPtr = &s;
			memcpy(sSave, &s, sizeof(String));
		}
		if (memcmp(sSave, sPtr, sizeof(String)))
			cout<<"You are doing uneccessary things in you deconstructor ~String()\n";
	}
	delete sSave;
}
void TestCapacitySetting() {
	//F r godk nt? ?
	{
		String s, r;
		int size = s.size();
		for (int i = 0; i < size * 2 + 16; ++i)
			r.push_back('a' + i % 26);
		s = r;
		assert(s.size() <= s.capacity());
	}
	{ //Will probably fail to catch any error :-(
		String r;
		for (int i = 0; i < 65; ++i)
			r.push_back('a' + i % 26);
		String s(r);
		assert(s.size() <= s.capacity());
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
		int size = 65;
		for (int i = 0; i < size; ++i)
			r.push_back('a' + i % 26);
		assert(r.size() <= r.capacity());
		assert(r.size() == size);
	}
	//Test No Extra Alloc
	{
		String r;
		int size = 65;
		for (size = 0; size < 65; ++size)
			r.push_back('a' + size % 26);
		while (r.size() >= r.capacity()) {
			r.push_back('a' + size % 26);
			++size;
		}
		const char* ptr = r.data();
		int cap = r.capacity();
		while (ptr == r.data())
		{
			r.push_back('x');
		}
		assert(r.size() == cap + 1);
	}
}
String* MakeString(size_t size) {
	String* str = new String();
	int i;
	for (i = 0; i < size; ++i)
		str->push_back('a' + i % 26);
	return str;
}

void TestFörGodkäntString() 
{
	
		//- String()
		String str0; AssertStrEqual(str0, "");
	//- String(Str ng str ng)? ?
	String s1("foo"); assert(s1 == "foo");
	String str(s1); assert(str == "foo");
	String s3("bar"); assert(s3 == "bar");
	//- ~String() Kom ih g destruktorn!?
	delete new String("hej");
	//TestDelete();
	// - operator =(Str ng str ng)? ?
	str = "hej";
	assert((str = s3) == s3);
	assert((str = str) == s3); //self assignment
	//Ej samma buffert
	str = "heja"; s3 = str;
	str[0] = 'x'; assert(s3[0] == 'h');
	s3[1] = 'y'; assert(str[1] == 'e');
	String str1("foo"), str2("bar"), str3("hej");
	str3 = str = str1;
	assert(str3 == str);
	assert(str1 == str);
	TestCapacitySetting();
	//No extra realloc
	AssertStrEqual(str1, "foo");
	auto xxx = str1.data();
	str1 = String("huj");
	assert(xxx == str1.data());
	//- operator==
	//testas verallt!?
	{
		String s, r;
		s.push_back(0);
		s.push_back(0);
		s.push_back(0);
		r.push_back(0);
		assert(s != r && !(r == s));
	}
	//- operator!=
	assert(str1 != str);
	assert(!(str1 != str1));
	//- operator[](size_t i) som indexerar utan range check.
	str = "bar";
	str[-1]; str[1000]; //No error
	assert(str[1] == 'a');
	str[1] = 'y';
	assert(str[1] == 'y');
	const String sc(str);
	assert(sc[1] == 'y');
	assert(std::is_const<std::remove_reference< decltype(sc[1])>::type>::value); 
	//Kolla att det blir en const resultat av indexering
		//- push_back(char c)
		TestPushBack();
	// data
	const char* temp = str.data();
	assert(temp == &str[0]);
	//- size(), capacity() and reloccation test;
	TestPushBackReallocation();
	cout << String("hej\n");
	cout << "Om det står hej på föregående rad så är TestFörGodkänt klar\n"; 
}

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	locale::global(locale("swedish"));
	TestFörGodkäntString(); 
		cout << "G test klart\n";
#
	new int;
	cout << "det finns en minnesl cka i main, avsiktligt!\n s ni kan se att er minnesläckstest fungerar\n";
		cin.get();
}