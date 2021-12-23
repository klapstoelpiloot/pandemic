#pragma once
#include <assert.h>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>

class String;
template <typename T> class List;

/*!
 * [Class] String
 *
 * [Short description]
 *    The String class is a wrapper around the std::string in the STL library.
 *    It provides the developer with a lot of useful extensions to the functionality
 *    that is currently offered by the std::string.
 *
 * [Detailed description]
 *    The String class is a wrapper around the std::string in the STL library.
 *    It provides the developer with a lot of useful extensions to the functionality
 *    that is currently offered by the std::string.
 * \n
 * \n The class is compatible with the following string types:
 *    <li>Single characters (char).
 *    <li>Character pointers (const char*)
 *    <li>STL strings (std::string)
 * \n
 * \n The class provides the following features over the std::string class:
 *    <li> Full integration of regular expressions.
 *    <li> Full support of comparison operators.
 *    <li> Append and Prepend of nummeric values.
 *    <li> Conversion to numeric values from a string.
 *    <li> Transformation functions (e.g. toUpper() and toLower()).
 *    <li> Extensive support for replacing parts of the string.
 *    <li> Extensive support for removing parts of the string.
 *    <li> Extensive support for splitting and joining strings.
 *    <li> Extensive support for creating substrings of the string.
 *    <li> Support for reading from and writing to a file.
 *    <li> Support of iterators.
 * \n
 */
class String final
{
    private:
        static const unsigned int _initialAllocSize;
        std::string _stringStorage;

    public:
        typedef std::string::iterator iterator;
        typedef std::string::const_iterator const_iterator;

    public:
        // Constructors and Destructor
        String();
        String(char c);
        String(char c, unsigned int len);
        String(const char* pchString);
        String(const wchar_t* pchString);
        String(const char* pchString, unsigned int len);
        String(const char* pchString, unsigned int start, unsigned int len);
        String(const std::string& stlString);
        String(const std::string& stlString, unsigned int start, unsigned int len);
        String(const String& otherString);
        String(const String& otherString, unsigned int start, unsigned int len);
        String(iterator start, iterator end);
        ~String();

        // Comparison operators
        bool operator==(const char* pchString) const;
        bool operator==(const std::string& stlString) const;
        bool operator==(const String& otherString) const;
        bool operator!=(const char* pchString) const;
        bool operator!=(const std::string& stlString) const;
        bool operator!=(const String& otherString) const;
        bool operator<(const char* pchString) const;
        bool operator<(const std::string& stlString) const;
        bool operator<(const String& otherString) const;
        bool operator<=(const char* pchString) const;
        bool operator<=(const std::string& stlString) const;
        bool operator<=(const String& otherString) const;
        bool operator>(const char* pchString) const;
        bool operator>(const std::string& stlString) const;
        bool operator>(const String& otherString) const;
        bool operator>=(const char* pchString) const;
        bool operator>=(const std::string& stlString) const;
        bool operator>=(const String& otherString) const;

        // Assign operators
        String& operator=(const char* pchString);
        String& operator=(const std::string& stlString);
        String& operator=(const String& otherString);

        // Add operators
        String operator+(const char* pchString) const;
        String operator+(const std::string& stlString) const;
        String operator+(const String& otherString) const;
        String operator+(int value) const;
        String operator+(unsigned int value) const;
        String operator+(long value) const;
        String operator+(unsigned long value) const;
        String operator+(float value) const;
        String operator+(double value) const;
        String operator+(char value) const;
        String operator+(bool value) const;

        // Assign operators
        String& operator+=(const char* pchString);
        String& operator+=(const std::string& stlString);
        String& operator+=(const String& otherString);
        String& operator+=(int value);
        String& operator+=(unsigned int value);
        String& operator+=(long value);
        String& operator+=(unsigned long value);
        String& operator+=(float value);
        String& operator+=(double value);
        String& operator+=(char value);
        String& operator+=(bool value);

        // Subscript operators
        char& operator[](unsigned int index);
        const char& operator[](unsigned int index) const;

        // Cast operators
        operator const char*() const;

        // String append functions
        String& Append(const char* pchString);
        String& Append(const std::string& stlString);
        String& Append(const String& otherString);
        String& Append(int value);
        String& Append(unsigned int value);
        String& Append(long value);
        String& Append(unsigned long value);
        String& Append(float value);
        String& Append(double value);
        String& Append(char value);
        String& Append(bool value);

        // String prepend functions
        String& Prepend(const char* pchString);
        String& Prepend(const std::string& stlString);
        String& Prepend(const String& otherString);
        String& Prepend(int value);
        String& Prepend(unsigned int value);
        String& Prepend(long value);
        String& Prepend(unsigned long value);
        String& Prepend(float value);
        String& Prepend(double value);
        String& Prepend(char value);
        String& Prepend(bool value);

        // String insert functions
        String& Insert(char value, unsigned int pos);
        String& Insert(char value, iterator it);
        String& Insert(const char* value, unsigned int pos);
        String& Insert(const char* value, iterator it);
        String& Insert(const std::string& value, unsigned int pos);
        String& Insert(const std::string& value, iterator it);
        String& Insert(const String& value, unsigned int pos);
        String& Insert(const String& value, iterator it);

        // String conversion functions
        bool ConvertTo(std::string& value) const;
        bool ConvertTo(String& value) const;
        bool ConvertTo(int& value) const;
        bool ConvertTo(unsigned int& value) const;
        bool ConvertTo(long& value) const;
        bool ConvertTo(unsigned long& value) const;
        bool ConvertTo(float& value) const;
        bool ConvertTo(double& value) const;
        bool ConvertTo(bool& value) const;

        // String transformation functions
        void ToUpper();
        String ToUpper() const;
        static String ToUpper(const String& input);
        void ToLower();
        String ToLower() const;
        static String ToLower(const String& input);
        void Reverse();
        String Reverse() const;
        static String Reverse(const String& input);
        void Capitalize();
        String Capitalize() const;
        static String Capitalize(const String& input);
        void Trim(bool leading, bool trailing);
        String Trim(bool leading, bool trailing) const;
        static String Trim(const String& input, bool leading, bool trailing);
		static String From(int value);
		static String From(unsigned int value);
		static String From(long value);
		static String From(unsigned long value);
		static String From(float value);
		static String From(double value);
		static String From(bool value);

        // Function to check the existence of a pattern in a String
        bool Contains(char ch, unsigned int start = 0) const;
        bool Contains(const char* pchString, unsigned int start = 0) const;
        bool Contains(const std::string& stlString, unsigned int start = 0) const;
        bool Contains(const String& otherString, unsigned int start = 0) const;

        // Functions for finding the index of a pattern in a String
        int Find(char ch, unsigned int start = 0) const;
        int Find(const char* pchString, unsigned int start = 0) const;
        int Find(const std::string& stlString, unsigned int start = 0) const;
        int Find(const String& otherString, unsigned int start = 0) const;

        // Functions for finding the index of a pattern starting at the end of the String
        // (i.e. a reverse find).
        int FindLast(char ch, int last = -1) const;
        int FindLast(const char* pchString, int last = -1) const;
        int FindLast(const std::string& stlString, int last = -1) const;
        int FindLast(const String& otherString, int last = -1) const;

        // Functions for counting the number of occurrences of a pattern in a String
        unsigned int Count(char ch, unsigned int start = 0) const;
        unsigned int Count(const char* pchString, unsigned int start = 0) const;
        unsigned int Count(const std::string& stlString, unsigned int start = 0) const;
        unsigned int Count(const String& otherString, unsigned int start = 0) const;

        // Functions for replacing a part of the string with another string.
        void Replace(unsigned int start, unsigned int len, char replacement);
        void Replace(unsigned int start, unsigned int len, const char* replacement);
        void Replace(unsigned int start, unsigned int len, const std::string& replacement);
        void Replace(unsigned int start, unsigned int len, const String& replacement);
        void Replace(iterator start, iterator end, char replacement);
        void Replace(iterator start, iterator end, const char* replacement);
        void Replace(iterator start, iterator end, const std::string& replacement);
        void Replace(iterator start, iterator end, const String& replacement);
        unsigned int Replace(char pattern, char replacement, bool global = true);
        unsigned int Replace(char pattern, const char* replacement, bool global = true);
        unsigned int Replace(char pattern, const std::string& replacement, bool global = true);
        unsigned int Replace(char pattern, const String& replacement, bool global = true);
        unsigned int Replace(const char* pattern, const char* replacement, bool global = true);
        unsigned int Replace(const char* pattern, const std::string& replacement, bool global = true);
        unsigned int Replace(const char* pattern, const String& replacement, bool global = true);
        unsigned int Replace(const std::string& pattern, const char* replacement, bool global = true);
        unsigned int Replace(const std::string& pattern, const std::string& replacement, bool global = true);
        unsigned int Replace(const std::string& pattern, const String& replacement, bool global = true);
        unsigned int Replace(const String& pattern, const std::string& replacement, bool global = true);
        unsigned int Replace(const String& pattern, const char* replacement, bool global = true);
        unsigned int Replace(const String& pattern, const String& replacement, bool global = true);

        // Function for making the string completely empty.
        void Clear();

        // Functions for erasing a part of the string.
        void Erase(unsigned int start, unsigned int len);
        void Erase(const char* pattern, unsigned int start = 0);
        void Erase(const std::string& pattern, unsigned int start = 0);
        void Erase(const String& pattern, unsigned int start = 0);
        void Erase(iterator it);
        void Erase(iterator start, iterator end);

        // Functions for splitting the string in multiple parts
        unsigned int Split(std::vector<std::string>& list, char separator, bool javastyle = true) const;
        unsigned int Split(std::vector<std::string>& list, const char* separator, bool javastyle = true) const;
        unsigned int Split(std::vector<std::string>& list, const std::string& separator, bool javastyle = true) const;
        unsigned int Split(std::vector<std::string>& list, const String& separator, bool javastyle = true) const;
        unsigned int Split(std::vector<String>& list, char separator, bool javastyle = true) const;
        unsigned int Split(std::vector<String>& list, const char* separator, bool javastyle = true) const;
        unsigned int Split(std::vector<String>& list, const std::string& separator, bool javastyle = true) const;
        unsigned int Split(std::vector<String>& list, const String& separator, bool javastyle = true) const;
        unsigned int Split(std::string list[], unsigned int len, char separator, bool javastyle = true) const;
        unsigned int Split(std::string list[], unsigned int len, const char* separator, bool javastyle = true) const;
        unsigned int Split(std::string list[], unsigned int len, const std::string& separator, bool javastyle = true) const;
        unsigned int Split(std::string list[], unsigned int len, const String& separator, bool javastyle = true) const;
        unsigned int Split(String list[], unsigned int len, char separator, bool javastyle = true) const;
        unsigned int Split(String list[], unsigned int len, const char* separator, bool javastyle = true) const;
        unsigned int Split(String list[], unsigned int len, const std::string& separator, bool javastyle = true) const;
        unsigned int Split(String list[], unsigned int len, const String& separator, bool javastyle = true) const;

        // Functions for joining multiple strings into one string.
        static String Join(const std::vector<std::string>& list, char separator);
        static String Join(const std::vector<std::string>& list, const char* separator);
        static String Join(const std::vector<std::string>& list, const std::string& separator);
        static String Join(const std::vector<std::string>& list, const String& separator);
        static String Join(const std::vector<std::string>::const_iterator& itBegin,
                           const std::vector<std::string>::const_iterator& itEnd, char separator);
        static String Join(const std::vector<std::string>::const_iterator& itBegin,
                           const std::vector<std::string>::const_iterator& itEnd, const char* separator);
        static String Join(const std::vector<std::string>::const_iterator& itBegin,
                           const std::vector<std::string>::const_iterator& itEnd, const std::string& separator);
        static String Join(const std::vector<std::string>::const_iterator& itBegin,
                           const std::vector<std::string>::const_iterator& itEnd, const String& separator);
        static String Join(const std::vector<String>& list, char separator);
        static String Join(const std::vector<String>& list, const char* separator);
        static String Join(const std::vector<String>& list, const std::string& separator);
        static String Join(const std::vector<String>& list, const String& separator);
        static String Join(const std::vector<String>::const_iterator& itBegin,
                           const std::vector<String>::const_iterator& itEnd, char separator);
        static String Join(const std::vector<String>::const_iterator& itBegin,
                           const std::vector<String>::const_iterator& itEnd, const char* separator);
        static String Join(const std::vector<String>::const_iterator& itBegin,
                           const std::vector<String>::const_iterator& itEnd, const std::string& separator);
        static String Join(const std::vector<String>::const_iterator& itBegin,
                           const std::vector<String>::const_iterator& itEnd, const String& separator);
        static String Join(const std::string list[], unsigned int len, char separator);
        static String Join(const std::string list[], unsigned int len, const char* separator);
        static String Join(const std::string list[], unsigned int len, const std::string& separator);
        static String Join(const std::string list[], unsigned int len, const String& separator);
        static String Join(const String list[], unsigned int len, char separator);
        static String Join(const String list[], unsigned int len, const char* separator);
        static String Join(const String list[], unsigned int len, const std::string& separator);
        static String Join(const String list[], unsigned int len, const String& separator);
        static String Join(const char* list[], unsigned int len, char separator);
        static String Join(const char* list[], unsigned int len, const char* separator);
        static String Join(const char* list[], unsigned int len, const std::string& separator);
        static String Join(const char* list[], unsigned int len, const String& separator);

