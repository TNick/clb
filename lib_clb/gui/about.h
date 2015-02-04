#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>

namespace Ui {
class About;
}

class SnippetsDlg;

class About : public QDialog
{
    Q_OBJECT

public:
    explicit About(SnippetsDlg *parent = 0);
    ~About();

private:
    Ui::About *ui;
    SnippetsDlg * parent_;
};

#endif // ABOUT_H
