#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Read configuration from config.ini
    QSettings settings("config.ini", QSettings::IniFormat);
    unsigned int rsaKeySize = settings.value("RSA/KeySize", 2048).toUInt();
    unsigned int dsaKeySize = settings.value("DSA/KeySize", 2048).toUInt();

    m_cryptoManager = new CryptoManager(rsaKeySize, dsaKeySize);
    showStatus("Application ready. Please generate keys first.", false);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_cryptoManager;
}

void MainWindow::showStatus(const QString& message, bool isError)
{
    if (isError) {
        ui->statusbar->setStyleSheet("color: red");
    } else {
        ui->statusbar->setStyleSheet("color: green");
    }
    ui->statusbar->showMessage(message, 5000); // Message disappears after 5 seconds
}


// --- RSA SLOTS ---

void MainWindow::on_btnGenerateRsaKeys_clicked()
{
    if (m_cryptoManager->generateRSAKeys()) {
        showStatus("RSA key pair generated successfully (rsa_public.key, rsa_private.key).", false);
    } else {
        showStatus("Error generating RSA key pair.", true);
    }
}

void MainWindow::on_btnUploadEncrypt_clicked()
{
    m_encryptInputPath = QFileDialog::getOpenFileName(this, "Select File to Encrypt");
    if (!m_encryptInputPath.isEmpty()) {
        ui->lineEncryptFile->setText(m_encryptInputPath);
        showStatus("File selected for encryption.", false);
    }
}

void MainWindow::on_btnEncrypt_clicked()
{
    if (m_encryptInputPath.isEmpty()) {
        showStatus("Please select a file to encrypt first.", true);
        return;
    }

    QString outputPath = QFileDialog::getSaveFileName(this, "Save Encrypted File As", "encrypted.bin");
    if (outputPath.isEmpty()) return;

    if (m_cryptoManager->encryptRSA(m_encryptInputPath, outputPath)) {
        showStatus("File encrypted successfully.", false);
    } else {
        showStatus("Encryption failed. Check if public key exists.", true);
    }
}

void MainWindow::on_btnUploadDecrypt_clicked()
{
    m_decryptInputPath = QFileDialog::getOpenFileName(this, "Select File to Decrypt");
    if (!m_decryptInputPath.isEmpty()) {
        ui->lineDecryptFile->setText(m_decryptInputPath);
        showStatus("File selected for decryption.", false);
    }
}

void MainWindow::on_btnDecrypt_clicked()
{
    if (m_decryptInputPath.isEmpty()) {
        showStatus("Please select a file to decrypt first.", true);
        return;
    }

    QString outputPath = QFileDialog::getSaveFileName(this, "Save Decrypted File As", "decrypted.txt");
    if (outputPath.isEmpty()) return;

    if (m_cryptoManager->decryptRSA(m_decryptInputPath, outputPath)) {
        showStatus("File decrypted successfully.", false);
    } else {
        showStatus("Decryption failed. Check if private key exists or if file is corrupt.", true);
    }
}


// --- DSA SLOTS ---

void MainWindow::on_btnGenerateDsaKeys_clicked()
{
    if (m_cryptoManager->generateDSAKeys()) {
        showStatus("DSA key pair generated successfully (dsa_public.key, dsa_private.key).", false);
    } else {
        showStatus("Error generating DSA key pair.", true);
    }
}

void MainWindow::on_btnUploadSign_clicked()
{
    m_signInputPath = QFileDialog::getOpenFileName(this, "Select File to Sign");
    if (!m_signInputPath.isEmpty()) {
        ui->lineSignFile->setText(m_signInputPath);
        showStatus("File selected for signing.", false);
    }
}

void MainWindow::on_btnSign_clicked()
{
    if (m_signInputPath.isEmpty()) {
        showStatus("Please select a file to sign first.", true);
        return;
    }

    QString signature;
    if (m_cryptoManager->signDSA(m_signInputPath, signature)) {
        ui->textSignature->setText(signature);

        // Optional: Save signature to file
        QString sigPath = QFileDialog::getSaveFileName(this, "Save Signature As", "signature.sig");
        if(!sigPath.isEmpty()){
            QFile file(sigPath);
            if(file.open(QIODevice::WriteOnly)){
                file.write(signature.toUtf8());
                file.close();
                showStatus("Signature generated and saved.", false);
            }
        } else {
            showStatus("Signature generated (not saved to file).", false);
        }

    } else {
        showStatus("Signature generation failed. Check if private key exists.", true);
    }
}

void MainWindow::on_btnUploadVerifyFile_clicked()
{
    m_verifyInputPath = QFileDialog::getOpenFileName(this, "Select Original File");
    if (!m_verifyInputPath.isEmpty()) {
        ui->lineVerifyFile->setText(m_verifyInputPath);
    }
}

void MainWindow::on_btnUploadVerifySig_clicked()
{
    m_verifySigInputPath = QFileDialog::getOpenFileName(this, "Select Signature File");
    if (!m_verifySigInputPath.isEmpty()) {
        ui->lineVerifySig->setText(m_verifySigInputPath);
        // Also load signature into the text box for verification
        QFile file(m_verifySigInputPath);
        if(file.open(QIODevice::ReadOnly)){
            ui->textSignature->setText(file.readAll());
            file.close();
        }
    }
}

void MainWindow::on_btnVerify_clicked()
{
    if (m_verifyInputPath.isEmpty() || m_verifySigInputPath.isEmpty()) {
        showStatus("Please select both the original file and its signature file.", true);
        return;
    }

    QFile sigFile(m_verifySigInputPath);
    if (!sigFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        showStatus("Could not open signature file.", true);
        return;
    }
    QString signature = sigFile.readAll();
    sigFile.close();


    bool isValid = m_cryptoManager->verifyDSA(m_verifyInputPath, signature);

    if (isValid) {
        QMessageBox::information(this, "Verification Result", "The signature is valid.");
        showStatus("Signature verification successful.", false);
    } else {
        QMessageBox::warning(this, "Verification Result", "The signature is NOT valid.");
        showStatus("Signature verification failed.", true);
    }
}