        String& Join(const std::vector<std::string>& list);
        String& Join(const std::vector<std::string>::const_iterator& itBegin,
                     const std::vector<std::string>::const_iterator& itEnd);
        String& Join(const std::vector<String>& list);
        String& Join(const std::vector<String>::const_iterator& itBegin,
                     const std::vector<String>::const_iterator& itEnd);
        String& Join(const std::string list[], unsigned int len);
        String& Join(const String list[], unsigned int len);
        String& Join(const char* list[], unsigned int len);

        // Functions for creating a sub-string out of a string
        String Substring(unsigned int start) const;
        String Substring(unsigned int start, unsigned int len) const;
        String Substring(const_iterator start) const;
        String Substring(const_iterator start, const_iterator end) const;

        String SubstringAfter(char ch, int start = 0) const;
        String SubstringAfter(const char* pchString, int start = 0) const;
        String SubstringAfter(const std::string& stlString, int start = 0) const;
        String SubstringAfter(const String& otherString, int start = 0) const;

        String SubstringBefore(char ch, int start = 0) const;
        String SubstringBefore(const char* pchString, int start = 0) const;
        String SubstringBefore(const std::string& stlString, int start = 0) const;
        String SubstringBefore(const String& otherString, int start = 0) const;

		// Checking start and end of a string
		bool StartsWith(const char* pchString);
		bool EndsWith(const char* pchString);

        // Accessor functions
        unsigned int Length() const;
        bool IsEmpty() const;

        const char* c_str() const;
        const std::string& stl() const;
        std::string& stl();

        char& Front();
        const char& Front() const;
        char& Back();
        const char& Back() const;

        // Iterator functions
        iterator Begin();
        const_iterator Begin() const;
        iterator End();
        const_iterator End() const;
        iterator get_iterator(unsigned int pos);
        const_iterator get_iterator(unsigned int pos) const;
		
        // Functions for reading from and writing to a file
        int ReadLine(std::istream& s);
        int Read(std::istream& s);
        int Write(std::ostream& s);
};

/*!
 * [Short description]
 *    Default constructor.
 */
inline String::String() :
        _stringStorage()
{
    _stringStorage.reserve(_initialAllocSize);
}

/*!
 * [Short description]
 *    Constructor.
 *
 * [Detailed description]
 *    Constructor for creating a String with a single character.
 *
 * [Parameters]
 *    in | c | Character used for creating a string.
 */
inline String::String(char c) :
        _stringStorage(1, c)
{
    _stringStorage.reserve(_initialAllocSize);
}

/*!
 * [Short description]
 *    Replication constructor.
 *
 * [Detailed description]
 *    Constructor for creating a String with a certain amount of
 *    replications of a single character.
 *
 * [Parameters]
 *    in | c   | Character used for creating a string.
 *    in | len | Number of times the character needs to be replicated.
 *
 * [TODO]
 *    Consider switching the arguments.
 */
inline String::String(char c, unsigned int len) :
        _stringStorage(len, c)
{
    if (len < _initialAllocSize)
    {
        _stringStorage.reserve(_initialAllocSize);
    }
}

/*!
 * [Short description]
 *    Constructor.
 *
 * [Detailed description]
 *    Constructor for creating a String based on a null terminating
 *    character array.
 *
 * [Parameters]
 *    in | pchString | Character array to copy into the String.
 */
inline String::String(const char* pchString)
{
	if(pchString == nullptr) return;
    if (strlen(pchString) < _initialAllocSize)
    {
        _stringStorage.reserve(_initialAllocSize);
    }
	_stringStorage.assign(pchString);
}

/*!
 * [Short description]
 *    Constructor.
 *
 * [Detailed description]
 *    Constructor for creating a String based on a character array.
 *    This character array doesn't need to be null terminated.
 *
 * [Parameters]
 *    in | pchString | Character array to copy into the String.
 *    in | len       | Number of character to copy from the character array.
 */
inline String::String(const char* pchString, unsigned int len)
{
	if(pchString == nullptr) return;
    if (len < _initialAllocSize)
    {
        _stringStorage.reserve(_initialAllocSize);
    }
	_stringStorage.assign(pchString, len);
}

/*!
 * [Short description]
 *    Constructor.
 *
 * [Detailed description]
 *    Constructor for creating a String based on a substring of a character array.
 *    This character array doesn't need to be null terminated.
 *
 * [Parameters]
 *    in | pchString | Character array to copy into the String.
 *    in | start     | First position in the character array to copy.
 *    in | len       | Number of character to copy from the character array.
 */
inline String::String(const char* pchString, unsigned int start, unsigned int len)
{
	if(pchString == nullptr) return;
    if (len < _initialAllocSize)
    {
        _stringStorage.reserve(_initialAllocSize);
    }
	_stringStorage.assign(pchString + start, len);
}

/*!
 * [Short description]
 *    Constructor.
 *
 * [Detailed description]
 *    Constructor for creating a String based on an STL string.
 *
 * [Parameters]
 *    in | stlString | STL string to copy.
 */
inline String::String(const std::string& stlString) :
        _stringStorage(stlString)
{
    if (stlString.size() < _initialAllocSize)
    {
        _stringStorage.reserve(_initialAllocSize);
    }
}

/*!
 * [Short description]
 *    Constructor.
 *
 * [Detailed description]
 *    Constructor for creating a String based on a substring of an STL string.
 *
 * [Parameters]
 *    in | stlString | STL string to copy.
 *    in | start     | First position in the STL string to copy.
 *    in | len       | Number of character to copy from the STL string.
 */
inline String::String(const std::string& stlString, unsigned int start, unsigned int len) :
        _stringStorage(stlString, start, len)
{
    if (len < _initialAllocSize)
    {
        _stringStorage.reserve(_initialAllocSize);
    }
}

/*!
 * [Short description]
 *    Copy constructor.
 *
 * [Detailed description]
 *    Constructor for creating a copy of a String object.
 *
 * [Parameters]
 *    in | otherString | String object to copy.
 */
inline String::String(const String& otherString) :
        _stringStorage(otherString._stringStorage)
{
    if (otherString._stringStorage.size() < _initialAllocSize)
    {
        _stringStorage.reserve(_initialAllocSize);
    }
}

/*!
 * [Short description]
 *    Constructor.
 *
 * [Detailed description]
 *    Constructor for creating a String based on a substring of a String.
 *
 * [Parameters]
 *    in | otherString | String object to copy.
 *    in | start       | First position in the String object to copy.
 *    in | len         | Number of character to copy from the String object.
 */
inline String::String(const String& otherString, unsigned int start, unsigned int len) :
        _stringStorage(otherString._stringStorage, start, len)
{
    if (len < _initialAllocSize)
    {
        _stringStorage.reserve(_initialAllocSize);
    }
}

/*!
 * [Short description]
 *    Constructor.
 *
 * [Detailed description]
 *    Constructor for creating a String based on a set of iterators.
 *    The iterators being passed into this constructor must point
 *    to the same String object, otherwise the behavior is undefined.
 *
 * [Parameters]
 *    in | start     | Iterator pointing to the first character to copy.
 *    in | end       | Iterator pointing to the last character to copy.
 */
inline String::String(iterator start, iterator end) :
        _stringStorage(start, end)
{
}

/*!
 * [Short description]
 *    Destructor.
 */
inline String::~String()
{
}

/*!
 * [Short description]
 *    Equality operator.
 *
 * [Detailed description]
 *    Operator to test if the argument is equal to the content of this String object.
 *
 * [Parameters]
 *    in | pchString | Pointer to null terminated character array to test against.
 *
 * [Return value]
 *    Returns true if the argument is equal to the content of this String,
 *    otherwise it returns false.
 */
inline bool String::operator==(const char* pchString) const
{
	if(pchString == nullptr) return false;
    return (_stringStorage == pchString);
}

/*!
 * [Short description]
 *    Equality operator.
 *
 * [Detailed description]
 *    Operator to test if the argument is equal to the content of this String object.
 *
 * [Parameters]
 *    in | stlString | STL string object to test against.
 *
 * [Return value]
 *    Returns true if the argument is equal to the content of this String,
 *    otherwise it returns false.
 */
inline bool String::operator==(const std::string& stlString) const
{
    return (_stringStorage == stlString);
}

/*!
 * [Short description]
 *    Equality operator.
 *
 * [Detailed description]
 *    Operator to test if the argument is equal to the content of this String object.
 *
 * [Parameters]
 *    in | otherString | String object to test against.
 *
 * [Return value]
 *    Returns true if the argument is equal to the content of this String,
 *    otherwise it returns false.
 */
inline bool String::operator==(const String& otherString) const
{
    return (_stringStorage == otherString._stringStorage);
}

/*!
 * [Short description]
 *    In-equality operator
 *
 * [Detailed description]
 *    Operator to test if the argument is not equal to the content of this String object.
 * \n The behavior of this operator is the inverse of the operator==() operator.
 *
 * [Parameters]
 *    in | pchString | Pointer to null terminated character array to test against.
 *
 * [Return value]
 *    Returns false if the argument is equal to the content of this String,
 *    otherwise it returns true.
 */
inline bool String::operator!=(const char* pchString) const
{
 	if(pchString == nullptr) return true;
   return (!operator==(pchString));
}

/*!
 * [Short description]
 *    In-equality operator.
 *
 * [Detailed description]
 *    Operator to test if the argument is not equal to the content of this String object.
 * \n The behavior of this operator is the inverse of the operator==() operator.
 *
 * [Parameters]
 *    in | stlString | STL string object to test against.
 *
 * [Return value]
 *    Returns false if the argument is equal to the content of this String,
 *    otherwise it returns true.
 */
inline bool String::operator!=(const std::string& stlString) const
{
    return (!operator==(stlString));
}

/*!
 * [Short description]
 *    In-equality operator.
 *
 * [Detailed description]
 *    Operator to test if the argument is not equal to the content of this String object.
 * \n The behavior of this operator is the inverse of the operator==() operator.
 *
 * [Parameters]
 *    in | otherString | String object to test against.
 *
 * [Return value]
 *    Returns false if the argument is equal to the content of this String,
 *    otherwise it returns true.
 */
inline bool String::operator!=(const String& otherString) const
{
    return (!operator==(otherString));
}

/*!
 * [Short description]
 *    Less than operator.
 *
 * [Detailed description]
 *    This String object and the passed string will be compared on a
 *    character by character basis until first differing character.
 *
 * [Parameters]
 *    in | pchString | Pointer to a null terminated character array to compare with.
 *
 * [Return value]
 *    Returns true if the first differing character of this String has
 *    a smaller ASCII value then the corresponding character in the
 *    string it is compared to, otherwise it returns false.
 */
inline bool String::operator<(const char* pchString) const
{
	if(pchString == nullptr) return false;
    return (_stringStorage < pchString);
}

/*!
 * [Short description]
 *    Less than operator.
 *
 * [Detailed description]
 *    This String object and the passed string will be compared on a
 *    character by character basis until first differing character.
 *
 * [Parameters]
 *    in | stlString | STL string object to compare with.
 *
 * [Return value]
 *    Returns true if the first differing character of this String has
 *    a smaller ASCII value then the corresponding character in the
 *    string it is compared to, otherwise it returns false.
 */
inline bool String::operator<(const std::string& stlString) const
{
    return (_stringStorage < stlString);
}

