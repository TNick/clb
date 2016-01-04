#ifndef SNIPITEM_H
#define SNIPITEM_H

#include <QTreeWidgetItem>

class SnipGroup;
class SnipSnip;

class SnipItem : public QTreeWidgetItem
{
public:
    SnipItem();
    virtual ~SnipItem();

    virtual bool
    isGrup() = 0;

    void
    setName (
            const QString & s_value) {
        setText (0, s_value);
    }

    void
    setLink (
            const QString & s_value) {
        setText (2, s_value);
    }

    void
    setIcon (
            const QString & s_value);

    const QString &
    iconString () {
        return s_icon_;
    }

    QString
    name () {
        return text (0);
    }

    QString
    link () {
        return text (2);
    }

    QIcon
    icon () {
        return QTreeWidgetItem::icon (0);
    }

    void
    setDefaultIcon();

    void
    setContent(
            const QString &s_value) {
        setText (1, s_value);
    }

    QString
    content() const {
        return text (1);
    }

    SnipGroup *parentSnip();
    QString s_icon_;
};

class SnipGroup : public SnipItem
{
public:
    SnipGroup() : SnipItem () {
        setFlags (Qt::ItemIsSelectable |
                  Qt::ItemIsEnabled |
                  Qt::ItemIsDragEnabled |
                  Qt::ItemIsDropEnabled);
    }
    virtual ~SnipGroup() {}

    virtual bool
    isGrup() {return true; }

    SnipGroup * group (
            const QString & s_name) const;

    SnipSnip * snip (
            const QString & s_name) const;

    SnipItem * item (
            const QString & s_name) const;

};

class SnipSnip : public SnipItem
{
public:
    SnipSnip() : SnipItem () {
        setFlags (Qt::ItemIsSelectable |
                  Qt::ItemIsEnabled |
                  Qt::ItemIsDragEnabled);
    }
    virtual ~SnipSnip() {}

    virtual bool
    isGrup() {return false; }

};


#endif // SNIPITEM_H
