// -------------------------------------------------------------- -*- C++ -*-
// File: ./include/ilconcert/ilosys.h
// --------------------------------------------------------------------------
// Licensed Materials - Property of IBM
//
// 5725-A06 5725-A29 5724-Y47 5724-Y48 5724-Y49 5724-Y54 5724-Y55 5655-Y21
// Copyright IBM Corp. 2000, 2013
//
// US Government Users Restricted Rights - Use, duplication or
// disclosure restricted by GSA ADP Schedule Contract with
// IBM Corp.
// ---------------------------------------------------------------------------

#ifndef __CONCERT_ilosysH
#define __CONCERT_ilosysH

#ifdef _WIN32
#pragma pack(push, 8)
#endif


#define ILO_SEGF_GENERIC_ALLOC
#define ILO_OPL_SUPPORT_SCHED
#define ILO_OPL_SUPPORT_CP


class IloAnyIndexI;

//#define ILO_INFINITY_FIX
#define ILO_BABYBEL

#define MERGE_ADV_STAT_FUNC
#define OVERLAP_EXPR
#define CUMUL_MINMAX_EXPR



#define ILOCONCERT20
#define ILOTOMOVE
#undef  ILOMOVED


// i86_solaris2.8_5.1
#if defined(__sun) && (defined(__i386) || defined(__amd64) || defined(__x86_64))
#include <unistd.h>
# if defined (ILOUSEMT)
#include <pthread.h>
# endif
#  undef  ILO_SUN55
#  define ILO_SUN55
# if defined (__i386)
#  undef  ILO_SUNPC
#  define ILO_SUNPC
# endif
#endif

//Sparc
#if defined(sparc)
#include <unistd.h>
# if defined (ILOUSEMT)
#include <pthread.h>
# endif
# if (__SUNPRO_CC >= 0x500)
#  undef  ILO_SUN55
#  define ILO_SUN55
#  if (__sparcv9)
#   undef ILO64
#   define ILO64
#  endif
# endif
#endif

// HPUX (for now we test against the compiler version)
#if defined(__hpux)
# undef  ILO_HP
# define ILO_HP
# if (__cplusplus >= 199707L)
#  undef  ILO_HP_aCC
#  define ILO_HP_aCC
#  undef  ILO_HP11
#  define ILO_HP11
#include <unistd.h>
#  if defined(ILOUSEMT)
#   include <pthread.h>
#  endif
#  if defined(__LP64__) // DD64 option (we could use _LP64 instead) .if DD32, it is _ILP32
#   undef ILO64
#   define ILO64
#  endif
#  if defined(__ia64)  // always defined on HP even with DD32 option. depends on the system only.
#   undef ILO_HPIA64
#   define ILO_HPIA64
#  endif
# endif
#endif

#if defined(_M_AMD64) || defined(__amd64) || defined(__x86_64)
# undef ILO_AMD64
# define ILO_AMD64
#endif
#if defined(__sun) && defined(ILO_AMD64)
# undef  ILO64
# define ILO64
# undef  ILO_SUNAMD64
# define ILO_SUNAMD64
# if defined(ILOUSEMT)
#include <pthread.h>
# endif
#endif

#if defined(_MSC_VER)
# undef  _Windows
# define _Windows
# undef  _WINDOWS
# define _WINDOWS
# undef  ILO_WINDOWS
# define ILO_WINDOWS
# undef  ILO_MSVC
# define ILO_MSVC
#include <stddef.h>
# if defined(_WIN64)
#  undef  ILO64
#  define ILO64
#  undef  ILO_WIN64
#  define ILO_WIN64
# else
#  undef  ILO_WIN32
#  define ILO_WIN32
# endif
//# if defined(ILO_MSVC8)
//#  undef _WIN32_WINNT
//#  define _WIN32_WINNT 0x0400
//# endif
#endif

#if defined(ILO_MSVC)
# if _MSC_VER>=1900 // MSVC14 = MSVC2015
#  undef  ILO_MSVC14
#  define ILO_MSVC14
# endif
#endif

// IBM RS6000
#if defined(__xlC__)
# undef  ILO_RS6000
# define ILO_RS6000
#include <unistd.h>
# if defined(__64BIT__)
#  undef ILO64
#  define ILO64
# endif
# if defined(ILOUSEMT)
#include <pthread.h>
# endif
#endif