/*!
 * [Short description]
 *    Less than operator.
 *
 * [Detailed description]
 *    This String object and the passed string will be compared on a
 *    character by character basis until first differing character.
 *
 * [Parameters]
 *    in | otherString | String object to compare with.
 *
 * [Return value]
 *    Returns true if the first differing character of this String has
 *    a smaller ASCII value then the corresponding character in the
 *    string it is compared to, otherwise it returns false.
 */
inline bool String::operator<(const String& otherString) const
{
    return (_stringStorage < otherString._stringStorage);
}

/*!
 * [Short description]
 *    Less than or equal to operator.
 *
 * [Detailed description]
 *    This String object and the passed string will be compared on a
 *    character by character basis until first differing character.
 *
 * [Parameters]
 *    in | pchString | Pointer to a null terminated character array to compare with.
 *
 * [Return value]
 *    Returns true if the first differing character of this String has
 *    a smaller ASCII value then the corresponding character in the
 *    string it is compared to or if the strings are equal to each other,
 *    otherwise it returns false.
 */
inline bool String::operator<=(const char* pchString) const
{
	if(pchString == nullptr) return false;
    return (_stringStorage <= pchString);
}

/*!
 * [Short description]
 *    Less than or equal to operator.
 *
 * [Detailed description]
 *    This String object and the passed string will be compared on a
 *    character by character basis until first differing character.
 *
 * [Parameters]
 *    in | stlString | STL string object to compare with.
 *
 * [Return value]
 *    Returns true if the first differing character of this String has
 *    a smaller ASCII value then the corresponding character in the
 *    string it is compared to or if the strings are equal to each other,
 *    otherwise it returns false.
 */
inline bool String::operator<=(const std::string& stlString) const
{
    return (_stringStorage <= stlString);
}

/*!
 * [Short description]
 *    Less than or equal to operator.
 *
 * [Detailed description]
 *    This String object and the passed string will be compared on a
 *    character by character basis until first differing character.
 *
 * [Parameters]
 *    in | otherString | String object to compare with.
 *
 * [Return value]
 *    Returns true if the first differing character of this String has
 *    a smaller ASCII value then the corresponding character in the
 *    string it is compared to or if the strings are equal to each other,
 *    otherwise it returns false.
 */
inline bool String::operator<=(const String& otherString) const
{
    return (_stringStorage <= otherString._stringStorage);
}

/*!
 * [Short description]
 *    Greater than operator.
 *
 * [Detailed description]
 *    This String object and the passed string will be compared on a
 *    character by character basis until first differing character.
 *
 * [Parameters]
 *    in | pchString | Pointer to a null terminated character array to compare with.
 *
 * [Return value]
 *    Returns true if the first differing character of this String has
 *    a larger ASCII value then the corresponding character in the
 *    string it is compared to, otherwise it returns false.
 */
inline bool String::operator>(const char* pchString) const
{
	if(pchString == nullptr) return true;
    return (_stringStorage > pchString);
}

/*!
 * [Short description]
 *    Greater than operator.
 *
 * [Detailed description]
 *    This String object and the passed string will be compared on a
 *    character by character basis until first differing character.
 *
 * [Parameters]
 *    in | stlString | STL string object to compare with.
 *
 * [Return value]
 *    Returns true if the first differing character of this String has
 *    a larger ASCII value then the corresponding character in the
 *    string it is compared to, otherwise it returns false.
 */
inline bool String::operator>(const std::string& stlString) const
{
    return (_stringStorage > stlString);
}

/*!
 * [Short description]
 *    Greater than operator.
 *
 * [Detailed description]
 *    This String object and the passed string will be compared on a
 *    character by character basis until first differing character.
 *
 * [Parameters]
 *    in | otherString | String object to compare with.
 *
 * [Return value]
 *    Returns true if the first differing character of this String has
 *    a larger ASCII value then the corresponding character in the
 *    string it is compared to, otherwise it returns false.
 */
inline bool String::operator>(const String& otherString) const
{
    return (_stringStorage > otherString._stringStorage);
}

/*!
 * [Short description]
 *    Greater than or equal to operator.
 *
 * [Detailed description]
 *    This String object and the passed string will be compared on a
 *    character by character basis until first differing character.
 *
 * [Parameters]
 *    in | pchString | Pointer to a null terminated character array to compare with.
 *
 * [Return value]
 *    Returns true if the first differing character of this String has
 *    a larger ASCII value then the corresponding character in the
 *    string it is compared to or if the strings are equal to each other,
 *    otherwise it returns false.
 */
inline bool String::operator>=(const char* pchString) const
{
	if(pchString == nullptr) return true;
    return (_stringStorage >= pchString);
}

/*!
 * [Short description]
 *    Greater than or equal to operator.
 *
 * [Detailed description]
 *    This String object and the passed string will be compared on a
 *    character by character basis until first differing character.
 *
 * [Parameters]
 *    in | stlString | STL string object to compare with.
 *
 * [Return value]
 *    Returns true if the first differing character of this String has
 *    a larger ASCII value then the corresponding character in the
 *    string it is compared to or if the strings are equal to each other,
 *    otherwise it returns false.
 */
inline bool String::operator>=(const std::string& stlString) const
{
    return (_stringStorage >= stlString);
}

/*!
 * [Short description]
 *    Greater than or equal to operator.
 *
 * [Detailed description]
 *    This String object and the passed string will be compared on a
 *    character by character basis until first differing character.
 *
 * [Parameters]
 *    in | otherString | String object to compare with.
 *
 * [Return value]
 *    Returns true if the first differing character of this String has
 *    a larger ASCII value then the corresponding character in the
 *    string it is compared to or if the strings are equal to each other,
 *    otherwise it returns false.
 */
inline bool String::operator>=(const String& otherString) const
{
    return (_stringStorage >= otherString._stringStorage);
}

/*!
 * [Short description]
 *    Assign operator.
 *
 * [Parameters]
 *    in | pchString | Pointer to a null terminated character array to assign.
 *
 * [Return value]
 *    Returns a reference to this string object after assignment.
 */
inline String& String::operator=(const char* pchString)
{
    _stringStorage = pchString ? pchString : "";
    return (*this);
}

/*!
 * [Short description]
 *    Assign operator.
 *
 * [Parameters]
 *    in | stlString | STL string object to assign.
 *
 * [Return value]
 *    Returns a reference to this string object after assignment.
 */
inline String& String::operator=(const std::string& stlString)
{
    _stringStorage = stlString;
    return (*this);
}

/*!
 * [Short description]
 *    Assign operator.
 *
 * [Parameters]
 *    in | otherString | String object to assign.
 *
 * [Return value]
 *    Returns a reference to this string object after assignment.
 */
inline String& String::operator=(const String& otherString)
{
    if (&otherString != this)
    {
        _stringStorage = otherString._stringStorage;
    }
    return (*this);
}

/*!
 * [Short description]
 *    Add operator.
 *
 * [Detailed description]
 *    Append a string to the end of the string in this String object.
 *    This operation will not change the content of this object.
 *
 * [Parameters]
 *    in | pchString | Pointer to a null terminated character array to add to this String.
 *
 * [Return value]
 *    Return a String object containing the result of the add operation.
 */
inline String String::operator+(const char* pchString) const
{
	String result(*this);
	if(pchString != nullptr) result += pchString;
    return (result);
}

/*!
 * [Short description]
 *    Add operator.
 *
 * [Detailed description]
 *    Append a string to the end of the string in this String object.
 *    This operation will not change the content of this object.
 *
 * [Parameters]
 *    in | stlString | STL string object to assign.
 *
 * [Return value]
 *    Return a String object containing the result of the add operation.
 */
inline String String::operator+(const std::string& stlString) const
{
    String result(*this);
    result += stlString;
    return (result);
}

/*!
 * [Short description]
 *    Add operator.
 *
 * [Detailed description]
 *    Append a string to the end of the string in this String object.
 *    This operation will not change the content of this object.
 *
 * [Parameters]
 *    in | otherString | String object to assign.
 *
 * [Return value]
 *    Return a String object containing the result of the add operation.
 */
inline String String::operator+(const String& otherString) const
{
    String result(*this);
    result += otherString;
    return (result);
}

/*!
 * [Short description]
 *    Add operator.
 *
 * [Detailed description]
 *    Append the string representation of specified integer value
 *    to the end of the string in this String object.
 *    This operation will not change the content of this object.
 *
 * [Parameters]
 *    in | value | Integer value to add to the string.
 *
 * [Return value]
 *    Return a String object containing the result of the add operation.
 */
inline String String::operator+(int value) const
{
    String result(*this);
    result += value;
    return (result);
}

/*!
 * [Short description]
 *    Add operator.
 *
 * [Detailed description]
 *    Append the string representation of specified integer value
 *    to the end of the string in this String object.
 *    This operation will not change the content of this object.
 *
 * [Parameters]
 *    in | value | Integer value to add to the string.
 *
 * [Return value]
 *    Return a String object containing the result of the add operation.
 */
inline String String::operator+(unsigned int value) const
{
    String result(*this);
    result += value;
    return (result);
}

/*!
 * [Short description]
 *    Add operator.
 *
 * [Detailed description]
 *    Append the string representation of specified long integer value
 *    to the end of the string in this String object.
 *    This operation will not change the content of this object.
 *
 * [Parameters]
 *    in | value | Long integer value to add to the string.
 *
 * [Return value]
 *    Return a String object containing the result of the add operation.
 */
inline String String::operator+(long value) const
{
    String result(*this);
    result += value;
    return (result);
}

/*!
 * [Short description]
 *    Add operator.
 *
 * [Detailed description]
 *    Append the string representation of specified long integer value
 *    to the end of the string in this String object.
 *    This operation will not change the content of this object.
 *
 * [Parameters]
 *    in | value | Long integer value to add to the string.
 *
 * [Return value]
 *    Return a String object containing the result of the add operation.
 */
inline String String::operator+(unsigned long value) const
{
    String result(*this);
    result += value;
    return (result);
}

/*!
 * [Short description]
 *    Add operator.
 *
 * [Detailed description]
 *    Append the string representation of specified single precision
 *    floating point value to the end of the string in this String object.
 *    This operation will not change the content of this object.
 *
 * [Parameters]
 *    in | value | Single precision floating point value to add to the string.
 *
 * [Return value]
 *    Return a String object containing the result of the add operation.
 */
inline String String::operator+(float value) const
{
    String result(*this);
    result += value;
    return (result);
}

/*!
 * [Short description]
 *    Add operator.
 *
 * [Detailed description]
 *    Append the string representation of specified double precision
 *    floating point value to the end of the string in this String object.
 *    This operation will not change the content of this object.
 *
 * [Parameters]
 *    in | value | Double precision floating point value to add to the string.
 *
 * [Return value]
 *    Return a String object containing the result of the add operation.
 */
inline String String::operator+(double value) const
{
    String result(*this);
    result += value;
    return (result);
}

/*!
 * [Short description]
 *    Add operator.
 *
 * [Detailed description]
 *    Append the specified character to the end of the string in this String object.
 *    This operation will not change the content of this object.
 *
 * [Parameters]
 *    in | value | Character to add to the string.
 *
 * [Return value]
 *    Return a String object containing the result of the add operation.
 */
inline String String::operator+(char value) const
{
    String result(*this);
    result += value;
    return (result);
}

/*!
 * [Short description]
 *    Add operator.
 *
 * [Detailed description]
 *    Append the string representation of specified boolean value
 *    to the end of the string in this String object.
 *    This operation will not change the content of this object.
 *
 * [Parameters]
 *    in | value | Boolean value to add to the string.
 *
 * [Return value]
 *    Return a String object containing the result of the add operation.
 */
inline String String::operator+(bool value) const
{
    String result(*this);
    result += value;
    return (result);
}

/*!
 * [Short description]
 *    Append operator.
 *
 * [Detailed description]
 *    Append the string representation of specified boolean value
 *    to the end of the string in this String object.
 *    This operation will change the content of this object.
 *
 * [Parameters]
 *    in | pchString | Pointer to null terminated character array to append to this String.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the append operation.
 */
inline String& String::operator+=(const char* pchString)
{
    if(pchString != nullptr) _stringStorage += pchString;
    return (*this);
}

/*!
 * [Short description]
 *    Append operator.
 *
 * [Detailed description]
 *    Append the specified string to the end of the string in this String object.
 *    This operation will change the content of this object.
 *
 * [Parameters]
 *    in | stlString | STL string object to append to this String.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the append operation.
 */
inline String& String::operator+=(const std::string& stlString)
{
    _stringStorage += stlString;
    return (*this);
}

