#ifndef BUYERMODEL_H
#define BUYERMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QString>
#include <QMessageBox>

struct HousingBuyer {
    QString fullName;
    QString passportNumber;
    QString phone;
    QString email;
    QString propertyType;
    QString budget;
    QString preferredArea;
    QString status;
};

class BuyerModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit BuyerModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    bool importFromFile(const QString &fileName);
    bool exportToFile(const QString &fileName) const;
    static bool verifyFileFormat(const QString &fileName);

    HousingBuyer getBuyer(int row) const;
    void setBuyer(int row, const HousingBuyer &buyer);

    QList<int> findItems(int column, const QString &text) const;
    QPair<int, int> findAllOccurrences(const QString &text) const;
    void arrangeByColumn(int column, Qt::SortOrder order = Qt::AscendingOrder);

    static QString checkInputData(const QString &value, int column);
    static QString formatPassportNumber(const QString &passport);
    static QString formatPhoneNumber(const QString &phone);
    static QString formatBudget(const QString &budget);

private:
    QVector<HousingBuyer> m_data;
    int m_sortColumn;
    Qt::SortOrder m_sortOrder;
};

#endif // BUYERMODEL_H
