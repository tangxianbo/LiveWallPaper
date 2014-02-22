#pragma once

#include <cmath>
#include <limits>
#include <core/types.h>
#include <type_traits>


namespace jenny
{

const f32 ROUNDING_ERROR_32 = 0.000001f;
const f64 ROUNDING_ERROR_64 = 0.00000001;

#ifdef PI 
#    undef PI
#endif

#ifdef PI64 
#    undef PI64
#endif

const f32 PI		= 3.14159265359f;
const f32 PI_INV	= 1.0f / PI;
const f32 PI_2		= PI * 0.5f;
const f32 TWO_PI	= 2.0f * PI;
const f64 PI64		= 3.1415926535897932384626433832795028841971693993751;

const f64 RECIPROCAL_PI64	= 1.0/PI64;
const f32 DEGTORAD			= PI / 180.0f;
const f32 RADTODEG		= 180.0f / PI;
const f64 DEGTORAD64	= PI64 / 180.0;
const f64 RADTODEG64	= 180.0 / PI64;


template <typename T>
inline T DegToRad(T x) {
	return x * T(0.0174532925);
}

template <typename T>
inline T RadToDeg(T x) {
	return x * T(57.2957795);
}

template <typename T>
inline T Clamp(T x, T low, T high) {
	return (x > high) ? high : ((x < low) ? low : x);
}

template <typename T>
inline T Sin(T x) {
	return std::sin(x);
}

template <typename T>
inline T Cos(T x) {
	return std::cos(x);
}

template <typename T>
inline T Tan(T x) {
	return std::tan(x);
}

template <typename T>
inline T Acos(T x) {
	return std::acos(x);
}

template <typename T>
inline T SafeAcos(T x) {
	return std::acos(Clamp(x, T(-1), T(1)));
}

template <typename T>
inline T Asin(T x) {
	return std::asin(x);
}

template <typename T>
inline T SafeAsin(T x) {
	return std::asin(Clamp(x, T(-1), T(1)));
}

template <typename T>
inline T Atan(T x) {
	return std::atan(x);
}

template <typename T>
inline T Atan2(T y, T x) {
	return std::atan2(y, x);
}

template <typename T>
inline T Sqrt(T x) {
	return std::sqrt(x);
}

inline int Sqrt(int x) {
	int sqrt = 0;

	for (int i = 0x10000000; i != 0; i >>= 2) {
		int temp = sqrt + i;
		sqrt >>= 1;

		if (temp <= x) {
			x -= temp;
			sqrt += i;
		}
	}

	return sqrt;
}

template <typename T>
inline T Fmod(T x, T y) {
	return std::fmod(x, y);
}

template <typename T>
inline T Ceil(T x) {
	return std::ceil(x);
}

template <typename T>
inline T Floor(T x) {
	return std::floor(x);
}

template <typename T>
inline T Log(T x) {
	return std::log(x);
}

template <typename T>
inline T Log10(T x) {
	return glf::Log(x) / glf::Log(T(10));
}

inline int Log10(int x) {
	return (int)(Log(float(x)) / Log(10.f));
}

template < typename T >
inline
void
minmax_(const T& a, const T& b, T& theMin, T& theMax)
{
	if(a < b)
	{
		theMin = a;
		theMax = b;
	}
	else
	{
		theMin = b;
		theMax = a;
	}
}


template < typename T >
inline
void
minmax_(const T& a, const T& b, const T& c, T& theMin, T& theMax)
{
	if(a < b)
	{
		if(a < c) // a < b, a < c
		{
			theMin = a;
			theMax = max_(b, c);
		}
		else // c <= a < b
		{
			theMin = c;
			theMax = b;
		}
	}
	else if(c < a) // b <= a, c < a
	{
		theMin = min_(b, c);
		theMax = a;
	}
	else // b <= a <= c
	{
		theMin = b;
		theMax = c;
	}
}


template< typename T >
inline
T
lerp(const T& a, const T& b, const f32 t)
{
	return T(a + (b - a) * t);
}

template< typename T >
inline
T
smoothStep (T a, T b, f32 t)
{
	f32 t2 = t * t;
	f32 t3 = t2 * t;

	f32 f = 3 * t2 - 2 * t3;

	return lerp(a, b, f);
}

template<typename T>
inline
T
catmullRom(const T& p0, const T& p1, const T& m0, const T& m1, const f32 t)
{
	const T t2 = t * t;
	const T t3 = t2 * t;
	const T a = 2.0f * t3 - 3.0f * t2;
	return p0 * (a + 1.0f) + m0 * (t3 - 2.0f * t2 + t) - p1 * a + m1 * (t3 - t2);
}


template<typename T>
inline
T
circularIndex(T i, const u32 size)
{
	while(i < 0)
	{
		i += size;
	}
	return i % size;
}


inline
bool
equals(const f32 a, const f32 b, const f32 tolerance = ROUNDING_ERROR_32)
{
	return (a + tolerance >= b) && (a - tolerance <= b);
}


inline
bool
equals(const s32 a, const s32 b, const s32 tolerance = 0)
{
	return (a + tolerance >= b) && (a - tolerance <= b);
}


inline
bool
equals(const u32 a, const u32 b, const u32 tolerance = 0)
{
	return (a + tolerance >= b) && (a - tolerance <= b);
}


inline
bool
iszero(const f32 a, const f32 tolerance = ROUNDING_ERROR_32)
{
	return std::abs(a) <= tolerance;
}


inline
bool
iszero(const double a, const double tolerance = ROUNDING_ERROR_64)
{
	return std::abs(a) <= tolerance;
}


inline
bool
iszero(const s32 a, const s32 tolerance = 0)
{
	return (a & 0x7ffffff) <= tolerance;
}

inline
bool
iszero(const u32 a, const u32 tolerance = 0)
{
	return a <= tolerance;
}

inline
f32
round_(f32 x)
{
	return std::floor(x + 0.5f);
}

template < typename F >
inline
s32
floor32(const F& v)
{
	return s32(glf::Floor(v));
}

template < typename F >
inline
s32
ceil32(const F& v)
{
	return s32(glf::Ceil(v));
}

template < typename F >
inline
s32
round32(const F& v)
{
	return s32(v + F(0.5));
}

inline
f32
reciprocal_squareroot(const f32 x)
{
#ifdef JENNY_FAST_MATH
	u32 tmp = (u32(0x3f800000 << 1) + 0x3f800000 - *(u32*)&x) >> 1;
	f32 y = *(f32*)&tmp;
	return y * (1.47f - 0.47f * x * y * y);
#else
	return 1.f / Sqrt(x);
#endif
}

template < typename Type >
inline
Type
pow2(Type v)
{
	return v * v;
}

template < typename Type >
inline
Type
pow3(Type v)
{
	return v * v * v;
}

inline
f32
fract(f32 x)
{
	return x - std::floor(x);
}

template < typename IntType >
inline
bool
isPowerOfTwo(IntType v)
{
	return (v & (v - 1)) == 0;
}

template < typename IntType >
inline
IntType
log2i(IntType v)
{
	IntType result = IntType(-1);
	while(v)
	{
		v >>= 1;
		++result;
	}
	return result;
}

template < typename IntType >
inline
IntType
roundup2multiple(IntType v, IntType base)
{
	return (v + (base - 1)) / base * base;
}

template < typename IntType >
inline
IntType
roundup2powerOfTwo(IntType v)
{
	IntType result = 1;

	while(result < v)
	{
		result <<= 1;
	}

	return result;
}

template < typename T >
inline
int sign(T v)
{
	return v < T(0) ? -1 : 1;
}

template < typename IntType >
inline
IntType
ensureAlign(IntType v, IntType a)
{
	IntType m = v % a;
	return v + ((a - m) % a);
}

// Returns a int where only the highest bit is set

template < typename IntType >
inline
typename std::enable_if<sizeof(IntType) == 1, IntType>::type
highestBitMask(IntType i)
{
	i |= i >> 1;
	i |= i >> 2;
	i |= i >> 4;

	i ^= i >> 1;

	return i;
}

template < typename IntType >
inline
typename std::enable_if<sizeof(IntType) == 2, IntType>::type
highestBitMask(IntType i)
{
	i |= i >> 1;
	i |= i >> 2;
	i |= i >> 4;
	i |= i >> 8;

	i ^= i >> 1;

	return i;
}

template < typename IntType >
inline
typename std::enable_if<sizeof(IntType) == 4, IntType>::type
highestBitMask(IntType i)
{
	i |= i >> 1;
	i |= i >> 2;
	i |= i >> 4;
	i |= i >> 8;
	i |= i >> 16;

	i ^= i >> 1;

	return i;
}

template < typename IntType >
inline
typename std::enable_if<sizeof(IntType) == 8, IntType>::type
highestBitMask(IntType i)
{
	i |= i >> 1;
	i |= i >> 2;
	i |= i >> 4;
	i |= i >> 8;
	i |= i >> 16;
	i |= i >> 32;

	i ^= i >> 1;

	return i;
}

template < typename IntType >
inline
IntType
lowestBit(IntType i)
{
	if(i == 0)
	{
		return 0;
	}

	IntType mask = 1;

	for(;;)
	{
		if((i & mask) > 0)
		{
			return mask;
		}
		
		mask <<= 1;
	}
}

template<typename T>
struct SBounded
{
	static T ubound()
	{
		return std::numeric_limits<T>::max();
	}

	static T lbound()
	{
		return std::numeric_limits<T>::min();
	}
};

#ifdef max
#undef max
#endif // max

#ifdef min
#undef min
#endif //min


template<>
struct SBounded<float>
{
	static float ubound()
	{
		return std::numeric_limits<float>::max();
	}

	static float lbound()
	{
		return -std::numeric_limits<float>::max();
	}
};

template<>
struct SBounded<double>
{
	static double ubound()
	{
		return std::numeric_limits<double>::max();
	}

	static double lbound()
	{
		return -std::numeric_limits<double>::max();
	}
};


} // end namespace jenny

