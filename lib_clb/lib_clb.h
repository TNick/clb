/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			dot-editor.h
  \date			February 2014
  \author		TNick

*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef LIB_CLB_MAIN_H_INCLUDE
#define LIB_CLB_MAIN_H_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <clb/clb.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */


//! terminate the application due to an error
///
CLB_MON_EXPORT void
clb_abrupt_exit (const QString & s_message);

//! tell if we're shutting down
///
CLB_MON_EXPORT bool
clb_shutting_down();


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
/*  CLASS        ----------------------------------------------------------- */

class CLB_MON_EXPORT LibCLB : public QObject {
    Q_OBJECT
    //
    //
    //
    //
    /*  DEFINITIONS    ----------------------------------------------------- */

public:

    /*  DEFINITIONS    ===================================================== */
    //
    //
    //
    //
    /*  DATA    ------------------------------------------------------------ */

private:

    QWidget * mw_; /**< main GUI object */
    bool gui_mode_; /**< is this a GUI application or not */

    static LibCLB * singleton_;

    /*  DATA    ============================================================ */
    //
    //
    //
    //
    /*  FUNCTIONS    ------------------------------------------------------- */

private:


    //! constructor
    ///
    LibCLB (
            bool b_gui_mode);

    //! copy constructor
    ///
    LibCLB (const LibCLB &) {}

    //! assignment operator
    ///
    LibCLB& operator=( const LibCLB& ) {
        return *this;
    }

    //! destructor
    ///
    ~LibCLB () {}


public:


    //! start-up the mbtiles
    ///
    bool
    continueInit ();

public:

    //! start-up the mbtiles
    ///
    static bool
    init (
            bool b_gui_mode);

    //! start-up the mbtiles
    ///
    static LibCLB *
    initEmpty (
            bool b_gui_mode);

    //! start the gui
    ///
    static bool
    startGui ();

    //! end-up the mbtiles
    ///
    static void
    end ();

    //! the singleton
    ///
    static LibCLB *
    uniq ();

    //! GUI or console mode
    ///
    static  bool
    isGUI ();

    //! GUI or console mode
    ///
    static  void
    setGUI (
            bool b_gui);

signals:

    //! library is starting
    ///
    void
    libStarting ();

    //! library is started
    ///
    void
    libStarted ();

    //! library is ending
    ///
    void
    libEnding ();

    //! library is ended
    ///
    void
    libEnded ();

    //! gui is ending
    ///
    void
    guiStarting ();

    //! gui is ended
    ///
    void
    guiStarted ();

    //! gui is ending
    ///
    void
    guiEnding ();

    //! gui is ended
    ///
    void
    guiEnded ();


private:

    //! end-up the mbtiles
    ///
    void
    _end ();

    //! start the gui
    ///
    bool
    _startGui ();
};

/*  CLASS        =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#endif /* LIB_CLB_MAIN_H_INCLUDE */
