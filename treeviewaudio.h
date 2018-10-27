#ifndef TREEVIEWAUDIO_H
#define TREEVIEWAUDIO_H

#include <QObject>
#include <QTreeView>
class TreeViewAudio : public QTreeView
{
      Q_OBJECT
public:
    TreeViewAudio();

    void resizeEvent(QResizeEvent *event);
public slots:
    void customContextMenu(QPoint);
signals:
   void treeAudioClicked(const QModelIndex);
   void editCurIndex(const QString);
private slots:
    void addCurentToPLaylist();
    void favoriteCurent();
     void editCurent();
signals:
    void treewidthChanged(int );
};

#endif // TREEVIEWAUDIO_H
