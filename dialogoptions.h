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

    bool clearData();

private slots:
    void on_toolButtonAdd_clicked();

    void on_toolButtonRemove_clicked();

    void on_buttonBox_accepted();

    void on_checkBoxRemove_toggled(bool checked);

private:
    Ui::DialogOptions *ui;
};

#endif // DIALOGOPTIONS_H
