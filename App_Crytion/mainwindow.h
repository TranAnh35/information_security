#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void checkTextEdits();

private:
    Ui::MainWindow *ui;
    QString readFile(const QString &fileName);
    void writeFile(const QString &fileName, const QString &text);
    QString Encryption(const QString &input, int key);
    QString Decryption(const QString &input, int key);
    QString currentFilePath;
};

#endif // MAINWINDOW_H
