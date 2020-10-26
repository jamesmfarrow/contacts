#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H

#include <QFileDialog>
#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QMap>
#include <QMessageBox>
#include <QTextStream>
#include <QRegExp>
#include "finddialog.h"

class AddressBook : public QWidget
{
    Q_OBJECT

public:
    AddressBook(QWidget *parent = nullptr);
    enum class Mode{  NavigationMode, AddingMode, EditingMode };

private:
    QLineEdit *nameLine;
    QTextEdit *addressText;

    QPushButton *createButton;
    QPushButton *addButton;
    QPushButton *cancelButton;
    QPushButton *nextButton;
    QPushButton *previousButton;
    QPushButton *editButton;
    QPushButton *removeButton;
    QPushButton *findButton;
    QPushButton *loadButton;
    QPushButton *saveButton;
    QPushButton *exportButton;
    FindDialog *dialog;

    Mode currentMode{Mode::NavigationMode};

    QMap<QString, QString> contacts;

    //used to hold name address of last contact displayed
    QString oldName;
    QString oldAddress;

public slots:
    void addContact();
    void submitContact();
    void cancel();
    void next();
    void previous();
    void editContact();
    void removeContact();
    void updateInterface(Mode mode);
    void findContact();

    void saveToFile();
    void loadFromFile();
    void exportAsVcard();


};

#endif // ADDRESSBOOK_H
