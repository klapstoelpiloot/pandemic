#include "String.h"
#include "Assert.h"
#include <stdio.h>
#include <sstream>
#include <algorithm>

const unsigned int String::_initialAllocSize = 8;

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
String::String(const wchar_t* pchString)
{
	if(pchString == nullptr) return;
	size_t len = wcslen(pchString);
    if(len < _initialAllocSize)
    {
        _stringStorage.reserve(_initialAllocSize);
    }
	if(len > 0)
	{
		size_t targetsize = len + 1;
		while(true)
		{
			char* mbstr = new char[targetsize];
			#if WIN32
			int result = wcstombs_s(&len, mbstr, targetsize, pchString, _TRUNCATE);
			if(result == ERANGE)
			{
				// Need a bigger target string!
				delete[] mbstr;
				targetsize += (targetsize / 2) + 1;
				continue;
			}
			#else
			size_t result = wcstombs(mbstr, pchString, targetsize);
			if(result < len)
			{
				// Need a bigger target string!
				delete[] mbstr;
				targetsize += (targetsize / 2) + 1;
				continue;
			}
			#endif
			
			ENSURE(result == static_cast<size_t>(0));
			ENSURE(len == targetsize);
			_stringStorage.assign(mbstr);
			delete[] mbstr;
			break;
		}
	}
}

/*!
 * [Short description]
 *    Append operator.
 *
 * [Detailed description]
 *    Append the string representation specified integer value
 *    to the end of the string in this String object.
 *    This operation will change the content of this object.
 *
 * [Parameters]
 *    in | value | Integer value to append to this String.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the append operation.
 */
String& String::operator+=(int integerValue)
{
	std::stringstream ss;
	ss.imbue(std::locale::classic());
	ss << integerValue;
	_stringStorage += ss.str();
    return (*this);
}

/*!
 * [Short description]
 *    Append operator.
 *
 * [Detailed description]
 *    Append the string representation specified integer value
 *    to the end of the string in this String object.
 *    This operation will change the content of this object.
 *
 * [Parameters]
 *    in | value | Integer value to append to this String.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the append operation.
 */
String& String::operator+=(unsigned int integerValue)
{
	std::stringstream ss;
	ss.imbue(std::locale::classic());
	ss << integerValue;
	_stringStorage += ss.str();
	return (*this);
}

/*!
 * [Short description]
 *    Append operator.
 *
 * [Detailed description]
 *    Append the string representation specified long integer value
 *    to the end of the string in this String object.
 *    This operation will change the content of this object.
 *
 * [Parameters]
 *    in | value | Long integer value to append to this String.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the append operation.
 */
String& String::operator+=(long longValue)
{
	std::stringstream ss;
	ss.imbue(std::locale::classic());
	ss << longValue;
	_stringStorage += ss.str();
	return (*this);
}

/*!
 * [Short description]
 *    Append operator.
 *
 * [Detailed description]
 *    Append the string representation specified long integer value
 *    to the end of the string in this String object.
 *    This operation will change the content of this object.
 *
 * [Parameters]
 *    in | value | Long integer value to append to this String.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the append operation.
 */
String& String::operator+=(unsigned long longValue)
{
	std::stringstream ss;
	ss.imbue(std::locale::classic());
	ss << longValue;
	_stringStorage += ss.str();
	return (*this);
}

/*!
 * [Short description]
 *    Append operator.
 *
 * [Detailed description]
 *    Append the string representation specified single precision floating point value
 *    to the end of the string in this String object.
 *    This operation will change the content of this object.
 *
 * [Parameters]
 *    in | value | Single precision floating point value to append to this String.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the append operation.
 */
String& String::operator+=(float floatValue)
{
/*
    char buf[64];
#ifdef WIN32
	_snprintf_s(buf, _countof(buf), 64, "%12g", floatValue);
#else
	snprintf(buf, 64, "%12g", floatValue);
#endif
    _stringStorage += buf;
*/
	std::stringstream ss;
	ss.imbue(std::locale::classic());
	ss << floatValue;
	_stringStorage += ss.str();
	return (*this);
}

/*!
 * [Short description]
 *    Append operator.
 *
 * [Detailed description]
 *    Append the string representation double precision floating point value
 *    to the end of the string in this String object.
 *    This operation will change the content of this object.
 *
 * [Parameters]
 *    in | value | Double precision floating point value to append to this String.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the append operation.
 */
String& String::operator+=(double doubleValue)
{
/*
    char buf[64];
#ifdef WIN32
    _snprintf_s(buf, _countof(buf), 64, "%12g", doubleValue);
#else
    snprintf(buf, 64, "%12g", doubleValue);
#endif
   _stringStorage += buf;
*/
	std::stringstream ss;
	ss.imbue(std::locale::classic());
	ss << doubleValue;
	_stringStorage += ss.str();
	return (*this);
}

/*!
 * [Short description]
 *    Prepend to this String object.
 *
 * [Detailed description]
 *    Insert the specified string at the start of the string in this String object.
 *    This operation will change the content of this object.
 *
 * [Parameters]
 *    in | pchString | Pointer to null terminated character array to prepend to this String.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the prepend operation.
 */
