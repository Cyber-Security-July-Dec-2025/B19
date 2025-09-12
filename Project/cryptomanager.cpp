#include "cryptomanager.h"
#include <stdexcept>
#include <QDebug>

// Crypto++ Headers
#include "cryptopp_headers/osrng.h" // AutoSeededRandomPool
#include "cryptopp_headers/rsa.h"    // RSA
#include "cryptopp_headers/dsa.h"    // DSA
#include "cryptopp_headers/files.h"  // FileSink, FileSource
#include "cryptopp_headers/hex.h"    // HexEncoder, HexDecoder
#include "cryptopp_headers/pssr.h"   // PSSR
#include "cryptopp_headers/sha.h"    // SHA256

// Use Crypto++ namespace
using namespace CryptoPP;

// --- Filenames for keys ---
const std::string RSA_PUBLIC_KEY = "rsa_public.key";
const std::string RSA_PRIVATE_KEY = "rsa_private.key";
const std::string DSA_PUBLIC_KEY = "dsa_public.key";
const std::string DSA_PRIVATE_KEY = "dsa_private.key";

CryptoManager::CryptoManager(unsigned int rsaKeySize, unsigned int dsaKeySize)
    : m_rsaKeySize(rsaKeySize), m_dsaKeySize(dsaKeySize)
{
}

// --- Key Save/Load Helpers ---
template<typename T>
void CryptoManager::saveKey(const T& key, const std::string& filename) {
    FileSink sink(filename.c_str());
    key.Save(sink);
}

template<typename T>
void CryptoManager::loadKey(T& key, const std::string& filename) {
    FileSource source(filename.c_str(), true);
    key.Load(source);
}


// --- RSA IMPLEMENTATION ---

bool CryptoManager::generateRSAKeys()
{
    try {
        AutoSeededRandomPool rng;
        RSA::PrivateKey privateKey;
        privateKey.GenerateRandomWithKeySize(rng, m_rsaKeySize);
        RSA::PublicKey publicKey(privateKey);

        saveKey(privateKey, RSA_PRIVATE_KEY);
        saveKey(publicKey, RSA_PUBLIC_KEY);
        return true;
    } catch (const Exception& ex) {
        qCritical() << "RSA Key Generation Error: " << ex.what();
        return false;
    }
}

bool CryptoManager::encryptRSA(const QString& inputFile, const QString& outputFile)
{
    try {
        AutoSeededRandomPool rng;
        RSA::PublicKey publicKey;
        loadKey(publicKey, RSA_PUBLIC_KEY);

        RSAES_OAEP_SHA_Encryptor e(publicKey);

        FileSource fs(inputFile.toStdString().c_str(), true,
            new PK_EncryptorFilter(rng, e,
                new FileSink(outputFile.toStdString().c_str())
            )
        );
        return true;
    } catch (const Exception& ex) {
        qCritical() << "RSA Encryption Error: " << ex.what();
        return false;
    }
}

bool CryptoManager::decryptRSA(const QString& inputFile, const QString& outputFile)
{
    try {
        AutoSeededRandomPool rng;
        RSA::PrivateKey privateKey;
        loadKey(privateKey, RSA_PRIVATE_KEY);

        RSAES_OAEP_SHA_Decryptor d(privateKey);

        FileSource fs(inputFile.toStdString().c_str(), true,
            new PK_DecryptorFilter(rng, d,
                new FileSink(outputFile.toStdString().c_str())
            )
        );
        return true;
    } catch (const Exception& ex) {
        qCritical() << "RSA Decryption Error: " << ex.what();
        return false;
    }
}


// --- DSA IMPLEMENTATION ---

bool CryptoManager::generateDSAKeys()
{
    try {
        AutoSeededRandomPool rng;
        DSA::PrivateKey privateKey;
        privateKey.GenerateRandomWithKeySize(rng, m_dsaKeySize);
        DSA::PublicKey publicKey;
        publicKey.AssignFrom(privateKey);

        saveKey(privateKey, DSA_PRIVATE_KEY);
        saveKey(publicKey, DSA_PUBLIC_KEY);
        return true;
    } catch (const Exception& ex) {
        qCritical() << "DSA Key Generation Error: " << ex.what();
        return false;
    }
}

bool CryptoManager::signDSA(const QString& inputFile, QString& signatureHex)
{
    try {
        AutoSeededRandomPool rng;
        DSA::PrivateKey privateKey;
        loadKey(privateKey, DSA_PRIVATE_KEY);

        DSA::Signer signer(privateKey);
        std::string signature; // To hold the raw binary signature

        // Sign the file and store the binary signature in the 'signature' string
        FileSource fs(inputFile.toStdString().c_str(), true,
            new SignerFilter(rng, signer,
                new StringSink(signature)
            )
        );

        // ** START OF FIX **
        // Now, hex-encode the binary signature into a new, stable string variable
        std::string hexEncodedSignature;
        StringSource ss(signature, true,
            new HexEncoder(
                new StringSink(hexEncodedSignature) // Correct: sink to our stable string
            )
        );

        // Convert the C++ string result to a Qt string for the GUI
        signatureHex = QString::fromStdString(hexEncodedSignature);
        // ** END OF FIX **

        return true;
    } catch (const Exception& ex) {
        qCritical() << "DSA Signing Error: " << ex.what();
        return false;
    }
}

bool CryptoManager::verifyDSA(const QString& inputFile, const QString& signatureHex)
{
    try {
        DSA::PublicKey publicKey;
        loadKey(publicKey, DSA_PUBLIC_KEY);

        // Define the verifier object
        DSA::Verifier verifier(publicKey);
        std::string decodedSignature;

        // Decode the hex signature back to binary
        StringSource ss(signatureHex.toStdString(), true,
            new HexDecoder(
                new StringSink(decodedSignature)
            )
        );

        // Create a signature filter
        SignatureVerificationFilter filter(verifier);
        filter.Put((const byte*)decodedSignature.data(), decodedSignature.size());

        // Process the original file through the filter
        FileSource fs(inputFile.toStdString().c_str(), true, new Redirector(filter));

        return filter.GetLastResult();
    } catch (const Exception& ex) {
        qCritical() << "DSA Verification Error: " << ex.what();
        return false;
    }
}

