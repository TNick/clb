#ifndef SNIPPETSDLG_H
#define SNIPPETSDLG_H

#include <clb/clb.h>
#include <QWidget>

namespace Ui {
class SnippetsDlg;
}

class SnipModel;
class QTreeWidgetItem;

class CLB_MON_EXPORT SnippetsDlg : public QWidget
{
    Q_OBJECT

public:
    explicit SnippetsDlg(QWidget *parent = 0);
    ~SnippetsDlg();



public slots:
    bool loadXMLFile(const QString &s_file);
    bool saveXMLFile(const QString &s_file);

    const QString & timerFile () const { return s_timer_file_; }
    const QString & lastSavedFile () const { return s_last_saved_file_; }

private slots:
    void on_database_browse_clicked();

    void on_database_path_editingFinished();

    void on_database_path_returnPressed();

    void on_tv_content_customContextMenuRequested(const QPoint &pos);

    void on_actionAdd_new_group_triggered();

    void on_actionAdd_new_snipped_triggered();

    void on_actionRemove_triggered();

    void on_actionSave_database_triggered();

    void on_actionReload_database_triggered();

    void on_tv_content_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_b_icon_clicked();

    void on_le_name_returnPressed();

    void on_minimize();

    void timerEvent ();

    void on_help ();

    void on_le_link_returnPressed();

    void on_goToLink_clicked();

    void on_actionShow_deleted_items_triggered();

    void on_actionCast_triggered();

protected:

    void
    closeEvent (
            QCloseEvent *);

private:
    Ui::SnippetsDlg *ui;
    SnipModel *model_;
    int save_timer_;
    QString s_timer_file_;
    QString s_last_saved_file_;


    bool _saveXMLFile(const QString &s_file);
};

#endif // SNIPPETSDLG_H
