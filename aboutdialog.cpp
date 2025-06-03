#include "aboutdialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("About"));
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *label = new QLabel(
        tr("<b>Housing Buyer Management</b><br><br>"
           "Institute: МГСУ<br>"
           "Group: ИЦТМС-25<br><br>"
           "Name: Иванов Иван Иванович"), this);

    layout->addWidget(label);
    QPushButton *okButton = new QPushButton(tr("OK"), this);
    connect(okButton, &QPushButton::clicked, this, &AboutDialog::accept);
    layout->addWidget(okButton);
    setLayout(layout);
}

AboutDialog::~AboutDialog() {}
