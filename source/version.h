#pragma once

#include "pluginterfaces/base/fplatform.h"

// Описание версии плагина

// Основная версия
#define MAJOR_VERSION_STR "0"
#define MAJOR_VERSION_INT 0

// Дополнительная версия
#define SUB_VERSION_STR "5"
#define SUB_VERSION_INT 5

// Номер выпуска
#define RELEASE_NUMBER_STR "1"
#define RELEASE_NUMBER_INT 1

// Номер сборки
#define BUILD_NUMBER_STR "3"
#define BUILD_NUMBER_INT 3

// Весия с номером сборки (например "1.0.3.342")
#define FULL_VERSION_STR MAJOR_VERSION_STR "." SUB_VERSION_STR "." RELEASE_NUMBER_STR "." BUILD_NUMBER_STR

// Версия без номера сборки (например "1.0.3")
#define VERSION_STR MAJOR_VERSION_STR "." SUB_VERSION_STR "." RELEASE_NUMBER_STR

#define stringOriginalFilename	"mathreverb.vst3" // Название выходного файла
#if PLATFORM_64
#define stringFileDescription	"MathReverb VST3-SDK (64Bit)" // Название при 64Bit архитектуре
#else
#define stringFileDescription	"MathReverb VST3-SDK" // Название при 32Bit архитектуре
#endif
#define stringCompanyName		"BMSTU\0" // Название учереждения
#define stringLegalCopyright	"2017 Dubrovin Egor dubrovin.en@gmail.com" // Информация о разработчике
#define stringLegalTrademarks	"VST is a trademark of Steinberg Media Technologies GmbH" // Информация о правах собственности
