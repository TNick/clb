# ----------------------------------------------------------------------------
# Support library makes use of internal and external resources.
# ============================================================================

# ----------------------------------------------------------------------------
# initialize
pileTarget( "support")

pileInclude (RefCnt)
refcntInit(PILE_SHARED)
pileInclude (UserMsg)
usermsgInit(PILE_SHARED)

# ============================================================================

# ----------------------------------------------------------------------------
# our library
set( SUPPORT_SOURCES

    ${REFCNT_SOURCES}
    ${USERMSG_SOURCES}
)

set( SUPPORT_HEADERS

    ${REFCNT_HEADERS}
    ${USERMSG_HEADERS}
)

set( SUPPORT_QT_MODS
    Core
    Widgets
)

# ============================================================================

# ----------------------------------------------------------------------------
# terminate
pileEndTarget( "support" "static")

add_dependencies(
	${SUPPORT_TARGET}
	"copy_refcnt_headers"
	"copy_usermsg_headers")
	
pileCopyHeaders(RefCnt)
pileCopyHeaders(UserMsg)
# ============================================================================
