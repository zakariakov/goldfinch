#ifndef DIALOGOPTIONS_H
#define DIALOGOPTIONS_H

#include <QDialog>

namespace Ui {
class DialogOptions;
}

class DialogOptions : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOptions(QWidget *parent = nullptr);
    ~DialogOptions();

private slots:
    void on_toolButtonAdd_clicked();

    void on_toolButtonRemove_clicked();

    void on_buttonBox_accepted();

private:
    Ui::DialogOptions *ui;
};

#endif // DIALOGOPTIONS_H
