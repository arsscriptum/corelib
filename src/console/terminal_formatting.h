
//==============================================================================
//
//  terminal_formatting.h - exported_h
//
//==============================================================================
//  cybercastor - made in quebec 2020 <cybercastor@icloud.com>
//==============================================================================


#ifndef __TERMINAL_FORMATTING_H__
#define __TERMINAL_FORMATTING_H__


#include <Windows.h>
#include <functional>
#include <iostream>
#include <sstream>
#include <iterator>
#include <stack>
#include <regex>
#include <string>
#include "baseterminal.h"
#include "console\log.h"

const std::string ANSI_TEXT_COLOR_RESET = "\u001b[0m";
const std::string ANSI_TEXT_COLOR_BLACK = "\u001b[30m";
const std::string ANSI_TEXT_COLOR_RED = "\u001b[31m";
const std::string ANSI_TEXT_COLOR_GREEN = "\u001b[32m";
const std::string ANSI_TEXT_COLOR_YELLOW = "\u001b[33m";
const std::string ANSI_TEXT_COLOR_BLUE = "\u001b[34m";
const std::string ANSI_TEXT_COLOR_MAGENTA = "\u001b[35m";
const std::string ANSI_TEXT_COLOR_CYAN = "\u001b[36m";
const std::string ANSI_TEXT_COLOR_WHITE = "\u001b[37m";
const std::string ANSI_TEXT_COLOR_BLACK_BRIGHT = "\u001b[30;1m";
const std::string ANSI_TEXT_COLOR_RED_BRIGHT = "\u001b[31;1m";
const std::string ANSI_TEXT_COLOR_GREEN_BRIGHT = "\u001b[32;1m";
const std::string ANSI_TEXT_COLOR_YELLOW_BRIGHT = "\u001b[33;1m";
const std::string ANSI_TEXT_COLOR_BLUE_BRIGHT = "\u001b[34;1m";
const std::string ANSI_TEXT_COLOR_MAGENTA_BRIGHT = "\u001b[35;1m";
const std::string ANSI_TEXT_COLOR_CYAN_BRIGHT = "\u001b[36;1m";
const std::string ANSI_TEXT_COLOR_WHITE_BRIGHT = "\u001b[37;1m";
const std::string ANSI_TEXT_COLOR_BLACK_BKGRND = "\u001b[40m";
const std::string ANSI_TEXT_COLOR_RED_BKGRND = "\u001b[41m";
const std::string ANSI_TEXT_COLOR_GREEN_BKGRND = "\u001b[42m";
const std::string ANSI_TEXT_COLOR_YELLOW_BKGRND = "\u001b[43m";
const std::string ANSI_TEXT_COLOR_BLUE_BKGRND = "\u001b[44m";
const std::string ANSI_TEXT_COLOR_MAGENTA_BKGRND = "\u001b[45m";
const std::string ANSI_TEXT_COLOR_CYAN_BKGRND = "\u001b[46m";
const std::string ANSI_TEXT_COLOR_WHITE_BKGRND = "\u001b[47m";

// ------------------------------------------------------//
/* --------- End of Line Formatting ------------------- */
// ------------------------------------------------------//
// This modifier wraps the text in a begin / end string pair
// This operates on a single line of code automatically.
// For example with
// EndOfLineEscapeTag HTML {"<HTML>", "</HTML>"};
// cout << HTML << myHtmlString << std::endl;
// will output <HTML> ((myHTMLString)) \n </HTML>
//
// EndOfLineEscapeStreamScope formatter by Steve132

typedef std::pair<std::string, std::string> EndOfLineEscapeTag;
/*
struct EndOfLineEscapeStreamScope
{
  protected:
    EndOfLineEscapeTag tag;
    std::ostream &os;

    EndOfLineEscapeStreamScope(const EndOfLineEscapeTag &ttag, std::ostream &tout) : tag(ttag),
                                                                                     os(tout)
    {
        os << tag.first; //you can overload this for custom ostream types with a different color interface
        //this might also have a stack interface for if you need multiple resets
    }
    friend EndOfLineEscapeStreamScope operator<<(std::ostream &out, const EndOfLineEscapeTag &tg);

  public:
    template <class T>
    EndOfLineEscapeStreamScope &operator<<(T &&t)
    {
        os << std::forward<T>(t);
        return *this;
    }

    EndOfLineEscapeStreamScope &operator<<(std::ostream &(&M)(std::ostream &))
    {
        M(os);
        return *this;
    }

    ~EndOfLineEscapeStreamScope()
    {
        os << tag.second;
    }
};

inline EndOfLineEscapeStreamScope operator<<(std::ostream &os, const EndOfLineEscapeTag &tg)
{
    return EndOfLineEscapeStreamScope(tg, os);
}
*/


