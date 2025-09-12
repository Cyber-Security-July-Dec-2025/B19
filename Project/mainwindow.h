#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cryptomanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // RSA Slots
    void on_btnGenerateRsaKeys_clicked();
    void on_btnUploadEncrypt_clicked();
    void on_btnEncrypt_clicked();
    void on_btnUploadDecrypt_clicked();
    void on_btnDecrypt_clicked();

    // DSA Slots
    void on_btnGenerateDsaKeys_clicked();
    void on_btnUploadSign_clicked();
    void on_btnSign_clicked();
    void on_btnUploadVerifyFile_clicked();
    void on_btnUploadVerifySig_clicked();
    void on_btnVerify_clicked();


private:
    Ui::MainWindow *ui;
    CryptoManager* m_cryptoManager;

    // File paths for processing
    QString m_encryptInputPath;
    QString m_decryptInputPath;
    QString m_signInputPath;
    QString m_verifyInputPath;
    QString m_verifySigInputPath;

    void showStatus(const QString& message, bool isError = false);
};
#endif // MAINWINDOW_H