/*!
 * [Short description]
 *    Append operator.
 *
 * [Detailed description]
 *    Append the specified string to the end of the string in this String object.
 *    This operation will change the content of this object.
 *
 * [Parameters]
 *    in | otherString | String object to append to this String.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the append operation.
 */
inline String& String::operator+=(const String& otherString)
{
    _stringStorage += otherString._stringStorage;
    return (*this);
}

/*!
 * [Short description]
 *    Append operator.
 *
 * [Detailed description]
 *    Append the specified character to the end of the string in this String object.
 *    This operation will change the content of this object.
 *
 * [Parameters]
 *    in | value | Character to append to this String.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the append operation.
 */
inline String& String::operator+=(char value)
{
    _stringStorage += value;
    return (*this);
}

/*!
 * [Short description]
 *    Append operator.
 *
 * [Detailed description]
 *    Append the string representation specified boolean value
 *    to the end of the string in this String object.
 *    This operation will change the content of this object.
 *
 * [Parameters]
 *    in | value | Boolean value to append to this String.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the append operation.
 */
inline String& String::operator+=(bool booleanValue)
{
    _stringStorage += (booleanValue) ? "true" : "false";
    return (*this);
}

/*!
 * [Short description]
 *    Subscript operator.
 *
 * [Parameters]
 *    in | index | Index of the character to extract from the String object.
 *
 * [Return value]
 *    Return a reference to specified character in the String object.
 */
inline char& String::operator[](unsigned int index)
{
    assert(index < _stringStorage.size());
    return _stringStorage[index];
}

/*!
 * [Short description]
 *    Subscript operator.
 *
 * [Parameters]
 *    in | index | Index of the character to extract from the String object.
 *
 * [Return value]
 *    Return a constant reference to specified character in the String object.
 */
inline const char& String::operator[](unsigned int index) const
{
    assert(index < _stringStorage.size());
    return _stringStorage[index];
}

/*!
 * [Short description]
 *    Cast operator for conversion to const char* type.
 *
 * [Return value]
 *    Return a constant character pointer to the string represented by this String object.
 */
inline String::operator const char*() const
{
    return _stringStorage.c_str();
}

/*!
 * [Short description]
 *    Append to this String object.
 *
 * [Detailed description]
 *    Append the specified string to the end of the string in this String object.
 *    This operation will change the content of this object.
 *
 * [Parameters]
 *    in | pchString | Pointer to null terminated character array to append to this String.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the append operation.
 */
inline String& String::Append(const char* pchString)
{
    return (operator+=(pchString));
}

/*!
 * [Short description]
 *    Append to this String object.
 *
 * [Detailed description]
 *    Append the specified string to the end of the string in this String object.
 *    This operation will change the content of this object.
 *
 * [Parameters]
 *    in | stlString | STL string object to append to this String.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the append operation.
 */
inline String& String::Append(const std::string& stlString)
{
    return (operator+=(stlString));
}

/*!
 * [Short description]
 *    Append to this String object.
 *
 * [Detailed description]
 *    Append the specified string to the end of the string in this String object.
 *    This operation will change the content of this object.
 *
 * [Parameters]
 *    in | otherString | String object to append to this String.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the append operation.
 */
inline String& String::Append(const String& otherString)
{
    return (operator+=(otherString));
}

/*!
 * [Short description]
 *    Append to this String object.
 *
 * [Detailed description]
 *    Append the string representation of the specified integer value
 *    to the end of the string in this String object.
 *    This operation will change the content of this object.
 *
 * [Parameters]
 *    in | value | Integer value to append to this String.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the append operation.
 */
inline String& String::Append(int value)
{
    return (operator+=(value));
}

/*!
 * [Short description]
 *    Append to this String object.
 *
 * [Detailed description]
 *    Append the string representation of the specified integer value
 *    to the end of the string in this String object.
 *    This operation will change the content of this object.
 *
 * [Parameters]
 *    in | value | Integer value to append to this String.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the append operation.
 */
inline String& String::Append(unsigned int value)
{
    return (operator+=(value));
}

/*!
 * [Short description]
 *    Append to this String object.
 *
 * [Detailed description]
 *    Append the string representation of the specified long integer value
 *    to the end of the string in this String object.
 *    This operation will change the content of this object.
 *
 * [Parameters]
 *    in | value | Long integer value to append to this String.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the append operation.
 */
inline String& String::Append(long value)
{
    return (operator+=(value));
}

/*!
 * [Short description]
 *    Append to this String object.
 *
 * [Detailed description]
 *    Append the string representation of the specified long integer value
 *    to the end of the string in this String object.
 *    This operation will change the content of this object.
 *
 * [Parameters]
 *    in | value | Long integer value to append to this String.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the append operation.
 */
inline String& String::Append(unsigned long value)
{
    return (operator+=(value));
}

/*!
 * [Short description]
 *    Append to this String object.
 *
 * [Detailed description]
 *    Append the string representation of the specified single precision
 *    floating point value to the end of the string in this String object.
 *    This operation will change the content of this object.
 *
 * [Parameters]
 *    in | value | Single precision floating point value to append to this String.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the append operation.
 */
inline String& String::Append(float value)
{
    return (operator+=(value));
}

/*!
 * [Short description]
 *    Append to this String object.
 *
 * [Detailed description]
 *    Append the string representation of the specified double precision
 *    floating point value to the end of the string in this String object.
 *    This operation will change the content of this object.
 *
 * [Parameters]
 *    in | value | Double precision floating point value to append to this String.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the append operation.
 */
inline String& String::Append(double value)
{
    return (operator+=(value));
}

/*!
 * [Short description]
 *    Append to this String object.
 *
 * [Detailed description]
 *    Append the specified character to the end of the string in this String object.
 *    This operation will change the content of this object.
 *
 * [Parameters]
 *    in | value | Character to append to this String.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the append operation.
 */
inline String& String::Append(char value)
{
    return (operator+=(value));
}

/*!
 * [Short description]
 *    Append to this String object.
 *
 * [Detailed description]
 *    Append the string representation of the specified boolean value
 *    to the end of the string in this String object.
 *    This operation will change the content of this object.
 *
 * [Parameters]
 *    in | value | Boolean value to append to this String.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the append operation.
 */
inline String& String::Append(bool value)
{
    return (operator+=(value));
}

/*!
 * [Short description]
 *    Insert a character at a specified position in the string.
 *
 * [Detailed description]
 *    Insert the specified character at the specified position in the string.
 *
 * [Parameters]
 *    in | value | Character to insert.
 *    in | pos   | Position in the string to insert the character.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the insert operation.
 */
inline String& String::Insert(char value, unsigned int pos)
{
    _stringStorage.insert(pos, 1, value);
    return *this;
}

/*!
 * [Short description]
 *    Insert a character at the position in the string indicated by an iterator.
 *
 * [Detailed description]
 *    Insert the specified character at the specified position in the string
 *    represented by this String object.
 *
 * [Parameters]
 *    in | value | Character to insert.
 *    in | it    | Position in the string to insert the character.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the insert operation.
 */
inline String& String::Insert(char value, iterator it)
{
    _stringStorage.insert(it, 1, value);
    return *this;
}

/*!
 * [Short description]
 *    Insert a string at the position in the string indicated by an iterator.
 *
 * [Detailed description]
 *    Insert the specified string at the specified position in the string
 *    represented by this String object.
 *
 * [Parameters]
 *    in | value | Pointer to a null terminated character array to insert.
 *    in | pos   | Position in the string to insert the character.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the insert operation.
 */
inline String& String::Insert(const char* value, unsigned int pos)
{
    if(value != nullptr) _stringStorage.insert(pos, value);
    return *this;
}

/*!
 * [Short description]
 *    Insert a string at the position in the string indicated by an iterator.
 *
 * [Detailed description]
 *    Insert the specified string at the specified position in the string
 *    represented by this String object.
 *
 * [Parameters]
 *    in | value | STL string to insert.
 *    in | pos   | Position in the string to insert the character.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the insert operation.
 */
inline String& String::Insert(const std::string& value, unsigned int pos)
{
    _stringStorage.insert(pos, value);
    return *this;
}

/*!
 * [Short description]
 *    Insert a string at the position in the string indicated by an iterator.
 *
 * [Detailed description]
 *    Insert the specified string at the specified position in the string
 *    represented by this String object.
 *
 * [Parameters]
 *    in | value | STL string to insert.
 *    in | it    | Position in the string to insert it character.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the insert operation.
 */
inline String& String::Insert(const std::string& value, iterator it)
{
    return (Insert(value.c_str(), it));
}

/*!
 * [Short description]
 *    Insert a string at the position in the string indicated by an iterator.
 *
 * [Detailed description]
 *    Insert the specified string at the specified position in the string
 *    represented by this String object.
 *
 * [Parameters]
 *    in | value | String to insert.
 *    in | pos   | Position in the string to insert it character.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the insert operation.
 */
inline String& String::Insert(const String& value, unsigned int pos)
{
    _stringStorage.insert(pos, value._stringStorage);
    return *this;
}

/*!
 * [Short description]
 *    Insert a string at the position in the string indicated by an iterator.
 *
 * [Detailed description]
 *    Insert the specified string at the specified position in the string
 *    represented by this String object.
 *
 * [Parameters]
 *    in | value | String to insert.
 *    in | it    | Position in the string to insert it character.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the insert operation.
 */
inline String& String::Insert(const String& value, iterator it)
{
    return (Insert(value.c_str(), it));
}

/*!
 * [Short description]
 *    Convert the string to a different type using lexical interpretation where needed.
 *
 * [Detailed description]
 *    Convert the string held by this String object to another type.
 *    If needed, lexical interpretation will take place to get the string
 *    converted to the correct type of the passed parameter.
 *
 * [Parameters]
 *    out | value | Reference to the object to store the result in.
 *
 * [Return value]
 *    Returns true if the conversion was successful, otherwise returns false.
 */
inline bool String::ConvertTo(String& value) const
{
    value._stringStorage = _stringStorage;
    return true;
}

/*!
 * [Short description]
 *    Convert the string to a different type using lexical interpretation where needed.
 *
 * [Detailed description]
 *    Convert the string held by this String object to another type.
 *    If needed, lexical interpretation will take place to get the string
 *    converted to the correct type of the passed parameter.
 *
 * [Parameters]
 *    out | value | Reference to the object to store the result in.
 *
 * [Return value]
 *    Returns true if the conversion was successful, otherwise returns false.
 */
inline bool String::ConvertTo(std::string& value) const
{
    value = _stringStorage;
    return true;
}

/*!
 * [Short description]
 *    Convert the string to a different type using lexical interpretation where needed.
 *
 * [Detailed description]
 *    Convert the string held by this String object to another type.
 *    If needed, lexical interpretation will take place to get the string
 *    converted to the correct type of the passed parameter.
 *
 * [Parameters]
 *    out | value | Reference to the object to store the result in.
 *
 * [Return value]
 *    Returns true if the conversion was successful, otherwise returns false.
 */
inline bool String::ConvertTo(unsigned long& value) const
{
    long signedValue = 0;
    if (ConvertTo(signedValue) && signedValue >= 0)
    {
        value = static_cast<unsigned long>(signedValue);
        return true;
    }
    return false;
}

/*!
 * [Short description]
 *    Convert the string to a different type using lexical interpretation where needed.
 *
 * [Detailed description]
 *    Convert the string held by this String object to another type.
 *    If needed, lexical interpretation will take place to get the string
 *    converted to the correct type of the passed parameter.
 *
 * [Parameters]
 *    out | value | Reference to the object to store the result in.
 *
 * [Return value]
 *    Returns true if the conversion was successful, otherwise returns false.
 */
inline bool String::ConvertTo(unsigned int& value) const
{
    int signedValue = 0;
    if (ConvertTo(signedValue) && signedValue >= 0)
    {
        value = static_cast<unsigned int>(signedValue);
        return true;
    }
    return false;
}

/*!
 * [Short description]
 *    Change all characters in the string to upper case.
 *
 * [Detailed description]
 *    Change all characters in the string to upper case.
 *    The result is stored in the string object itself.
 */
inline void String::ToUpper()
{
	std::transform(_stringStorage.begin(), _stringStorage.end(), _stringStorage.begin(), ::toupper);
}

/*!
 * [Short description]
 *    Change all characters in the string to upper case.
 *
 * [Detailed description]
 *    Change all characters in the string to upper case.
 *    The string in the String object itself is not affected.
 *
 * [Return value]
 *    Returns a new String object containing the converted string.
 */
