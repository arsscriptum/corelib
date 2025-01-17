//==============================================================================
//
//   file_utilities.cpp
//
//==============================================================================
//  Copyright (C) Guilaume Plante 2020 <cybercastor@icloud.com>
//==============================================================================

#include "file_utilities.h"

#ifdef TEST_FILEUTILITIES

using namespace Utilities;

int main()
{
  showDirContents(".", "test showDirContents");
  displayFileContents("test displayFileContents", "./FileUtilities.h");
  showFileLines("fileUtilities.h", 0, 10, "test showFileLines");
  return 0;
}

#endif
