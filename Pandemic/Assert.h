#pragma once
#include "Defines.h"

#ifndef DEBUG
	#define __assert(x,y,z) false
#endif

#ifdef __cplusplus
extern "C"
{
#endif
	void __OnAssert(const char* file, unsigned line, const char* description);
#ifdef __cplusplus
}
#endif

// Assert can be used to test an expression which is expected to be true.
// When the expression if false, the specified description is given as the error message.
#define ASSERT(test_, description_) { ((test_) ? (void)0 : __OnAssert(MAKE_STRING(__FILE__), __LINE__, description_)); }

// Require is used as a sanity check on function parameters.
#define REQUIRE(test_) ASSERT((test_), MAKE_STRING(#test_))

// Ensure is used as a sanity check on return values and expressions.
#define ENSURE(test_) ASSERT((test_), MAKE_STRING(#test_))

// Fail is used for code that should not be reached (such as the default case, or else block)
#undef FAIL
#define FAIL(description_) ASSERT(false, description_)