inline String String::ToUpper() const
{
    String result(_stringStorage);
	std::transform(result._stringStorage.begin(), result._stringStorage.end(), result._stringStorage.begin(), ::toupper);
	return result;
}

/*!
 * [Short description]
 *    Change all characters in the specified string to upper case.
 *
 * [Parameters]
 *    in | input | String to convert to lower case.
 *
 * [Return value]
 *    Returns a new String object containing the converted string.
 */
inline String String::ToUpper(const String& input)
{
    String result(input);
	std::transform(result._stringStorage.begin(), result._stringStorage.end(), result._stringStorage.begin(), ::toupper);
	return result;
}

/*!
 * [Short description]
 *    Change all characters in the string to lower case.
 *
 * [Detailed description]
 *    Change all characters in the string to lower case.
 *    The result is stored in the string object itself.
 */
inline void String::ToLower()
{
	std::transform(_stringStorage.begin(), _stringStorage.end(), _stringStorage.begin(), ::tolower);
}

/*!
 * [Short description]
 *    Change all characters in the string to lower case.
 *
 * [Detailed description]
 *    Change all characters in the string to lower case.
 *    The string in the String object itself is not affected.
 *
 * [Return value]
 *    Returns a new String object containing the converted string.
 */
inline String String::ToLower() const
{
    String result(_stringStorage);
	std::transform(result._stringStorage.begin(), result._stringStorage.end(), result._stringStorage.begin(), ::tolower);
    return result;
}

/*!
 * [Short description]
 *    Change all characters in the specified string to lower case.
 *
 * [Parameters]
 *    in | input | String to convert to lower case.
 *
 * [Return value]
 *    Returns a new String object containing the converted string.
 */
inline String String::ToLower(const String& input)
{
    String result(input);
	std::transform(result._stringStorage.begin(), result._stringStorage.end(), result._stringStorage.begin(), ::tolower);
	return result;
}

/*!
 * [Short description]
 *    Put the characters in the string in reverse order.
 *
 * [Detailed description]
 *    Put the characters in the string in reverse order.
 *    The result is stored in the string object itself.
 */
inline void String::Reverse()
{
    std::reverse(_stringStorage.begin(), _stringStorage.end());
}

/*!
 * [Short description]
 *    Put the characters in the string in reverse order.
 *
 * [Detailed description]
 *    Put the characters in the string in reverse order.
 *    The string in the String object itself is not affected.
 *
 * [Return value]
 *    Returns a new String object containing the reversed string.
 */
inline String String::Reverse() const
{
    String result(_stringStorage);
    result.Reverse();
    return result;
}

/*!
 * [Short description]
 *    Put the characters in the string in reverse order.
 *
 * [Parameters]
 *    in | input | String to put in reverse order.
 *
 * [Return value]
 *    Returns a new String object containing the reversed string.
 */
inline String String::Reverse(const String& input)
{
    String result(input);
    result.Reverse();
    return result;
}

/*!
 * [Short description]
 *    Convert the first letter of each word in the string to upper case
 *    and the reset to lower case.
 *
 * [Detailed description]
 *    Convert the first letter of each word in the string to upper case
 *    and the reset to lower case.
 * \n Example:
 * \n    "THIS STRING is capitalized!"
 * \n becomes:
 * \n    "This String Is Capitalized!"
 *    The string in the String object itself is not affected.
 *
 * [Return value]
 *    Returns a new String object containing the capitalized string.
 */
inline String String::Capitalize() const
{
    String result(_stringStorage);
    result.Capitalize();
    return result;
}

/*!
 * [Short description]
 *    Convert the first letter of each word in the string to upper case
 *    and the reset to lower case.
 *
 * [Detailed description]
 *    Convert the first letter of each word in the string to upper case
 *    and the reset to lower case.
 * \n Example:
 * \n    "THIS STRING is capitalized!"
 * \n becomes:
 * \n    "This String Is Capitalized!"
 *
 * [Parameters]
 *    in | input | String to capitalize.
 *
 * [Return value]
 *    Returns a new String object containing the capitalized string.
 */
inline String String::Capitalize(const String& input)
{
    String result(input);
    result.Capitalize();
    return result;
}

/*!
 * [Short description]
 *    Trim leading and/or trailing white spaces from the string.
 *
 * [Detailed description]
 *    Trim leading and/or trailing white spaces from the string.
 *    The string in the String object itself is not affected.
 *
 * [Parameters]
 *    in | leading  | Indicates whether to trim leading white spaces.
 *    in | trailing | Indicates whether to trim trailing white spaces.
 *
 * [Return value]
 *    Returns a new String object containing the trimmed string.
 */
inline String String::Trim(bool leading, bool trailing) const
{
    String result(_stringStorage);
    result.Trim(leading, trailing);
    return result;
}

/*!
 * [Short description]
 *    Trim leading and/or trailing white spaces from the string.
 *
 * [Detailed description]
 *    Trim leading and/or trailing white spaces from the string.
 *    The string in the String object itself is not affected.
 *
 * [Parameters]
 *    in | input    | String to trim.
 *    in | leading  | Indicates whether to trim leading white spaces.
 *    in | trailing | Indicates whether to trim trailing white spaces.
 *
 * [Return value]
 *    Returns a new String object containing the trimmed string.
 */
inline String String::Trim(const String& input, bool leading, bool trailing)
{
    String result(input);
    result.Trim(leading, trailing);
    return result;
}

/*!
 * [Short description]
 *    Check whether the string contains the specified character.
 *
 * [Detailed description]
 *    Check whether the string contains the specified character starting
 *    from the indicated position.
 *
 * [Parameters]
 *    in | ch    | Character to check for.
 *    in | start | Position in the string to start the check from.
 *
 * [Return value]
 *    Returns true if the character is available in the string, otherwise
 *    it returns false.
 */
inline bool String::Contains(char ch, unsigned int start) const
{
    return (_stringStorage.find(ch, start) != std::string::npos);
}

/*!
 * [Short description]
 *    Check whether the string contains the specified string.
 *
 * [Detailed description]
 *    Check whether the string contains the specified string starting
 *    from the indicated position.
 *
 * [Parameters]
 *    in | pchString | Pointer to a null terminated character array to check for.
 *    in | start     | Position in the string to start the check from.
 *
 * [Return value]
 *    Returns true if the string is available in the string, otherwise
 *    it returns false.
 */
inline bool String::Contains(const char* pchString, unsigned int start) const
{
	if(pchString == nullptr) return false;
    return (_stringStorage.find(pchString, start) != std::string::npos);
}

/*!
 * [Short description]
 *    Check whether the string contains the specified string.
 *
 * [Detailed description]
 *    Check whether the string contains the specified string starting
 *    from the indicated position.
 *
 * [Parameters]
 *    in | stlString | STL string to check for.
 *    in | start     | Position in the string to start the check from.
 *
 * [Return value]
 *    Returns true if the string is available in the string, otherwise
 *    it returns false.
 */
inline bool String::Contains(const std::string& stlString, unsigned int start) const
{
    return (_stringStorage.find(stlString, start) != std::string::npos);
}

/*!
 * [Short description]
 *    Check whether the string contains the specified string.
 *
 * [Detailed description]
 *    Check whether the string contains the specified string starting
 *    from the indicated position.
 *
 * [Parameters]
 *    in | otherString | String to check for.
 *    in | start       | Position in the string to start the check from.
 *
 * [Return value]
 *    Returns true if the string is available in the string, otherwise
 *    it returns false.
 */
inline bool String::Contains(const String& otherString, unsigned int start) const
{
    return (_stringStorage.find(otherString._stringStorage, start) != std::string::npos);
}

/*!
 * [Short description]
 *    Find the position of the first occurrence of the specified character.
 *
 * [Detailed description]
 *    Find the position of the first occurrence of the specified character starting
 *    from the indicated position.
 *
 * [Parameters]
 *    in | ch    | Character to find.
 *    in | start | Position in the string to start the find.
 *
 * [Return value]
 *    Returns index of the found character in the string, if found.
 *    Returns -1 if not found.
 */
inline int String::Find(char ch, unsigned int start) const
{
    std::size_t foundPos = _stringStorage.find(ch, start);
    return ((foundPos == std::string::npos) ? -1 : static_cast<int>(foundPos));
}

/*!
 * [Short description]
 *    Find the position of the first occurrence of the specified string.
 *
 * [Detailed description]
 *    Find the position of the first occurrence of the specified string starting
 *    from the indicated position.
 *
 * [Parameters]
 *    in | pchString | Pointer to null terminated character array to find.
 *    in | start     | Position in the string to start the find.
 *
 * [Return value]
 *    Returns index of the found character in the string, if found.
 *    Returns -1 if not found.
 */
inline int String::Find(const char* pchString, unsigned int start) const
{
	if(pchString == nullptr) return -1;
    std::size_t foundPos = _stringStorage.find(pchString, start);
    return ((foundPos == std::string::npos) ? -1 : static_cast<int>(foundPos));
}

/*!
 * [Short description]
 *    Find the position of the first occurrence of the specified string.
 *
 * [Detailed description]
 *    Find the position of the first occurrence of the specified string starting
 *    from the indicated position.
 *
 * [Parameters]
 *    in | stlString | STL string to find.
 *    in | start     | Position in the string to start the find.
 *
 * [Return value]
 *    Returns index of the found character in the string, if found.
 *    Returns -1 if not found.
 */
inline int String::Find(const std::string& stlString, unsigned int start) const
{
    std::size_t foundPos = _stringStorage.find(stlString, start);
    return ((foundPos == std::string::npos) ? -1 : static_cast<int>(foundPos));
}

/*!
 * [Short description]
 *    Find the position of the first occurrence of the specified string.
 *
 * [Detailed description]
 *    Find the position of the first occurrence of the specified string starting
 *    from the indicated position.
 *
 * [Parameters]
 *    in | otherString | String to find.
 *    in | start       | Position in the string to start the find.
 *
 * [Return value]
 *    Returns index of the found character in the string, if found.
 *    Returns -1 if not found.
 */
inline int String::Find(const String& otherString, unsigned int start) const
{
    std::size_t foundPos = _stringStorage.find(otherString._stringStorage, start);
    return ((foundPos == std::string::npos) ? -1 : static_cast<int>(foundPos));
}

/*!
 * [Short description]
 *    Find the position of the first occurrence of the specified character in
 *    reverse direction (from the end to the begin).
 *
 * [Detailed description]
 *    Find the position of the first occurrence of the specified character in
 *    reverse direction (from the end to the begin). The find action starts at
 *    the indicated position.
 *
 * [Parameters]
 *    in | ch   | Character to find.
 *    in | last | Last position in the string to consider in the find.
 *
 * [Return value]
 *    Returns index of the found character in the string, if found.
 *    Returns -1 if not found.
 */
inline int String::FindLast(char ch, int last) const
{
    std::size_t startPos = last >= 0 ? last : std::string::npos;
    std::size_t foundPos = _stringStorage.rfind(ch, startPos);
    return ((foundPos == std::string::npos) ? -1 : static_cast<int>(foundPos));
}

/*!
 * [Short description]
 *    Find the position of the first occurrence of the specified string in
 *    reverse direction (from the end to the begin).
 *
 * [Detailed description]
 *    Find the position of the first occurrence of the specified string in
 *    reverse direction (from the end to the begin). The find action starts at
 *    the indicated position.
 *
 * [Parameters]
 *    in | pchString | Pointer to null terminated character array to find.
 *    in | last      | Last position in the string to consider in the find.
 *
 * [Return value]
 *    Returns index of the found character in the string, if found.
 *    Returns -1 if not found.
 */
inline int String::FindLast(const char* pchString, int last) const
{
	if(pchString == nullptr) return -1;
    std::size_t startPos = last >= 0 ? last : std::string::npos;
    std::size_t foundPos = _stringStorage.rfind(pchString, startPos);
    return ((foundPos == std::string::npos) ? -1 : static_cast<int>(foundPos));
}

/*!
 * [Short description]
 *    Find the position of the first occurrence of the specified string in
 *    reverse direction (from the end to the begin).
 *
 * [Detailed description]
 *    Find the position of the first occurrence of the specified string in
 *    reverse direction (from the end to the begin). The find action starts at
 *    the indicated position.
 *
 * [Parameters]
 *    in | stlString | STL string to find.
 *    in | last      | Last position in the string to consider in the find.
 *
 * [Return value]
 *    Returns index of the found character in the string, if found.
 *    Returns -1 if not found.
 */
