#pragma once

#include "pluginterfaces/base/fplatform.h"

// Plugin version

// Major version
#define MAJOR_VERSION_STR "2"
#define MAJOR_VERSION_INT 2

// Sub version
#define SUB_VERSION_STR "0"
#define SUB_VERSION_INT 0

// Release number
#define RELEASE_NUMBER_STR "0"
#define RELEASE_NUMBER_INT 0

// Build number
#define BUILD_NUMBER_STR "1"
#define BUILD_NUMBER_INT 1

// Ver with build number (example "1.0.3.342")
#define FULL_VERSION_STR MAJOR_VERSION_STR "." SUB_VERSION_STR "." RELEASE_NUMBER_STR "." BUILD_NUMBER_STR

// Ver without build number (example "1.0.3")
#define VERSION_STR MAJOR_VERSION_STR "." SUB_VERSION_STR "." RELEASE_NUMBER_STR

#define stringOriginalFilename	"mathreverb.vst3" // Output filename
#if PLATFORM_64
#define stringFileDescription	"MathReverb (64Bit)" // 64Bit name
#else
#define stringFileDescription	"MathReverb" // 32Bit name
#endif
#define stringCompanyName		"BMSTU\0" // Company
#define stringLegalCopyright	"2017 Dubrovin Egor dubrovin.en@gmail.com" // Developer info
#define stringLegalTrademarks	"VST is a trademark of Steinberg Media Technologies GmbH" // Legal