// Linux
#if defined(__linux) && !(defined(linux))
#define linux
#endif

#if defined(linux)
# undef  ILO_LINUX
# define ILO_LINUX
#include <climits>
#include <stdio.h>
#include <unistd.h>
# if defined(ILOUSEMT)
#include <pthread.h>
# endif
# if defined(__ia64__) || defined(__x86_64__) || defined(__s390x__)
# undef ILO64
# define ILO64
# endif
#endif

#if defined(__powerpc64__)
# undef ILO64
# define ILO64
#endif

#if defined(__MVS__) || defined(MVS)
#undef ILO_MVS
#define ILO_MVS
#undef ILO_ZOS
#define ILO_ZOS
# if defined(ILOUSEMT)
#include <pthread.h>
# endif
# if defined(_LP64)
# undef ILO64
# define ILO64
# endif
#endif


#if defined(__APPLE__)
#undef  ILO_APPLE
#define ILO_APPLE
# if defined(__x86_64__)
#   undef ILO64
#   define ILO64
# endif
#endif

#if (defined(ILO_LINUX) && !defined(__powerpc64__) && !defined(__s390__)) || defined(ILO_WINDOWS) || defined(ILO_SUNPC) || defined(ILO_SUNAMD64) || defined(ILO_APPLE)
# define ILO_LITTLE_ENDIAN
#else
# define ILO_BIG_ENDIAN
#endif

// For PPC64LE, we can find endianness in endian.h
#if defined(__powerpc64__)
#include <endian.h>
#endif

#if (defined(__BIG_ENDIAN__))
# undef ILO_BIG_ENDIAN
# undef ILO_LITTLE_ENDIAN
# define ILO_BIG_ENDIAN
#endif

#if (defined(__LITTLE_ENDIAN__))
# undef ILO_BIG_ENDIAN
# undef ILO_LITTLE_ENDIAN
# define ILO_LITTLE_ENDIAN
#endif

// end detection of machine and/or compiler
//--------------------------------------------------------------------------

// and the following behavior macros

// ILOBADTEMPLATE        -> bad template implementations (rs6000 usually)
// ILONEWOPERATOR        -> defined new[]
// ILODELETEOPERATOR     -> define corresponding delete operator
// name2                 -> concatenation operator for the preprocessor
// ILOPTHREAD            -> posix threads
// ILOUNREACHABLE        -> unreachable code after throw

#if defined(ILO_RS6000) || defined (ILO_MSVC)
# undef  ILOBADTEMPLATE
# define ILOBADTEMPLATE
#endif

#if defined(ILO_HP_aCC) || defined(ILO_LINUX) || defined(ILO_APPLE) || defined(ILO_RS6000) || defined(ILO_SUN55) || defined(ILO_MSVC) || defined(ILO_SUNAMD64) || defined(ILO_ZOS)
# undef  ILONEWOPERATOR
# define ILONEWOPERATOR
#endif

#if defined(ILO_WIN64) || defined(ILO_HPIA64) || defined(ILO_MSVC)
# undef  ILODELETEOPERATOR
# define ILODELETEOPERATOR
#endif

#if !(defined(name2))
# if defined(ILO_MSVC) || defined(ILO_LINUX) || defined(ILO_APPLE) || defined(ILO_HP11)
#  undef name2
#  define name2(a,b)      _name2_aux(a,b)
#  define _name2_aux(a,b)      a##b
# else
#include "generic.h"
# endif
#endif

// The ILOPTHREAD tells whether we use posix threads
#if defined(ILO_LINUX) || defined(ILO_APPLE) || defined(ILO_HP11) || defined(ILO_SUN55) || defined(ILO_RS6000) || defined(ILO_SUNAMD64)|| defined(ILO_ZOS)
# undef  ILOPTHREAD
# define ILOPTHREAD
#endif

// The ILOUNREACHABLE preempts some code that is unreachable for some
// compilers
#if defined(ILO_MSVC) || defined(ILO_SUN55)
# undef  ILOUNREACHABLE
# define ILOUNREACHABLE(x)
#else
# undef  ILOUNREACHABLE
# define ILOUNREACHABLE(x) x
#endif

#if defined(ILO_RS6000)
# undef ILOBADRANGE
# define ILOBADRANGE
#endif

