#include "buyermodel.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <algorithm>

BuyerModel::BuyerModel(QObject *parent)
    : QAbstractTableModel(parent), m_sortColumn(-1), m_sortOrder(Qt::AscendingOrder) {}

int BuyerModel::rowCount(const QModelIndex &) const { return m_data.size(); }
int BuyerModel::columnCount(const QModelIndex &) const { return 8; }

QVariant BuyerModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_data.size() || index.column() >= 8)
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        const HousingBuyer &buyer = m_data[index.row()];
        switch (index.column()) {
        case 0: return buyer.fullName;
        case 1: return buyer.passportNumber;
        case 2: return buyer.phone;
        case 3: return buyer.email;
        case 4: return buyer.propertyType;
        case 5: return buyer.budget;
        case 6: return buyer.preferredArea;
        case 7: return buyer.status;
        default: return QVariant();
        }
    }
    return QVariant();
}

QVariant BuyerModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return tr("Full Name");
        case 1: return tr("Passport");
        case 2: return tr("Phone");
        case 3: return tr("Email");
        case 4: return tr("Property Type");
        case 5: return tr("Budget");
        case 6: return tr("Preferred Area");
        case 7: return tr("Status");
        default: return QVariant();
        }
    }
    return QVariant();
}

bool BuyerModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid() || index.row() >= m_data.size() || index.column() >= 8)
        return false;

    if (role == Qt::EditRole) {
        HousingBuyer &buyer = m_data[index.row()];
        QString validatedValue = checkInputData(value.toString(), index.column());

        if ((index.column() == 1 || index.column() == 2 || index.column() == 5) && validatedValue.isEmpty()) {
            return false;
        }

        switch (index.column()) {
        case 0: buyer.fullName = value.toString(); break;
        case 1: buyer.passportNumber = validatedValue; break;
        case 2: buyer.phone = validatedValue; break;
        case 3: buyer.email = value.toString(); break;
        case 4: buyer.propertyType = value.toString(); break;
        case 5: buyer.budget = validatedValue; break;
        case 6: buyer.preferredArea = value.toString(); break;
        case 7: buyer.status = value.toString(); break;
        default: return false;
        }
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Qt::ItemFlags BuyerModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

bool BuyerModel::insertRows(int row, int count, const QModelIndex &parent) {
    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        m_data.insert(row, HousingBuyer());
    }
    endInsertRows();
    return true;
}

bool BuyerModel::removeRows(int row, int count, const QModelIndex &parent) {
    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        m_data.removeAt(row);
    }
    endRemoveRows();
    return true;
}

bool BuyerModel::importFromFile(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    QVector<HousingBuyer> tempData;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(";");
        if (parts.size() != 8)
            continue;

        HousingBuyer buyer;
        buyer.fullName = parts[0];
        buyer.passportNumber = parts[1];
        buyer.phone = parts[2];
        buyer.email = parts[3];
        buyer.propertyType = parts[4];
        buyer.budget = parts[5];
        buyer.preferredArea = parts[6];
        buyer.status = parts[7];

        tempData.append(buyer);
    }

    beginResetModel();
    m_data = tempData;
    endResetModel();
    return true;
}

bool BuyerModel::exportToFile(const QString &fileName) const {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);
    for (const HousingBuyer &buyer : m_data) {
        out << buyer.fullName << ";"
            << buyer.passportNumber << ";"
            << buyer.phone << ";"
            << buyer.email << ";"
            << buyer.propertyType << ";"
            << buyer.budget << ";"
            << buyer.preferredArea << ";"
            << buyer.status << "\n";
    }
    return true;
}

bool BuyerModel::verifyFileFormat(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.split(";").size() != 8)
            return false;
    }
    return true;
}

HousingBuyer BuyerModel::getBuyer(int row) const {
    if (row < 0 || row >= m_data.size())
        return HousingBuyer();
    return m_data[row];
}

void BuyerModel::setBuyer(int row, const HousingBuyer &buyer) {
    if (row < 0 || row >= m_data.size())
        return;
    m_data[row] = buyer;
    emit dataChanged(index(row, 0), index(row, 7));
}

QList<int> BuyerModel::findItems(int column, const QString &text) const {
    QList<int> result;
    for (int i = 0; i < m_data.size(); ++i) {
        QString value;
        switch (column) {
        case 0: value = m_data[i].fullName; break;
        case 1: value = m_data[i].passportNumber; break;
        case 2: value = m_data[i].phone; break;
        case 3: value = m_data[i].email; break;
        case 4: value = m_data[i].propertyType; break;
        case 5: value = m_data[i].budget; break;
        case 6: value = m_data[i].preferredArea; break;
        case 7: value = m_data[i].status; break;
        }
        if (value.contains(text, Qt::CaseInsensitive))
            result.append(i);
    }
    return result;
}

