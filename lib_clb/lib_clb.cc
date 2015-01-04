/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
\file		lib_clb.cc
\date		Jul 2011
\author		TNick

\brief		General routines for the library


*//*


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Please read COPYING and README files in root folder
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "lib_clb.h"

#include <clb/snippetsdlg.h>
#include <clb/gui_main.h>

#include <usermsg/usermsg.h>
#include <usermsg/usermsgman.h>

#include <QDebug>
#include <QTranslator>
#include <QDir>
#include <QDesktopServices>
#include <QMessageBox>
#include <QCoreApplication>
#include <QSettings>

#ifdef TARGET_SYSTEM_WIN32
#include <windows.h>
#include <tchar.h>
#endif // TARGET_SYSTEM_WIN32

#include <stdio.h>


/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

#if 1
#define LIBCLB_DEBUG DBG_PMESSAGE
#else
#define LIBCLB_DEBUG black_hole
#endif

//! tell if we're shutting down
static bool clb_shutting_down_ = false;
LibCLB * LibCLB::singleton_ = NULL;
static bool clb_patial_init_ = false;

extern "C" {

//! show main gui
CLB_EXPORT void
clb_side_thread ();

}


/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
bool  clb_shutting_down()
{
    return clb_shutting_down_;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool start_translation()
{

    // application is translatable
    QString locale = QSettings().value(
                STG_LANGUAGE,
                QLocale::system().name()
                ).toString ();
    QTranslator translator;
    QDir	d(
            #if QT_VERSION < 0x050000
                QDesktopServices::storageLocation(
                    QDesktopServices::DataLocation)
            #else
                QStandardPaths::standardLocations(
                    QStandardPaths::DataLocation).at(0)
            #endif
                );

    d.cd(DATA_FLD_TRANSL);
    translator.load(
                d.absoluteFilePath(
                    LANG_FILE(locale)));
    qApp->installTranslator(&translator);

    LIBCLB_DEBUG("LIBCLB: translator was installed\n");
    return true;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void clb_abrupt_exit (const QString & s_message)
{
    LIBCLB_DEBUG("LIBCLB: %s\n", TMP_A(s_message));
    QMessageBox mbox(
                QMessageBox::Critical,
                QObject::tr("Fatal Error"),
                QObject::tr("%1 has encountered a fatal error and cannot continue to work.\n"
                            "Please press OK button to quit.").arg(qApp->applicationName()),
                QMessageBox::Ok,
                NULL);
    mbox.setDetailedText(s_message);
    mbox.exec();
    qApp->quit();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
LibCLB::LibCLB (bool b_gui_mode) :
    mw_(NULL),
    gui_mode_(b_gui_mode)
{
    DBG_ASSERT (singleton_ == NULL);
    singleton_ = this;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool LibCLB::init (bool b_gui_mode)
{
    if (singleton_ == NULL) {
        initEmpty (b_gui_mode);
        return singleton_->continueInit();
    }
    return true;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool LibCLB::continueInit ()
{
    bool b_ret = false;
    for (;;) {
        if (clb_patial_init_ == false) {
            return true;
        }
        clb_patial_init_ = false;

        LIBCLB_DEBUG("LIBCLB: ==========================================\n");
        LIBCLB_DEBUG("LIBCLB: lib clb is being initialized\n");
        LIBCLB_DEBUG("LIBCLB: ==========================================\n");
        emit libStarting();

#	if defined(CLB_FORCE_DEBUG)
        LIBCLB_DEBUG ("LIBCLB: Release version with debug information\n");
#	elif defined(CLB_DEBUG)
        LIBCLB_DEBUG ("LIBCLB: Debug version\n");
#	else
        LIBCLB_DEBUG ("LIBCLB: Release version\n");
#	endif

        // prepare Qt system
        Q_INIT_RESOURCE(application);

        // prepare messaging system
        UserMsgMan::init();
        // prepare translations
        start_translation ();

        LIBCLB_DEBUG("LIBCLB: ==========================================\n");
        LIBCLB_DEBUG("LIBCLB: lib clb was started\n");
        LIBCLB_DEBUG("LIBCLB: ==========================================\n");

        emit libStarted();

        b_ret = true;
        break;
    }
    return b_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
LibCLB * LibCLB::initEmpty (bool b_gui_mode)
{
    if (singleton_ == NULL) {
        new LibCLB (b_gui_mode);
        clb_patial_init_ = true;
    }
    return singleton_;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void LibCLB::end ()
{
    if (singleton_ != NULL) {
        singleton_->_end();
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void LibCLB::_end ()
{
    LIBCLB_DEBUG("LIBCLB: ==========================================\n");
    LIBCLB_DEBUG("LIBCLB: lib clb is being terminated\n");
    LIBCLB_DEBUG("LIBCLB: ==========================================\n");
    emit libEnding();

    clb_shutting_down_ = true;
    UserMsgMan::disable();

    // stop messaging system
    UserMsgMan::end ();

    emit libEnded();
    LIBCLB_DEBUG("LIBCLB: ==========================================\n");
    LIBCLB_DEBUG("LIBCLB: lib clb was terminated\n");
    LIBCLB_DEBUG("LIBCLB: ==========================================\n");

    singleton_->deleteLater();
    singleton_ = NULL;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool LibCLB::startGui ()
{
    if (singleton_ == NULL) {
        init (true);
    }
    return singleton_->_startGui();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool LibCLB::_startGui ()
{
    emit guiStarting ();
    mw_ = clb_lib_show_main_gui (QString());
    //    connect (mw_, SIGNAL(guiEnding()), this, SIGNAL(guiEnding()));
    //    connect (mw_, SIGNAL(guiEnded()), this, SIGNAL(guiEnded()));
    emit guiStarted();
    return true;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
LibCLB * LibCLB::uniq ()
{
    if (singleton_ == NULL) init (false);
    return singleton_;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool LibCLB::isGUI ()
{
    if (uniq() == NULL) return false;
    return uniq()->gui_mode_;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void LibCLB::setGUI (bool b_gui)
{
    if (uniq() == NULL) return;
    uniq()->gui_mode_ = b_gui;
}
/* ========================================================================= */

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
