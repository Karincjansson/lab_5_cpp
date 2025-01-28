#include "String.h"
#include <cassert>
#include<iostream>
// Default Constructor: Initializes an empty string with a single null-terminator.
String::String(): str_size(0), str_capacity(0), data_(new char[1]{})
{
	Invariant();// Ensure the class invariants are valid after construction.
}
String::~String()// Destructor: Cleans up allocated memory.
{
	Invariant();//checks the thingy is correct
	delete[] data_;// Free the allocated memory.
	
}
// Copy Constructor: Creates a new String as a deep copy of another String.
String::String(const String& rhs) : str_size(rhs.str_size), str_capacity(rhs.str_capacity),data_(new char[str_capacity] {})
{

	std::memcpy(data_, rhs.data_, str_size); // Copy the content of the source string.
	Invariant();// Ensure the class invariants are valid after copying.
	
}
// Constructor from C-string: Creates a String from a null-terminated C-string.'
String::String(const char* cstr)
	:str_size(strlen(cstr)),  // Copy size from the source object.
	str_capacity(str_size+10), // Copy capacity from the source object.
	data_(new char[str_capacity] {})// Allocate memory for the new string.
{
	//data_ = new char[str_capacity];//unsure why it is in there??
	std::memcpy(data_, cstr, str_size); // Copy the content of the source string.
	data_[str_size] = '\0';// Add null-terminator.
	Invariant(); // Ensure the class invariants are valid after copying.
}

// Copy Assignment Operator: Assigns one String to another (deep copy).
String& String::operator=(const String& rhs) {
	Invariant(); // Check class invariants before assignment.

	// Handle self-assignment to avoid overwriting the object with itself.
	if (this == &rhs) {
		return *this;
	}

	// If the current capacity is insufficient, allocate new memory.
	if (str_capacity < rhs.str_size) {
		delete[] data_; // Free the old memory.
		str_capacity = rhs.str_capacity; // Update capacity.
		data_ = new char[str_capacity]; // Allocate new memory.
	}

	str_size = rhs.str_size; // Update size.
	std::memcpy(data_, rhs.data_, str_size); // Copy the content of the source string.
	Invariant(); // Ensure the class invariants are valid after assignment.
	return *this; // Return the current object to allow chaining.
}

// Subscript Operator (Non-const): Provides mutable access to a character in the string.
char& String::operator[](size_t i) {
	return data_[i]; // Return a reference to the character at index `i`.
}


void String::Invariant()
{
	// Check that the capacity and size are not negative and that size <= capacity.
	assert(str_capacity >= 0);
	assert(str_size >= 0);
	assert(str_size <= str_capacity);
}

// Returns the current size of the string (number of characters excluding null-terminator).
size_t String::size() const 
{
	return str_size;
}

// Returns the current capacity of the string (total allocated memory).
size_t String::capacity()const
{
	return str_capacity;
}
// Adds a character to the end of the string, resizing if necessary
void String::push_back(char c)
{
	Invariant();// Check class invariants before modifying the object.

	// If the current size equals the capacity, reallocate memory.
	if (str_size == str_capacity) {
		reserve(str_capacity == 0 ? 1 : str_capacity * 2); // Double the capacity or initialize to 1.
	}

	data_[str_size++] = c; // Add the character and increment the size.

	Invariant(); // Check after modification.

}
// Reserves memory for the string, ensuring at least `new_capacity` bytes of storage.
void String::reserve(size_t new_capacity) 
{

	// Only reallocate if the new capacity is larger than the current capacity.
	if (new_capacity > str_capacity)
	{
		//std::cout << "Reserving memory: " << new_capacity << " bytes" << std::endl;
		char* new_data = new char[new_capacity]; // Allocate new memory.

		std::memcpy(new_data, data_, str_size); // Copy existing data to the new buffer.

		delete[] data_; // Free the old memory.

		data_ = new_data; // Update the data pointer.

		str_capacity = new_capacity; // Update the capacity.
	}
	Invariant(); // Check class invariants after modification.
}
// Inequality Operator: Compares two strings for inequality.
bool operator!=(const String& lhs, const String rhs) 
{
	return !(lhs == rhs); // Use the equality operator to determine inequality.
}
// Equality Operator: Compares two strings for equality.

bool operator==(const String& lhs, const String& rhs)
{

	// If sizes differ, the strings are not equal.
	if (lhs.size() != rhs.size())
	{ 
		return false;
	}

	// Compare each character in the strings.
	for(size_t i=0;i<lhs.size();++i)
	{	
		if (lhs[i]!=rhs[i]) 
	       {return false;}
	}

	return true;
}
// Returns a pointer to the internal character array (C-string compatibility).
const char* String::data() const
{
	return data_; 
}


// Subscript Operator (Const): Provides read-only access to a character in the string.
const char& String::operator[](size_t i) const
{
	return data_[i];// Return a const reference to the character at index `i`.
}
// Stream Output Operator: Outputs the contents of the string to an output stream.
std::ostream& operator<<(std::ostream& out, const String& rhs)
{
	// Write each character of the string to the stream.
	for(size_t i=0; i<rhs.size();++i)
	{
		out << rhs[i];
		
	}
	return out;// Return the stream to allow chaining.
}