inline int String::FindLast(const std::string& stlString, int last) const
{
    std::size_t startPos = last >= 0 ? last : std::string::npos;
    std::size_t foundPos = _stringStorage.rfind(stlString, startPos);
    return ((foundPos == std::string::npos) ? -1 : static_cast<int>(foundPos));
}

/*!
 * [Short description]
 *    Find the position of the first occurrence of the specified string in
 *    reverse direction (from the end to the begin).
 *
 * [Detailed description]
 *    Find the position of the first occurrence of the specified string in
 *    reverse direction (from the end to the begin). The find action starts at
 *    the indicated position.
 *
 * [Parameters]
 *    in | otherString | String to find.
 *    in | last        | Last position in the string to consider in the find.
 *
 * [Return value]
 *    Returns index of the found character in the string, if found.
 *    Returns -1 if not found.
 */
inline int String::FindLast(const String& otherString, int last) const
{
    std::size_t startPos = last >= 0 ? last : std::string::npos;
    std::size_t foundPos = _stringStorage.rfind(otherString._stringStorage, startPos);
    return ((foundPos == std::string::npos) ? -1 : static_cast<int>(foundPos));
}

/*!
 * [Short description]
 *    Count the number of times the specified string occurs in the string.
 *
 * [Detailed description]
 *    Count the number of times the specified string occurs in the string
 *    starting at the specified position. A string or a part of a string
 *    can only be counted once.
 * \n Example, the string "blabla" only occurs once in the string "blablabla".
 *
 * [Parameters]
 *    in | stlString | STL string to count occurrences for.
 *    in | start     | Position in the string to start counting from.
 *
 * [Return value]
 *    Returns the number of times the specified string occurs in the string.
 */
inline unsigned int String::Count(const std::string& stlString, unsigned int start) const
{
    return Count(stlString.c_str(), start);
}

/*!
 * [Short description]
 *    Count the number of times the specified string occurs in the string.
 *
 * [Detailed description]
 *    Count the number of times the specified string occurs in the string
 *    starting at the specified position. A string or a part of a string
 *    can only be counted once.
 * \n Example, the string "blabla" only occurs once in the string "blablabla".
 *
 * [Parameters]
 *    in | otherString | String to count occurrences for.
 *    in | start       | Position in the string to start counting from.
 *
 * [Return value]
 *    Returns the number of times the specified string occurs in the string.
 */
inline unsigned int String::Count(const String& otherString, unsigned int start) const
{
    return Count(otherString.c_str(), start);
}

/*!
 * [Short description]
 *    Replace a sub-string of the string with a single character.
 *
 * [Parameters]
 *    in | start       | First position of the sub string to replace.
 *    in | len         | Number of characters in the string to replace.
 *    in | replacement | Character to replace the sub-string with.
 */
inline void String::Replace(unsigned int start, unsigned int len, char replacement)
{
    _stringStorage.replace(start, len, 1, replacement);
}

/*!
 * [Short description]
 *    Replace a sub-string of the string with a string.
 *
 * [Parameters]
 *    in | start       | First position of the sub string to replace.
 *    in | len         | Number of characters in the string to replace.
 *    in | replacement | Pointer to null-terminated character array to replace the sub-string with.
 */
inline void String::Replace(unsigned int start, unsigned int len, const char* replacement)
{
    _stringStorage.replace(start, len, replacement ? replacement : "");
}

/*!
 * [Short description]
 *    Replace a sub-string of the string with a string.
 *
 * [Parameters]
 *    in | start       | First position of the sub string to replace.
 *    in | len         | Number of characters in the string to replace.
 *    in | replacement | STL string to replace the sub-string with.
 */
inline void String::Replace(unsigned int start, unsigned int len, const std::string& replacement)
{
    _stringStorage.replace(start, len, replacement);
}

/*!
 * [Short description]
 *    Replace a sub-string of the string with a string.
 *
 * [Parameters]
 *    in | start       | First position of the sub string to replace.
 *    in | len         | Number of characters in the string to replace.
 *    in | replacement | String to replace the sub-string with.
 */
inline void String::Replace(unsigned int start, unsigned int len, const String& replacement)
{
    _stringStorage.replace(start, len, replacement._stringStorage);
}

/*!
 * [Short description]
 *    Replace the first or all occurrences of the specified character with a string.
 *
 * [Parameters]
 *    in | pattern     | Character to replace.
 *    in | replacement | STL string to replace the character with.
 *    in | global      | Indicates whether to replace all occurrences or not (true => all occurrences).
 *
 * [Return value]
 *    Returns the number of occurrences that have been replaced in the string.
 */
inline unsigned int String::Replace(char pattern, const std::string& replacement, bool global)
{
    return Replace(pattern, replacement.c_str(), global);
}

/*!
 * [Short description]
 *    Replace the first or all occurrences of the specified character with a string.
 *
 * [Parameters]
 *    in | pattern     | Character to replace.
 *    in | replacement | String to replace the character with.
 *    in | global      | Indicates whether to replace all occurrences or not (true => all occurrences).
 *
 * [Return value]
 *    Returns the number of occurrences that have been replaced in the string.
 */
inline unsigned int String::Replace(char pattern, const String& replacement, bool global)
{
    return Replace(pattern, replacement.c_str(), global);
}

/*!
 * [Short description]
 *    Replace the first or all occurrences of the specified string with a string.
 *
 * [Parameters]
 *    in | pattern     | Pointer to null-terminated character array to replace.
 *    in | replacement | STL string to replace the pattern with.
 *    in | global      | Indicates whether to replace all occurrences or not (true => all occurrences).
 *
 * [Return value]
 *    Returns the number of occurrences that have been replaced in the string.
 */
inline unsigned int String::Replace(const char* pattern, const std::string& replacement, bool global)
{
	if(pattern == nullptr) return 0;
    return Replace(pattern, replacement.c_str(), global);
}

/*!
 * [Short description]
 *    Replace the first or all occurrences of the specified string with a string.
 *
 * [Parameters]
 *    in | pattern     | Pointer to null-terminated character array to replace.
 *    in | replacement | String to replace the pattern with.
 *    in | global      | Indicates whether to replace all occurrences or not (true => all occurrences).
 *
 * [Return value]
 *    Returns the number of occurrences that have been replaced in the string.
 */
inline unsigned int String::Replace(const char* pattern, const String& replacement, bool global)
{
	if(pattern == nullptr) return 0;
    return Replace(pattern, replacement.c_str(), global);
}

/*!
 * [Short description]
 *    Replace the first or all occurrences of the specified string with a string.
 *
 * [Parameters]
 *    in | pattern     | STL string to replace.
 *    in | replacement | Pointer to null-terminated character array to replace the pattern with.
 *    in | global      | Indicates whether to replace all occurrences or not (true => all occurrences).
 *
 * [Return value]
 *    Returns the number of occurrences that have been replaced in the string.
 */
inline unsigned int String::Replace(const std::string& pattern, const char* replacement, bool global)
{
    return Replace(pattern.c_str(), replacement ? replacement : "", global);
}

/*!
 * [Short description]
 *    Replace the first or all occurrences of the specified string with a string.
 *
 * [Parameters]
 *    in | pattern     | STL string to replace.
 *    in | replacement | STL string to replace the pattern with.
 *    in | global      | Indicates whether to replace all occurrences or not (true => all occurrences).
 *
 * [Return value]
 *    Returns the number of occurrences that have been replaced in the string.
 */
inline unsigned int String::Replace(const std::string& pattern, const std::string& replacement, bool global)
{
    return Replace(pattern.c_str(), replacement.c_str(), global);
}

/*!
 * [Short description]
 *    Replace the first or all occurrences of the specified string with a string.
 *
 * [Parameters]
 *    in | pattern     | STL string  to replace.
 *    in | replacement | String to replace the pattern with.
 *    in | global      | Indicates whether to replace all occurrences or not (true => all occurrences).
 *
 * [Return value]
 *    Returns the number of occurrences that have been replaced in the string.
 */
inline unsigned int String::Replace(const std::string& pattern, const String& replacement, bool global)
{
    return Replace(pattern.c_str(), replacement.c_str(), global);
}

/*!
 * [Short description]
 *    Replace the first or all occurrences of the specified string with a string.
 *
 * [Parameters]
 *    in | pattern     | String to replace.
 *    in | replacement | Pointer to null-terminated character array to replace the pattern with.
 *    in | global      | Indicates whether to replace all occurrences or not (true => all occurrences).
 *
 * [Return value]
 *    Returns the number of occurrences that have been replaced in the string.
 */
inline unsigned int String::Replace(const String& pattern, const char* replacement, bool global)
{
    return Replace(pattern.c_str(), replacement ? replacement : "", global);
}

/*!
 * [Short description]
 *    Replace the first or all occurrences of the specified string with a string.
 *
 * [Parameters]
 *    in | pattern     | String to replace.
 *    in | replacement | STL string to replace the pattern with.
 *    in | global      | Indicates whether to replace all occurrences or not (true => all occurrences).
 *
 * [Return value]
 *    Returns the number of occurrences that have been replaced in the string.
 */
inline unsigned int String::Replace(const String& pattern, const std::string& replacement, bool global)
{
    return Replace(pattern.c_str(), replacement.c_str(), global);
}

/*!
 * [Short description]
 *    Replace the first or all occurrences of the specified string with a string.
 *
 * [Parameters]
 *    in | pattern     | String to replace.
 *    in | replacement | String to replace the pattern with.
 *    in | global      | Indicates whether to replace all occurrences or not (true => all occurrences).
 *
 * [Return value]
 *    Returns the number of occurrences that have been replaced in the string.
 */
inline unsigned int String::Replace(const String& pattern, const String& replacement, bool global)
{
    return Replace(pattern.c_str(), replacement.c_str(), global);
}

/*!
 * [Short description]
 *    Replace a sub-string of the string with a character.
 *
 * [Parameters]
 *    in | start       | Iterator pointing to the first character of the sub string to replace.
 *    in | end         | Iterator pointing to the last character of the sub string to replace.
 *    in | replacement | Character to replace the sub-string with.
 */
inline void String::Replace(iterator start, iterator end, char replacement)
{
    _stringStorage.replace(start, end, 1, replacement);
}

/*!
 * [Short description]
 *    Replace a sub-string of the string with a string.
 *
 * [Parameters]
 *    in | start       | Iterator pointing to the first character of the sub string to replace.
 *    in | end         | Iterator pointing to the last character of the sub string to replace.
 *    in | replacement | Pointer to a null-terminated character array to replace the sub-string with.
 */
inline void String::Replace(iterator start, iterator end, const char* replacement)
{
    _stringStorage.replace(start, end, replacement ? replacement : "");
}

/*!
 * [Short description]
 *    Replace a sub-string of the string with a string.
 *
 * [Parameters]
 *    in | start       | Iterator pointing to the first character of the sub string to replace.
 *    in | end         | Iterator pointing to the last character of the sub string to replace.
 *    in | replacement | STL string to replace the sub-string with.
 */
inline void String::Replace(iterator start, iterator end, const std::string& replacement)
{
    _stringStorage.replace(start, end, replacement);
}

/*!
 * [Short description]
 *    Replace a sub-string of the string with a string.
 *
 * [Parameters]
 *    in | start       | Iterator pointing to the first character of the sub string to replace.
 *    in | end         | Iterator pointing to the last character of the sub string to replace.
 *    in | replacement | String to replace the sub-string with.
 */
inline void String::Replace(iterator start, iterator end, const String& replacement)
{
    _stringStorage.replace(start, end, replacement._stringStorage);
}

/*!
 * [Short description]
 *    Clear the string.
 *
 * [Detailed description]
 *    Clear the string. After this operation the content of the String object is an empty
 *    string with length 0.
 */
inline void String::Clear()
{
    _stringStorage.clear();
}

/*!
 * [Short description]
 *    Erase a sub-string of the string.
 *
 * [Parameters]
 *    in | start | Position of the first character of the sub-string to erase.
 *    in | len   | Length of the sub string to erase.
 */
inline void String::Erase(unsigned int start, unsigned int len)
{
    _stringStorage.erase(start, len);
}

/*!
 * [Short description]
 *    Erase a character from the string.
 *
 * [Parameters]
 *    in | it | Iterator pointing to the character to erase.
 */
inline void String::Erase(iterator it)
{
    _stringStorage.erase(it);
}

