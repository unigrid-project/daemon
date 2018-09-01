#ifndef CREATEMASTERNODEDLG_H
#define CREATEMASTERNODEDLG_H

#include <QDialog>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace Ui {
class CreateMasternodeDlg;
}


class CreateMasternodeDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CreateMasternodeDlg(QWidget *parent = 0);
    ~CreateMasternodeDlg();

protected:

private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();
    void on_AddEditAddressPasteButton_clicked();
    void on_AddEditPrivkeyPasteButton_clicked();
    void on_AddEditTxhashPasteButton_clicked();

signals:

private:
    Ui::CreateMasternodeDlg *ui;
};

#endif // CREATEMASTERNODEDLG_H
