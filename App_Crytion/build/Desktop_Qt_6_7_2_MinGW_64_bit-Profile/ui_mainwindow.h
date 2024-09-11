/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTextEdit *inputTextEdit;
    QSpinBox *keySpinBox;
    QLabel *label;
    QLabel *label_2;
    QPushButton *browseButton;
    QPushButton *encryptButton;
    QTextEdit *encryptedTextEdit;
    QTextEdit *decryptedTextEdit;
    QPushButton *decryptButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->setEnabled(true);
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        inputTextEdit = new QTextEdit(centralwidget);
        inputTextEdit->setObjectName("inputTextEdit");
        inputTextEdit->setGeometry(QRect(20, 40, 701, 111));
        keySpinBox = new QSpinBox(centralwidget);
        keySpinBox->setObjectName("keySpinBox");
        keySpinBox->setGeometry(QRect(320, 180, 111, 31));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(20, 10, 151, 21));
        label->setTextFormat(Qt::TextFormat::AutoText);
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(210, 180, 91, 31));
        browseButton = new QPushButton(centralwidget);
        browseButton->setObjectName("browseButton");
        browseButton->setGeometry(QRect(719, 40, 61, 111));
        encryptButton = new QPushButton(centralwidget);
        encryptButton->setObjectName("encryptButton");
        encryptButton->setGeometry(QRect(150, 260, 111, 41));
        encryptedTextEdit = new QTextEdit(centralwidget);
        encryptedTextEdit->setObjectName("encryptedTextEdit");
        encryptedTextEdit->setGeometry(QRect(20, 350, 371, 192));
        encryptedTextEdit->setReadOnly(true);
        decryptedTextEdit = new QTextEdit(centralwidget);
        decryptedTextEdit->setObjectName("decryptedTextEdit");
        decryptedTextEdit->setGeometry(QRect(410, 350, 371, 192));
        decryptedTextEdit->setReadOnly(true);
        decryptButton = new QPushButton(centralwidget);
        decryptButton->setObjectName("decryptButton");
        decryptButton->setGeometry(QRect(540, 260, 111, 41));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:11pt;\">Nh\341\272\255p n\341\273\231i dung</span></p></body></html>", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:12pt;\">Ch\341\273\215n kh\303\263a:</span></p></body></html>", nullptr));
        browseButton->setText(QCoreApplication::translate("MainWindow", "Browse", nullptr));
        encryptButton->setText(QCoreApplication::translate("MainWindow", "Encrypt", nullptr));
        decryptButton->setText(QCoreApplication::translate("MainWindow", "Decrypt", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
