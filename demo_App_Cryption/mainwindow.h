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

#include "Cipher.h"

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

    void set_Caesar_Form(bool status);
    void set_Affine_Form(bool status);
    void set_Vigenere_Form(bool status);
    void set_Hill_Form(bool status);
    void set_Permutation_Form(bool status);
    void set_Playfair_Form(bool status);
    void set_RailFence_Form(bool status);

private:
    Ui::MainWindow *ui;
    Cipher cipher;

    QString readFile(const QString &fileName);
    void writeFile(const QString &fileName, const QString &text);
    
    QString getExecutablePath();
    QString currentFilePath;
};

#endif // MAINWINDOW_H
