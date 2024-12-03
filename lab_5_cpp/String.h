#pragma once
#ifndef STRING_H
#include <ostream>
#include <cassert>
#define string_h
#include "String.h"
class String
{

public:
	String();
	~String();
	String(const String& rhs);
	String(const char* cstr);
	String& operator =(const String& rhs);
	char& operator[](size_t i);
	const char& operator[](size_t i) const;
	size_t size()const;
	size_t capacity() const;
	void push_back(char c);
	void reserve(size_t new_capacity);
	friend bool operator!=(const String& lhs, const String rhs);
	friend bool operator==(const String& lhs, const String& rhs);
	const char* data() const;
	friend std::ostream& operator<<(std::ostream& out, const String& rhs);
private:
	char* data_;// pekare till array av tecken
	size_t str_size;
	size_t str_capacity;


	void Invariant();

};

#endif //STRING_H