String& String::Prepend(const char* pchString)
{
    // Potential candidate for move constructor??
    std::string copy(_stringStorage);
    _stringStorage = pchString ? pchString : "";
    _stringStorage += copy;
    return (*this);
}

/*!
 * [Short description]
 *    Prepend to this String object.
 *
 * [Detailed description]
 *    Insert the specified string at the start of the string in this String object.
 *    This operation will change the content of this object.
 *
 * [Parameters]
 *    in | stlString | STL string to prepend to this String.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the prepend operation.
 */
String& String::Prepend(const std::string& stlString)
{
    // Potential candidate for move constructor??
    std::string copy(_stringStorage);
    _stringStorage = stlString;
    _stringStorage += copy;
    return (*this);
}

/*!
 * [Short description]
 *    Prepend to this String object.
 *
 * [Detailed description]
 *    Insert the specified string at the start of the string in this String object.
 *    This operation will change the content of this object.
 *
 * [Parameters]
 *    in | otherString | String to prepend to this String.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the prepend operation.
 */
String& String::Prepend(const String& otherString)
{
    // Potential candidate for move constructor??
    std::string copy(_stringStorage);
    _stringStorage = otherString;
    _stringStorage += copy;
    return (*this);
}

/*!
 * [Short description]
 *    Prepend to this String object.
 *
 * [Detailed description]
 *    Insert the string representation of the specified integer value
 *    at the start of the string in this String object.
 *    This operation will change the content of this object.
 *
 * [Parameters]
 *    in | value | Integer value to prepend to this String.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the prepend operation.
 */
String& String::Prepend(int value)
{
    // Potential candidate for move constructor??
    std::string copy(_stringStorage);
    _stringStorage.clear();
    Append(value);
    _stringStorage += copy;
    return (*this);
}

/*!
 * [Short description]
 *    Prepend to this String object.
 *
 * [Detailed description]
 *    Insert the string representation of the specified integer value
 *    at the start of the string in this String object.
 *    This operation will change the content of this object.
 *
 * [Parameters]
 *    in | value | Integer value to prepend to this String.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the prepend operation.
 */
String& String::Prepend(unsigned int value)
{
    // Potential candidate for move constructor??
    std::string copy(_stringStorage);
    _stringStorage.clear();
    Append(value);
    _stringStorage += copy;
    return (*this);
}

/*!
 * [Short description]
 *    Prepend to this String object.
 *
 * [Detailed description]
 *    Insert the string representation of the specified long integer value
 *    at the start of the string in this String object.
 *    This operation will change the content of this object.
 *
 * [Parameters]
 *    in | value | Long integer value to prepend to this String.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the prepend operation.
 */
String& String::Prepend(long value)
{
    // Potential candidate for move constructor??
    std::string copy(_stringStorage);
    _stringStorage.clear();
    Append(value);
    _stringStorage += copy;
    return (*this);
}

/*!
 * [Short description]
 *    Prepend to this String object.
 *
 * [Detailed description]
 *    Insert the string representation of the specified long integer value
 *    at the start of the string in this String object.
 *    This operation will change the content of this object.
 *
 * [Parameters]
 *    in | value | Long integer value to prepend to this String.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the prepend operation.
 */
String& String::Prepend(unsigned long value)
{
    // Potential candidate for move constructor??
    std::string copy(_stringStorage);
    _stringStorage.clear();
    Append(value);
    _stringStorage += copy;
    return (*this);
}

/*!
 * [Short description]
 *    Prepend to this String object.
 *
 * [Detailed description]
 *    Insert the string representation of the specified single precision floating point value
 *    at the start of the string in this String object.
 *    This operation will change the content of this object.
 *
 * [Parameters]
 *    in | value | Single precision floating point value to prepend to this String.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the prepend operation.
 */
String& String::Prepend(float value)
{
    // Potential candidate for move constructor??
    std::string copy(_stringStorage);
    _stringStorage.clear();
    Append(value);
    _stringStorage += copy;
    return (*this);
}

/*!
 * [Short description]
 *    Prepend to this String object.
 *
 * [Detailed description]
 *    Insert the string representation of the specified double precision floating point value
 *    at the start of the string in this String object.
 *    This operation will change the content of this object.
 *
 * [Parameters]
 *    in | value | Double precision floating point value to prepend to this String.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the prepend operation.
 */
String& String::Prepend(double value)
{
    // Potential candidate for move constructor??
    std::string copy(_stringStorage);
    _stringStorage.clear();
    Append(value);
    _stringStorage += copy;
    return (*this);
}

/*!
 * [Short description]
 *    Prepend to this String object.
 *
 * [Detailed description]
 *    Insert the character at the start of the string in this String object.
 *    This operation will change the content of this object.
 *
 * [Parameters]
 *    in | value | Character to prepend to this String.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the prepend operation.
 */
