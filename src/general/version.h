
//==============================================================================
//
//   version.h - exported_h
//
//==============================================================================
//  Copyright (C) Guilaume Plante 2020 <cybercastor@icloud.com>
//==============================================================================



#ifndef __VERSION_H__
#define __VERSION_H__


#ifndef CORE_MAKE_VERSION
// Generates generic API versions
#define CORE_MAKE_VERSION( _major, _minor )  (( _major << 16 )|( _minor & 0x0000ffff))
#endif // CORE_MAKE_VERSION

///////////////////////////////////////////////////////////////////////////////
#ifndef CORE_MAJOR_VERSION
// Extracts major version
#define CORE_MAJOR_VERSION( _ver )  ( _ver >> 16 )
#endif // CORE_MAJOR_VERSION

///////////////////////////////////////////////////////////////////////////////
#ifndef CORE_MINOR_VERSION
// Extracts minor version
#define CORE_MINOR_VERSION( _ver )  ( _ver & 0x0000ffff )
#endif // CORE_MINOR_VERSION


///////////////////////////////////////////////////////////////////////////////
// Supported API versions

typedef enum _core_api_version_t
{
    CORE_API_VERSION_1_0 = CORE_MAKE_VERSION( 1, 0 ),   ///< version 1.0
    CORE_API_VERSION_1_1 = CORE_MAKE_VERSION( 1, 1 ),   ///< version 1.1
    CORE_API_VERSION_1_2 = CORE_MAKE_VERSION( 1, 2 ),   ///< version 1.2
    CORE_API_VERSION_CURRENT = CORE_MAKE_VERSION( 1, 2 ),   ///< latest known version
    CORE_API_VERSION_FORCE_UINT32 = 0x7fffffff

} core_api_version_t;



#endif // __VERSION_H__
