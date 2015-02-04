#include "snippetsdlg.h"
#include "ui_snippetsdlg.h"
#include "clb/config.h"
#include "about.h"

#include <clb/snipitem.h>
#include <clb/snipmodel.h>
#include <usermsg/usermsg.h>

#include <QFileDialog>
#include <QSettings>
#include <QStandardPaths>
#include <QFile>
#include <QDomDocument>
#include <QTextStream>
#include <QMenu>
#include <QMessageBox>
#include <QShortcut>
#include <QTemporaryFile>
#include <QToolTip>
#include <QDesktopServices>

#define STG_SNIPP_GEOMETRY "GUI/SNIPP/GEOMETRY"
#define STG_SNIPP_TV_STATE "GUI/SNIPP/TV/STATE"
#define STG_SNIPP_CODE_STATE "GUI/SNIPP/CODE/STATE"
#define STG_SNIPP_DB        "SNIPPETS/DATABASE"
#define STG_ENVAR_DB        "ENV_VAR/DATABASE"

QDomDocument createNewXml()
{
    QDomDocument document;
    QDomProcessingInstruction header = document.createProcessingInstruction(
                "xml", "version=\"1.0\" encoding=\"UTF-8\"");
    document.appendChild (header);

    QDomElement root = document.createElement ("snippets");
    document.appendChild(root);

    return document;
}

SnippetsDlg::SnippetsDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SnippetsDlg),
    model_(NULL),
    save_timer_(0),
    s_timer_file_(),
    s_last_saved_file_()
{

    ui->setupUi(this);
    model_ = new SnipModel (ui->tv_content);

    new QShortcut(QKeySequence(Qt::Key_Escape), this, SLOT(on_minimize()));
    new QShortcut(QKeySequence(Qt::Key_F1), this, SLOT(on_help()));
    // enum ColorGroup { Active, Disabled, Inactive, NColorGroups, Current, All, Normal = Active };

    QPalette p = ui->tv_content->palette();
    p.setColor(QPalette::Inactive, QPalette::Highlight, qRgb (255, 85, 0));
    p.setColor(QPalette::Inactive, QPalette::HighlightedText, qRgb(255, 255, 127));
    ui->tv_content->setPalette(p);

    QSettings stg;

    QString s_path = stg.value (STG_SNIPP_DB).toString ();
    if (s_path.isEmpty ()) {
        s_path = QStandardPaths::standardLocations (
                    QStandardPaths::DataLocation).at (0);
        QDir d(s_path);
        if (!d.exists ()) {
            d.mkpath (".");
        }
        s_path = d.absoluteFilePath ("snippets.xml");
        QFile file (s_path);
        if (!file.exists ()) {
            if (file.open (QIODevice::WriteOnly)) {
                QTextStream txs (&file);

                txs << "<?xml version='1.0' encoding='UTF-8'?>\n"
                    << "<snippets>\n"
                    << "</snippets>\n";
                file.close();
            }
        }
    }
    ui->database_path->setText (s_path);


    restoreGeometry (
                stg.value(
                    STG_SNIPP_GEOMETRY).toByteArray());
    ui->tv_content->header()->restoreState (
                stg.value(
                    STG_SNIPP_TV_STATE).toByteArray());
    stg.beginGroup (STG_SNIPP_CODE_STATE);
    ui->tx_content->restoreState (stg);
    stg.endGroup ();

    on_tv_content_currentItemChanged (NULL, NULL);

    loadXMLFile (s_path);

    QTemporaryFile tmr_file;
    if (tmr_file.open ()) {
        tmr_file.close ();
        s_timer_file_ = tmr_file.fileName ();
    } else {
        s_timer_file_ = ".clb_temp_file";
    }

    save_timer_ = startTimer (5000);
}

SnippetsDlg::~SnippetsDlg()
{
    QFile tmr(s_timer_file_);
    if (tmr.exists ()) {
        tmr.remove ();
    }

    delete ui;
}

void SnippetsDlg::on_minimize()
{
    QWidget::setWindowState (Qt::WindowMinimized);
}

void SnippetsDlg::on_help()
{
    About ab(this);
    ab.exec ();
}

void SnippetsDlg::timerEvent()
{
    QFile tmr(s_timer_file_);
    if (tmr.exists ()) {
        tmr.remove ();
    }

    _saveXMLFile (s_timer_file_);
}

void SnippetsDlg::closeEvent(QCloseEvent *)
{
    saveXMLFile (ui->database_path->text ());

    QSettings s;
    s.setValue (STG_SNIPP_GEOMETRY, saveGeometry ());
    s.setValue (STG_SNIPP_TV_STATE, ui->tv_content->header()->saveState ());
    s.beginGroup (STG_SNIPP_CODE_STATE);
    ui->tx_content->saveState (s);
    s.endGroup ();
}