// DLL
#if defined(ILO_WINDOWS)
#  if defined(_MSC_VER)
#	define ILODECLSPEC __declspec
#    if !defined(ILO_BASEIMPORTED)
#	define ILO_BASEIMPORTED ILODECLSPEC(dllimport)
#    endif 
#    if !defined(ILO_BASEEXPORTED)
#	define ILO_BASEEXPORTED ILODECLSPEC(dllexport)
#    endif 
#    if !defined(ILO_BASEEXPORTEDFUNCTION)
#	define ILO_BASEEXPORTEDFUNCTION(type) ILO_BASEEXPORTED type
#	define ILO_BASEIMPORTEDFUNCTION(type) ILO_BASEIMPORTED type
#    endif 
#    if !defined(ILO_EXPORTEDDEF)
#       define ILO_EXPORTEDDEF(type) ILO_BASEEXPORTED type
#    endif 
#  endif 
#endif 

#if defined ILO_DLL
#   if defined(ILO_BUILD_LIB)
#       define ILO_EXPORTED ILO_BASEEXPORTED
#       define ILO_EXPORTEDFUNCTION(type) ILO_BASEEXPORTEDFUNCTION(type)
#       define ILO_EXPORTEDVAR(type) ILO_BASEEXPORTEDFUNCTION(type)
#   else
#       define ILO_EXPORTED ILO_BASEIMPORTED
#       define ILO_EXPORTEDFUNCTION(type) ILO_BASEIMPORTEDFUNCTION(type)
#       define ILO_EXPORTEDVAR(type) ILO_BASEIMPORTEDFUNCTION(type)
#   endif 
#else
#   define ILO_EXPORTED
#   define ILO_EXPORTEDFUNCTION(type) type
#   define ILO_EXPORTEDVAR(type) type
#endif


#ifndef CPPREF_GENERATION
// STL things
#if defined(IL_STD)
# undef  ILOUSESTL
# define ILOUSESTL
# undef  ILOSTLBEGIN
# define ILOSTLBEGIN using namespace std;
# undef  ILOSTD
# define ILOSTD(x) std:: x
#else
# undef  ILOSTLBEGIN
# define ILOSTLBEGIN
# undef  ILOSTD
# define ILOSTD(x) x
#endif
#else

# define ILOSTLBEGIN
#endif


//http://msdn2.microsoft.com/en-us/library/kftdy56f(VS.71).aspx
#if defined(ILO_MSVC)
# undef  ILOPROTECTNEW
# define ILOPROTECTNEW(x, n) { try { x = n; } catch (std::bad_alloc&) { throw IloMemoryException(); } }  if (x == 0) throw IloMemoryException();
# undef  ILONEWNOTHROW
# define ILONEWNOTHROW (std::nothrow)
#else
#if defined(ILOUSESTL) && !defined(ILO_RS6000) && !defined(__MVS__)
# undef  ILOPROTECTNEW
# define ILOPROTECTNEW(x, n) { try { x = n; } catch (std::bad_alloc&) { throw IloMemoryException(); } }
# undef  ILONEWNOTHROW
# define ILONEWNOTHROW (std::nothrow)
#else
# undef  ILOPROTECTNEW
# define ILOPROTECTNEW(x, n) { x = n; if (x == 0) throw IloMemoryException(); }
# undef  ILONEWNOTHROW
# define ILONEWNOTHROW
#endif
#endif


#if (defined(ILO_LINUX) && (__GNUC__ >= 3)) || (defined(_MSC_VER) && (_MSC_VER >= 1400)) || defined(ILO_APPLE)
# undef ILOSTRINGSTL
# define ILOSTRINGSTL
#endif

#ifdef ILOUSESTL
#include <string>
#include <iostream>
#include <limits>
#include <fstream>
#include <iomanip>
#ifdef ILOSTRINGSTL
#include <sstream>
#include <istream>
#include <ostream>
#elif defined ILO_WINDOWS
#include <sstream>
#include <istream>
#include <ostream>
#include <strstream>
#else
#include <strstream>
#endif
#else
#include <string.h>
#include <iostream.h>
#include <limits.h>
#include <fstream.h>
#include <string.h>
#include <iomanip.h>
#ifdef ILO_WINDOWS
#include <strstrea.h>
#else
#include <strstream.h>
#endif
#endif

