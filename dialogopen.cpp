#include "dialogopen.h"
#include "ui_dialogopen.h"
#include <QFileDialog>
DialogOpen::DialogOpen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOpen)
{
    ui->setupUi(this);
}

DialogOpen::~DialogOpen()
{
    delete ui;
}

void DialogOpen::on_pushButton_clicked()
{
   mlist= QFileDialog::getOpenFileUrls(this,QString());//TODO :fix this
    if(!mlist.isEmpty()){
        accept();
    }

}

void DialogOpen::on_buttonBox_accepted()
{
    QUrl url=ui->lineEdit->text();
    if(url.isValid())
        mlist.append(url);
}

 bool DialogOpen::autoSave()
 {
    return ui->checkBox->isChecked();
 }
