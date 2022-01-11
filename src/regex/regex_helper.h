

//==============================================================================
//
//   regex_helper.h - exported_h
//
//==============================================================================
//  cybercastor - made in quebec 2020 <cybercastor@icloud.com>
//==============================================================================


#ifndef __REGEX_HELPER_H__
#define __REGEX_HELPER_H__

/*
*  Package Operations:
*  -------------------
*  Reads lines from source stream and applies a regular expression to each.
*  Reports matches and returns a collection of lines from the source with
*  matching line, the matched text, and line number.
*
*  RegExHelper accepts on the command line:
*    - path to source file.  If file does not exist, takes default text as source.
*    - regular expression string (see note below)
*    - options:
*      - /m -> show only search matches
*      - /h -> hide source lines during search
*      - /n -> no output, returns true on first match, else false at end
*
*  Note:
*  The windows command line interprets metacharacters (, ), %, !, ^, ", <, >, &, and |.
*  This means that you can't directly put most regular expressions as arguments in the 
*  command line of a console application.  However, the console ^ metacharacter causes
*  the next character to be passed literally, with no interpretation.
*
*  "<a.*?</a>"                       // finds HTML anchor statements
*  "\#include[ ]*?[<\"].+[>\"].*?$"  // finds include statements
*
*  Both of these reqular expressions, above, will cause console to fail to load properly.*
*  Here's how to fix that:
*
*  "^<a.*?^</a^>" will result in the cmd line arg "<a.*?</a>"
*  "\#include[ ]*?[^<\"].+[^>\"].*?$" will result in "\#include[ ]*?[<\"].+[>\"].*?$"
*
*  The command line interpreter seems inconsistant about when it returns the metacharactar ^,
*  so, to get the results above, I removed all of the ^ characters from the argv argument.
*
*  That, of course, would be a problem if you need to search for something with ^.  Fixing that
*  is left as an exercise for students.
*
*  Required Files:
*  ---------------
*  RegExHelper.h, RegExHelper.cpp  // applications only need RegExHelper.h
*  CodeUtilities.h                 // used only by RegExHelper.cpp
*  StringUtilities.h               // used only by RegExHelper.cpp
*
*  Maintenance History:
*  --------------------
*  ver 1.0 : 14 Aug 2018
*  - first release
*
*/

#include <regex>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <memory>
#include <tuple>


// aliases used to increase readability of code

using Line = std::string;
using MatchString = std::string;
using Matches = bool;
using LineCount = size_t;
using MatchLine = std::tuple<Line, MatchString, LineCount, Matches>;
using MatchLines = std::vector<MatchLine>;
using iterator = std::vector<MatchLine>::iterator;
using srcStreamPtr = std::shared_ptr<std::istream>;

enum MatchLineTypes { line = 0, matchString = 1, lineCount = 2, matches = 3 };

///////////////////////////////////////////////////////////////////
// RegExHelper class searches for matches in lines of text
// - extracts lines from source and looks for match in each
// - RegExHelper holds a stream via a std::shared_ptr which
//   is correctly copyable and assignable.
// - std::regex is also copyable and assignable.
// - That means that RegExHelper is by default copyable and assignable.
// - I've reinforced that with the = default declarations, below.
//   They are not needed, but make it obvious that instances are
//   copyable and assignable.

class RegExHelper
{
public:
	RegExHelper();
	RegExHelper(const std::string& re);
	RegExHelper(const RegExHelper&) = default;
	RegExHelper& operator=(const RegExHelper&) = default;

	void regExp(const std::string& regex);
	bool search();
	void showResults();

	bool addSource(srcStreamPtr in);
	bool makeFileStreamSrc(const std::string& filename);
	bool makeStringStreamSrc(const std::string& text);

	void setShowOnlyMatches(bool soml = true);
	bool onlyMatches();
	void setHideSourceLines(bool sml = true);
	bool hideSourceLines();
	void setNoTextOutput(bool nto = true);
	bool noTextOutput();

	MatchLine& operator[](size_t n);
	MatchLine operator[](size_t n) const;
	iterator begin();
	iterator end();
	size_t size();
	void clear();
private:
	Line getLine();
	MatchLines srcLines_;  // std::vector<std::tuple<std::string, std::string, size_t, bool>
	srcStreamPtr src_ = nullptr;
	std::regex regex_;
	bool showOnlyMatches_ = false;
	bool hideSourceLines_ = false;
	bool noTextOutput_ = false;
};



