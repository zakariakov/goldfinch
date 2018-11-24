/***************************************************************************
 *      Project created by QtCreator 2018-06-01T17:15:24                   *
 *                                                                         *
 *    goldfinch Copyright (C) 2014 AbouZakaria <yahiaui@gmail.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "dialogoptions.h"
#include "defines.h"
#include "tumb.h"
#include "ui_dialogoptions.h"
#include <QFileDialog>
#include <QListWidgetItem>
#include <QSettings>
#include <QApplication>
#include <QMessageBox>
#include <QStyleFactory>
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

    //-------------------------------------------------------------------------
    QStringList list=QStyleFactory::keys();
    list.insert(0,tr("Default"));
    ui->comboBoxStyls->addItems(list);
        settings.beginGroup("Window");
        ui->comboBoxStyls->setCurrentText(settings.value("Style").toString());
      settings.endGroup();
    
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


}

void DialogOptions::on_checkBoxRemove_toggled(bool checked)
{
    if(checked)
        QMessageBox::warning(this,tr("Warning"),tr("This process will delete all your data and previous edits"));
}

 bool DialogOptions::clearData()
 {
     return ui->checkBoxRemove->isChecked();
 }

void DialogOptions::on_comboBoxStyls_activated(const QString &arg1)
{
    if(ui->comboBoxStyls->currentIndex()!=0)
         QApplication::setStyle( QStyleFactory::create(arg1));

    QSettings settings;
        settings.beginGroup("Window");
        settings.setValue("Style",arg1);
        settings.endGroup();

}
