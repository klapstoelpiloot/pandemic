#pragma once
#include <memory>
#include <vector>
#include <mutex>
#include <functional>

#define _MAKE_WIDE_CHAR_STRING(_String) (L ## _String)
#define MAKE_WIDE_CHAR_STRING(_String) _MAKE_WIDE_CHAR_STRING(_String)

#define _MAKE_STRING(_String) #_String
#define MAKE_STRING(_String) _MAKE_STRING(_String)

#define NOT_IMPLEMENTED ASSERT(false, "This is not yet implemented.");
#define STATIC_NOT_IMPLEMENTED static_assert(false, "This is not yet implemented.");

#define NOT_SUPPORTED ASSERT(false, "This operation is not supported.");

#define SAFE_DELETE(p)       { if((p) != nullptr) { delete (p); (p) = nullptr; } }
#define SAFE_DELETE_ARRAY(p) { if((p) != nullptr) { delete[] (p); (p) = nullptr; } }
#define SAFE_FREE(p)      { if((p) != nullptr) { free(p); (p) = nullptr; } }

#define FOR_EACH_1(WHAT, X) WHAT(X) 
#define FOR_EACH_2(WHAT, X, ...) WHAT(X)FOR_EACH_1(WHAT, __VA_ARGS__)
#define FOR_EACH_3(WHAT, X, ...) WHAT(X)FOR_EACH_2(WHAT, __VA_ARGS__)
#define FOR_EACH_4(WHAT, X, ...) WHAT(X)FOR_EACH_3(WHAT, __VA_ARGS__)
#define FOR_EACH_5(WHAT, X, ...) WHAT(X)FOR_EACH_4(WHAT, __VA_ARGS__)
#define FOR_EACH_6(WHAT, X, ...) WHAT(X)FOR_EACH_5(WHAT, __VA_ARGS__)
#define FOR_EACH_7(WHAT, X, ...) WHAT(X)FOR_EACH_6(WHAT, __VA_ARGS__)
#define FOR_EACH_8(WHAT, X, ...) WHAT(X)FOR_EACH_7(WHAT, __VA_ARGS__)
#define FOR_EACH_9(WHAT, X, ...) WHAT(X)FOR_EACH_8(WHAT, __VA_ARGS__)
#define FOR_EACH_10(WHAT, X, ...) WHAT(X)FOR_EACH_9(WHAT, __VA_ARGS__)
#define MACRO_BY_ARGS_COUNT(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, NAME, ...) NAME
#define FOR_EACH(action, ...) \
  MACRO_BY_ARGS_COUNT(__VA_ARGS__, FOR_EACH_10, FOR_EACH_9, FOR_EACH_8, FOR_EACH_7, FOR_EACH_6, FOR_EACH_5, FOR_EACH_4, FOR_EACH_3, FOR_EACH_2, FOR_EACH_1)(action, __VA_ARGS__)

// Types we commonly use.
typedef uint16_t ushort;
typedef uint32_t uint;
typedef int64_t int64;
typedef uint64_t uint64;
typedef uint8_t byte;
typedef int8_t sbyte;
typedef unsigned char uchar;

// Common types from other namespaces.
// List them here instead of 'using namespace'!
template<class T> using ptr = std::shared_ptr<T>;
using std::static_pointer_cast;
using std::dynamic_pointer_cast;
template<class T, class A = std::allocator<T>> using vector = std::vector<T, A>;
using mutex = std::mutex;
using recursive_mutex = std::recursive_mutex;
template<class T> using lock_guard = std::lock_guard<T>;
template<class T> using unique_guard = std::unique_lock<T>;
template<class T> using atomic = std::atomic<T>;
namespace ch = std::chrono;
typedef ch::high_resolution_clock Clock;
typedef ch::time_point<Clock> TimePoint;
using namespace std::placeholders;

namespace std::chrono
{
	inline int64 ToMilliseconds(ch::nanoseconds duration)
	{
		return ch::duration_cast<ch::milliseconds>(duration).count();
	}

	inline int64 ToMilliseconds(TimePoint time)
	{
		return ch::duration_cast<ch::milliseconds>(time.time_since_epoch()).count();
	}

	inline bool IsTimeSet(TimePoint time)
	{
		return time.time_since_epoch() != TimePoint::duration(0);
	}
}

namespace std
{
	// Tool to easily (and correctly) combine hashes together. Taken from boost.
	template <class T>
	inline void hash_combine(std::size_t& seed, const T& v)
	{
		std::hash<T> hasher;
		seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
	};
}
