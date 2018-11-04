#include "searchbar.h"
#include "tumb.h"
#include <QBoxLayout>
#include <QLineEdit>
#include <QToolButton>
#include <QActionGroup>
#include <QMenu>
SearchBar::SearchBar(QWidget *parent) : QWidget(parent)
{

  //  setMinimumHeight(25);
    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    QBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);

    QWidget *widcontent=new QWidget;
    widcontent->setObjectName("SearchBar");

    lineSearch=new QLineEdit;
    lineSearch->setObjectName("LineEditSearch");
    lineSearch->setFrame(false);
    lineSearch->setPlaceholderText("search");

    QToolButton *tbClear=new QToolButton;
    tbClear->setObjectName("TBSearch");
    tbClear->setIcon(Tumb::icon(I_CLEAR));
    tbClear->setToolTip(tr("Clear Search"));

    QToolButton *tbOption=new QToolButton;
    tbOption->setObjectName("TBSearch");
    tbOption->setIcon(Tumb::icon(I_OPTIONS));
    tbOption->setPopupMode(QToolButton::InstantPopup);
    tbOption->setToolTip(tr("Search Options"));

    QToolButton *tbCloseSearch=new QToolButton;
    tbCloseSearch->setObjectName("TBSearch");
    tbCloseSearch->setIcon(Tumb::icon(I_Close));
    tbCloseSearch->setToolTip(tr("Hide search bar"));

    QBoxLayout *layoutSearch = new QHBoxLayout;
    layoutSearch->setContentsMargins(3,2,3,2);
    layoutSearch->setSpacing(0);

    layoutSearch->addWidget(tbClear);
    layoutSearch->addWidget(lineSearch);
    layoutSearch->addWidget(tbOption);
    layoutSearch->addWidget(tbCloseSearch);

    widcontent->setLayout(layoutSearch);

    layout->addWidget(widcontent);
    setLayout(layout);

    QActionGroup *actGroup =new QActionGroup(this);
    QMenu *menu=new QMenu;

    QAction *actTitle=menu->addAction(tr("Title"));
    actTitle->setData(1);
    actTitle->setCheckable(true);
    actTitle->setChecked(true);

    QAction *actAlbum=menu->addAction(tr("Album"));
    actAlbum->setData(3);
    actAlbum->setCheckable(true);

    QAction *actArtist=menu->addAction(tr("Artist"));
    actArtist->setData(2);
    actArtist->setCheckable(true);

    actGroup->addAction(actTitle);
    actGroup->addAction(actAlbum);
    actGroup->addAction(actArtist);

    tbOption->setMenu(menu);

    connect(actGroup,&QActionGroup::triggered,this,&SearchBar::setSarchOption);
    connect(lineSearch,&QLineEdit::textChanged,this,&SearchBar::setSearchText);
    connect(tbClear,&QToolButton::clicked,this,&SearchBar::clearText);
    connect(tbCloseSearch,&QToolButton::clicked,this,&SearchBar::hide);
}

void SearchBar::setSarchOption(QAction *act)
{

    int colum=act->data().toInt();
    mColumn=colum;
    setSearchText(lineSearch->text());
}

void SearchBar::setSearchText(const QString &text)
{

    if(text.length()<3)return;
    emit searchTextChanged(mColumn,text);

}

void SearchBar::clearText()
{

    lineSearch->clear();
    emit clearSearching();

}
