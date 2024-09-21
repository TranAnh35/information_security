#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFile>
#include <QString>
#include <algorithm>
#include <QMainWindow>
#include <QTextStream>
#include <QMessageBox>
#include <qnamespace.h>
#include <QRandomGenerator>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_encryptButton_clicked();
    void on_decryptButton_clicked();
    void on_browseButton_clicked();
    void on_saveEncryptedButton_clicked();
    void on_saveDecryptedButton_clicked();
    void on_encryptionTypeComboBox_currentIndexChanged();
    void checkTextEdits();

private:
    Ui::MainWindow *ui;
    QString readFile(const QString &fileName);
    void writeFile(const QString &fileName, const QString &text);

    // Caesar Cipher
    QString caesarEncrypt(const QString &input, int key);
    QString caesarDecrypt(const QString &input, int key);

    // Substitution Cipher
    QString generateSubstitutionKey();
    QString readSubstitutionKey(const QString &fileName);
    void saveSubstitutionKey(const QString &key, const QString &fileName);
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

    QString getExecutablePath();
    QString currentFilePath;
};

#endif // MAINWINDOW_H
