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

#ifndef STRINGIFY
#   define STRINGIFY(s) STRINGIFY_HELPER(s)
#   define STRINGIFY_HELPER(s) #s
#endif


#ifndef NULL
#   define NULL ((void*)0)
#endif


#ifndef VAR_UNUSED
#   define VAR_UNUSED(expr) do { (void)(expr); } while (0)
#endif


//! debug break point
#ifdef CLB_DEBUG
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


//! our assert
#ifdef CLB_DEBUG
#  define DBG_ASSERT(a) if (!(a)) {\
        printf("\nDBG_ASSERT Failed: " STRINGIFY(a) "\n"); \
        printf("%s[%d]: %s\n\n",__FILE__, __LINE__, __func__); \
        YT_BREAKPOINT; }
#else
#  define DBG_ASSERT(a)
#endif

//! assert and return
#ifdef CLB_DEBUG
#  define DBG_ASSERT_RET(a) \
    if (!(a)) {\
        printf("\nDBG_ASSERT Failed: " STRINGIFY(a) "\n"); \
        printf("%s[%d]: %s\n\n",__FILE__, __LINE__, __func__); \
        YT_BREAKPOINT; \
        return; }
#else
#  define DBG_ASSERT_RET(a) if (!(a)) return;
#endif

#ifndef HUGE_VALD
#define HUGE_VALD (INFINITY)
#endif

#define DOUBLE_IS_NAN(__d__) \
    (__d__ != __d__)
#define DOUBLE_IS_INF(__d__) ( \
    (__d__ == HUGE_VALD) || \
    (__d__ == -HUGE_VALD))

#define DOUBLE_IS_VALID(__d__) \
    (!DOUBLE_IS_NAN (__d__) && !DOUBLE_IS_INF (__d__))
#define POINT_IS_VALID(__p__) \
    (DOUBLE_IS_VALID(__p__.x()) && DOUBLE_IS_VALID(__p__.y()))
#define RECT_IS_VALID(__r__) (\
    POINT_IS_VALID(__r__.topLeft()) && \
    POINT_IS_VALID(__r__.topRight()) && \
    POINT_IS_VALID(__r__.bottomRight()) && \
    POINT_IS_VALID(__r__.bottomLeft()))
#define TRANSFORM_IS_VALID(__t__) (\
    DOUBLE_IS_VALID(__t__.m11()) && \
    DOUBLE_IS_VALID(__t__.m12()) && \
    DOUBLE_IS_VALID(__t__.m13()) && \
    DOUBLE_IS_VALID(__t__.m21()) && \
    DOUBLE_IS_VALID(__t__.m22()) && \
    DOUBLE_IS_VALID(__t__.m23()) && \
    DOUBLE_IS_VALID(__t__.m32()) && \
    DOUBLE_IS_VALID(__t__.m33()))


#define DBG_ASSERT_DOUBLE(__d__) \
    DBG_ASSERT(DOUBLE_IS_VALID(__d__))
#define DBG_ASSERT_POINT(__p__) \
    DBG_ASSERT(POINT_IS_VALID(__p__))
#define DBG_ASSERT_RECT(__r__) \
    DBG_ASSERT(RECT_IS_VALID(__r__))
#define DBG_ASSERT_TRANSF(__t__) \
    DBG_ASSERT(TRANSFORM_IS_VALID(__t__))

//! print debug message
#ifdef CLB_DEBUG
#  define DBG_MESSAGE(__m__) \
    printf("%s[%d]: %s\n%s",__FILE__, __LINE__, __func__, __m__);
#else
#  define DBG_MESSAGE(__m__)
#endif

//! print debug message
#ifdef CLB_DEBUG
#  define QDBG_MESSAGE(__m__) \
    qDebug() << "DEBUG: " << __m__ << " > " << __func__ << " > " << __FILE__ << "[" << __LINE__ << "] ";
#else
#  define QDBG_MESSAGE(__m__)
#endif

#ifdef CLB_DEBUG
#  define DBG_PMESSAGE printf
#else
#  define DBG_PMESSAGE black_hole
#endif

#ifdef CLB_DEBUG
#  define DBG_TRACE_ENTRY printf("ENTRY %s in %s[%d]\n", __func__, __FILE__, __LINE__)
#else
#  define DBG_TRACE_ENTRY
#endif

#ifdef CLB_DEBUG
#  define DBG_TRACE_EXIT printf("EXIT %s in %s[%d]\n", __func__, __FILE__, __LINE__)
#else
#  define DBG_TRACE_EXIT
#endif

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
#define		OUT_SUCCESS( o )	( o != OUTCOME_ERROR )

//! was the result a succes (excludes warnings)?
///
#define		OUT_FULL_SUCCESS( o )	( o == OUTCOME_OK )


#define CuttilesOpenGdal(__file__, __access__) \
    GDALOpen(__file__, __access__);

#define CuttilesCloseGdal(__var__) {\
    if (GDALDereferenceDataset(__var__) <= 0) { \
    GDALClose(__var__); }}

// actually the count may be 0 but we have no means to find out
// so we increase it to make it at least one
#define CuttilesForceCloseGdal(__var__) if (__var__ != NULL) {\
    GDALReferenceDataset (__var__); \
    while (GDALDereferenceDataset(__var__) > 0) {} \
    GDALClose(__var__); }



// due to a bug in OutputFormat we had to trace down allocations
// this switch enables / disables it
#ifdef TRACE_OUTPUT_FORMAT_LIFE
#define TRACE_OUTPUT_FORMAT_LIFE_MSG DBG_PMESSAGE
#else
#define TRACE_OUTPUT_FORMAT_LIFE_MSG black_hole
#endif



#define DEBUG_ON   1
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
