#include "about.h"
#include "ui_about.h"
#include "snippetsdlg.h"

#include <QPlainTextEdit>
#include <QApplication>

About::About(SnippetsDlg *parent) :
    QDialog(parent),
    ui(new Ui::About),
    parent_(parent)
{
    ui->setupUi(this);
    QString s_value;
    s_value =
            QApplication::applicationDisplayName () + "\n" +
            "Version: " + QApplication::applicationVersion () + "\n" +
            "\n" +
            "Application path: " + QApplication::applicationDirPath () + "\n" +
            "Temporary backup file (5 sec): " + parent_->timerFile() + "\n" +
            "Last saved file: " + parent_->lastSavedFile() + "\n"
            ;

    ui->ab_txt->setPlainText (s_value);
}

About::~About()
{
    delete ui;
}