void SnippetsDlg::on_database_browse_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(
                this, tr("Open Database"),
                ui->database_path->text (), tr("XML files (*.xml)"));

    if (!filePath.isEmpty()) {
        ui->database_path->setText (filePath);
        loadXMLFile (ui->database_path->text());
    }
}

void SnippetsDlg::on_database_path_editingFinished()
{
}

bool SnippetsDlg::loadXMLFile(const QString & s_file)
{
    bool b_ret = false;

    for (;;) {


        // open the file
        QFile file (s_file);
        if (!file.open(QIODevice::ReadOnly)) {
            UserMsg::err (
                        tr("Failed to open file %1\n%2")
                        .arg(s_file)
                        .arg(file.errorString ()));
            break;
        }

        QSettings stg;
        stg.setValue (STG_SNIPP_DB, s_file);

        // get the tree parsed
        QDomDocument document;
        if (!document.setContent (&file)) {
            UserMsg::err (
                        tr ("The document %1 is not a valid XML file")
                        .arg(s_file));
            break;
        }

        // build the model
        if (model_ != NULL) delete model_;
        model_ = NULL;
        SnipModel *newModel = new SnipModel (ui->tv_content, document);
        model_ = newModel;

        // close the source file
        file.close();

        b_ret = true;
        break;
    }

    return b_ret;
}


bool SnippetsDlg::saveXMLFile (const QString & s_file)
{
    bool b_ret = false;

    for (;;) {

        // back-up the original file, if any.
        QFile file (s_file);
        for (;;) {
            if (file.exists ()) {
                if (file.size () > 0) {
                    QFile bk1 (s_file + ".bak");
                    if (bk1.exists ()) {
                        if (bk1.size () > 0) {
                            QFile bk2(s_file + ".bk2");
                            if (bk2.exists ()) bk2.remove ();
                            bk1.copy (bk2.fileName ());
                        }
                        bk1.remove ();
                    }
                    file.copy (bk1.fileName ());
                }
            }

            break;
        }

        b_ret = _saveXMLFile (s_file);
        if (b_ret) s_last_saved_file_ = s_file;
        break;
    }

    return b_ret;
}

bool SnippetsDlg::_saveXMLFile (const QString & s_file)
{
    bool b_ret = false;

    for (;;) {

        if (model_ == NULL) {
            UserMsg::err ("No document to save");
            break;
        }

        QTreeWidgetItem * tvi = ui->tv_content->currentItem ();
        if (tvi != NULL)
            on_tv_content_currentItemChanged (tvi, tvi);

        // open the file
        QFile file (s_file);
       if (!file.open(QIODevice::WriteOnly)) {
            UserMsg::err (
                        tr("Failed to open file %1\n%2")
                        .arg (s_file)
                        .arg (file.errorString ()));
            break;
        }

        QTextStream txs (&file);
        QDomDocument doc ("snippets");
        model_->saveXML (doc);
        doc.save (txs, 2);

        // close the source file
        file.close();

        b_ret = true;
        break;
    }

    return b_ret;
}

void SnippetsDlg::on_database_path_returnPressed()
{
    loadXMLFile (ui->database_path->text());
}

void SnippetsDlg::on_tv_content_customContextMenuRequested(
        const QPoint &pos)
{
    SnipItem * crt = model_->currentItem ();

    QMenu mnu;
    mnu.addAction (ui->actionAdd_new_group);
    mnu.addAction (ui->actionAdd_new_snipped);
    mnu.addAction (ui->actionCast);

    if (crt == NULL) {
        ui->actionCast->setEnabled (false);
        ui->actionCast->setText ("Cast to item");
    } else if (crt != model_->root_) {
        if (crt->isGrup ()) {
            SnipGroup * sgrp = static_cast<SnipGroup *>(crt);
            ui->actionCast->setEnabled (sgrp->childCount () == 0);
            ui->actionCast->setText ("Cast to item");
        } else {
            ui->actionCast->setEnabled (true);
            ui->actionCast->setText ("Cast to group");
        }
    } else {
        ui->actionCast->setEnabled (false);
        ui->actionCast->setText ("Cast to item");
    }
    mnu.addSeparator ();
    mnu.addAction (ui->actionRemove);
    mnu.addSeparator ();
    mnu.addAction (ui->actionShow_deleted_items);
    ui->actionShow_deleted_items->setEnabled (model_->deletedCount() > 0);
    mnu.addSeparator ();
    mnu.addAction (ui->actionReload_database);
    mnu.addAction (ui->actionSave_database);

    mnu.exec (mapToGlobal (pos));
}

void SnippetsDlg::on_actionAdd_new_group_triggered()
{
    for (;;) {
        if (model_ == NULL) {
            UserMsg::err ("Please select a file, first");
            break;
        }

        SnipGroup * msel = model_->currentGroup();
        if (msel != NULL) {
            model_->addGroup (tr("New group"), msel);
            ui->le_name->setFocus ();
            ui->le_name->selectAll ();
            break;
        }

        model_->addRootGroup (tr("New group"));
        ui->le_name->setFocus ();
        ui->le_name->selectAll ();
        break;
    }

}

