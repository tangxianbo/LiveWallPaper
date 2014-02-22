#pragma once

#include <limits.h>
#include <string.h>
#include <string>


namespace jenny 
{

inline bool IsPowerOf2(unsigned int value) {
	return value && !(value & (value - 1));
}


template<class T> 
void MarkUsed( const T& ) { }

template<typename T1, typename T2>
bool TestFlag(T1 f1, T2 f2) {
	return (f1 & f2) == f2;
}

template <typename T>
inline T Abs(T a) {
	return (a < 0) ? static_cast<T>(-a) : a;
}

template <typename T>
inline T Sign(T a) {
	return (a < T(0)) ? static_cast<T>(-1) : static_cast<T>(1);
}

template <typename T>
inline T Min(T a, T b) {
	return (a > b) ? b: a;
}

template <typename T>
inline T Max(T a, T b) {
	return (a < b) ? b: a;
}

template <typename T>
inline T Clamp(T x, T low, T high) {
	return (x > high) ? high : ( (x < low) ? low : x );
}

template <typename T>
inline T RoundUp(T x, unsigned int base) {
	GLFi_ASSERT(IsPowerOf2(base));

	if(x == 0)
		return 0;
	return static_cast<T>( (x + (base - 1)) & ~(base - 1) );
}

template <typename T>
inline T RoundDown(T x, unsigned int base) {
	GLFi_ASSERT(IsPowerOf2(base));

	return static_cast<T>( x & ~(base - 1) );
}

inline bool IsBigEndian() {
	unsigned int x = 1;

	return 0 == *(unsigned char*)(&x);
}

inline bool IsLittleEndian() {
	return !IsBigEndian();
}

template<typename T, T lowest, T highest>
struct IntegerLimit {
	static T Lowest() {
		return lowest;
	}
	static T Highest() {
		return highest;
	}
};

template<typename T>
struct Limit {
};


template<> struct Limit<unsigned char> : public IntegerLimit<unsigned char, 0, UCHAR_MAX> {};
template<> struct Limit<char> : public IntegerLimit<char, CHAR_MIN, CHAR_MAX> {};
template<> struct Limit<unsigned short> : public IntegerLimit<unsigned short, 0, USHRT_MAX> {};
template<> struct Limit<short> : public IntegerLimit<short, SHRT_MIN, SHRT_MAX> {};
template<> struct Limit<unsigned int> : public IntegerLimit<unsigned int, 0, UINT_MAX> {};
template<> struct Limit<int> : public IntegerLimit<int, INT_MIN, INT_MAX> {};
template<> struct Limit<unsigned long> : public IntegerLimit<unsigned long, 0, LONG_MAX> {};
template<> struct Limit<long> : public IntegerLimit<long, LONG_MIN, LONG_MAX> {};

template<typename T>
struct Bounds {
	static T Lowest() {
		return Limit<T>::Lowest();
	}
	static T Highest() {
		return Limit<T>::Highest();
	}
};

//	swapendian
inline void SwapShort(short& v) {
	v = ((v & 0x00ff) << 8) |
		((v & 0xff00) >> 8);
}

inline void SwapInt(int& v) {
	v = ((v & 0xff000000) >> 24) |
		((v & 0x00ff0000) >> 8) |
		((v & 0x0000ff00) << 8) |
		((v & 0x000000ff) << 24);
}

template<typename T>
static inline T SwapEndian(T& value) {
	if(sizeof(T) == 1) {
	} else if(sizeof(T) == 2) {
		SwapShort((short&)value);
	} else if(sizeof(T) == 4) {
		SwapInt((int&)value);
	} else {
		GLF_ASSERT(false);
	}

	return value;
}

class NonCopyable {
protected:
	NonCopyable() {}
	~NonCopyable() {}
private:
	NonCopyable(const NonCopyable&);
	const NonCopyable& operator=(const NonCopyable&);
};

class SingletonDeleteTrick : private NonCopyable
{
public:	
	SingletonDeleteTrick() : mIsDeleted(false){}
	virtual ~SingletonDeleteTrick(){ mIsDeleted = true; }
protected:
    // Trick to find out if a static instance has been destroyed (at least it
    // means the destructor was called). 
    //
    // This should prevent using a singleton after it's destructor was called
    // even if the static memory instance is still valid (object destroyed, 
    // but not freed)
	bool	mIsDeleted;
};

template < typename T >
class Singleton : public SingletonDeleteTrick
{
public:	
	static T* GetInstance() {
		static T inst;
		return (inst.mIsDeleted) ? 0 : &inst;
	}
};

template < typename T, typename D >
class SingletonWithDep : private SingletonDeleteTrick
{
public:
	static T* GetInstance() {
		D::GetInstance();
		static T inst;
		return (inst.mIsDeleted) ? 0 : &inst;
	}
};

template<typename T>
inline void ZeroMem(T& obj) {
	memset(&obj, 0, sizeof(obj));
}

template<typename T, size_t N>
inline void ZeroMemArray(T (&arr)[N]) {
	memset(&arr, 0, sizeof(arr));
}

// here until I find a better place for it
struct color_rgb {
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

struct color_rgba {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

struct color_argb {
	unsigned char a;
	unsigned char r;
	unsigned char g;
	unsigned char b;