/*!
 * [Short description]
 *    Erase a sub-string of the string.
 *
 * [Parameters]
 *    in | start       | Iterator pointing to the first character of the sub string to erase.
 *    in | end         | Iterator pointing to the last character of the sub string to erase.
 */
inline void String::Erase(iterator start, iterator end)
{
    _stringStorage.erase(start, end);
}

/*!
 * [Short description]
 *    Split the string at the specified separator.
 *
 * [Detailed description]
 *    Split the string at the specified separator and store the results in the given list.
 *    The separator will be discarded during the split operation.
 * \n In Java style split, there will be no empty string at the end
 *    of the list if the separator appears at the end of the string.
 *
 * [Parameters]
 *    out | list      | Reference to a vector of STL string objects to store the results in.
 *    in  | separator | STL string pattern to split the string at.
 *    in  | javastyle | Indicates whether java style split should be used.
 *
 * [Return value]
 *    Returns the number of parts stored in the list.
 */
inline unsigned int String::Split(std::vector<std::string>& list, const std::string& separator, bool javastyle) const
{
    return (Split(list, separator.c_str(), javastyle));
}

/*!
 * [Short description]
 *    Split the string at the specified separator.
 *
 * [Detailed description]
 *    Split the string at the specified separator and store the results in the given list.
 *    The separator will be discarded during the split operation.
 * \n In Java style split, there will be no empty string at the end
 *    of the list if the separator appears at the end of the string.
 *
 * [Parameters]
 *    out | list      | Reference to a vector of STL string objects to store the results in.
 *    in  | separator | String pattern to split the string at.
 *    in  | javastyle | Indicates whether java style split should be used.
 *
 * [Return value]
 *    Returns the number of parts stored in the list.
 */
inline unsigned int String::Split(std::vector<std::string>& list, const String& separator, bool javastyle) const
{
    return (Split(list, separator.c_str(), javastyle));
}

/*!
 * [Short description]
 *    Split the string at the specified separator.
 *
 * [Detailed description]
 *    Split the string at the specified separator and store the results in the given list.
 *    The separator will be discarded during the split operation.
 * \n In Java style split, there will be no empty string at the end
 *    of the list if the separator appears at the end of the string.
 *
 * [Parameters]
 *    out | list      | Reference to a vector of String objects to store the results in.
 *    in  | separator | STL string pattern to split the string at.
 *    in  | javastyle | Indicates whether java style split should be used.
 *
 * [Return value]
 *    Returns the number of parts stored in the list.
 */
inline unsigned int String::Split(std::vector<String>& list, const std::string& separator, bool javastyle) const
{
    return (Split(list, separator.c_str(), javastyle));
}

/*!
 * [Short description]
 *    Split the string at the specified separator.
 *
 * [Detailed description]
 *    Split the string at the specified separator and store the results in the given list.
 *    The separator will be discarded during the split operation.
 * \n In Java style split, there will be no empty string at the end
 *    of the list if the separator appears at the end of the string.
 *
 * [Parameters]
 *    out | list      | Reference to a vector of String objects to store the results in.
 *    in  | separator | String pattern to split the string at.
 *    in  | javastyle | Indicates whether java style split should be used.
 *
 * [Return value]
 *    Returns the number of parts stored in the list.
 */
inline unsigned int String::Split(std::vector<String>& list, const String& separator, bool javastyle) const
{
    return (Split(list, separator.c_str(), javastyle));
}

/*!
 * [Short description]
 *    Split the string at the specified separator.
 *
 * [Detailed description]
 *    Split the string at the specified separator and store the results in the given list.
 *    The separator will be discarded during the split operation.
 * \n In Java style split, there will be no empty string at the end
 *    of the list if the separator appears at the end of the string.
 *
 * [Parameters]
 *    out | list      | Array of STL string object to store the results in.
 *    in  | len       | Number of elements that fit in the array.
 *    in  | separator | STL string pattern to split the string at.
 *    in  | javastyle | Indicates whether java style split should be used.
 *
 * [Return value]
 *    Returns the number of parts stored in the list.
 */
inline unsigned int String::Split(std::string list[], unsigned int len, const std::string& separator,
                                  bool javastyle) const
{
    return (Split(list, len, separator.c_str(), javastyle));
}

/*!
 * [Short description]
 *    Split the string at the specified separator.
 *
 * [Detailed description]
 *    Split the string at the specified separator and store the results in the given list.
 *    The separator will be discarded during the split operation.
 * \n In Java style split, there will be no empty string at the end
 *    of the list if the separator appears at the end of the string.
 *
 * [Parameters]
 *    out | list      | Array of STL string object to store the results in.
 *    in  | len       | Number of elements that fit in the array.
 *    in  | separator | String pattern to split the string at.
 *    in  | javastyle | Indicates whether java style split should be used.
 *
 * [Return value]
 *    Returns the number of parts stored in the list.
 */
inline unsigned int String::Split(std::string list[], unsigned int len, const String& separator, bool javastyle) const
{
    return (Split(list, len, separator.c_str(), javastyle));
}

/*!
 * [Short description]
 *    Split the string at the specified separator.
 *
 * [Detailed description]
 *    Split the string at the specified separator and store the results in the given list.
 *    The separator will be discarded during the split operation.
 * \n In Java style split, there will be no empty string at the end
 *    of the list if the separator appears at the end of the string.
 *
 * [Parameters]
 *    out | list      | Array of String object to store the results in.
 *    in  | len       | Number of elements that fit in the array.
 *    in  | separator | STL string pattern to split the string at.
 *    in  | javastyle | Indicates whether java style split should be used.
 *
 * [Return value]
 *    Returns the number of parts stored in the list.
 */
inline unsigned int String::Split(String list[], unsigned int len, const std::string& separator, bool javastyle) const
{
    return (Split(list, len, separator.c_str(), javastyle));
}

/*!
 * [Short description]
 *    Split the string at the specified separator.
 *
 * [Detailed description]
 *    Split the string at the specified separator and store the results in the given list.
 *    The separator will be discarded during the split operation.
 * \n In Java style split, there will be no empty string at the end
 *    of the list if the separator appears at the end of the string.
 *
 * [Parameters]
 *    out | list      | Array of String object to store the results in.
 *    in  | len       | Number of elements that fit in the array.
 *    in  | separator | String pattern to split the string at.
 *    in  | javastyle | Indicates whether java style split should be used.
 *
 * [Return value]
 *    Returns the number of parts stored in the list.
 */