#ifdef ILOSTRINGSTL
# undef ILOSTD_ISTREAM
# define ILOSTD_ISTREAM ILOSTD(istringstream)
# undef ILOSTD_OSTREAM
# define ILOSTD_OSTREAM ILOSTD(ostringstream)
#else
# undef ILOSTD_ISTREAM
# define ILOSTD_ISTREAM ILOSTD(istrstream)
# undef ILOSTD_OSTREAM
# define ILOSTD_OSTREAM ILOSTD(ostrstream)
#endif

# undef ILOBADDELETE
# define ILOBADDELETE delete []

#include <assert.h>
#ifdef NDEBUG

#define IloAssert(x,y)
#define IloInternalAssert(x,y)

#else

inline int ilo_stop_assert() {
	return 0;
}
#define IloAssert(x,y)  assert ((x) || (ILOSTD(cerr) << y << ILOSTD(endl), ilo_stop_assert()))
#define IloInternalAssert(x,y)  assert ((x) || (ILOSTD(cerr) << "Internal " << y << ILOSTD(endl), ilo_stop_assert()))

#endif

// IloInt

#if defined(ILO_WIN64)
typedef __int64 IloInt;
typedef unsigned __int64 IloUInt;
#else

typedef long IloInt;
typedef unsigned long IloUInt;
#endif

#ifdef CPPREF_GENERATION

const IloInt IloIntMax=9007199254740991; // (2^53 - 1)

const IloInt IloIntMin=-IloIntMax;
#else

#ifdef ILO64
#define IloIntMax ((IloInt)9007199254740991) // (2^53 - 1)
#else
#define IloIntMax ((IloInt)LONG_MAX)
#endif
#define IloIntMin -IloIntMax

#endif

inline int IloZero(){
	return 0;
}

#if !defined(OL)
#define OL IloZero()
#endif

#include <stdarg.h>



class IloEnv;
class IloEnvI;
class IloAlgorithm;
class IloAlgorithmI;
class IloNumVar;
class IloIntVar;
class IloBoolVar;
class IloNumVarI;
class IloNumVarArray;
class IloIntVarArray;
class IloModel;
class IloModelI;
class IloConstraint;
class IloConstraintArray;
class IloConstraintI;
class IloObjective;
class IloObjectiveI;
class IloChange;
class IloChangeI;
class IloAddNumVar;
class IloNumColumn;
class IloNumColumnArray;
class IloExtractable;
class IloExtractableI;
class IloExtractableList;
class IloExtractableArray;
class IloIntExprArray;
class IloNumExprArray;
class IloExtension;
class IloAnySet;
class IloNumSet;
class IloAnyArray;

class IloRange;
class IloRangeI;
class IloRangeArray;
class IloAddValueToRange;
class IloSetRangeBounds;
class IloSetRangeArrayBounds;
class IloSetRangeCoef;
class IloSetRangeCoefs;
class IloSetRangeExpr;
class IloAddVarToAllDiff;
class IloSetNumVarBounds;
class IloSetNumVarArrayBounds;
class IloExpr;
class IloExprArray;
class IloLinExprTerm;
class IloExprBase;
class IloExprNode;
class IloSemiContVar;
class IloSemiContVarI;
class IloAllDiffI;
class IloPackI;
class IloInverseI;
class IloSetInverseI;
class IloStrongI;
class IloSequenceI;
class IloDistributeI;
class IloPathLengthI;
class IloAllDiffI;
class IloDiffI;
class IloIfThenI;
class IloOrI;
class IloAndI;
class IloNumLinExprTermI;
class IloNumQuadExprTermI;
class IloNumExprArg;
class IloIntExprArg;
class IloNumLinExprTerm;
class IloIntLinExprTerm;
class IloNumQuadExprTerm;
class IloIntQuadExprTerm;

class IloTimer;

// CONCERT-80
#if defined(ILO_LINUX) || defined(ILO_APPLE)
#define ILO_MAY_ALIAS __attribute__((may_alias))
#else
#define ILO_MAY_ALIAS
#endif

#if defined(ILO_LINUX) || (defined(_MSC_VER) && (_MSC_VER >= 1400)) || defined(ILO_APPLE)
#define ILO_RESTRICT __restrict
#else
#define ILO_RESTRICT
#endif




typedef double IloNum;
typedef float IloShortNum;

typedef IloInt IloBool;


typedef void* ILO_MAY_ALIAS IloAny;



