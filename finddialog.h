#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QLabel>


class FindDialog :public QDialog
{
    Q_OBJECT

public:
    FindDialog(QWidget *parent = nullptr);

    //used by classes that instantiate FindDialog
    QString getFindText();
    void findContact();

public slots:
    //handle search string when clicked
    void findClicked();

private:
    QPushButton *findButton;
    QLineEdit *lineEdit;
    QString findText;
};

#endif // FINDDIALOG_H
