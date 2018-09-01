#include "createmasternodedlg.h"
#include "ui_createmasternodedlg.h"
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
#include <QMessageBox>
#include <QClipboard>

#include <boost/program_options/detail/config_file.hpp>
#include <boost/program_options/parsers.hpp>

#include <set>

using namespace std;

CreateMasternodeDlg::CreateMasternodeDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateMasternodeDlg)
{
    ui->setupUi(this);

    //Labels
    ui->aliasLineEdit->setPlaceholderText("Enter your Masternode alias");
    ui->addressLineEdit->setPlaceholderText("Enter your IP & port");
    ui->privkeyLineEdit->setPlaceholderText("Enter your Masternode private key");
    ui->txhashLineEdit->setPlaceholderText("Enter your 2, 000 HUZU TXID");
    ui->outputindexLineEdit->setPlaceholderText("Enter your transaction output index");
}

CreateMasternodeDlg::~CreateMasternodeDlg()
{
    delete ui;
}


void CreateMasternodeDlg::on_okButton_clicked()
{
    if(ui->aliasLineEdit->text() == "")
    {
        QMessageBox msg;
        msg.setText("Please enter an alias.");
        msg.exec();
        return;
    }
    else if(ui->addressLineEdit->text() == "")
    {
        QMessageBox msg;
        msg.setText("Please enter an ip address and port. (123.45.67.89:29921)");
        msg.exec();
        return;
    }
    else if(ui->privkeyLineEdit->text() == "")
    {
        QMessageBox msg;
        msg.setText("Please enter a masternode private key. This can be found using the \"masternode genkey\" command in the console.");
        msg.exec();
        return;
    }
    else if(ui->txhashLineEdit->text() == "")
    {
        QMessageBox msg;
        msg.setText("Please enter the transaction hash for the transaction that has 10000 coins");
        msg.exec();
        return;
    }
    else if(ui->outputindexLineEdit->text() == "")
    {
        QMessageBox msg;
        msg.setText("Please enter a transaction output index. This can be found using the \"masternode outputs\" command in the console.");
        msg.exec();
        return;
    }
    else
    {
        std::string sAlias = ui->aliasLineEdit->text().toStdString();
        std::string sAddress = ui->addressLineEdit->text().toStdString();
        std::string sMasternodePrivKey = ui->privkeyLineEdit->text().toStdString();
        std::string sTxHash = ui->txhashLineEdit->text().toStdString();
        std::string sOutputIndex = ui->outputindexLineEdit->text().toStdString();

        boost::filesystem::path pathConfigFile = GetDataDir() / "masternode.conf";
        boost::filesystem::ofstream stream (pathConfigFile.string(), ios::out | ios::app);
        if (stream.is_open())
        {
            stream << sAlias << " " << sAddress << " " << sMasternodePrivKey << " " << sTxHash << " " << sOutputIndex;
            stream << std::endl;
            stream.close();
        }
        masternodeConfig.add(sAlias, sAddress, sMasternodePrivKey, sTxHash, sOutputIndex);
        
        boost::filesystem::ifstream streamConfig(GetConfigFile());
    unsigned char rand_pwd[32];
    GetRandBytes(rand_pwd, 32);
    for (int i = 0; i < 32; i++) {
    	rand_pwd[i] = (rand_pwd[i] % 26) + 97;
    }
    if (!streamConfig.good()) {
    	return;
    }
    std::string path_1 = GetConfigFile().string() + "bak";
  	FILE* configFile = fopen(path_1.c_str(), "a");
        if (configFile == NULL) {
        		return;
        }
        //return; // No HUZUcoin.conf file is OK

    set<string> setOptions;
    setOptions.insert("*");

	 bool fExistMasternode = false;
	 bool fExistPrivKey = false;
	 bool fExistTxIndex = false;
	 bool fExistMnAddr = false;
    for (boost::program_options::detail::config_file_iterator it(streamConfig, setOptions), end; it != end; ++it)
    {
        // Don't overwrite existing settings so command line settings override HUZUcoin.conf
        string strKey = it->string_key;
        strKey += "=";
        if (it->string_key == "masternode") {
        	fExistMasternode = true;
     		strKey += "1";
        }
        else if (it->string_key == "masternodeprivkey") {
        	fExistPrivKey = true;
     		strKey += sMasternodePrivKey;
             continue;
        } 
        else if (it->string_key == "txindex") {
        	fExistTxIndex=true;
        	strKey += "1";
        }
        else if (it->string_key == "masternodeaddr") {
        	fExistMnAddr = true;
        	strKey += sAddress;
            continue;
        }
        else {
        	strKey += it->value[0];
        }

        strKey += "\n";
        fwrite(strKey.c_str(), std::strlen(strKey.c_str()), 1, configFile);
    }
    if (!fExistMasternode) {
    	string strMasternode = "masternode=1\n";
    	fwrite(strMasternode.c_str(), std::strlen(strMasternode.c_str()), 1, configFile);
    }
    if (!fExistTxIndex) {
    	string strTxIndex = "txindex=1\n";
    	fwrite(strTxIndex.c_str(), std::strlen(strTxIndex.c_str()), 1, configFile);
    }
    
    streamConfig.close();
    fclose(configFile);
    
#ifdef WIN32
    std::string removeCommand = "del " + GetConfigFile().string();
	 std::string renameCommand = "move " + path_1 + " " + GetConfigFile().string();
#else
    std::string removeCommand = "rm -f " + GetConfigFile().string();
	 std::string renameCommand = "mv -f " + path_1 + " " + GetConfigFile().string();
#endif
	 std::system(removeCommand.c_str());
	 std::system(renameCommand.c_str());
	 
    // If datadir is changed in .conf file:
//    ClearDatadirCache();

   std::string strStatusHtml;
	strStatusHtml += "<center>Masternode was successfully added";
    strStatusHtml += "</center>";

    QMessageBox msg;
    msg.setText(QString::fromStdString(strStatusHtml));
    msg.exec();

        accept();
    }
}

void CreateMasternodeDlg::on_cancelButton_clicked()
{
    reject();
}

void CreateMasternodeDlg::on_AddEditAddressPasteButton_clicked()
{
    // Paste text from clipboard into recipient field
    ui->addressLineEdit->setText(QApplication::clipboard()->text());
}

void CreateMasternodeDlg::on_AddEditPrivkeyPasteButton_clicked()
{
    // Paste text from clipboard into recipient field
    ui->privkeyLineEdit->setText(QApplication::clipboard()->text());
}

void CreateMasternodeDlg::on_AddEditTxhashPasteButton_clicked()
{
    // Paste text from clipboard into recipient field
    ui->txhashLineEdit->setText(QApplication::clipboard()->text());
}
