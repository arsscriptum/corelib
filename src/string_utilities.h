

//==============================================================================
//
//  StringUtilities.h  - exported_h
//   small, generally useful, helper classes
//
//==============================================================================
//  cybercastor - made in quebec 2020 <cybercastor@icloud.com>
//==============================================================================


#ifndef __STRINGUTILITIES_H__
#define __STRINGUTILITIES_H__

/*
* Package Operations:
* -------------------
* This package provides functions:
* - Title(text)           display title
* - title(text)           display subtitle
* - putline(n)            display n newlines
* - trim(str)             remove leading and trailing whitespace
* - split(str, 'delim')   break string into vector of strings separated by delim char 
* - showSplit(vector)     display splits
*/
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <functional>
#include <locale>


class StringHelpers
{
public:

	static StringHelpers* getInstance();
	static std::vector<std::string> split_string(const std::string &input, const std::string &regex);
	static std::string trim_string(const std::string &input, const std::string &find = " ");
	static std::string trim_string_remove_new_line(const std::string &input);
	static std::string get_string_between_two_chars(const std::string &input, const char start, const char end);
	static bool string_last_char_is(const std::string &input, const char character);
	static bool string_first_char_is(const std::string &input, const char character);
	static bool string_remove_last_char_if(std::string &input, const char character);
	static bool string_remove_first_char_if(std::string &input, const char character);
	static std::vector<std::string> split_string_once(const std::string &input, const char split_char);

	void Title(const std::string& text, std::ostream& out = std::cout, char underline = '=');

	void title(const std::string& text, std::ostream& out = std::cout, char underline = '-');

	void putline(size_t j = 1, std::ostream& out = std::cout);

	std::string sTitle(const std::string& text, char underline = '=');
	std::string stitle(const std::string& text, char underline = '-');

	std::string sputline(size_t j = 1);
	//-- remove whitespace from front and back of string argument ---
	template <typename T>
	std::basic_string<T> trim(const std::basic_string<T>& toTrim);
	//--- remove newlines from front and back of string argument

	template <typename T>
	std::basic_string<T> trimNewLines(const std::basic_string<T>& toTrim);

	// -- split sentinel separated strings into a vector of trimmed strings 

	template <typename T>
	std::vector<std::basic_string<T>> split(const std::basic_string<T>& toSplit, T splitOn = ',');

	template <typename T>
	void showSplits(const std::vector<std::basic_string<T>>& splits, std::ostream& out = std::cout);

private:
	StringHelpers() {};
	static StringHelpers *instance;
};


#endif // __STRINGUTILITIES_H__