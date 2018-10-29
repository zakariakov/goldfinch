#ifndef PROPERTIESFILE_H
#define PROPERTIESFILE_H

#include <QDialog>

namespace Ui {
class PropertiesFile;
}

class PropertiesFile : public QDialog
{
    Q_OBJECT

public:
    explicit PropertiesFile(QWidget *parent = nullptr);
    ~PropertiesFile();
void setInformations(const QVariantMap &map);
private:
    Ui::PropertiesFile *ui;
};

#endif // PROPERTIESFILE_H
