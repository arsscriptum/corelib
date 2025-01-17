//==============================================================================
//
//   file_utilities.h - exported_h
//
//==============================================================================
//  Copyright (C) Guilaume Plante 2020 <cybercastor@icloud.com>
//==============================================================================

#ifndef __FILE_UTILITIES_H__
#define __FILE_UTILITIES_H__

/*
* Package Operations:
* -------------------
* This package provides three global functions:
* - displayFileContents(msg, fileSpec, ostream)
*   Writes msg, then file contents to ostream
* - showDirContents(path, msg, patterns, ostream)
*   Display msg, then contents of directory path matching patterns.
* - showFileLines(path, startLine, endLine, msg, ostream)
*   Display msg, then segment of file between startLine and endLine.
*/

#include <iostream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>   // defines std::getline(std::istream, std::string)
#include <vector>
#include <string>
#include <algorithm>
#include "string\string_utilities.h"
#include "file_system.h"
#include "log.h"
using File = std::string;
using Message = std::string;
using Line = size_t;
using PathStr = std::string;
using Files = std::vector<File>;
using Pattern = std::string;
using Patterns = std::vector<Pattern>;

  ///////////////////////////////////////////////////////////////////
  // Defined in UtilitiesEnvironment/Environment.h
  // using PathStr = std::string;
  // using File = std::string;
  // using Files = std::vector<File>;
  // using Pattern = std::string;
  // using Patterns = std::vector<Pattern>;

  ///////////////////////////////////////////////////////////////////
  // displayFileContents function
  // - attempts to open fileSpec
  // - if successful, reads lines until ostream.good() returns false
  // - displays each line on the console

  inline bool displayFileContents(
    const std::string& msg,        // msg to display first
    const std::string& fileSpec,   // path to file
    std::ostream& out = std::cout  // stream to write output
  )
  {
    if (msg.size() > 0)
    {
      out << "\n\n  " << msg.c_str();
    }
    std::ifstream inFileStrm_(fileSpec);
    if (!inFileStrm_.good())
    {
		COUTERR("cannot open file %s", fileSpec.c_str());
		return false;
    }
	
    while (inFileStrm_.good())
    {
      std::string line;
      std::getline(inFileStrm_, line);
	  COUTRS(line.c_str());
    }
	
    return true;
  }

  //----< show all C++ source files in path >------------------------

  inline bool showDirContents(
    const PathStr& path, 
    const Message& msg = "",
    const Patterns& patterns = Patterns(),
    std::ostream& out = std::cout
  )
  {
	//  if (msg.size() > 0) {
	//	  title(msg);
	//  }
    out << "\n  " << FileSystem::Path::getFullFileSpec(path);
    if (!FileSystem::Directory::exists(path))
    {
      out << "\n  path does not exist";
      return false;
    }
    Patterns tempPatts = patterns;
    if (tempPatts.size() == 0)
      tempPatts.push_back("*.*");
    Files files;
    for (auto patt : tempPatts)
    {
      Files currentFiles = FileSystem::Directory::getFiles(path, patt);
      files.insert(files.end(), currentFiles.begin(), currentFiles.end());
    }
    std::sort(files.begin(), files.end());
    for (auto file : files)
    {
      out << "\n    " << file;
    }
    out << "\n";
    return true;
  }
  //----< show specified contiguous lines from a code file >---------

  inline bool showFileLines(
    const PathStr& path, 
    Line start, Line end, 
    const Message& msg = "", 
    std::ostream& out = std::cout
  )
  {
  //  if (msg.size() > 0)
    //  title(msg);
   // out << "\n  " << FileSystem::Path::getFullFileSpec(path);

    std::ifstream file(path);
    if (!file.good())
    {
      out << "\n  can't open file";
      return false;
    }

    std::string line;
    size_t count = 0;
    while (std::getline(file, line))
    {
      if (++count < start) continue;
      if (count > end) break;
      out << "\n  " << std::setw(4) << count << " " << line;
    }
    out << "\n";
    return true;
  }



  std::string getFileContents(const std::string& fileSpec)
  {
	  std::string fileContent = "";
	  std::ifstream inFileStrm_(fileSpec);
	  if (!inFileStrm_.good()){
		  return "";
	  }

	  while (inFileStrm_.good())
	  {
		  std::string line;
		  std::getline(inFileStrm_, line);
		  fileContent += line;
	  }

	  return fileContent;
  }

#endif //__FILE_UTILITIES_H__