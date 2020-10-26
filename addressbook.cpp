#include "addressbook.h"


//constructor
AddressBook::AddressBook(QWidget *parent) : QWidget(parent)
{
    QLabel *nameLabel = new QLabel(tr("Name:"));
    nameLine = new QLineEdit;
    nameLine->setReadOnly(true);

    QLabel *addressLabel = new QLabel(tr("Address:"));
    addressText = new QTextEdit;
    addressText->setReadOnly(true);//set readonly so can only display and not edit

    //show the add push button
    createButton = new QPushButton(tr("&Create"));
    createButton->setToolTip(tr("Create a new contact"));
    createButton->show();

    //these button are hidden using hide()
    //only displayed when add is clicked and
    //handled by addContact() function
    addButton = new QPushButton(tr("&Add"));
    addButton->hide();
    addButton->setToolTip(tr("Add contact to address book"));
    cancelButton = new QPushButton(tr("&Cancel"));
    cancelButton->hide();

    //create next / previous buttons but disable by default
    //omly show when > 1 contacts
    nextButton = new QPushButton(tr("&Next"));
    nextButton->setEnabled(false);
    previousButton = new QPushButton(tr("&Previous"));
    previousButton->setEnabled(false);

    //disabled by default zero contacts at start
    editButton = new QPushButton(tr("&Edit"));
    editButton->setEnabled(false);
    editButton->setToolTip(tr("Edit current contact"));
    removeButton = new QPushButton(tr("&Remove"));
    removeButton->setEnabled(false);

    //find buttons
    findButton = new QPushButton(tr("&Find"));
    findButton->setToolTip(tr("Search for a contact"));
    findButton->setEnabled(false);
    dialog = new FindDialog(this);

    //load and save buttons
    loadButton = new QPushButton(tr("&Load"));
    loadButton->setEnabled(true);
    loadButton->setToolTip(tr("Load contacts from a file"));
    saveButton = new QPushButton(tr("&Save"));
    saveButton->setEnabled(true);
    saveButton->setToolTip(tr("Save contacts to a file"));



    //connect the push buttons to clicked
    connect(createButton, &QPushButton::clicked, this, &AddressBook::addContact);
    connect(addButton, &QPushButton::clicked, this, &AddressBook::submitContact);
    connect(cancelButton, &QPushButton::clicked, this, &AddressBook::cancel);
    connect(nextButton, &QPushButton::clicked, this, &AddressBook::next);
    connect(previousButton, &QPushButton::clicked, this, &AddressBook::previous);
    connect(editButton, &QPushButton::clicked, this, &AddressBook::editContact);
    connect(removeButton, &QPushButton::clicked, this, &AddressBook::removeContact);
    connect(findButton, &QPushButton::clicked, this, &AddressBook::findContact);
    connect(loadButton, &QPushButton::clicked, this, &AddressBook::loadFromFile);
    connect(saveButton, &QPushButton::clicked, this, &AddressBook::saveToFile);

    //arrange and align push buttons
    QVBoxLayout *buttonLayout1 = new QVBoxLayout;
    buttonLayout1->addWidget(createButton, Qt::AlignTop);
    buttonLayout1->addWidget(addButton);
    buttonLayout1->addWidget(cancelButton);
    buttonLayout1->addWidget(editButton);
    buttonLayout1->addWidget(removeButton);
    buttonLayout1->addWidget(findButton);
    buttonLayout1->addWidget(loadButton);
    buttonLayout1->addWidget(saveButton);
    //addStretch() function is used to ensure the push buttons are not evenly spaced, but arranged closer to the top of the widget.
    buttonLayout1->addStretch();

    QHBoxLayout *buttonLayout2 = new QHBoxLayout;
    buttonLayout2->addWidget(previousButton);
    buttonLayout2->addWidget(nextButton);

    //create the layout
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(nameLabel, 0, 0);
    mainLayout->addWidget(nameLine, 0, 1);
    mainLayout->addWidget(addressLabel, 1, 0, Qt::AlignTop);
    mainLayout->addWidget(addressText, 1, 1);
    mainLayout->addLayout(buttonLayout1, 1, 2);
    mainLayout->addLayout(buttonLayout2, 2, 1);
    setLayout(mainLayout);
    setWindowTitle("Contacts");

}

//add a contact to the address book

