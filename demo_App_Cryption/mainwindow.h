#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QUrl>
#include <QFile>
#include <QMimeData>
#include <QString>
#include <QMainWindow>
#include <QTextStream>
#include <QMessageBox>
#include <qnamespace.h>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QDragEnterEvent>
#include <QTextStream>
#include <QFileDialog>

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
    void on_reScanButton_clicked();
    void on_saveEncryptedButton_clicked();
    void on_saveDecryptedButton_clicked();
    void on_encryptionTypeComboBox_currentIndexChanged();
    void checkTextEdits();

    void set_SpinBox(bool status);
    void set_LineEdit(bool status);

    void set_Affine_Form(bool status);
    void set_Hill_Form(bool status);

    void on_browseFileTxtButton_clicked();
    void on_file_browseInputButton_clicked();
    void on_file_browseOutputButton_clicked();

private:
    Ui::MainWindow *ui;
    Cipher cipher;

    void showImportOptions();
    void showTextEntry();
    void showFileOptions();
    void browseFile();

    void importEncryptFile();
    void importDecryptFile();
    void startProcess();
    void backToHome();
    void backToImport();

    QString readFile(const QString &fileName);
    void writeFile(const QString &fileName, const QString &text);

    void check_Exception_Key(const QString& key);

    QString encryptText(const QString& text, const QString& type);
    QString decryptText(const QString& text, const QString& type);
    
    QString getExecutablePath();
    QString currentFilePath;

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

};

#endif // MAINWINDOW_H
