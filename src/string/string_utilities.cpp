//==============================================================================
//
//  StringUtilities.cpp - small, generally useful, helper classes
//
//==============================================================================
//  cybercastor - made in quebec 2020 <cybercastor@icloud.com>
//==============================================================================

#include "string_utilities.h"


#include <regex>

StringHelpers *StringHelpers::instance = nullptr;

StringHelpers* StringHelpers::getInstance()
{
	if (instance == nullptr)
	{
		instance = new StringHelpers();
	}

	return instance;
}

void StringHelpers::putline(size_t j, std::ostream& out)
{
	for (size_t i = 0; i < j; ++i)
		out << "\n";
}

std::string StringHelpers::sputline(size_t j)
{
	std::ostringstream out;
	for (size_t i = 0; i < j; ++i)
		out << "\n";
	return out.str();
}

//-- remove whitespace from front and back of string argument ---
template <typename T>
std::basic_string<T> StringHelpers::trim(const std::basic_string<T>& toTrim)
{
	if (toTrim.size() == 0)
		return toTrim;
	std::basic_string<T> temp;
	std::locale loc;
	typename std::basic_string<T>::const_iterator iter = toTrim.begin();
	while (isspace(*iter, loc) && *iter != '\n')
	{
		if (++iter == toTrim.end())
		{
			break;
		}
	}
	for (; iter != toTrim.end(); ++iter)
	{
		temp += *iter;
	}
	typename std::basic_string<T>::reverse_iterator riter;
	size_t pos = temp.size();
	for (riter = temp.rbegin(); riter != temp.rend(); ++riter)
	{
		--pos;
		if (!isspace(*riter, loc) || *riter == '\n')
		{
			break;
		}
	}
	if (0 <= pos && pos < temp.size())
		temp.erase(++pos);
	return temp;
}

//--- remove newlines from front and back of string argument

template <typename T>
std::basic_string<T> StringHelpers::trimNewLines(const std::basic_string<T>& toTrim)
{
	if (toTrim.size() == 0)
		return toTrim;
	std::basic_string<T> temp;
	std::locale loc;
	typename std::basic_string<T>::const_iterator iter = toTrim.begin();
	while (isspace(*iter, loc))
	{
		if (++iter == toTrim.end())
		{
			break;
		}
	}
	for (; iter != toTrim.end(); ++iter)
	{
		temp += *iter;
	}
	typename std::basic_string<T>::reverse_iterator riter;
	size_t pos = temp.size();
	for (riter = temp.rbegin(); riter != temp.rend(); ++riter)
	{
		--pos;
		if (!isspace(*riter, loc))
		{
			break;
		}
	}
	if (0 <= pos && pos < temp.size())
		temp.erase(++pos);
	return temp;
}


// -- split sentinel separated strings into a vector of trimmed strings 

template <typename T>
std::vector<std::basic_string<T>> StringHelpers::split(const std::basic_string<T>& toSplit, T splitOn)
{
	std::vector<std::basic_string<T>> splits;
	std::basic_string<T> temp;
	typename std::basic_string<T>::const_iterator iter;
	for (iter = toSplit.begin(); iter != toSplit.end(); ++iter)
	{
		if (*iter != splitOn)
		{
			temp += *iter;
		}
		else
		{
			splits.push_back(trim(temp));
			temp.clear();
		}
	}
	if (temp.length() > 0)
		splits.push_back(trim(temp));
	return splits;
}
//--- show collection of string splits

template <typename T>
void StringHelpers::showSplits(const std::vector<std::basic_string<T>>& splits, std::ostream& out)
{
	out << "\n";
	for (auto item : splits)
	{
		if (item == "\n")
			out << "\n--" << "newline";
		else
			out << "\n--" << item;
	}
	out << "\n";
}


/**
* @brief Split the string according to the specified regular expression
* @param input The input string
* @param regex The regular expression
* @return Vector/list of strings
*/
std::vector<std::string> StringHelpers::split_string(const std::string &input, const std::string &regex)
{
	const std::regex re(regex);
	std::sregex_token_iterator first{ input.begin(), input.end(), re, -1 }, last;

	return{ first, last };
}

