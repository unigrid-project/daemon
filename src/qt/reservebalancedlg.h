#ifndef RESERVEBALACEDLG_H
#define RESERVEBALACEDLG_H

#include <QDialog>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace Ui {
class ReserveBalanceDlg;
}


class ReserveBalanceDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ReserveBalanceDlg(QWidget *parent = 0);
    ~ReserveBalanceDlg();
    std::string getReserveBalance();

protected:

private slots:
    void on_okButton_clicked();
//    void on_cancelButton_clicked();

signals:

private:
    Ui::ReserveBalanceDlg *ui;
    std::string strReserveBalance;
};

#endif // RESERVEBALACEDLG_H
