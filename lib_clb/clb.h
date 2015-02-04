/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			clb.h
  \author		TNick

*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef CLB_MAIN_INCLUDE
#define CLB_MAIN_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

// generated on the fly from config.h.in by CMake
#include <clb/config.h>

// standard headers
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef TARGET_COMPILER_MSVC
#include <intrin.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif


/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

// M$ does not care about the rest of the world
#ifdef TARGET_COMPILER_MSVC
#define __func__ __FUNCTION__
#endif

/**
 * @def STRINGIFY
 * @brief Convert a variable name to a string.
 */
#ifndef STRINGIFY
#   define STRINGIFY(s) STRINGIFY_HELPER(s)
#   define STRINGIFY_HELPER(s) #s
#endif

/**
 * @def NULL
 * @brief An invalid pointer.
 */
#ifndef NULL
#   define NULL ((void*)0)
#endif

/**
 * @def VAR_UNUSED
 * @brief A variable is defined but unused.
 */
#ifndef VAR_UNUSED
#   define VAR_UNUSED(expr) do { (void)(expr); } while (0)
#endif

/**
 * @def YT_BREAKPOINT
 * @brief A breakpoint only defined in debug mode.
 */
#ifdef PROWIZ_DEBUG
#   ifdef TARGET_COMPILER_MSVC
//#     define YT_BREAKPOINT  __asm {int 3}
#       define YT_BREAKPOINT  __debugbreak()
#   else
//#       define YT_BREAKPOINT  asm volatile ("int3;")
#       define YT_BREAKPOINT  __builtin_trap()
#   endif
#else
#   define YT_BREAKPOINT
#endif


/**
 * @def DBG_ASSERT
 * @brief Asserts a value in debug and does NOTHING in release.
 */
#ifdef PROWIZ_DEBUG
#  define DBG_ASSERT(a) if (!(a)) {\
        printf("\nDBG_ASSERT Failed: " STRINGIFY(a) "\n"); \
        printf("%s[%d]: %s\n\n",__FILE__, __LINE__, __func__); \
        YT_BREAKPOINT; }
#else
#  define DBG_ASSERT(a)
#endif

/**
 * @def DBG_ASSERT
 * @brief Asserts and returns.
 */
#ifdef PROWIZ_DEBUG
#  define DBG_ASSERT_RET(a) \
    if (!(a)) {\
        printf("\nDBG_ASSERT Failed: " STRINGIFY(a) "\n"); \
        printf("%s[%d]: %s\n\n",__FILE__, __LINE__, __func__); \
        YT_BREAKPOINT; \
        return; }
#else
#  define DBG_ASSERT_RET(a) if (!(a)) return;
#endif

/**
 * @def HUGE_VALD
 * @brief Highest value.
 */
#ifndef HUGE_VALD
#define HUGE_VALD (INFINITY)
#endif

/**
 * @def DOUBLE_IS_NAN
 * @brief Tell if a double is NotANumber.
 */
#define DOUBLE_IS_NAN(__d__) \
    (__d__ != __d__)

/**
 * @def DOUBLE_IS_INF
 * @brief Tell if a double is Infinity.
 */
#define DOUBLE_IS_INF(__d__) ( \
    (__d__ == HUGE_VALD) || \
    (__d__ == -HUGE_VALD))

/**
 * @def DOUBLE_IS_VALID
 * @brief Tell if a double is Infinity or NaN.
 */
#define DOUBLE_IS_VALID(__d__) \
    (!DOUBLE_IS_NAN (__d__) && !DOUBLE_IS_INF (__d__))

/**
 * @def POINT_IS_VALID
 * @brief Tell if a point is valid.
 */
#define POINT_IS_VALID(__p__) \
    (DOUBLE_IS_VALID(__p__.x()) && DOUBLE_IS_VALID(__p__.y()))

/**
 * @def RECT_IS_VALID
 * @brief Tell if a point is valid.
 */
#define RECT_IS_VALID(__r__) (\
    POINT_IS_VALID(__r__.topLeft()) && \
    POINT_IS_VALID(__r__.topRight()) && \
    POINT_IS_VALID(__r__.bottomRight()) && \
    POINT_IS_VALID(__r__.bottomLeft()))


