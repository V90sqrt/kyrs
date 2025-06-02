#ifndef BUYERCONTROLLER_H
#define BUYERCONTROLLER_H

#include <QObject>
#include "buyermodel.h"

class BuyerController : public QObject {
    Q_OBJECT
public:
    explicit BuyerController(BuyerModel *model, QObject *parent = nullptr);

    int columnCount() const;
    void addBuyer(const HousingBuyer &buyer);
    void removeBuyer(int row);
    void updateBuyer(int row, const HousingBuyer &buyer);

    bool exportData(const QString &fileName) const;
    bool importData(const QString &fileName);

    QPair<int, int> findText(const QString &text) const;
    QList<int> findInColumn(int column, const QString &text) const;

    QVariant getHeaderData(int section, Qt::Orientation orientation) const;
    void notifyHeaderChanged(Qt::Orientation orientation, int first, int last);

    void arrangeData(int column, Qt::SortOrder order = Qt::AscendingOrder);

private:
    BuyerModel *m_model;
};

#endif // BUYERCONTROLLER_H