void AddressBook::addContact() {

    //disable previous and next buttons
    nextButton->setEnabled(false);
    previousButton->setEnabled(false);

    //store last displayed contact details
    oldName = nameLine->text();
    oldAddress = addressText->toPlainText();

    //clear the fields
    nameLine->clear();
    addressText->clear();

    //turn off readonly flag
    nameLine->setReadOnly(false);
    nameLine->setFocus(Qt::OtherFocusReason);
    addressText->setReadOnly(false);

    //display submit and cancel buttons
    createButton->setEnabled(false);
    addButton->show();
    cancelButton->show();

    updateInterface(Mode::AddingMode);
}

void AddressBook::submitContact() {

    QString name = nameLine->text();
    QString address = addressText->toPlainText();

    //check for empty fields
    if (name.isEmpty() || address.isEmpty()) {
           QMessageBox::information(this, tr("Empty Field"),
               tr("Please enter a name and address."));
           return;
       }

    //check if contact exists if not add the contact
    //and display whether succesfull or not
    //duplicates are not added
    if(currentMode == Mode::AddingMode) {
        if (!contacts.contains(name)) {
                contacts.insert(name, address);
                QMessageBox::information(this, tr("Add Successful"),
                tr("\"%1\" has been added to your address book.").arg(name));
        }
        else {
                QMessageBox::information(this, tr("Add Unsuccessful"),
                tr("Sorry, \"%1\" is already in your address book.").arg(name));
                return;
        }
    } else if(currentMode == Mode::EditingMode) {
        if (oldName != name) {
            if (!contacts.contains(name)) {
                QMessageBox::information(this, tr("Edit Successful"),
                tr("\"%1\" has been edited in your address book.").arg(oldName));
                 contacts.remove(oldName);
                 contacts.insert(name, address);
            } else {
                  QMessageBox::information(this, tr("Edit Unsuccessful"),
                  tr("Sorry, \"%1\" is already in your address book.").arg(name));
              }
        } else if (oldAddress != address) {
                   QMessageBox::information(this, tr("Edit Successful"),
                   tr("\"%1\" has been edited in your address book.").arg(name));
                   contacts[name] = address;
               }
    }

    updateInterface(Mode::NavigationMode);

    //once both cases have been handled restore push buttons to normal state
    // and readonly flags
    if (contacts.isEmpty()) {
            nameLine->clear();
            addressText->clear();
    }

    nameLine->setReadOnly(true);
    addressText->setReadOnly(true);
    createButton->setEnabled(true);
    addButton->hide();
    cancelButton->hide();

    //enable next previous buttons
    //navigation only enable when more than 1 contact
    int number = contacts.size();
    nextButton->setEnabled(number > 1);
    previousButton->setEnabled(number > 1);
    findButton->setEnabled(number > 1);

}

void AddressBook::cancel() {

    //rstores last displayed contact
    //enables add button, hides submit cancel

    nameLine->setText(oldName);
    nameLine->setReadOnly(true);

    addressText->setText(oldAddress);
    addressText->setReadOnly(true);

    createButton->setEnabled(true);
    addButton->hide();
    cancelButton->hide();

    //enable next previous buttons
    //navigation only enable when more than 1 contact
    int number = contacts.size();
    nextButton->setEnabled(number > 1);
    previousButton->setEnabled(number > 1);
}

void AddressBook::next()
{
    QString name = nameLine->text();

    //create an iterator
    QMap<QString, QString>::iterator i = contacts.find(name);

    //if iterator not at end of map increment
    if (i != contacts.end()) i++;

    //if oterator is at end return to begining
    if (i == contacts.end()) i = contacts.begin();

    nameLine->setText(i.key());
    addressText->setText(i.value());
}

void AddressBook::previous()
{
    QString name = nameLine->text();

    //cretae an iterator
    QMap<QString, QString>::iterator i = contacts.find(name);

    //iterator at en dof contacts clear display and return
    if (i == contacts.end()){
        nameLine->clear();
        addressText->clear();
        return;
    }

    //iterator at begining move to end of map
    if (i == contacts.begin()) i = contacts.end();

    i--;
    nameLine->setText(i.key());
    addressText->setText(i.value());
}

void AddressBook::editContact()
{
    //store old contact details
    oldName = nameLine->text();
    oldAddress = addressText->toPlainText();

    //before switching to edit mode
    //this enables submit cancel buttons
    updateInterface(Mode::EditingMode);
}