#define DBG_ASSERT_DOUBLE(__d__) \
    DBG_ASSERT(DOUBLE_IS_VALID(__d__))
#define DBG_ASSERT_POINT(__p__) \
    DBG_ASSERT(POINT_IS_VALID(__p__))
#define DBG_ASSERT_RECT(__r__) \
    DBG_ASSERT(RECT_IS_VALID(__r__))
#define DBG_ASSERT_TRANSF(__t__) \
    DBG_ASSERT(TRANSFORM_IS_VALID(__t__))

/**
 * @def DBG_MESSAGE
 * @brief Print a message in debug builds.
 */
#ifdef CLB_DEBUG
#  define DBG_MESSAGE(__m__) \
    printf("%s[%d]: %s\n%s",__FILE__, __LINE__, __func__, __m__);
#else
#  define DBG_MESSAGE(__m__)
#endif

/**
 * @def QDBG_MESSAGE
 * @brief Print a message in debug builds.
 */
#ifdef CLB_DEBUG
#  define QDBG_MESSAGE(__m__) \
    qDebug() << "DEBUG: " << __m__ << " > " << __func__ << " > " << __FILE__ << "[" << __LINE__ << "] ";
#else
#  define QDBG_MESSAGE(__m__)
#endif

/**
 * @def DBG_PMESSAGE
 * @brief Print a message in debug builds.
 */
#ifdef CLB_DEBUG
#  define DBG_PMESSAGE printf
#else
#  define DBG_PMESSAGE black_hole
#endif

/**
 * @def DBG_TRACE_EXIT
 * @brief Print a message in debug builds.
 */
#ifdef CLB_DEBUG
#define DBG_TRACE_ENTRY printf("ENTRY %s in %s[%d]\n", __func__, __FILE__, __LINE__)
#else
#  define DBG_TRACE_ENTRY
#endif

#ifdef CLB_DEBUG
#  define DBG_TRACE_EXIT printf("EXIT %s in %s[%d]\n", __func__, __FILE__, __LINE__)
#else
#  define DBG_TRACE_EXIT
#endif

/**
 * @def DBG_PQSTRING
 * @brief Print a message in debug builds.
 */
#ifdef CLB_DEBUG
#  define DBG_PQSTRING(__s__) printf("%s\n", TMP_A(__s__))
#else
#  define DBG_PQSTRING(__s__)
#endif

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable: 4127) // conditional expression is constant
//#pragma warning(disable: 9025) // conditional expression is constant
#endif


#define	STG_LANGUAGE		"ui-language"
#define	DATA_FLD_TRANSL		"translations"
#define	LANG_FILE_PREFIX	"clb_"
#define	LANG_FILE_SUFFIX	".qm"
#define	LANG_FILE(loc)		(LANG_FILE_PREFIX + loc + LANG_FILE_SUFFIX)
#define	RECENT_FILES_COUNT	"recent_files_count"
#define	RECENT_FILES_LIST	"recent_files_list"

//! Get s Utf8 string from a QString
///
#define TMP_A(__s__) __s__.toLatin1().constData()

//! describes the result of a function
///
enum	OUTCOME			{
    OUTCOME_WARNING = -1,
    OUTCOME_OK = 0,
    OUTCOME_ERROR = 1
};

//! what to do if an input file has missing info
///
enum AfterLoadPolicy {
    AlwaysEdit,
    EditIfMissingProj, /**< editing trigered even if there is a parameter list*/
    EditIfMissingParams, /**< editing not trigered if missing projectio - a default will be implied */
    NeverEdit
};

//! was the result a succes (includes warnings)?
///
#define OUT_SUCCESS( o )	( o != OUTCOME_ERROR )

//! was the result a succes (excludes warnings)?
///
#define OUT_FULL_SUCCESS( o )	( o == OUTCOME_OK )

//! Debug enabled
///
#define DEBUG_ON   1

//! Debug disabled
///
#define DEBUG_OFF  0

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

static inline void black_hole (...)
{}

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifdef __cplusplus
}
#endif
#endif // CLB_MAIN_INCLUDE
