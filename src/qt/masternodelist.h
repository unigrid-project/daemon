#ifndef MASTERNODELIST_H
#define MASTERNODELIST_H

#include "primitives/transaction.h"
#include "sync.h"
#include "util.h"
#include "guiutil.h"

#include <QMenu>
#include <QTimer>
#include <QWidget>
#include <QDialog>
#include <QHeaderView>
#include <QItemSelection>
#include <QKeyEvent>
#include <QPoint>
#include <QVariant>

#define MY_MASTERNODELIST_UPDATE_SECONDS                 60
#define MASTERNODELIST_UPDATE_SECONDS                    15
#define MASTERNODELIST_FILTER_COOLDOWN_SECONDS            3

namespace Ui {
    class MasternodeList;
}

class WalletModel;

QT_BEGIN_NAMESPACE
class QModelIndex;
QT_END_NAMESPACE

/** Masternode Manager page widget */
class MasternodeList : public QWidget
{
    Q_OBJECT

public:
    explicit MasternodeList(QWidget *parent = 0);
    ~MasternodeList();

    void setModel(WalletModel *model);
    void StartAlias(std::string strAlias);
    void RemoveAlias(std::string strAlias);
    void StartAll(std::string strCommand = "start-all");

private:
    QMenu *contextMenu;
    int64_t nTimeFilterUpdated;
    bool fFilterUpdated;

public Q_SLOTS:
    void updateMyMasternodeInfo(QString strAlias, QString strAddr, const COutPoint& outpoint);
    void updateMyNodeList(bool fForce = false);
    void updateNodeList();

Q_SIGNALS:

private:
    QTimer *timer;
    Ui::MasternodeList *ui;
    WalletModel *model;

    // Protects tableWidgetMasternodes
    CCriticalSection cs_mnlist;

    // Protects tableWidgetMyMasternodes
    CCriticalSection cs_mymnlist;

    QString strCurrentFilter;

private Q_SLOTS:
    void showContextMenu(const QPoint &);
    void on_filterLineEdit_textChanged(const QString &strFilterIn);
    void on_startButton_clicked();
    void on_startAllButton_clicked();
    void on_createMNButton_clicked();
    void on_removeMNButton_clicked();
    void on_tableWidgetMyMasternodes_itemSelectionChanged();
    void on_UpdateButton_clicked();
};
#endif // MASTERNODELIST_H