/**
* @brief Trim spaces at the start and end of the string
* @param input Input string to operate the trim on
* @param find Optional other than default " " to search/trim on
* @return Returns trimmed string or same string if there was nothing to trim
*/
std::string StringHelpers::trim_string(const std::string &input, const std::string &find)
{
	const size_t first = input.find_first_not_of(find);
	if (first == std::string::npos)
	{
		return input;
	}
	const size_t last = input.find_last_not_of(find);

	return input.substr(first, (last - first + 1));
}

/**
* @brief Trim spaces and newline of the string
* @param input Input string
* @return Trimmed string or same string if there was nothing to trim
*/
std::string StringHelpers::trim_string_remove_new_line(const std::string &input)
{
	static std::vector<char> end_checks = { '\r', '\n' };
	std::string intermediate_result = trim_string(input);
	for (const char &c : end_checks)
	{
		(void)string_remove_last_char_if(intermediate_result, c);
	}

	return intermediate_result;
}

/**
* @brief Get (sub)string between two chars, for example [..string..]
* @param input string to search in
* @param start start character (excluding)
* @param end end character (excluding)
* @return returns string between start and end character or empty string in case of failure
*/
std::string StringHelpers::get_string_between_two_chars(const std::string &input, const char start, const char end)
{
	const size_t sstart = input.find(start);
	const size_t send = input.find(end, sstart + 1);

	if ((sstart != std::string::npos) &&
		(send != std::string::npos) &&
		(sstart != send + 1))
	{
		return input.substr(sstart + 1, (send - sstart) - 1);
	}

	return "";
}

/**
* @brief Determine if the input string ends with a character
* @param input string to search in
* @param character character to check
* @return True if the string ends with character if non empty, false otherwise
*/
bool StringHelpers::string_last_char_is(const std::string &input, const char character)
{
	if (input.empty())
	{
		return false;
	}

	const char last_string_character = input[input.size() - 1];
	if (last_string_character == character)
	{
		return true;
	}

	return false;
}

/**
* @brief Determine if the input string starts with a character
* @param input string to search in
* @param character character to check
* @return True if the string starts with character if non empty, false otherwise
*/
bool StringHelpers::string_first_char_is(const std::string &input, const char character)
{
	if (input.empty())
	{
		return false;
	}

	const char first_string_character = input[0];
	if (first_string_character == character)
	{
		return true;
	}

	return false;
}

/**
* @brief Remove last character of a string if it matches a certain character
* @param input The input string to operate on
* @param character The character to check as last char on the input string
* @return True if last character matched & removed, false otherwise
*/
bool StringHelpers::string_remove_last_char_if(std::string &input, const char character)
{
	if (string_last_char_is(input, character))
	{
		input.resize(input.size() - 1);
		return true;
	}

	return false;
}

/**
* @brief Remove first character of a string if it matches a certain character
* @param input The input string to operate on
* @param character The character to check as first char on the input string
* @return True if first character matches & removed, false otherwise
*/
bool StringHelpers::string_remove_first_char_if(std::string &input, const char character)
{
	if (string_first_char_is(input, character))
	{
		input.erase(0, 1);
		return true;
	}

	return false;
}

/**
* @brief Split a string by split_char and only return two parts, ignoring all following split_char
* @param input input data to split on
* @param split_char split character
* @return vector of max two strings, vector of one string if split_char not found
*/
std::vector<std::string> StringHelpers::split_string_once(const std::string &input, const char split_char)
{
	bool split_found = false;
	std::vector<std::string> tmp;
	std::string tmp_string;

	for (const char c : input)
	{
		if (split_found)
		{
			tmp_string.push_back(c);
		}
		else
		{
			if (c == split_char)
			{
				split_found = true;
				tmp.push_back(tmp_string);
				tmp_string.clear();
			}
			else
			{
				tmp_string.push_back(c);
			}
		}
	}

	if (!tmp_string.empty())
	{
		tmp.push_back(tmp_string);
	}

	return tmp;
}