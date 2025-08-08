#pragma once 

// Clang attributes
// https://clang.llvm.org/docs/AttributeReference.html#always-inline-force-inline
// Clang builtins
// https://clang.llvm.org/docs/LanguageExtensions.html

// Msvc attributes
// https://learn.microsoft.com/en-us/cpp/cpp/declspec?view=msvc-170
// Msvc SAL
// https://learn.microsoft.com/en-us/cpp/code-quality/using-sal-annotations-to-reduce-c-cpp-code-defects?view=msvc-170

// Gcc attributes
// https://ohse.de/uwe/articles/gcc-attributes.html and https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html

#include <base/core/compatibility/AlignmentMacros.h>
#include <base/core/compatibility/BranchPrediction.h>

#include <base/core/compatibility/CallingConventions.h>
#include <base/core/compatibility/CompilerVisibility.h>

#include <base/core/compatibility/CxxVersionDetection.h>
#include <base/core/compatibility/FunctionAttributes.h>

#include <base/core/compatibility/Inline.h>
#include <base/core/compatibility/MemoryMacros.h>

#include <base/core/compatibility/MsvcSpecificMacros.h>
#include <base/core/compatibility/Nodiscard.h>

#include <base/core/compatibility/UnreachableCode.h>
#include <base/core/compatibility/Warnings.h>

#include <base/core/compatibility/CompilerDetection.h>
#include <base/core/compatibility/LanguageFeatures.h>

base_disable_warning_msvc(4067)