QPair<int, int> BuyerModel::findAllOccurrences(const QString &text) const {
    for (int row = 0; row < m_data.size(); ++row) {
        const HousingBuyer &buyer = m_data[row];

        for (int col = 0; col < 8; ++col) {
            QString value;
            switch (col) {
            case 0: value = buyer.fullName; break;
            case 1: value = buyer.passportNumber; break;
            case 2: value = buyer.phone; break;
            case 3: value = buyer.email; break;
            case 4: value = buyer.propertyType; break;
            case 5: value = buyer.budget; break;
            case 6: value = buyer.preferredArea; break;
            case 7: value = buyer.status; break;
            }

            if (value.contains(text, Qt::CaseInsensitive))
                return qMakePair(row, col);
        }
    }
    return qMakePair(-1, -1);
}

void BuyerModel::arrangeByColumn(int column, Qt::SortOrder order) {
    if (m_sortColumn == column && m_sortOrder == order) {
        m_sortColumn = -1;
        m_sortOrder = Qt::AscendingOrder;
        beginResetModel();
        endResetModel();
        return;
    }

    m_sortColumn = column;
    m_sortOrder = order;

    beginResetModel();
    std::sort(m_data.begin(), m_data.end(), [this](const HousingBuyer &a, const HousingBuyer &b) {
        QString valueA, valueB;
        switch (m_sortColumn) {
        case 0: valueA = a.fullName; valueB = b.fullName; break;
        case 1: valueA = a.passportNumber; valueB = b.passportNumber; break;
        case 2: valueA = a.phone; valueB = b.phone; break;
        case 3: valueA = a.email; valueB = b.email; break;
        case 4: valueA = a.propertyType; valueB = b.propertyType; break;
        case 5: {
            bool okA, okB;
            double numA = a.budget.toDouble(&okA);
            double numB = b.budget.toDouble(&okB);
            if (okA && okB) {
                return m_sortOrder == Qt::AscendingOrder ? numA < numB : numA > numB;
            }
            valueA = a.budget; valueB = b.budget;
            break;
        }
        case 6: valueA = a.preferredArea; valueB = b.preferredArea; break;
        case 7: valueA = a.status; valueB = b.status; break;
        default: return false;
        }

        return m_sortOrder == Qt::AscendingOrder ? valueA < valueB : valueA > valueB;
    });
    endResetModel();
}

QString BuyerModel::checkInputData(const QString &value, int column) {
    if (value.isEmpty()) return QString();

    switch (column) {
    case 1: { // Passport
        if (value.length() < 8) {
            QMessageBox::warning(nullptr, tr("Invalid Data"),
                                 tr("Passport must be at least 8 characters"));
            return QString();
        }
        return formatPassportNumber(value);
    }
    case 2: { // Phone
        QString digits;
        for (const QChar &c : value) {
            if (c.isDigit()) digits.append(c);
        }
        if (digits.length() < 7) {
            QMessageBox::warning(nullptr, tr("Invalid Data"),
                                 tr("Phone must contain at least 7 digits"));
            return QString();
        }
        return formatPhoneNumber(value);
    }
    case 5: { // Budget
        bool ok;
        value.toDouble(&ok);
        if (!ok) {
            QMessageBox::warning(nullptr, tr("Invalid Data"),
                                 tr("Budget must be a number"));
            return QString();
        }
        return formatBudget(value);
    }
    default:
        return value;
    }
}

QString BuyerModel::formatPassportNumber(const QString &passport) {
    return passport.toUpper();
}

QString BuyerModel::formatPhoneNumber(const QString &phone) {
    QString digits;
    for (const QChar &c : phone) {
        if (c.isDigit()) digits.append(c);
    }

    if (digits.length() == 7) {
        return digits.left(3) + "-" + digits.mid(3, 2) + "-" + digits.right(2);
    }
    else if (digits.length() == 11 && digits.startsWith("7")) {
        return "+7 (" + digits.mid(1, 3) + ") " + digits.mid(4, 3) +
               "-" + digits.mid(7, 2) + "-" + digits.right(2);
    }
    return phone;
}

QString BuyerModel::formatBudget(const QString &budget) {
    bool ok;
    double value = budget.toDouble(&ok);
    if (ok) {
        return QString::number(value, 'f', 2);
    }
    return budget;
}
