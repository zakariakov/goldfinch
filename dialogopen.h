#ifndef DIALOGOPEN_H
#define DIALOGOPEN_H

#include <QDialog>
#include <QUrl>
namespace Ui {
class DialogOpen;
}

class DialogOpen : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOpen(QWidget *parent = nullptr);
    ~DialogOpen();
  QList<QUrl>listUrls(){return  mlist;}
  bool autoSave();
private slots:
    void on_pushButton_clicked();

    void on_buttonBox_accepted();

private:
    Ui::DialogOpen *ui;
    QList<QUrl>mlist;
};

#endif // DIALOGOPEN_H
