/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file   main.cc
  \author TNick

  \brief  Contains application's entry point


*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <clb/clb.h>
#include <clb/lib_clb.h>
#include <clb/gui_main.h>
#include <usermsg/usermsg.h>

#include <QDebug>
#include <QTranslator>
#include <QDir>
#include <QDesktopServices>
#include <QApplication>
#include <QThread>

#ifdef TARGET_SYSTEM_WIN32
#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <Fcntl.h>
#endif

/*  INCLUDES    ============================================================ */
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

/* ------------------------------------------------------------------------- */
void  printUsage   ( void )
{
    qDebug()
            << "Usage:\n"
            << "clb\n"
            << QObject::tr( "  to start the program" ) << "\n\n"
               ;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool  parseArgs   (
        int argc, char *argv[], int * ret_val, QString & s_init_file )
{

    int b = 0;

    for ( int i = 1; i < argc; i++ ) {

        b = 0;
        if ( argv[i][0] == '-' ) {
            b = 1;
            if ( argv[i][1] == '-' )
            {
                b = 2;
            }
            qDebug() << QString( QObject::tr(
                                     "ERROR! Argument %1 was not understood." ) )
                        .arg( QString( argv[i]+b ) )
                     << "\n\n";
            printUsage();
            *ret_val = -1;
            return false;
        } else {
            if ( s_init_file.isEmpty() ) {
                s_init_file = argv[i];
            } else {
                qDebug() << QObject::tr( "ERROR! Only one file should be provided as argument." )
                         << "\n\n";
                printUsage();
                *ret_val = -1;
                return false;
            }
        }
    }

    return true;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
int   actual_main(int argc, char *argv[])
{

    int ret_val = 0;

    /* prepare to start */
    QApplication apl (argc, argv);

    /* prepare the ground for easy access to settings */
    QCoreApplication::setOrganizationName("clb");
    QCoreApplication::setOrganizationDomain("clb.org");
    QCoreApplication::setApplicationName("clb");


    // prepare the library
    if (!LibCLB::init (true)) {
        printf("%s\n\n", TMP_A(QObject::tr("ERROR! Failed to initialize CutTiles library.")));
        return 1;
    }

    // parse arguments; may exit
    QString s_init_file;
    if (parseArgs (argc, argv, &ret_val, s_init_file) == false) {
        LibCLB::end();
        return ret_val;
    }

    // prepare the gui and show them
    if (!LibCLB::startGui ()) {
        printf("%s\n\n", TMP_A(QObject::tr("ERROR! Failed to initialize GUI components of  CutTiles library.")));
        LibCLB::end();
        return 1;
    }

    // run main loop
    ret_val = apl.exec();


    // terminate the library
    LibCLB::end ();

    return ret_val;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
int   main    ( int argc, char *argv[] )
{
    return actual_main(argc, argv);
}
/* ========================================================================= */

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