void AddressBook::removeContact()
{
    QString name = nameLine->text();
    QString address = addressText->toPlainText();

    if (contacts.contains(name)) {

        int button = QMessageBox::question(this,
            tr("Confirm Remove"),
            tr("Are you sure you want to remove \"%1\"?").arg(name),
            QMessageBox::Yes | QMessageBox::No);

        if (button == QMessageBox::Yes) {

            previous();
            contacts.remove(name);

            QMessageBox::information(this, tr("Remove Successful"),
                tr("\"%1\" has been removed from your address book.").arg(name));
        }
    }

    updateInterface(Mode::NavigationMode);
}

void AddressBook::updateInterface(Mode mode)
{
    currentMode = mode;

    switch (currentMode) {

    case Mode::AddingMode:
    case Mode::EditingMode:

        nameLine->setReadOnly(false);
        nameLine->setFocus(Qt::OtherFocusReason);
        addressText->setReadOnly(false);

        createButton->setEnabled(false);
        editButton->setEnabled(false);
        removeButton->setEnabled(false);

        nextButton->setEnabled(false);
        previousButton->setEnabled(false);

        addButton->show();
        cancelButton->show();
        break;

    case Mode::NavigationMode:

            if (contacts.isEmpty()) {
                nameLine->clear();
                addressText->clear();
            }

            nameLine->setReadOnly(true);
            addressText->setReadOnly(true);
            createButton->setEnabled(true);

            int number = contacts.size();
            editButton->setEnabled(number >= 1);
            removeButton->setEnabled(number >= 1);
            nextButton->setEnabled(number > 1);
            previousButton->setEnabled(number >1 );

            addButton->hide();
            cancelButton->hide();
            break;
        }
}

void AddressBook::findContact()
{
    dialog->show();

    if (dialog->exec() == QDialog::Accepted) {
        QString contactName = dialog->getFindText();

        if (contacts.contains(contactName)) {
            nameLine->setText(contactName);
            addressText->setText(contacts.value(contactName));
        } else {
            QMessageBox::information(this, tr("Contact Not Found"),
                tr("Sorry, \"%1\" is not in your address book.").arg(contactName));
            return;
        }
    }

    updateInterface(Mode::NavigationMode);
}

void AddressBook::saveToFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Address Book"), "",
        tr("Address Book (*.abk);;All Files (*)"));
    if (fileName.isEmpty()) return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
            file.errorString());
            return;
         }
         QDataStream out(&file);
         out.setVersion(QDataStream::Qt_4_5);
         out << contacts;
    }//end else
}


void AddressBook::loadFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Address Book"), "", tr("Address Book (*.abk);;All Files (*)"));
    if (fileName.isEmpty()) return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
            file.errorString());
            return;
        }
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_4_5);
        contacts.clear();   // clear existing contacts
        in >> contacts;
        if (contacts.isEmpty()) {
            QMessageBox::information(this, tr("No contacts in file"),
            tr("The file you are attempting to open contains no contacts."));
        }
        else {
            QMap<QString, QString>::iterator i = contacts.begin();
            nameLine->setText(i.key());
            addressText->setText(i.value());
        }
    }

    updateInterface(Mode::NavigationMode);
}


void AddressBook::exportAsVcard() {

    QString name = nameLine->text();
    QString address = addressText->toPlainText();
    QString firstName;
    QString lastName;
    QStringList nameList;

    int index = name.indexOf(" ");

    if (index != -1) {
        nameList = name.split(QRegExp("\\s+"), QString::SkipEmptyParts);
        firstName = nameList.first();
        lastName = nameList.last();
    } else {
        firstName = name;
        lastName = "";
    }

    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Export Contact"), "",
        tr("vCard Files (*.vcf);;All Files (*)"));

    if (fileName.isEmpty())
        return;

    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly)) {
           QMessageBox::information(this, tr("Unable to open file"),
               file.errorString());
           return;
       }

     QTextStream out(&file);
     out << "BEGIN:VCARD" << '\n';
     out << "VERSION:2.1" << '\n';
     out << "N:" << lastName << ';' << firstName << '\n';
     if (!nameList.isEmpty()) out << "FN:" << nameList.join(' ') << '\n';
     else out << "FN:" << firstName << '\n';

     address.replace(";", "\\;", Qt::CaseInsensitive);
     address.replace('\n', ";", Qt::CaseInsensitive);
     address.replace(",", " ", Qt::CaseInsensitive);

     out << "ADR;HOME:;" << address << '\n';
     out << "END:VCARD" << '\n';

     QMessageBox::information(this, tr("Export Successful"), tr("\"%1\" has been exported as a vCard.").arg(name));
    }
