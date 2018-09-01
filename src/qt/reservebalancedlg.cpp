#include "reservebalancedlg.h"
#include "ui_reservebalancedlg.h"
#include "masternodeconfig.h"
#include "masternodelist.h"
#include "ui_masternodelist.h"

#include "walletdb.h"
#include "wallet.h"
#include "ui_interface.h"
#include "util.h"
#include "key.h"
//#include "script.h"
#include "init.h"
#include "base58.h"
#include "rpcserver.h"

#include <QMessageBox>
#include <QClipboard>

using namespace std;

ReserveBalanceDlg::ReserveBalanceDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReserveBalanceDlg)
{
    ui->setupUi(this);

    //Labels
    
    
    ui->balanceEdit->setPlaceholderText(QString::number(pwalletMain->GetBalance() / COIN));
    ui->reserveBalanceEdit->setPlaceholderText("Enter the reservebalance value");
}

ReserveBalanceDlg::~ReserveBalanceDlg()
{
    delete ui;
}

std::string ReserveBalanceDlg::getReserveBalance() {
	return strReserveBalance;
}

void ReserveBalanceDlg::on_okButton_clicked()
{
    if(ui->reserveBalanceEdit->text() == "")
    {
        QMessageBox msg;
        msg.setText("Please enter a reservebalance value!");
        msg.exec();
        strReserveBalance = "999999999999999";
        return;
    }
    else
    {
        std::string sBalance = ui->balanceEdit->text().toStdString();
        std::string sReserveBalance = ui->reserveBalanceEdit->text().toStdString();
        strReserveBalance = sReserveBalance;
        accept();
    }
}
/*
void ReserveBalanceDlg::on_cancelButton_clicked()
{
    reject();
}
*/