	void Set(unsigned char a, unsigned char r, unsigned char g, unsigned char b) {
		this->a = a;
		this->r = r;
		this->g = g;
		this->b = b;
	}

	bool Test() {
		return true;
	}
};

struct color_cmyk {
	unsigned char c;
	unsigned char m;
	unsigned char y;
	unsigned char k;
};

struct color_hsv {
	unsigned char h;
	unsigned char s;
	unsigned char v;
};

struct Color {
	enum Type {
		TYPE_RGB,
		TYPE_RGBA,
		TYPE_ARGB,
		TYPE_CMYK,
		TYPE_HSV
	};
	union {
		color_rgb	rgb;
		color_rgba	rgba;
		color_argb	argb;
		color_cmyk	cmyk;
		color_hsv	hsv;
	};
};

template<typename T>
class stlallocator {
public:
    // types that are required for allocators
    typedef T                   value_type;
    typedef value_type*         pointer;
    typedef const value_type*   const_pointer;
    typedef value_type&         reference;
    typedef const value_type&   const_reference;
    typedef size_t              size_type;
    typedef ptrdiff_t           difference_type;

    // rebind structure
    template <class U> 
	struct rebind { 
        typedef stlallocator<U> other; 
    };

    // constructor/destructor
    stlallocator() { 
	}
    stlallocator(const stlallocator&) { 
	}
    template <class U>          
	stlallocator(const stlallocator<U>&) { 
	}
	~stlallocator() {
	}

    pointer address(reference x) const { 
		return &x; 
	}
    const_pointer address(const_reference x) const { 
		return &x; 
	}

    pointer allocate(size_type n, void* = 0) { 
		return static_cast<pointer>(GlfAlloc(n*sizeof(T)));
	}
    void deallocate(pointer p, size_type) { 
		GlfFree(p);
	}
    void construct(pointer p, const value_type& x)   { 
		new(p) value_type(x); 
	}
    void destroy(pointer p) { 
		p->~value_type(); 
	}
    size_type max_size(void) const { 
		return static_cast<size_type>(-1) / sizeof(T); 
	}
    void operator=(const stlallocator&) { 
	}
};

template<> 
class stlallocator<void> {
    // types that are required for allocators
    typedef void                value_type;
    typedef void*               pointer;
    typedef const void*         const_pointer;

    // rebind structure
    template <class U> 
	struct rebind { 
        typedef stlallocator<U> other; 
    };
};

// operator== for allocator
template <class T>
inline bool operator == ( const stlallocator<T>&, const stlallocator<T>& ) {
    return true;
}

// operator!= for allocator
template <class T>
inline bool operator != ( const stlallocator<T>&, const stlallocator<T>& ) {
    return false;
}

typedef std::basic_string< char, std::char_traits<char>, std::allocator<char> > stringc;

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4244)
#endif

template<typename T1, typename T2>
bool safe_conversion(T2 value) {
	return T2(T1(value)) == value;
}

template<typename T1, typename T2>
T1 safe_cast(T2 value) {
	GLF_ASSERT(safe_conversion<T1>(value));
	return T1(value);
}
#ifdef _MSC_VER
#pragma warning(pop)
#endif

inline bool Test(int flags, int values) {
	return (flags & values) == values;
}
inline bool TestAny(int flags, int values) {
	return (flags & values) != 0;
}

} //namespace jenny
