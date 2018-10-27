#include "dialogoptions.h"
#include "defines.h"
#include "ui_dialogoptions.h"
#include <QFileDialog>
#include <QListWidgetItem>
#include <QSettings>
#include <QApplication>

DialogOptions::DialogOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOptions)
{
    ui->setupUi(this);

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
    item->setCheckState(Qt::Unchecked);

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

}
