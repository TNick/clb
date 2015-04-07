# ----------------------------------------------------------------------------
# GUI components.
# ============================================================================

# ----------------------------------------------------------------------------
# initialize
pileTarget( "clbgui")

# ============================================================================

# ----------------------------------------------------------------------------
# our library
set( CLBGUI_SOURCES
    snippetsdlg.cc
    about.cc
    gui_main.cc
    editors/codeeditor.cc
)

set( CLBGUI_HEADERS
    snippetsdlg.h
    about.h
    gui_main.h
)

set( CLBGUI_UIS
    snippetsdlg.ui
    about.ui
)

set( CLBGUI_LIBS
	"logic"
    "support"
)

set( CLBGUI_QT_MODS
    Core
    Widgets
    Xml
    Network
)

# ============================================================================

# ----------------------------------------------------------------------------
# terminate
pileEndTarget( "clbgui" "static" COPY)

# ============================================================================
