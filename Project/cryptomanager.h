#ifndef CRYPTOMANAGER_H
#define CRYPTOMANAGER_H

#include <QString>

class CryptoManager
{
public:
    CryptoManager(unsigned int rsaKeySize, unsigned int dsaKeySize);

    // RSA Functions
    bool generateRSAKeys();
    bool encryptRSA(const QString& inputFile, const QString& outputFile);
    bool decryptRSA(const QString& inputFile, const QString& outputFile);

    // DSA Functions
    bool generateDSAKeys();
    bool signDSA(const QString& inputFile, QString& signatureHex);
    bool verifyDSA(const QString& inputFile, const QString& signatureHex);

private:
    unsigned int m_rsaKeySize;
    unsigned int m_dsaKeySize;

    // Helper to save/load keys to/from files
    template<typename T>
    void saveKey(const T& key, const std::string& filename);

    template<typename T>
    void loadKey(T& key, const std::string& filename);
};

#endif // CRYPTOMANAGER_H
