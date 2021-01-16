﻿/***************************************************************************
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
#include <QDebug>
DialogOptions::DialogOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOptions)
{
    ui->setupUi(this);
    ui->toolButtonAdd->setIcon(Tumb::icon(I_ADD));
    ui->toolButtonRemove->setIcon(Tumb::icon(I_LIST_REMOVE));
    // Styles
    QStringList listStyles=QStyleFactory::keys();
    listStyles.insert(0,tr("Default"));
    ui->comboBoxStyls->addItems(listStyles);

    //l anguages
    QDir appDir(QApplication::applicationDirPath());
    appDir.cdUp();
    QString dirPath=  appDir.absolutePath()+"/share/"+APP_NAME+"/translations";
    QDir dir(dirPath);

       ui->comboBoxLanguage->addItem(tr("Default"));
     foreach (QString name, dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot|QDir::NoSymLinks)) {
         QLocale local(name);
            ui->comboBoxLanguage->addItem(local.nativeLanguageName(),name);
     }

    //-------------------------------------------------------------------------
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

    //-------------------------------------------------------------------------
    ui->checkBoxNotify->blockSignals(true);
    ui->comboBoxStyls->blockSignals(true);
    ui->checkBoxTrayIcon->blockSignals(true);
    ui->comboBoxLanguage->blockSignals(true);

    //-------------------------------------------------------------------------



    settings.beginGroup("Window");
    ui->checkBoxNotify->setChecked(settings.value("ShowNotification",false).toBool());
    ui->comboBoxStyls->setCurrentText(settings.value("Style").toString());
    ui->checkBoxTrayIcon->setChecked(settings.value("TrayIcon",true).toBool());
    ui->checkBoxShowMenu->setChecked(settings.value("ShowMenu",true).toBool());

    QLocale local(settings.value("Language").toString());
       ui->comboBoxLanguage->setCurrentText(local.nativeLanguageName());
   // ui->comboBoxLanguage->setCurrentText(settings.value("Language").toString());
    settings.endGroup();

    //-------------------------------------------------------------------------
    ui->checkBoxNotify->blockSignals(false);
    ui->comboBoxStyls->blockSignals(false);
    ui->checkBoxTrayIcon->blockSignals(false);
    ui->comboBoxLanguage->blockSignals(false);
        //-------------------------------------------------------------------------
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

void DialogOptions::on_checkBoxNotify_toggled(bool checked)
{

    QSettings settings;
        settings.beginGroup("Window");
        settings.setValue("ShowNotification",checked);
        settings.endGroup();
}

void DialogOptions::on_checkBoxTrayIcon_toggled(bool checked)
{
   //   ui->checkBoxTrayIcon->setChecked(settings.value("TrayIcon",true).toBool());
      QSettings settings;
          settings.beginGroup("Window");
          settings.setValue("TrayIcon",checked);
          settings.endGroup();
}

void DialogOptions::on_comboBoxLanguage_activated(const QString &arg1)
{
       QMessageBox::information(this,tr("Information"),tr("the language %1  will be changed only when the program is restarted").arg(arg1));
       QSettings settings;
           settings.beginGroup("Window");
           settings.setValue("Language",ui->comboBoxLanguage->currentData().toString());
           settings.endGroup();
}

void DialogOptions::on_checkBoxShowMenu_toggled(bool checked)
{
    QSettings settings;
        settings.beginGroup("Window");
        settings.setValue("ShowMenu",checked);
        settings.endGroup();
}
