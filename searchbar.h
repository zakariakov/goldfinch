#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>

//#define COL_I_TITLE      1
//#define COL_I_ARTIST     2
//#define COL_I_ALBUM      3
class SearchBar: public QWidget
{
    Q_OBJECT
public:
    SearchBar(QWidget *parent = nullptr);
public slots:
   void showHid(){isHidden()?show():hide();lineSearch->setFocus();}
signals:

    void searchTextChanged(int,const QString &);
    void clearSearching();

private slots:

    void setSarchOption(QAction *act);
    void setSearchText(const QString &text);
    void clearText();

private:

    QLineEdit *lineSearch;
    int mColumn=1;

};

#endif // SEARCHBAR_H