inline RegExHelper::RegExHelper() : regex_() {};



inline RegExHelper::RegExHelper(const std::string& re) : regex_(re) {};



void RegExHelper::regExp(const std::string& regexp)
{
	regex_ = regexp;
}


void RegExHelper::setShowOnlyMatches(bool soml)
{
	showOnlyMatches_ = soml;
}


bool RegExHelper::onlyMatches()
{
	return showOnlyMatches_;
}


void RegExHelper::setHideSourceLines(bool soml)
{
	hideSourceLines_ = soml;
}


bool RegExHelper::hideSourceLines()
{
	return hideSourceLines_;
}


void RegExHelper::setNoTextOutput(bool nto)
{
	noTextOutput_ = nto;
}

/*
*  If no text output, search() returns true on first match,
*  else returns false after searching entire source.
*/
bool RegExHelper::noTextOutput()
{
	return noTextOutput_;
}

/*
*  typical invocation: addSource(new std::ifstream(filename));
*/
inline bool RegExHelper::addSource(srcStreamPtr in)
{
	src_ = in;
	if (!src_->good())
		return false;
	return true;
}


inline bool RegExHelper::makeFileStreamSrc(const std::string& filename)
{
	std::ifstream* sPtr = new std::ifstream(filename);
	if (sPtr->good())
	{
		src_ = srcStreamPtr(sPtr);  // wrap C++ raw ptr in std::shared_ptr
		return true;
	}
	else
	{
		src_ = nullptr;
		return false;
	}
}


inline bool RegExHelper::makeStringStreamSrc(const std::string& text)
{
	std::istringstream* sPtr = new std::istringstream(text);
	if (sPtr->good())
	{
		src_ = srcStreamPtr(sPtr);
		return true;
	}
	else
	{
		src_ = nullptr;
		return false;
	}
}


Line RegExHelper::getLine()
{
	Line temp;
	int ch;
	while (true)
	{
		if (!src_->good())
			break;
		ch = src_->get();
		if (ch != '\n')
			temp.push_back(ch);
		else
			break;
	}
	return temp;
}


inline bool RegExHelper::search()
{
	// MatchLine = std::tuple<Line, MatchString, LineCount, Matches>;

	size_t count = 0;
	do
	{
		++count;
		Line line = getLine();
		std::smatch match;
		bool matches = std::regex_search(line, match, regex_);
		MatchString ms;
		if (matches)
		{
			ms = match[0];  // only returning first match in each line
			if (noTextOutput_)
			{
				clear();
				return true;
			}
		}
		else
		{
			ms = "";
		}
		MatchLine mline = std::make_tuple(line, ms, count, matches);
		srcLines_.push_back(mline);
	} while (src_->good());
	return bool(matches);  // cast eliminates an unnecessary warning
}


void RegExHelper::showResults()
{
	for (auto smartLine : srcLines_)
	{
		// MatchLine = std::tuple<Line, MatchString, LineCount, Matches>;
		Line line = std::get<MatchLineTypes::line>(smartLine);
		MatchString matchStr = std::get<MatchLineTypes::matchString>(smartLine);
		LineCount lineCnt = std::get<MatchLineTypes::lineCount>(smartLine);
		Matches match = std::get<MatchLineTypes::matches>(smartLine);

		if (match)
		{
			if (!hideSourceLines_)
			{
				std::cout << "\n  " << std::setw(4) << std::right << lineCnt << " ";
				std::cout << std::left << "\"" + line + "\"";
			}
			std::cout << "\n  " << matchStr;
		}
		else
		{
			if (!showOnlyMatches_ && !hideSourceLines_)
			{
				std::cout << "\n  " << std::setw(4) << std::right << lineCnt << " ";
				std::cout << std::left << "\"" + line + "\"";
			}
		}
	}

}


inline MatchLine& RegExHelper::operator[](size_t n)
{
	return srcLines_[n];
}


inline MatchLine RegExHelper::operator[](size_t n) const
{
	return srcLines_[n];
}


inline iterator RegExHelper::begin()
{
	return srcLines_.begin();
}


inline iterator RegExHelper::end()
{
	return srcLines_.end();
}


inline size_t RegExHelper::size()
{
	return srcLines_.size();
}


inline void RegExHelper::clear()
{
	srcLines_.clear();
}


inline void stripHatMC(std::string& regexp)
{
	regexp.erase(std::remove(regexp.begin(), regexp.end(), '^'), regexp.end());
}


#endif //__REGEX_HELPER_H__