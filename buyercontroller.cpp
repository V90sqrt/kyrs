#include "buyercontroller.h"

BuyerController::BuyerController(BuyerModel *model, QObject *parent)
    : QObject(parent), m_model(model) {}

int BuyerController::columnCount() const {
    return m_model->columnCount();
}

void BuyerController::addBuyer(const HousingBuyer &buyer) {
    int row = m_model->rowCount();
    m_model->insertRows(row, 1);
    m_model->setBuyer(row, buyer);
}

bool BuyerController::exportData(const QString &fileName) const {
    return m_model->exportToFile(fileName);
}

void BuyerController::removeBuyer(int row) {
    m_model->removeRows(row, 1);
}

QVariant BuyerController::getHeaderData(int section, Qt::Orientation orientation) const {
    return m_model->headerData(section, orientation, Qt::DisplayRole);
}

bool BuyerController::importData(const QString &fileName) {
    return m_model->importFromFile(fileName);
}

void BuyerController::updateBuyer(int row, const HousingBuyer &buyer) {
    m_model->setBuyer(row, buyer);
}

QList<int> BuyerController::findInColumn(int column, const QString &text) const {
    return m_model->findItems(column, text);
}

void BuyerController::arrangeData(int column, Qt::SortOrder order) {
    m_model->arrangeByColumn(column, order);
}

void BuyerController::notifyHeaderChanged(Qt::Orientation orientation, int first, int last) {
    m_model->headerDataChanged(orientation, first, last);
}

QPair<int, int> BuyerController::findText(const QString &text) const {
    return m_model->findAllOccurrences(text);
}
