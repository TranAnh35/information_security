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
    // Caesar Cipher
    QString caesarEncrypt(const QString &input, int key);
    QString caesarDecrypt(const QString &input, int key);

    // Substitution Cipher
    QString generateSubstitutionKey();
    void saveSubstitutionKey(const QString &key, const QString &fileName);
    QString readSubstitutionKey(const QString &fileName);
    QString SubstitutionEncryption(const QString &input, const QString &key);
    QString SubstitutionDecryption(const QString &input, const QString &key);

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
};

#endif // CIPHER_H
