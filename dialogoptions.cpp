#include "dialogoptions.h"
#include "defines.h"
#include "tumb.h"
#include "ui_dialogoptions.h"
#include <QFileDialog>
#include <QListWidgetItem>
#include <QSettings>
#include <QApplication>
#include <QMessageBox>
DialogOptions::DialogOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOptions)
{
    ui->setupUi(this);
ui->toolButtonAdd->setIcon(Tumb::icon(I_ADD));
ui->toolButtonRemove->setIcon(Tumb::icon(I_LIST_REMOVE));

    QSettings settings;
    int count = settings.beginReadArray("Directory");

    if(count==0){
        QListWidgetItem *item=new QListWidgetItem(D_DMUSIC,ui->listWidget);
        item->setCheckState(Qt::Checked);
    }

    for (int i = 0; i < count; ++i) {
        settings.setArrayIndex(i);

        QString dir =settings.value("Dir").toString();
     //   bool checked=settings.value("Checked").toBool();

        QListWidgetItem *item=new QListWidgetItem(dir,ui->listWidget);
        item->setCheckState(Qt::Unchecked);
       // item->setCheckState(checked ? Qt::Checked:Qt::Unchecked);

    }

    settings.endArray();

}

DialogOptions::~DialogOptions()
{
    delete ui;
}

void DialogOptions::on_toolButtonAdd_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    QDir::homePath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);


    if(dir.isEmpty())return;

    QListWidgetItem *item=new QListWidgetItem(dir,ui->listWidget);
    item->setCheckState(Qt::Checked);

}

void DialogOptions::on_toolButtonRemove_clicked()
{
     QListWidgetItem *item=ui->listWidget->currentItem();
     if(item)
         ui->listWidget->takeItem(ui->listWidget->currentRow());
}


void DialogOptions::on_buttonBox_accepted()
{


    QSettings settings;

    settings.beginWriteArray("Directory");

   foreach (QString key,  settings.allKeys()) {
       settings.remove(key);
   }
    int count= ui->listWidget->count();
    for (int i = 0; i < count; ++i) {
        QListWidgetItem *item=ui->listWidget->item(i);
        QString dir =item->text();
        Qt::CheckState ch=item->checkState();
        bool cheked=ch==Qt::Checked ? true : false;


        settings.setArrayIndex(i);
        settings.setValue("Dir", dir);
        settings.setValue("Checked", cheked);
    }
      settings.endArray();
 settings.beginGroup("Options") ;
 settings.setValue("Clear",ui->checkBoxRemove->isChecked());
settings.endGroup();

}

void DialogOptions::on_checkBoxRemove_toggled(bool checked)
{
    if(checked)
        QMessageBox::warning(this,tr("Warning"),tr("This process will delete all your data and previous edits"));
}
