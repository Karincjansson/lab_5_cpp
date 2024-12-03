#include "String.h"
#include <cassert>
#include<iostream>
String::String(): str_size(0), str_capacity(0), data_(nullptr)
{
	Invariant();
}
String::~String()//destructor
{
	delete[] data_;//lol något mer men...
	
}
//CopyConstructor
String::String(const String& rhs): str_size(rhs.str_size), str_capacity(rhs.str_capacity)
{
	data_ = new char[str_capacity];
	std::copy(rhs.data_, rhs.data_ + str_size, data_);

	Invariant();
	
}
String::String(const char* cstr)
{
	data_ = new char[str_capacity];
	std::memcpy(data_, cstr, str_size);
	Invariant();
}

String& String::operator=(const String& rhs)
{
	if(this !=&rhs)
	{
		delete[] data_;
		str_size = rhs.str_size;
		str_capacity = rhs.str_capacity;
		data_ = new char[str_capacity];
		std::copy(rhs.data_, rhs.data_ + str_size, data_);
	}
	return *this;
}
char& String::operator[](size_t i)
{
	return data_[i];
}
void String::Invariant()
{
	//huh?
	assert(str_size <= str_capacity);
}
size_t String::size()const
{
	return str_size;
}
size_t String::capacity()const
{
	return str_capacity;
}
void String::push_back(char c)
{
	if(str_size==str_capacity)
	{
		reserve(str_capacity == 0 ? 1 : str_capacity * 2);
	}
	data_[str_size++]=c;
	Invariant();

}
void String::reserve(size_t new_capacity) {
	if (new_capacity > str_capacity)
	{
		std::cout << "Reserving memory: " << new_capacity << " bytes" << std::endl;
		char* new_data = new char[new_capacity];
		std::memcpy(new_data, data_, str_size);
		delete[] data_;
		data_ = new_data;
		str_capacity = new_capacity;
	}
	Invariant();
}
bool operator!=(const String& lhs, const String rhs)
{
	return!(lhs == rhs);
}
bool operator==(const String& lhs, const String& rhs)
{
	if (lhs.size() != rhs.size()) { return false; }

	for(size_t i=0;i<lhs.size();++i){
		if (lhs[i]!=rhs[i]) {return false;}}

	return true;
}
const char* String::data() const
{
	return data_;
}
const char& String::operator[](size_t i) const
{
	return data_[i];
}
std::ostream& operator<<(std::ostream& out, const String& rhs)
{
	for(size_t i=0; i<rhs.size();++i)
	{
		out << rhs[i];
		
	}
	return out;
}

