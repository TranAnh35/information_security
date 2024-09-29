#ifndef CIPHER_H
#define CIPHER_H

#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QByteArray>
#include <QTextStream>
#include <QMessageBox>
#include <qnamespace.h>
#include <QRandomGenerator>
#include <QCoreApplication>
#include <QRegularExpression>
#include <QCryptographicHash>

#include <gcrypt.h>

class Cipher{
public:

    /*
    * **************************************************
    * Mã hóa file text
    * **************************************************
    */

    // Caesar Cipher
    QString caesarEncrypt(const QString &input, int key);
    QString caesarDecrypt(const QString &input, int key);

    // Substitution Cipher
    QString SubstitutionEncryption(const QString &input);
    QString SubstitutionDecryption(const QString &input);

    // Affine Cipher
    QString affineEncrypt(const QString &text, int a, int b);
    QString affineDecrypt(const QString &text, int a, int b);

    // Vigenère Cipher
    QString vigenereEncrypt(const QString &text, const QString &key);
    QString vigenereDecrypt(const QString &text, const QString &key);

    // Hill Cipher
    QVector<QVector<int>> parseHillKey(const QString &key);
    QString hillEncrypt(const QString &text, const QVector<QVector<int>> &keyMatrix);
    QString hillDecrypt(const QString &text, const QVector<QVector<int>> &keyMatrix);

    // Permutation Cipher
    QString encryptPermutationCipher(const QString &text, const QString &key);
    QString decryptPermutationCipher(const QString &text, const QString &key);

    // Playfair Cipher
    QVector<QVector<QChar>> generatePlayfairTable(const QString &key);
    QString playfairEncrypt(const QString &text, const QVector<QVector<QChar>> &table);
    QString playfairDecrypt(const QString &text, const QVector<QVector<QChar>> &table);

    // Rail Fence Cipher
    QString railFenceEncrypt(const QString &text, int key);
    QString railFenceDecrypt(const QString &cipherText, int key);

    // DES Cipher
    QString desEncrypt(const QString &plainText, const QString &keyStr);
    QString desDecrypt(const QString &encryptedText, const QString &keyStr);

    // AES Cipher
    QString aesEncrypt(const QString &plaintext, const QString &key);
    QString aesDecrypt(const QString &ciphertext, const QString &key);

    /*
    * **************************************************
    * Mã hóa nhiều file
    * **************************************************
    */

    // Hàm mã 
    static QByteArray encryptAES(const QByteArray &data);
    static QByteArray encryptDES(const QByteArray &data);

    // Giải mã
    static QByteArray decryptAES(const QByteArray &data);
    static QByteArray decryptDES(const QByteArray &data);

private:
    // Substitution Cipher
    static QString generateSubstitutionKey();
    static QString checkSubstitutionKey();

    // DES Cipher
    static QByteArray checkDESKey();
    static QByteArray generateDESKey();

    // AES Cipher
    static QByteArray checkAESKey();
    static QByteArray generateAESKey();
    static void saveKeyToFile(const QString &filePath, const QByteArray &key);
    static QByteArray loadKeyFromFile(const QString &filePath);
};

#endif // CIPHER_H