String& String::Prepend(char value)
{
    // Potential candidate for move constructor??
    std::string copy(_stringStorage);
    _stringStorage.clear();
    Append(value);
    _stringStorage += copy;
    return (*this);
}

/*!
 * [Short description]
 *    Prepend to this String object.
 *
 * [Detailed description]
 *    Insert the string representation of the specified boolean value
 *    at the start of the string in this String object.
 *    This operation will change the content of this object.
 *
 * [Parameters]
 *    in | value | Boolean value to prepend to this String.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the prepend operation.
 */
String& String::Prepend(bool value)
{
    // Potential candidate for move constructor??
    std::string copy(_stringStorage);
    _stringStorage.clear();
    Append(value);
    _stringStorage += copy;
    return (*this);
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
 *    The capitalized string is stored in the String object itself.
 */
void String::Capitalize()
{
    if (IsEmpty())
    {
        return;
    }

    ToLower();

    bool toup = true;
    size_t strsize = _stringStorage.size();
    for (size_t i = 0; i < strsize; ++i)
    {
        if (std::isalpha(_stringStorage[i]))
        {
            if (toup)
            {
                _stringStorage[i] = static_cast<char>(std::toupper(_stringStorage[i]));
                toup = false;
            }
        }
        else
        {
            toup = true;
        }
    }
}

/*!
 * [Short description]
 *    Trim leading and/or trailing white spaces from the string.
 *
 * [Detailed description]
 *    Trim leading and/or trailing white spaces from the string.
 *    The trimmed string is stored in the String object itself.
 *
 * [Parameters]
 *    in | leading  | Indicates whether to trim leading white spaces.
 *    in | trailing | Indicates whether to trim trailing white spaces.
 */
void String::Trim(bool leading, bool trailing)
{
    if (leading)
    {
        size_t strsize = _stringStorage.size();

		size_t i = 0;
        while (std::isspace(_stringStorage[i]) && ++i < strsize)
            ;

        _stringStorage.erase(0, i);
    }

    if (trailing)
    {
		size_t i = _stringStorage.size() - 1;
        while (std::isspace(_stringStorage[i]) && --i >= 0)
            ;

        _stringStorage.erase(i + 1, _stringStorage.size() - i + 1);
    }
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
 *    in | ch    | Character to count occurrences for.
 *    in | start | Position in the string to start counting from.
 *
 * [Return value]
 *    Returns the number of times the specified string occurs in the string.
 */
unsigned int String::Count(char ch, unsigned int start) const
{
    unsigned int count = 0;
    for (size_t i = _stringStorage.size() - 1; i >= static_cast<size_t>(start); --i)
    {
        if (_stringStorage[i] == ch)
        {
            count++;
        }
    }

    return count;
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
 *    in | otherString | Pointer to null-terminated character array to count occurrences for.
 *    in | start       | Position in the string to start counting from.
 *
 * [Return value]
 *    Returns the number of times the specified string occurs in the string.
 */
unsigned int String::Count(const char* pchString, unsigned int start) const
{
	const char* fpchString = pchString ? pchString : "";
    unsigned int count = 0;
    unsigned int startpos = start;
	std::size_t len = strlen(fpchString);
    std::size_t pos = std::string::npos;

    while ((pos = _stringStorage.find(fpchString, startpos)) != std::string::npos)
    {
        count++;
        startpos = static_cast<unsigned int>(pos + len);
    }

    return count;
}

/*!
 * [Short description]
 *    Replace the first or all occurrences of the specified character with another character.
 *
 * [Parameters]
 *    in | pattern     | Character to replace.
 *    in | replacement | Character to replace the character with.
 *    in | global      | Indicates whether to replace all occurrences or not (true => all occurrences).
 *
 * [Return value]
 *    Returns the number of occurrences that have been replaced in the string.
 */
unsigned int String::Replace(char pattern, char replacement, bool global)
{
    unsigned int count = 0;

    if (global)
    {
        for (std::size_t i = _stringStorage.size() - 1; i >= 0; --i)
        {
            if (_stringStorage[i] == pattern)
            {
                _stringStorage[i] = replacement;
                ++count;
            }
        }
    }
    else
    {
        int position = Find(pattern);
        if (position >= 0)
        {
            _stringStorage[position] = replacement;
            ++count;
        }
    }

    return count;
}

/*!
 * [Short description]
 *    Replace the first or all occurrences of the specified character with a string.
 *
 * [Parameters]
 *    in | pattern     | Character to replace.
 *    in | replacement | Pointer to null-terminated character array to replace the character with.
 *    in | global      | Indicates whether to replace all occurrences or not (true => all occurrences).
 *
 * [Return value]
 *    Returns the number of occurrences that have been replaced in the string.
 */
unsigned int String::Replace(char pattern, const char* replacement, bool global)
{
    unsigned int count = 0;
	const char* freplacement = replacement ? replacement : "";

    if (global)
    {
        for (std::size_t i = _stringStorage.size() - 1; i >= 0; --i)
        {
            if (_stringStorage[i] == pattern)
            {
                _stringStorage.replace(i, 1, freplacement);
                count++;
            }
        }
    }
    else
    {
        int position = Find(pattern);
        if (position >= 0)
        {
            _stringStorage.replace(position, 1, freplacement);
            ++count;
        }
    }

    return count;
}

/*!
 * [Short description]
 *    Replace the first or all occurrences of the specified string with another string.
 *
 * [Parameters]
 *    in | pattern     | Pointer to null-terminated character array to replace.
 *    in | replacement | Pointer to null-terminated character array to replace the character with.
 *    in | global      | Indicates whether to replace all occurrences or not (true => all occurrences).
 *
 * [Return value]
 *    Returns the number of occurrences that have been replaced in the string.
 */
unsigned int String::Replace(const char* pattern, const char* replacement, bool global)
{
    unsigned int count = 0;
    std::size_t foundPos = std::string::npos;
	const char* freplacement = replacement ? replacement : "";

    if (global)
    {
        while ((foundPos = _stringStorage.rfind(pattern, foundPos)) != std::string::npos)
        {
            _stringStorage.replace(foundPos, strlen(pattern), freplacement);
            count++;

            if (foundPos == 0)
                break;
            foundPos--;
        }
    }
    else
    {
        int position = Find(pattern);
        if (position >= 0)
        {
            _stringStorage.replace(position, strlen(pattern), freplacement);
            ++count;
        }
    }

    return count;
}

/*!
 * [Short description]
 *    Erase the first occurrence of the specified string from the string.
 *
 * [Detailed description]
 *    Erase the first occurrence of the specified string from the string
 *    starting from the specified position.
 *
 * [Parameters]
 *    in | pattern | Pointer to null-terminated character array to replace.
 *    in | start   | First position to consider for this operation.
 */
void String::Erase(const char* pattern, unsigned int start)
{
	const char* fpattern = pattern ? pattern : "";
    int patpos = Find(fpattern, start);
    if (patpos >= 0)
    {
        _stringStorage.erase(patpos, strlen(fpattern));
    }
}

/*!
 * [Short description]
 *    Erase the first occurrence of the specified string from the string.
 *
 * [Detailed description]
 *    Erase the first occurrence of the specified string from the string
 *    starting from the specified position.
 *
 * [Parameters]
 *    in | pattern | STL string to replace.
 *    in | start   | First position to consider for this operation.
 */
void String::Erase(const std::string& pattern, unsigned int start)
{
    int patpos = Find(pattern, start);
    if (patpos >= 0)
    {
        _stringStorage.erase(patpos, pattern.size());
    }
}

/*!
 * [Short description]
 *    Erase the first occurrence of the specified string from the string.
 *
 * [Detailed description]
 *    Erase the first occurrence of the specified string from the string
 *    starting from the specified position.
 *
 * [Parameters]
 *    in | pattern | String to replace.
 *    in | start   | First position to consider for this operation.
 */
void String::Erase(const String& pattern, unsigned int start)
{
    int patpos = Find(pattern, start);
    if (patpos >= 0)
    {
        _stringStorage.erase(patpos, pattern.Length());
    }
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
 *    in  | separator | Character to split the string at.
 *    in  | javastyle | Indicates whether java style split should be used.
 *
 * [Return value]
 *    Returns the number of parts stored in the list.
 */
unsigned int String::Split(std::vector<std::string>& list, char separator, bool javastyle) const
{
    unsigned int startpos = 0;
    int seppos = -1;

    list.clear();
    while ((seppos = Find(separator, startpos)) >= 0)
    {
        list.push_back(_stringStorage.substr(startpos, seppos - startpos));
        startpos = seppos + 1;
    }

    // In Java style split, there will be no empty string at the end
    // of the list if the separator appears at the end of the string.
    if (!javastyle || startpos < Length())
    {
        list.push_back(_stringStorage.substr(startpos));
    }

    return static_cast<int>(list.size());
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
 *    in  | separator | Pointer to null-terminated character array pattern to split the string at.
 *    in  | javastyle | Indicates whether java style split should be used.
 *
 * [Return value]
 *    Returns the number of parts stored in the list.
 */
unsigned int String::Split(std::vector<std::string>& list, const char* separator, bool javastyle) const
{
	REQUIRE(separator != nullptr);
	
    unsigned int startpos = 0;
    unsigned int seplen = static_cast<unsigned int>(strlen(separator));
    int seppos = -1;

    list.clear();
    while ((seppos = Find(separator, startpos)) >= 0)
    {
        list.push_back(_stringStorage.substr(static_cast<size_t>(startpos), static_cast<size_t>(seppos - startpos)));
        startpos = static_cast<unsigned int>(seppos) + seplen;
    }

    // In Java style split, there will be no empty string at the end
    // of the list if the separator appears at the end of the string.
    if (!javastyle || (startpos < Length()))
    {
        list.push_back(_stringStorage.substr(static_cast<size_t>(startpos)));
    }

    return static_cast<unsigned int>(list.size());
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
 *    in  | separator | Character pattern to split the string at.
 *    in  | javastyle | Indicates whether java style split should be used.
 *
 * [Return value]
 *    Returns the number of parts stored in the list.
 */
unsigned int String::Split(std::vector<String>& list, char separator, bool javastyle) const
{
    unsigned int startpos = 0;
    int seppos = -1;

    list.clear();
    while ((seppos = Find(separator, startpos)) >= 0)
    {
        list.push_back(_stringStorage.substr(startpos, seppos - startpos));
        startpos = static_cast<unsigned int>(seppos + 1);
    }

    // In Java style split, there will be no empty string at the end
    // of the list if the separator appears at the end of the string.
    if (!javastyle || startpos < Length())
    {
        list.push_back(_stringStorage.substr(startpos));
    }

    return static_cast<unsigned int>(list.size());
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
 *    in  | separator | Pointer to null-terminated character array pattern to split the string at.
 *    in  | javastyle | Indicates whether java style split should be used.
 *
 * [Return value]
 *    Returns the number of parts stored in the list.
 */
unsigned int String::Split(std::vector<String>& list, const char* separator, bool javastyle) const
{
	REQUIRE(separator != nullptr);

    unsigned int startpos = 0;
    unsigned int seplen = static_cast<unsigned int>(strlen(separator));
    int seppos = -1;

    list.clear();
    while ((seppos = Find(separator, startpos)) >= 0)
    {
        list.push_back(_stringStorage.substr(startpos, seppos - startpos));
        startpos = static_cast<unsigned int>(seppos) + seplen;
    }

    // In Java style split, there will be no empty string at the end
    // of the list if the separator appears at the end of the string.
    if (!javastyle || startpos < Length())
    {
        list.push_back(_stringStorage.substr(startpos));
    }

    return static_cast<unsigned int>(list.size());
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
 *    in  | separator | Character to split the string at.
 *    in  | javastyle | Indicates whether java style split should be used.
 *
 * [Return value]
 *    Returns the number of parts stored in the list.
 */
unsigned int String::Split(std::string list[], unsigned int len, char separator, bool javastyle) const
{
    unsigned int startpos = 0;
    unsigned int i = 0;
    int seppos = -1;

    while (i < len && (seppos = Find(separator, startpos)) >= 0)
    {
        list[i] = _stringStorage.substr(startpos, seppos - startpos);
        startpos = static_cast<unsigned int>(seppos + 1);
        ++i;
    }

    // In Java style split, there will be no empty string at the end
    // of the list if the separator appears at the end of the string.
    if (i < len && (!javastyle || startpos < Length()))
    {
        list[i] = _stringStorage.substr(startpos);
        ++i;
    }

    return i;
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
 *    in  | separator | Pointer to null-terminated character array to split the string at.
 *    in  | javastyle | Indicates whether java style split should be used.
 *
 * [Return value]
 *    Returns the number of parts stored in the list.
 */
unsigned int String::Split(std::string list[], unsigned int len, const char* separator, bool javastyle) const
{
	REQUIRE(separator != nullptr);

    unsigned int startpos = 0;
    unsigned int seplen = static_cast<unsigned int>(strlen(separator));
    unsigned int i = 0;
    int seppos = -1;

    while (i < len && (seppos = Find(separator, startpos)) >= 0)
    {
        list[i] = _stringStorage.substr(startpos, seppos - startpos);
        startpos = seppos + seplen;
        ++i;
    }

    // In Java style split, there will be no empty string at the end
    // of the list if the separator appears at the end of the string.
    if (i < len && (!javastyle || startpos < Length()))
    {
        list[i] = _stringStorage.substr(startpos);
        ++i;
    }

    return i;
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
 *    in  | separator | Character to split the string at.
 *    in  | javastyle | Indicates whether java style split should be used.
 *
 * [Return value]
 *    Returns the number of parts stored in the list.
 */
unsigned int String::Split(String list[], unsigned int len, char separator, bool javastyle) const
{
    unsigned int startpos = 0;
    unsigned int i = 0;
    int seppos = -1;

    while (i < len && (seppos = Find(separator, startpos)) >= 0)
    {
        list[i] = _stringStorage.substr(startpos, seppos - startpos);
        startpos = seppos + 1;
        ++i;
    }

    // In Java style split, there will be no empty string at the end
    // of the list if the separator appears at the end of the string.
    if (i < len && (!javastyle || startpos < Length()))
    {
        list[i] = _stringStorage.substr(startpos);
        ++i;
    }

    return i;
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
 *    in  | separator | Pointer to null-terminated character array to split the string at.
 *    in  | javastyle | Indicates whether java style split should be used.
 *
 * [Return value]
 *    Returns the number of parts stored in the list.
 */
unsigned int String::Split(String list[], unsigned int len, const char* separator, bool javastyle) const
{
	REQUIRE(separator != nullptr);

    unsigned int startpos = 0;
    unsigned int seplen = static_cast<unsigned int>(strlen(separator));
    unsigned int i = 0;
    int seppos = -1;

    while (i < len && (seppos = Find(separator, startpos)) >= 0)
    {
        list[i] = _stringStorage.substr(startpos, seppos - startpos);
        startpos = seppos + seplen;
        ++i;
    }

    // In Java style split, there will be no empty string at the end
    // of the list if the separator appears at the end of the string.
    if (i < len && (!javastyle || startpos < Length()))
    {
        list[i] = _stringStorage.substr(startpos);
        ++i;
    }

    return i;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the string stored in this object.
 *
 * [Parameters]
 *    in | list | Vector of STL string object to join.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
String& String::Join(const std::vector<std::string>& list)
{
    std::string separator = _stringStorage;

    _stringStorage.clear();

    unsigned int listsize = static_cast<unsigned int>(list.size());
    for (unsigned int i = 0; i < listsize; ++i)
    {
        _stringStorage += list[i];
        if (i < listsize - 1)
        {
            _stringStorage += separator;
        }
    }

    return *this;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the string stored in this object.
 *
 * [Parameters]
 *    in | list | Vector of String object to join.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
String& String::Join(const std::vector<String>& list)
{
    std::string separator = _stringStorage;

    _stringStorage.clear();

    unsigned int listsize = static_cast<unsigned int>(list.size());
    for (unsigned int i = 0; i < listsize; ++i)
    {
        _stringStorage += list[i];
        if (i < listsize - 1)
        {
            _stringStorage += separator;
        }
    }

    return *this;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the string stored in this object.
 *
 * [Parameters]
 *    in | list | Array of STL string object to join.
 *    in | len  | Number of elements in the array.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
String& String::Join(const std::string list[], unsigned int len)
{
    std::string separator = _stringStorage;

    _stringStorage.clear();

    for (unsigned int i = 0; i < len; ++i)
    {
        _stringStorage += list[i];
        if (i < len - 1)
        {
            _stringStorage += separator;
        }
    }

    return *this;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the string stored in this object.
 *
 * [Parameters]
 *    in | list | Array of String object to join.
 *    in | len  | Number of elements in the array.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
String& String::Join(const String list[], unsigned int len)
{
    std::string separator = _stringStorage;

    _stringStorage.clear();

    for (unsigned int i = 0; i < len; ++i)
    {
        _stringStorage += list[i];
        if (i < len - 1)
        {
            _stringStorage += separator;
        }
    }

    return *this;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the string stored in this object.
 *
 * [Parameters]
 *    in | list | Array of Pointers to null-terminated character arrays to join.
 *    in | len  | Number of elements in the array.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
String& String::Join(const char* list[], unsigned int len)
{
    std::string separator = _stringStorage;

    _stringStorage.clear();

    for (unsigned int i = 0; i < len; ++i)
    {
        _stringStorage += list[i];
        if (i < len - 1)
        {
            _stringStorage += separator;
        }
    }

    return *this;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the string stored in this object.
 *
 * [Parameters]
 *    in | itBegin   | Iterator to the first element in a vector of STL strings to join.
 *    in | itEnd     | Iterator to the last element in a vector of STL strings to join.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
String& String::Join(const std::vector<std::string>::const_iterator& itBegin,
                     const std::vector<std::string>::const_iterator& itEnd)
{
    std::string separator = _stringStorage;

    _stringStorage.clear();

    for (std::vector<std::string>::const_iterator it = itBegin; it != itEnd; ++it)
    {
        _stringStorage += *it;
        if (it != itEnd - 1)
        {
            _stringStorage += separator;
        }
    }

    return *this;
}

/*!
 * [Short description]
 *    Join the strings provided into one string separated by the string stored in this object.
 *
 * [Parameters]
 *    in | itBegin   | Iterator to the first element in a vector of Strings to join.
 *    in | itEnd     | Iterator to the last element in a vector of Strings to join.
 *
 * [Return value]
 *    Return a String object with the result of the join operation.
 */
String& String::Join(const std::vector<String>::const_iterator& itBegin,
                     const std::vector<String>::const_iterator& itEnd)
{
    std::string separator = _stringStorage;

    _stringStorage.clear();

    for (std::vector<String>::const_iterator it = itBegin; it != itEnd; ++it)
    {
        _stringStorage += *it;
        if (it != itEnd - 1)
        {
            _stringStorage += separator;
        }
    }

    return *this;
}

/*!
 * [Short description]
 *    Create a sub-string.
 *
 * [Detailed description]
 *    Create a sub-string starting at the first provided position in the string up to the last provided position in the string.
 *
 * [Parameters]
 *    in | start | Iterator pointing to the first character in the string in include in the sub-string.
 *    in | end   | Iterator pointing to the last character in the string in include in the sub-string.
 *
 * [Return value]
 *    Return a String object with the requested sub-string.
 */
String String::Substring(const_iterator start, const_iterator end) const
{
    unsigned int startpos = static_cast<unsigned int>(start - Begin());
    unsigned int endpos = static_cast<unsigned int>(end - Begin());
    unsigned int len = endpos - startpos;
    String result = _stringStorage.substr(startpos, len);
    return result;
}

/*!
 * [Short description]
 *    Create a sub-string after the first occurrence of the character provided.
 *
 * [Detailed description]
 *    Create a sub-string after the first occurrence of the character provided starting at the given position.
 *    The character that is searched for will NOT be included in the substring.
 *
 * [Parameters]
 *    in | ch    | Character to sub-string after.
 *    in | start | First position in the string to consider. A negative value means that the n-th character from the end.
 *
 * [Return value]
 *    Return a String object with the requested sub-string.
 */
String String::SubstringAfter(char ch, int start) const
{
    String results;
    int pos = -1;
    if (start >= 0)
    {
        pos = Find(ch, start);
    }
    else
    {
        // NOTE: start is negative!
        pos = FindLast(ch, Length() + start);
    }

    if (pos >= 0)
    {
        results = _stringStorage.substr(pos + 1);
    }

    return results;
}

/*!
 * [Short description]
 *    Create a sub-string after the first occurrence of the string provided.
 *
 * [Detailed description]
 *    Create a sub-string after the first occurrence of the string provided starting at the given position.
 *    The string that is searched for will NOT be included in the substring.
 *
 * [Parameters]
 *    in | pchString | Pointer to null-terminated character array to sub-string after.
 *    in | start     | First position in the string to consider. A negative value means that the n-th character from the end.
 *
 * [Return value]
 *    Return a String object with the requested sub-string.
 */
String String::SubstringAfter(const char* pchString, int start) const
{
	REQUIRE(pchString != nullptr);

    String results;
    int pos = -1;
    if (start >= 0)
    {
        pos = Find(pchString, start);
    }
    else
    {
        // NOTE: start is negative!
        pos = FindLast(pchString, Length() + start);
    }

    if (pos >= 0)
    {
        results = _stringStorage.substr(pos + strlen(pchString));
    }

    return results;
}

/*!
 * [Short description]
 *    Create a sub-string after the first occurrence of the string provided.
 *
 * [Detailed description]
 *    Create a sub-string after the first occurrence of the string provided starting at the given position.
 *    The string that is searched for will NOT be included in the substring.
 *
 * [Parameters]
 *    in | stlString | STL string to sub-string after.
 *    in | start     | First position in the string to consider. A negative value means that the n-th character from the end.
 *
 * [Return value]
 *    Return a String object with the requested sub-string.
 */
String String::SubstringAfter(const std::string& stlString, int start) const
{
    String results;
    int pos = -1;
    if (start >= 0)
    {
        pos = Find(stlString, start);
    }
    else
    {
        // NOTE: start is negative!
        pos = FindLast(stlString, Length() + start);
    }

    if (pos >= 0)
    {
        results = _stringStorage.substr(pos + stlString.size());
    }

    return results;
}

/*!
 * [Short description]
 *    Create a sub-string after the first occurrence of the string provided.
 *
 * [Detailed description]
 *    Create a sub-string after the first occurrence of the string provided starting at the given position.
 *    The string that is searched for will NOT be included in the substring.
 *
 * [Parameters]
 *    in | otherString | String to sub-string after.
 *    in | start       | First position in the string to consider. A negative value means that the n-th character from the end.
 *
 * [Return value]
 *    Return a String object with the requested sub-string.
 */
String String::SubstringAfter(const String& otherString, int start) const
{
    String results;
    int pos = -1;
    if (start >= 0)
    {
        pos = Find(otherString, start);
    }
    else
    {
        // NOTE: start is negative!
        pos = FindLast(otherString, Length() + start);
    }

    if (pos >= 0)
    {
        results = _stringStorage.substr(pos + otherString.Length());
    }

    return results;
}

/*!
 * [Short description]
 *    Create a sub-string before the first occurrence of the character provided.
 *
 * [Detailed description]
 *    Create a sub-string before the first occurrence of the character provided starting at the given position.
 *    The string that is searched for will NOT be included in the substring.
 *
 * [Parameters]
 *    in | ch    | Character to sub-string before.
 *    in | start | First position in the string to consider. A negative value means that the n-th character from the end.
 *
 * [Return value]
 *    Return a String object with the requested sub-string.
 */
String String::SubstringBefore(char ch, int start) const
{
    String results;
    int pos = -1;
    if (start >= 0)
    {
        pos = Find(ch, start);
    }
    else
    {
        // NOTE: start is negative!
        pos = FindLast(ch, Length() + start);
    }

    if (pos >= 0)
    {
        results = _stringStorage.substr(0, pos);
    }

    return results;
}

/*!
 * [Short description]
 *    Create a sub-string before the first occurrence of the string provided.
 *
 * [Detailed description]
 *    Create a sub-string before the first occurrence of the string provided starting at the given position.
 *    The string that is searched for will NOT be included in the substring.
 *
 * [Parameters]
 *    in | pchString | Pointer to null-terminated character array to sub-string before.
 *    in | start     | First position in the string to consider. A negative value means that the n-th character from the end.
 *
 * [Return value]
 *    Return a String object with the requested sub-string.
 */
String String::SubstringBefore(const char* pchString, int start) const
{
	REQUIRE(pchString != nullptr);

    String results;
    int pos = -1;
    if (start >= 0)
    {
        pos = Find(pchString, start);
    }
    else
    {
        // NOTE: start is negative!
        pos = FindLast(pchString, Length() + start);
    }

    if (pos >= 0)
    {
        results = _stringStorage.substr(0, pos);
    }

    return results;
}

/*!
 * [Short description]
 *    Create a sub-string before the first occurrence of the string provided.
 *
 * [Detailed description]
 *    Create a sub-string before the first occurrence of the string provided starting at the given position.
 *    The string that is searched for will NOT be included in the substring.
 *
 * [Parameters]
 *    in | stlString | STL string to sub-string before.
 *    in | start     | First position in the string to consider. A negative value means that the n-th character from the end.
 *
 * [Return value]
 *    Return a String object with the requested sub-string.
 */
String String::SubstringBefore(const std::string& stlString, int start) const
{
    String results;
    int pos = -1;
    if (start >= 0)
    {
        pos = Find(stlString, start);
    }
    else
    {
        // NOTE: start is negative!
        pos = FindLast(stlString, Length() + start);
    }

    if (pos >= 0)
    {
        results = _stringStorage.substr(0, pos);
    }

    return results;
}

/*!
 * [Short description]
 *    Create a sub-string before the first occurrence of the string provided.
 *
 * [Detailed description]
 *    Create a sub-string before the first occurrence of the string provided starting at the given position.
 *    The string that is searched for will NOT be included in the substring.
 *
 * [Parameters]
 *    in | pchString | String to sub-string before.
 *    in | start     | First position in the string to consider. A negative value means that the n-th character from the end.
 *
 * [Return value]
 *    Return a String object with the requested sub-string.
 */
String String::SubstringBefore(const String& otherString, int start) const
{
    String results;
    int pos = -1;
    if (start >= 0)
    {
        pos = Find(otherString, start);
    }
    else
    {
        // NOTE: start is negative!
        pos = FindLast(otherString, Length() + start);
    }

    if (pos >= 0)
    {
        results = _stringStorage.substr(0, pos);
    }

    return results;
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
 *    in | it    | Position in the string to insert the character.
 *
 * [Return value]
 *    Return a reference to this String object containing the result of the insert operation.
 */
String& String::Insert(const char* value, iterator it)
{
	if(value != nullptr)
	{
		String before = Substring(_stringStorage.begin(), it);
		String after = Substring(it);

		_stringStorage = before;
		Append(value);
		Append(after);
	}
    return (*this);
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
bool String::ConvertTo(float& value) const
{
	std::istringstream ss(_stringStorage);
	ss.imbue(std::locale::classic());
	ss >> value;
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
bool String::ConvertTo(double& value) const
{
	std::istringstream ss(_stringStorage);
	ss.imbue(std::locale::classic());
	ss >> value;
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
bool String::ConvertTo(int& value) const
{
	std::istringstream ss(_stringStorage);
	ss.imbue(std::locale::classic());
	ss >> value;
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
bool String::ConvertTo(long& value) const
{
	std::istringstream ss(_stringStorage);
	ss.imbue(std::locale::classic());
	ss >> value;
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
bool String::ConvertTo(bool& value) const
{
    String lowercase = ToLower();
    if (_stringStorage == "1" || lowercase._stringStorage == "true")
    {
        value = true;
        return true;
    }
    else if (_stringStorage == "0" || lowercase._stringStorage == "false")
    {
        value = false;
        return true;
    }
    return false;
}

/*!
 * [Short description]
 *    Read one line of text from the given stream object.
 *
 * [Detailed description]
 *    Read one line of text from the given stream object and store the result in this object.
 *    The string is cleared before the data is stored.
 *
 * [Parameters]
 *    inout | s | Stream object to read from.
 *
 * [Return value]
 *    Returns the number of character read from the stream.
 */
int String::ReadLine(std::istream& s)
{
    Clear();
    std::getline(s, _stringStorage);
    return Length();
}

/*!
 * [Short description]
 *    Read all text available in the given stream object.
 *
 * [Detailed description]
 *    Read all text available in the given stream object and store the result in this object.
 *    The string is cleared before the data is stored.
 *
 * [Parameters]
 *    inout | s | Stream object to read from.
 *
 * [Return value]
 *    Returns the number of character read from the stream.
 */
int String::Read(std::istream& s)
{
    std::string tmp;
    bool doLF = false;

    Clear();
    while (!s.eof() && !s.bad())
    {
        if (doLF)
        {
            _stringStorage += "\n";
        }
        doLF = true;

        std::getline(s, tmp);
        _stringStorage += tmp;
    }
    return Length();
}

/*!
 * [Short description]
 *    Write the string stored in this object to the given stream object.
 *
 * [Parameters]
 *    inout | s | Stream object to write the string to.
 *
 * [Return value]
 *    Returns the number of character written to the stream.
 */
int String::Write(std::ostream& s)
{
    s << _stringStorage;
    return Length();
}
