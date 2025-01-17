

//==============================================================================
//
//   regex_helper.cpp
//
//==============================================================================
//  Copyright (C) Guilaume Plante 2020 <cybercastor@icloud.com>
//==============================================================================


/*
*  Options:
*  --------
*  /
*/
#ifdef TEST_REGEXHELPER

#include "RegExHelper.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/CodeUtilities/CodeUtilities.h"
#include <iomanip>

using namespace RegExUtilities;
using namespace Utilities;

std::string defaultSourceText()
{
  std::string testStr = "  You have a problem that involves searching text,\n  so you decide to use Regular Expressions.";
  testStr += "\n  Now you have two problems.\n";
  testStr += "\n  This is a link: <a href=\"#\">a link</a> that refers\n  to another place on the same page.";
  testStr += "\n  <a href=\"r.txt\">another link</a> and <a href=\"s.txt\">more</a>";
  testStr += "\n  #include <regex>  // first include\n  #include \"RegExHelper.h\"  // second include\n";
  return testStr;
}

int main(int argc, char* argv[])
{
  Title("Demonstating RegExHelper");

  //----< set default regular expression >---------------------------

  std::string regexp_anchor = "<a.*?</a>";                   // finds anchor statements
  std::string regexp_include = "\#include[ ]*?[<\"].+[>\"]";  // finds #include statements
  std::string regexp = regexp_anchor;    // default regular expression

  ProcessCmdLine pcl(argc, argv);

  preface("Command Line: ");
  pcl.showCmdLine();

  /*
    Note:
    The windows command line interprets metacharacters (, ), %, !, ^, ", <, >, &, and |. 
    This means that you can't directly put most regular expressions as arguments in the command line.
    However, the ^ metacharacter causes the next character to be passed literally, with no interpretation.
    
    Both of the reqular expressions, above, will cause the console to fail to load properly.
    Here's how to fix that:

      "^<a.*?^</a^>" will result in the cmd line arg "<a.*?</a>"
      "\#include[ ]*?[^<\"].+[^>\"].*?$" will result in "\#include[ ]*?[<\"].+[>\"].*?$"

    The command line interpreter seems inconsistant about when it returns the metacharactar ^,
    so, to get the results above, I removed all of the ^ characters from the argument.

    That, of course, would be a problem if you need to search for something with ^.  Fixing that 
    is left as an exercise for students.
  */

  RegExHelper reh;

  //----< Attempt to extract regular expression from cmd line >------
  //
  //   If that fails, use default, already set above.

  ProcessCmdLine::Patterns patterns = pcl.patterns();  // only expects one pattern
  if (patterns.size() > 0)
  {
    regexp = patterns[0];
    stripHatMC(regexp);
  }

  try {
    reh.regExp(regexp);
  }
  catch (...) {
    std::cout << " - invalid RegExp\n\n";
    return 0;
  }

  std::cout << "\n  regexp: \"" << regexp << "\"\n";

  //----< extract search options from command line >-----------------

  ProcessCmdLine::Options attribs = pcl.options();
  if (pcl.hasOption('m'))
    reh.setShowOnlyMatches(true);
  if (pcl.hasOption('h'))
    reh.setHideSourceLines(true);
  if (pcl.hasOption('n'))
    reh.setNoTextOutput(true);

  //----< Attempt to open source file >------------------------------
  //
  //    If that fails, use default source text.

  bool fileOpenSucceeded = false;
  std::string path = pcl.path();

  if (reh.makeFileStreamSrc(path))
  {
    std::cout << "\n  Analyzing " << path;
    fileOpenSucceeded = true;
  }
  else
  {
    std::cout << "\n  invalid path, so analyzing default text";
  }

  if (!fileOpenSucceeded)
  {
    std::cout << "\n  Source Text:\n" << defaultSourceText() << "\n";

    reh.makeStringStreamSrc(defaultSourceText());
  }
  
  if(reh.search())
    std::cout << "\n  found match\n";
  
  reh.showResults();

  std::cout << "\n\n";
  std::cout << "press enter to quit: ";
  std::cin.get();
  return 0;
}

#endif