// ------------------------------------------------------//
/* --------- Streambuf Swapper-------------------------- */
// ------------------------------------------------------//

/// streambuf_swapper
/// Steve132
/// RAII method of swapping a global iostream buffer
/// https://stackoverflow.com/questions/14860267/what-can-go-wrong-if-cout-rdbuf-is-used-to-switch-buffer-and-never-set-it-back
struct streambuf_swapper {
    std::ios &m_s;
    std::streambuf *m_buf;

    streambuf_swapper(std::ios &s, std::streambuf *buf) : m_s(s), m_buf(s.rdbuf(buf)) {}
    ~streambuf_swapper() { m_s.rdbuf(m_buf); }
};

// ------------------------------------------------------//
/* --------- Windows terminal setter ------------------- */
// ------------------------------------------------------//





struct RegexFormatter
{
    struct Rule
    {
        std::regex rule;
        std::function<std::string (const std::string&)> filter = DO_NOTHING;

        inline static std::string DO_NOTHING(const std::string& s){return s;}
    };

    typedef std::vector<Rule> RuleSet;
    
    RuleSet rules;

    std::string matched(Rule &tok, const std::string &str)
    {
        return tok.filter(str);
    }

    std::string unmatched(const std::string &str)
    {
        return str;
    }

    std::string format(const std::string &str)
    {
        std::stringstream sstr;
        
        struct SearchParams
        {
            std::smatch match;
            std::size_t searchStartedAt = 0;
            std::string result;
            std::string prefix;
            std::string suffix;
            bool hasResult = true;

            std::size_t offsetOfMatch() const
            {
                return match.prefix().length() + searchStartedAt;
            }
        };

        std::vector<SearchParams> matches;

        matches.resize(rules.size());

        int segmentStart = 0;
        int priorityMatch = -1;

        do
        {
            int lastPriorityMatch = priorityMatch;
            priorityMatch = -1;

            unsigned int startLocation = 0xffffffff;

            for (int i = 0; i < rules.size(); i++)
            {
                SearchParams &match = matches[i];

                if (!match.hasResult)
                    continue;

                // update the next search result for the regex we chose, or for any regex whose latest result
                // is now behind the edge of the string we've consumed
                if ((i == lastPriorityMatch) || (match.offsetOfMatch() < segmentStart) || (!segmentStart))
                {
                    match.hasResult = std::regex_search((str.begin() + segmentStart), str.end(), match.match, rules[i].rule);

                    match.searchStartedAt = segmentStart;

                    if (match.hasResult)
                    {
                        //std::cout << "\tpossible match "<<match.match.str()<<std::endl;
                        match.prefix = match.match.prefix();
                        match.suffix = match.match.suffix();
                        match.result = match.match.str();
                    }
                }

                // get the nearest for any
                if (match.hasResult && match.offsetOfMatch() < startLocation)
                {
                    priorityMatch = i;
                    std::size_t off = match.offsetOfMatch();
                    startLocation = (uint32_t)off;
                }
            }

            if (priorityMatch == -1)
            {
                sstr << unmatched(str.substr(segmentStart, str.size() - segmentStart));

                return sstr.str();
            }

            // handle first match

            const std::string prefix = matches[priorityMatch].prefix;
            const std::string matchStr = matches[priorityMatch].result;

            if (prefix.length())
            {
                std::size_t offset = segmentStart - matches[priorityMatch].searchStartedAt;
                if (prefix.length() > offset && prefix.length())
                {
                    std::size_t len = prefix.length() - offset;
                    sstr << unmatched(prefix.substr(offset, len));
                }
            }

            sstr << matched(rules[priorityMatch], matchStr);
            
            segmentStart = (uint32_t)matches[priorityMatch].offsetOfMatch() + matchStr.length();

        } while (true);
    }
};


inline std::string makeKeywordsRegexStr(const std::string keywords[], std::size_t numKeywords)
{
    std::stringstream sstr;

    sstr << "\\b" << keywords[0] << "\\b";
    for (int i = 1; i < numKeywords; i++)
    {
        sstr << '|' << "\\b" << keywords[i] << "\\b";
    }

    return (sstr.str());
}

// use std bind to set this as a 'filter' for regex formatter.  see guiTest.cpp example in 'demos' folder for example glsl syntax highlighting
inline std::string highlightKeyword(const std::string& format, const std::string& str)
{
    return format + str + "\u001b[0m";
}

// ----------------------
// -------- MISC --------
// ----------------------

struct membuf: std::streambuf
{
    membuf(const char* begin, const char* end)
    {
         this->setg((char*)begin, (char*)begin, (char*)end);
    }
};



#endif //__TERMINAL_FORMATTING_H__