inline unsigned int String::Split(String list[], unsigned int len, const String& separator, bool javastyle) const
{
    return (Split(list, len, separator.c_str(), javastyle));
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the given separator.
 *
 * [Parameters]
 *    in | list      | Vector of STL string object to join.
 *    in | separator | Character to put in between the STL strings.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
inline String String::Join(const std::vector<std::string>& list, char separator)
{
    String result(separator);
    result.Join(list);
    return result;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the given separator.
 *
 * [Parameters]
 *    in | list      | Vector of STL string object to join.
 *    in | separator | Pointer to null-terminated character array to put in between the STL strings.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
inline String String::Join(const std::vector<std::string>& list, const char* separator)
{
    String result(separator ? separator : "");
    result.Join(list);
    return result;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the given separator.
 *
 * [Parameters]
 *    in | list      | Vector of STL string object to join.
 *    in | separator | STL string to put in between the STL strings.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
inline String String::Join(const std::vector<std::string>& list, const std::string& separator)
{
    String result(separator);
    result.Join(list);
    return result;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the given separator.
 *
 * [Parameters]
 *    in | list      | Vector of STL string object to join.
 *    in | separator | String to put in between the STL strings.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
inline String String::Join(const std::vector<std::string>& list, const String& separator)
{
    String result(separator);
    result.Join(list);
    return result;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the given separator.
 *
 * [Parameters]
 *    in | list      | Vector of String object to join.
 *    in | separator | Character to put in between the Strings.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
inline String String::Join(const std::vector<String>& list, char separator)
{
    String result(separator);
    result.Join(list);
    return result;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the given separator.
 *
 * [Parameters]
 *    in | list      | Vector of String object to join.
 *    in | separator | Pointer to null-terminated character array to put in between the Strings.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
inline String String::Join(const std::vector<String>& list, const char* separator)
{
    String result(separator ? separator : "");
    result.Join(list);
    return result;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the given separator.
 *
 * [Parameters]
 *    in | list      | Vector of String object to join.
 *    in | separator | STL string to put in between the Strings.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
inline String String::Join(const std::vector<String>& list, const std::string& separator)
{
    String result(separator);
    result.Join(list);
    return result;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the given separator.
 *
 * [Parameters]
 *    in | list      | Vector of String object to join.
 *    in | separator | String to put in between the Strings.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
inline String String::Join(const std::vector<String>& list, const String& separator)
{
    String result(separator);
    result.Join(list);
    return result;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the given separator.
 *
 * [Parameters]
 *    in | list      | Array of STL string object to join.
 *    in | len       | Number of elements in the array.
 *    in | separator | Character to put in between the STL strings.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
inline String String::Join(const std::string list[], unsigned int len, char separator)
{
    String result(separator);
    result.Join(list, len);
    return result;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the given separator.
 *
 * [Parameters]
 *    in | list      | Array of STL string object to join.
 *    in | len       | Number of elements in the array.
 *    in | separator | Pointer to null-terminated character array to put in between the STL strings.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
inline String String::Join(const std::string list[], unsigned int len, const char* separator)
{
    String result(separator ? separator : "");
    result.Join(list, len);
    return result;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the given separator.
 *
 * [Parameters]
 *    in | list      | Array of STL string object to join.
 *    in | len       | Number of elements in the array.
 *    in | separator | STL string to put in between the STL strings.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
inline String String::Join(const std::string list[], unsigned int len, const std::string& separator)
{
    String result(separator);
    result.Join(list, len);
    return result;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the given separator.
 *
 * [Parameters]
 *    in | list      | Array of STL string object to join.
 *    in | len       | Number of elements in the array.
 *    in | separator | String to put in between the STL strings.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
inline String String::Join(const std::string list[], unsigned int len, const String& separator)
{
    String result(separator);
    result.Join(list, len);
    return result;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the given separator.
 *
 * [Parameters]
 *    in | list      | Array of String object to join.
 *    in | len       | Number of elements in the array.
 *    in | separator | Character to put in between the Strings.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
inline String String::Join(const String list[], unsigned int len, char separator)
{
    String result(separator);
    result.Join(list, len);
    return result;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the given separator.
 *
 * [Parameters]
 *    in | list      | Array of String object to join.
 *    in | len       | Number of elements in the array.
 *    in | separator | Pointer to null-terminated character array to put in between the Strings.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
inline String String::Join(const String list[], unsigned int len, const char* separator)
{
    String result(separator ? separator : "");
    result.Join(list, len);
    return result;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the given separator.
 *
 * [Parameters]
 *    in | list      | Array of String object to join.
 *    in | len       | Number of elements in the array.
 *    in | separator | STL string to put in between the Strings.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
inline String String::Join(const String list[], unsigned int len, const std::string& separator)
{
    String result(separator);
    result.Join(list, len);
    return result;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the given separator.
 *
 * [Parameters]
 *    in | list      | Array of String object to join.
 *    in | len       | Number of elements in the array.
 *    in | separator | String to put in between the Strings.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
inline String String::Join(const String list[], unsigned int len, const String& separator)
{
    String result(separator);
    result.Join(list, len);
    return result;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the given separator.
 *
 * [Parameters]
 *    in | list      | Array of pointers to null-terminated character arrays to join.
 *    in | len       | Number of elements in the array.
 *    in | separator | Character to put in between the strings.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
inline String String::Join(const char* list[], unsigned int len, char separator)
{
    String result(separator);
    result.Join(list, len);
    return result;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the given separator.
 *
 * [Parameters]
 *    in | list      | Array of pointers to null-terminated character arrays to join.
 *    in | len       | Number of elements in the array.
 *    in | separator | Pointer to null-terminated character array to put in between the strings.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
inline String String::Join(const char* list[], unsigned int len, const char* separator)
{
    String result(separator ? separator : "");
    result.Join(list, len);
    return result;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the given separator.
 *
 * [Parameters]
 *    in | list      | Array of pointers to null-terminated character arrays to join.
 *    in | len       | Number of elements in the array.
 *    in | separator | STL string to put in between the strings.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
inline String String::Join(const char* list[], unsigned int len, const std::string& separator)
{
    String result(separator);
    result.Join(list, len);
    return result;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the given separator.
 *
 * [Parameters]
 *    in | list      | Array of pointers to null-terminated character arrays to join.
 *    in | len       | Number of elements in the array.
 *    in | separator | String to put in between the strings.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
inline String String::Join(const char* list[], unsigned int len, const String& separator)
{
    String result(separator);
    result.Join(list, len);
    return result;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the given separator.
 *
 * [Parameters]
 *    in | itBegin   | Iterator to the first element in a vector of STL strings to join.
 *    in | itEnd     | Iterator to the last element in a vector of STL strings to join.
 *    in | separator | Character to put in between the STL strings.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
inline String String::Join(const std::vector<std::string>::const_iterator& itBegin,
                           const std::vector<std::string>::const_iterator& itEnd, char separator)
{
    String result(separator);
    result.Join(itBegin, itEnd);
    return result;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the given separator.
 *
 * [Parameters]
 *    in | itBegin   | Iterator to the first element in a vector of STL strings to join.
 *    in | itEnd     | Iterator to the last element in a vector of STL strings to join.
 *    in | separator | Pointer to null-terminated character array to put in between the STL strings.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
inline String String::Join(const std::vector<std::string>::const_iterator& itBegin,
                           const std::vector<std::string>::const_iterator& itEnd, const char* separator)
{
    String result(separator ? separator : "");
    result.Join(itBegin, itEnd);
    return result;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the given separator.
 *
 * [Parameters]
 *    in | itBegin   | Iterator to the first element in a vector of STL strings to join.
 *    in | itEnd     | Iterator to the last element in a vector of STL strings to join.
 *    in | separator | STL string to put in between the STL strings.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
inline String String::Join(const std::vector<std::string>::const_iterator& itBegin,
                           const std::vector<std::string>::const_iterator& itEnd, const std::string& separator)
{
    String result(separator);
    result.Join(itBegin, itEnd);
    return result;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the given separator.
 *
 * [Parameters]
 *    in | itBegin   | Iterator to the first element in a vector of STL strings to join.
 *    in | itEnd     | Iterator to the last element in a vector of STL strings to join.
 *    in | separator | String to put in between the STL strings.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
inline String String::Join(const std::vector<std::string>::const_iterator& itBegin,
                           const std::vector<std::string>::const_iterator& itEnd, const String& separator)
{
    String result(separator);
    result.Join(itBegin, itEnd);
    return result;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the given separator.
 *
 * [Parameters]
 *    in | itBegin   | Iterator to the first element in a vector of Strings to join.
 *    in | itEnd     | Iterator to the last element in a vector of Strings to join.
 *    in | separator | Character to put in between the Strings.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
inline String String::Join(const std::vector<String>::const_iterator& itBegin,
                           const std::vector<String>::const_iterator& itEnd, char separator)
{
    String result(separator);
    result.Join(itBegin, itEnd);
    return result;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the given separator.
 *
 * [Parameters]
 *    in | itBegin   | Iterator to the first element in a vector of Strings to join.
 *    in | itEnd     | Iterator to the last element in a vector of Strings to join.
 *    in | separator | Pointer to null-terminated character array to put in between the Strings.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
inline String String::Join(const std::vector<String>::const_iterator& itBegin,
                           const std::vector<String>::const_iterator& itEnd, const char* separator)
{
    String result(separator ? separator : "");
    result.Join(itBegin, itEnd);
    return result;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the given separator.
 *
 * [Parameters]
 *    in | itBegin   | Iterator to the first element in a vector of Strings to join.
 *    in | itEnd     | Iterator to the last element in a vector of Strings to join.
 *    in | separator | STL string to put in between the Strings.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
inline String String::Join(const std::vector<String>::const_iterator& itBegin,
                           const std::vector<String>::const_iterator& itEnd, const std::string& separator)
{
    String result(separator);
    result.Join(itBegin, itEnd);
    return result;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the given separator.
 *
 * [Parameters]
 *    in | itBegin   | Iterator to the first element in a vector of Strings to join.
 *    in | itEnd     | Iterator to the last element in a vector of Strings to join.
 *    in | separator | String to put in between the Strings.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
inline String String::Join(const std::vector<String>::const_iterator& itBegin,
                           const std::vector<String>::const_iterator& itEnd, const String& separator)
{
    String result(separator);
    result.Join(itBegin, itEnd);
    return result;
}

/*!
 * [Short description]
 *    Create a sub-string.
 *
 * [Detailed description]
 *    Create a sub-string starting at the provided position in the string to the end of the string.
 *
 * [Parameters]
 *    in | start | Position of the first character in the string in include in the sub-string.
 *
 * [Return value]
 *    Return a String object with the requested sub-string.
 */
inline String String::Substring(unsigned int start) const
{
    String result = _stringStorage.substr(start, std::string::npos);
    return result;
}

/*!
 * [Short description]
 *    Create a sub-string.
 *
 * [Detailed description]
 *    Create a sub-string starting at the provided position with the specified length.
 *
 * [Parameters]
 *    in | start | Position of the first character in the string in include in the sub-string.
 *    in | len   | Number of characters to include in the sub-string.
 *
 * [Return value]
 *    Return a String object with the requested sub-string.
 */
inline String String::Substring(unsigned int start, unsigned int len) const
{
    String result = _stringStorage.substr(start, len);
    return result;
}

/*!
 * [Short description]
 *    Create a sub-string.
 *
 * [Detailed description]
 *    Create a sub-string starting at the provided position in the string to the end of the string.
 *
 * [Parameters]
 *    in | start | Iterator pointing to the first character in the string in include in the sub-string.
 *
 * [Return value]
 *    Return a String object with the requested sub-string.
 */
inline String String::Substring(const_iterator start) const
{
    String result = _stringStorage.substr(start - Begin(), std::string::npos);
    return result;
}

/*!
 * [Short description]
 *    Get the length of the string.
 *
 * [Return value]
 *    Returns the length of the string.
 */
inline unsigned int String::Length() const
{
    return static_cast<unsigned int>(_stringStorage.size());
}

/*!
 * [Short description]
 *    Check whether the String object is empty.
 *
 * [Return value]
 *    Returns true if the String object is empty, otherwise it returns false.
 */
inline bool String::IsEmpty() const
{
    return (_stringStorage.size() == 0);
}

/*!
 * [Short description]
 *    Get a pointer to the null-terminated character array view of the string stored in this String object.
 *
 * [Return value]
 *    Return a pointer to the first character of the null-terminated character array view of the string stored int this String object.
 */
inline const char* String::c_str() const
{
    return _stringStorage.c_str();
}

/*!
 * [Short description]
 *    Get a reference to the STL string view of the string stored in this String object.
 *
 * [Return value]
 *    Return a reference to the STL string view of the string stored int this String object.
 */
inline const std::string& String::stl() const
{
    return _stringStorage;
}

/*!
 * [Short description]
 *    Get a reference to the STL string view of the string stored in this String object.
 *
 * [Return value]
 *    Return a reference to the STL string view of the string stored int this String object.
 */
inline std::string& String::stl()
{
    return _stringStorage;
}

/*!
 * [Short description]
 *    Get the first character of the string.
 *
 * [Return value]
 *    Return a reference to the first character of the string.
 *    
 * [Note]
 *  Calling this function with an empty string can cause the end of string 
 *  character to be removed.
 */
inline char& String::Front()
{
    return *(_stringStorage.begin());
}

/*!
 * [Short description]
 *    Get the first character of the string.
 *
 * [Return value]
 *    Return a reference to the first character of the string.
 */
inline const char& String::Front() const
{
    return *(_stringStorage.begin());
}

/*!
 * [Short description]
 *    Get the last character of the string.
 *
 * [Return value]
 *   If string is empty the end of string reference,
 *   otherwise a reference to the last character of the string.
 *    
 * [Note]
 *  Calling this function with an empty string can cause the end of string 
 *  character to be removed.
 */
inline char& String::Back()
{
	if ( _stringStorage.length() == 0 )
	{
		return *(_stringStorage.end());
	}
	return *(_stringStorage.end() - 1);
}

/*!
 * [Short description]
 *    Get the last character of the string.
 *
 * [Return value]
 *   If string is empty the end of string reference,
 *   otherwise a reference to the last character of the string.
 */
inline const char& String::Back() const
{
	if ( _stringStorage.length() == 0 )
	{
		return *(_stringStorage.end());
	}
	return *(_stringStorage.end() - 1);
}

/*!
 * [Short description]
 *    Get an iterator to the first character of the string.
 *
 * [Return value]
 *    Return an iterator to the first character of the string.
 */
inline String::iterator String::Begin()
{
    return _stringStorage.begin();
}

/*!
 * [Short description]
 *    Get an iterator to the first character of the string.
 *
 * [Return value]
 *    Return an iterator to the first character of the string.
 */
inline String::const_iterator String::Begin() const
{
    return _stringStorage.begin();
}

/*!
 * [Short description]
 *    Get an iterator to the first position after the last character of the string.
 *
 * [Return value]
 *    Return an iterator to the first position after the last character of the string.
 */
inline String::iterator String::End()
{
    return _stringStorage.end();
}

/*!
 * [Short description]
 *    Get an iterator to the first position after the last character of the string.
 *
 * [Return value]
 *    Return an iterator to the first position after the last character of the string.
 */
inline String::const_iterator String::End() const
{
    return _stringStorage.end();
}

/*!
 * [Short description]
 *    Get an iterator to the character at the specified index in the string.
 *
 * [Return value]
 *    Return an iterator to the character at the specified index in the string.
 */
inline String::iterator String::get_iterator(unsigned int pos)
{
    return Begin() + pos;
}

/*!
 * [Short description]
 *    Get an iterator to the character at the specified index in the string.
 *
 * [Return value]
 *    Return an iterator to the character at the specified index in the string.
 */
inline String::const_iterator String::get_iterator(unsigned int pos) const
{
    return Begin() + pos;
}

/*!
 * [Short description]
 *    Stream the given string to the given stream object.
 *
 * [Parameters]
 *    in | s   | Stream object to stream the string to.
 *    in | str | String object to stream to the stream object.
 *
 * [Return value]
 *    Returns a reference to the stream object after the streaming of the string as completed.
 */
inline std::ostream& operator<<(std::ostream& s, const String& str)
{
    s << str.c_str();
    return s;
}

/*!
 * [Short description]
 *    Stream a string from the given stream object.
 *
 * [Parameters]
 *    in  | s   | Stream object to stream the string from.
 *    out | str | String object to store the streamed string in.
 *
 * [Return value]
 *    Returns a reference to the stream object after the streaming of the string as completed.
 */
inline std::istream& operator>>(std::istream& s, String& str)
{
    s >> str.stl();
    return s;
}

/*!
 * [Short description]
 *    Add operator.
 *
 * [Parameters]
 *    in | lhs | Pointer to a null-terminated character array to concatenate a string to.
 *    in | rhs | String object to concatenate to the character array.
 *
 * [Return value]
 *    Returns a String object containing the concatenated string.
 */
inline String operator+(const char* lhs, const String& rhs)
{
    String result(lhs ? lhs : "");
    result += rhs;
    return result;
}

/*!
Returns True when the string begins with the given string.
*/
inline bool String::StartsWith(const char* pchString)
{
	// TODO: This is a lazy implementation. A more efficient implementation starts comparing chars immediately.
	int index = Find(pchString);
	return (index == 0);
}


/*!
Returns True when the string ends with the given string.
*/
inline bool String::EndsWith(const char* pchString)
{
	// TODO: This is a lazy implementation. A more efficient implementation starts comparing chars immediately.
	int index = FindLast(pchString);
	return (index == static_cast<int>(this->Length() - strlen(pchString)));
}

/*!
Returns a String representation of the specified value
*/
inline String String::From(int value)
{
	String str;
	str += value;
	return str;
}

/*!
Returns a String representation of the specified value
*/
inline String String::From(unsigned int value)
{
	String str;
	str += value;
	return str;
}

/*!
Returns a String representation of the specified value
*/
inline String String::From(long value)
{
	String str;
	str += value;
	return str;
}

/*!
Returns a String representation of the specified value
*/
inline String String::From(unsigned long value)
{
	String str;
	str += value;
	return str;
}

/*!
Returns a String representation of the specified value
*/
inline String String::From(float value)
{
	String str;
	str += value;
	return str;
}

/*!
Returns a String representation of the specified value
*/
inline String String::From(double value)
{
	String str;
	str += value;
	return str;
}

/*!
Returns a String representation of the specified value
*/
inline String String::From(bool value)
{
	String str;
	str += value;
	return str;
}