void SnippetsDlg::on_actionAdd_new_snipped_triggered()
{
    for (;;) {
        if (model_ == NULL) {
            UserMsg::err ("Please select a file, first");
            break;
        }

        SnipGroup * msel = model_->currentGroup();
        if (msel != NULL) {
            model_->addSnip (tr("New snippet"), msel);
            ui->le_name->setFocus ();
            ui->le_name->selectAll ();
        } else {
            UserMsg::err (tr ("Please select the location, first"));
        }
        break;
    }

}

void SnippetsDlg::on_actionRemove_triggered()
{
    SnipItem * msel = model_->currentItem ();
    model_->removeItem (msel);
}

void SnippetsDlg::on_actionSave_database_triggered()
{
    saveXMLFile (ui->database_path->text());
}

void SnippetsDlg::on_actionReload_database_triggered()
{
    int res = QMessageBox::question (
                this, tr("Reload content"),
                tr("You are about to reload the content "
                   "and discard all changes made to this file\n\n"
                   "Are you sure?"),
                QMessageBox::Yes, QMessageBox::No);
    if (res == QMessageBox::No) return;
    loadXMLFile (ui->database_path->text());
}

void SnippetsDlg::on_tv_content_currentItemChanged(
        QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    model_->saveToItem(
                static_cast<SnipItem*>(previous),
                ui->le_name->text (),
                ui->le_link->text (),
                ui->le_icon->currentText (),
                ui->tx_content->toPlainText ());

    QString s_name;
    QString s_link;
    QString s_icon;
    QString s_content;

    model_->getFromItem(
                static_cast<SnipItem*>(current),
                s_name,
                s_link,
                s_icon,
                s_content);
    ui->le_name->setText (s_name);
    ui->le_link->setText (s_link);
    ui->le_icon->setCurrentText (s_icon);
    ui->tx_content->setPlainText (s_content);

    if (current == NULL) {
        ui->le_name->setEnabled (false);
        ui->le_icon->setEnabled (false);
        ui->tx_content->setEnabled (false);
        ui->icon_sample->setPixmap (QPixmap());
        ui->le_link->setEnabled (false);
    } else {
        SnipItem * sp = static_cast<SnipItem*>(current);
        QIcon ic = sp->icon();
        ui->icon_sample->setPixmap (ic.pixmap (16, 16));
        if (sp->isGrup () || (sp->parent() == NULL)) {
            ui->le_name->setEnabled (true);
            ui->le_icon->setEnabled (true);
            ui->tx_content->setEnabled (true);
            ui->le_link->setEnabled (true);
        } else {
            ui->le_name->setEnabled (true);
            ui->le_icon->setEnabled (true);
            ui->tx_content->setEnabled (true);
            ui->le_link->setEnabled (true);
        }
    }
}

void SnippetsDlg::on_b_icon_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(
                this, tr("Open Icon"),
                ui->le_icon->currentText (), tr("Image files (*.png)"));

    if (!filePath.isEmpty()) {
        ui->le_icon->setCurrentText (filePath);
        QIcon ic (filePath);
        if (ic.isNull ()) {
            ui->icon_sample->setPixmap (QPixmap());
        } else {
            ui->icon_sample->setPixmap (ic.pixmap (16, 16));
        }
    }
}

void SnippetsDlg::on_le_name_returnPressed()
{
    QTreeWidgetItem * tvi = ui->tv_content->currentItem ();
    if (tvi != NULL)
        on_tv_content_currentItemChanged (tvi, tvi);
}

void SnippetsDlg::on_le_link_returnPressed()
{
    on_le_name_returnPressed ();
}

void SnippetsDlg::on_goToLink_clicked()
{
    QUrl ur_x(ui->le_link->text ());
    if (ur_x.isValid ()) {
        QDesktopServices::openUrl (ur_x);
    } else {
        QToolTip::showText (
                    ui->le_name->mapToGlobal (
                        QPoint( 0, 0 ) ), "Link is not valid");
    }
}

void SnippetsDlg::on_actionShow_deleted_items_triggered()
{
    if (model_->deletedCount() > 0) {
        model_->restoreDeleted ();
    }
}

void SnippetsDlg::on_actionCast_triggered()
{
    SnipItem * crt = model_->currentItem ();
    if (crt->isGrup ()) {
        SnipGroup * grp = static_cast<SnipGroup*>(crt);
        if (grp == model_->root_) return;
        model_->castToItem (grp);
    } else {
        SnipSnip * snp = static_cast<SnipSnip*>(crt);
        model_->castToGroup (snp);
    }
}