typedef IloNum (*IloNumFunction)(IloNum);

void IloInitGlobals();
#define IloTrue  ((IloInt)1L)
#define IloFalse ((IloInt)0L)


#ifdef CPPREF_GENERATION

extern const double IloInfinity;
#else
IloNum IloGetInfinity();
#if (defined(ILO_LINUX) || defined(ILO_APPLE)) && !defined(ILO_RS6000)
#define IloInfinity __builtin_inf()
#elif defined(ILO_MSVC)
#define IloInfinity HUGE_VAL
#else
#define IloInfinity  (IloGetInfinity())
#endif
#endif


#define IloQuarterPi ((IloNum)0.78539816339744830962)

#define IloHalfPi ((IloNum)1.57079632679489661923)

#define IloPi ((IloNum)3.14159265358979323846)

#define IloThreeHalfPi ((IloNum)4.71238898038468985769)

#define IloTwoPi ((IloNum)6.28318530717958647692)

extern "C" void ilo_exception_stop_here();


class ILO_EXPORTED IloQuietException {
public:
	IloQuietException(const IloQuietException&);
	virtual ~IloQuietException();
	IloQuietException& operator=(const IloQuietException&);
	virtual const char* getMessage() const;
	virtual void print(ILOSTD(ostream)& out) const;
	virtual void end();
protected:
	IloQuietException(const char* message = 0, IloBool deleteMessage=IloFalse);
	const char* _message;
	IloBool _deleteMessage;
};


class ILO_EXPORTED IloException : public IloQuietException { // here because needed by threads
public:
	IloException(const IloException&);
	~IloException();
	IloException& operator=(const IloException&);
	
	virtual const char* getMessage() const;
	virtual void print(ILOSTD(ostream)& out) const;
	
	virtual void end();
protected:
	
	IloException(const char* message = 0, IloBool deleteMessage=IloFalse);
};

class ILO_EXPORTED IloWrongUsage : public IloException {
public:
	IloWrongUsage(const char* message = 0, IloBool deleteMessage = IloFalse);
	~IloWrongUsage();
};


class ILO_EXPORTED IloMemoryException : public IloException {
public:
	
	IloMemoryException();
	
	virtual ~IloMemoryException();
};

#define IloTestAndRaise(x, y) if (!(x)) throw IloWrongUsage(y);


inline ILOSTD(ostream)& operator<<(ILOSTD(ostream)& o, const IloException& e)
{
	e.print(o);
	return o;
}

inline ILOSTD(ostream)& operator<<(ILOSTD(ostream)& o, const IloQuietException& e)
{
	e.print(o);
	return o;
}



void IloEnableNANDetection();




void IloDisableNANDetection();



int IloIsNAN(double);


#ifdef CPPREF_GENERATION


extern ILO_NO_MEMORY_MANAGER;
#endif

// enum used by the classes Ilo(Product)Version
enum IloReleaseType { Standard=0L, Beta=10, Alpha=20, Special=30};

char * IloGetString(ILOSTD_OSTREAM & stream);

#ifdef ILOUSESTL
IloNum IloParseNum(const char* str);
IloInt IloParseInt(const char* str);
#endif

#include<setjmp.h>

#if defined(ILO_MSVC)
# ifndef _DLL
extern "C" int  __cdecl _setjmp(jmp_buf);
extern "C" void __cdecl longjmp(jmp_buf, int);
# endif
# define ILOSETJMP(x) _setjmp(x)
# define ILOLONGJMP(x,y) longjmp(x,y)
#elif defined(ILO_SUN55)
# define ILOSETJMP(x)  setjmp(x)
# define ILOLONGJMP(x,y) longjmp(x,y)
#else
# define ILOSETJMP(x)  _setjmp(x)
# define ILOLONGJMP(x,y) _longjmp(x,y)
#endif



inline unsigned short IloGetPrecisionControl() {
#if defined(ILO_WIN32)
	unsigned short ctl;
	__asm { fstcw ctl };
	return ctl;
#else
	return 0;
#endif
}

#if defined(ILO_WIN32)
inline void IloSetPrecisionControl(unsigned short ctl) {
	__asm { fldcw ctl }
}
#else
inline void IloSetPrecisionControl(unsigned short) { }
#endif



#ifdef _WIN32
#pragma pack(pop)
#endif

#endif


