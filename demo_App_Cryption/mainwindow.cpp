#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->encryptedTextEdit->setReadOnly(true);
    ui->decryptedTextEdit->setReadOnly(true);
    ui->saveEncryptedButton->setVisible(false);
    ui->saveDecryptedButton->setVisible(false);

    connect(ui->inputTextEdit, &QTextEdit::textChanged, this, &MainWindow::checkTextEdits);
    connect(ui->encryptedTextEdit, &QTextEdit::textChanged, this, &MainWindow::checkTextEdits);
    connect(ui->decryptedTextEdit, &QTextEdit::textChanged, this, &MainWindow::checkTextEdits);

    QVector<QString> items = {"Caesar Cipher", "Substitution Cipher", "Affine Cipher", "Vigenère Cipher",
                              "Hill Cipher", "Permutation Cipher", "Playfair Cipher", "Rail Fence Cipher"};

    for(const auto& item : items){
        ui->encryptionTypeComboBox->addItem(item);
    }

}

MainWindow::~MainWindow() {
    delete ui;
}

/*
Các hàm xử lý sự kiện
*/

// Hàm mã hóa
void MainWindow::on_encryptButton_clicked() {
    QString text = ui->inputTextEdit->toPlainText();
    QString encryptedText;
    if (ui->encryptionTypeComboBox->currentText() == "Caesar Cipher") {
        int key = ui->caesar_keySpinBox->value();
        if (text.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please provide both plaintext and a key.");
            return;
        }
        encryptedText = cipher.caesarEncrypt(text, key);
    } else if (ui->encryptionTypeComboBox->currentText() == "Substitution Cipher") {
        QString keyFilePath = getExecutablePath() + "/substitution_key.txt";
        QString substitutionKey;
        if (QFile::exists(keyFilePath)) {
            substitutionKey = cipher.readSubstitutionKey(keyFilePath);
        } else {
            substitutionKey = cipher.generateSubstitutionKey();
            cipher.saveSubstitutionKey(substitutionKey, keyFilePath);
        }
        encryptedText = cipher.SubstitutionEncryption(text, substitutionKey);
    } else if (ui->encryptionTypeComboBox->currentText() == "Affine Cipher") {
        int a = ui->affine_a_SpinBox->value();
        int b = ui->affine_b_SpinBox->value();
        if (text.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please provide both plaintext and a key.");
            return;
        }
        encryptedText = cipher.affineEncrypt(text, a, b);
    } else if (ui->encryptionTypeComboBox->currentText() == "Vigenère Cipher") {
        QString key = ui->vigen_keyLineEdit->text();
        if (text.isEmpty() || key.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please provide both plaintext and a key.");
            return;
        }
        encryptedText = cipher.vigenereEncrypt(text, key);
    } else if (ui->encryptionTypeComboBox->currentText() == "Hill Cipher") {
        QString key = ui->hillMatrixTextEdit->toPlainText();
        QVector<QVector<int>> keyMatrix = cipher.parseHillKey(key);
        if (text.isEmpty() || keyMatrix.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please provide both plaintext and a key.");
            return;
        }
        encryptedText = cipher.hillEncrypt(text, keyMatrix);
    } else if (ui->encryptionTypeComboBox->currentText() == "Permutation Cipher") {
        QString key = ui->permutation_KeyLineEdit->text();
        if (text.isEmpty() || key.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please provide both plaintext and a key.");
            return;
        }
        encryptedText = cipher.encryptPermutationCipher(text, key);
    } else if (ui->encryptionTypeComboBox->currentText() == "Playfair Cipher"){
        QString keyText = ui->playfair_KeyLineEdit->text();
        QVector<QVector<QChar>> key = cipher.generatePlayfairTable(keyText);
        if (text.isEmpty() || key.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please provide both plaintext and a key.");
            return;
        }
        encryptedText = cipher.playfairEncrypt(text, key);
    } else if (ui->encryptionTypeComboBox->currentText() == "Rail Fence Cipher") {
        int key = ui->railFence_SpinBox->value();
        if (text.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please provide both plaintext and a key.");
            return;
        }
        encryptedText = cipher.railFenceEncrypt(text, key);
    }

    ui->encryptedTextEdit->setPlainText(encryptedText);
}

// Hàm giải mã
void MainWindow::on_decryptButton_clicked() {
    QString text = ui->inputTextEdit->toPlainText();
    QString decryptedText;
    if (ui->encryptionTypeComboBox->currentText() == "Caesar Cipher") {
        int key = ui->caesar_keySpinBox->value();
        if (text.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please provide both plaintext and a key.");
            return;
        }
        decryptedText = cipher.caesarDecrypt(text, key);
    } else if (ui->encryptionTypeComboBox->currentText() == "Substitution Cipher") {
        QString keyFilePath = getExecutablePath() + "/substitution_key.txt";
        QString substitutionKey = cipher.readSubstitutionKey(keyFilePath);
        decryptedText = cipher.SubstitutionDecryption(text, substitutionKey);
    } else if (ui->encryptionTypeComboBox->currentText() == "Affine Cipher") {
        int a = ui->affine_a_SpinBox->value();
        int b = ui->affine_b_SpinBox->value();
        if (text.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please provide both plaintext and a key.");
            return;
        }
        decryptedText = cipher.affineDecrypt(text, a, b);
    } else if (ui->encryptionTypeComboBox->currentText() == "Vigenère Cipher") {
        QString key = ui->vigen_keyLineEdit->text();
        if (text.isEmpty() || key.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please provide both plaintext and a key.");
            return;
        }
        decryptedText = cipher.vigenereDecrypt(text, key);
    } else if (ui->encryptionTypeComboBox->currentText() == "Hill Cipher") {
        QString key = ui->hillMatrixTextEdit->toPlainText();
        QVector<QVector<int>> keyMatrix = cipher.parseHillKey(key);
        if (text.isEmpty() || keyMatrix.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please provide both plaintext and a key.");
            return;
        }
        decryptedText = cipher.hillDecrypt(text, keyMatrix);
    } else if (ui->encryptionTypeComboBox->currentText() == "Permutation Cipher") {
        QString key = ui->permutation_KeyLineEdit->text();
        if (text.isEmpty() || key.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please provide both plaintext and a key.");
            return;
        }
        decryptedText = cipher.decryptPermutationCipher(text, key);
    } else if (ui->encryptionTypeComboBox->currentText() == "Playfair Cipher") {
        QString keyText = ui->playfair_KeyLineEdit->text();
        QVector<QVector<QChar>> key = cipher.generatePlayfairTable(keyText);
        if (text.isEmpty() || key.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please provide both plaintext and a key.");
            return;
        }
        decryptedText = cipher.playfairDecrypt(text, key);
    } else if (ui->encryptionTypeComboBox->currentText() == "Rail Fence Cipher") {
        int key = ui->railFence_SpinBox->value();
        if(text.isEmpty()){
            QMessageBox::warning(this, "Input Error", "Please provide both plaintext and a key.");
            return;
        }
        decryptedText = cipher.railFenceDecrypt(text, key);
    }

    ui->decryptedTextEdit->setPlainText(decryptedText);
}

// Hàm mở file
void MainWindow::on_browseButton_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Text File", "", "Text Files (*.txt)");
    if (!fileName.isEmpty()) {
        QString text = readFile(fileName);
        ui->inputTextEdit->setPlainText(text);
        ui->filePathLabel->setText(fileName);
        currentFilePath = fileName;
        ui->saveEncryptedButton->setVisible(true);
        ui->saveDecryptedButton->setVisible(true);
    }
}

// Hàm lưu kết quả mã hóa vào file
void MainWindow::on_saveEncryptedButton_clicked() {
    if (currentFilePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No file selected!");
        return;
    }
    QString encryptedText = ui->encryptedTextEdit->toPlainText();
    writeFile(currentFilePath, encryptedText);
    QMessageBox::information(this, "Success", "Encrypted text saved to file.");
}

// Hàm lưu kết quả giải mã vào file
void MainWindow::on_saveDecryptedButton_clicked() {
    if (currentFilePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No file selected!");
        return;
    }
    QString decryptedText = ui->decryptedTextEdit->toPlainText();
    writeFile(currentFilePath, decryptedText);
    QMessageBox::information(this, "Success", "Decrypted text saved to file.");
}

// Hàm kiểm tra các ô nhập liệu
void MainWindow::checkTextEdits() {
    bool enableSave = !ui->inputTextEdit->toPlainText().isEmpty() &&
                      (!ui->encryptedTextEdit->toPlainText().isEmpty() ||
                       !ui->decryptedTextEdit->toPlainText().isEmpty());
    ui->saveEncryptedButton->setEnabled(enableSave);
    ui->saveDecryptedButton->setEnabled(enableSave);
}

/*
**********************************************
Các hàm xử lý sự kiện khi thay đổi loại mã hóa
**********************************************
*/

void MainWindow::set_Caesar_Form(bool status){
    ui->label_caesar->setVisible(status);
    ui->caesar_keySpinBox->setVisible(status);
}

void MainWindow::set_Affine_Form(bool status){
    ui->label_affine_a->setVisible(status);
    ui->label_affine_b->setVisible(status);
    ui->affine_a_SpinBox->setVisible(status);
    ui->affine_b_SpinBox->setVisible(status);
}

void MainWindow::set_Vigenere_Form(bool status){
    ui->label_vigen->setVisible(status);
    ui->vigen_keyLineEdit->setVisible(status);
}

void MainWindow::set_Hill_Form(bool status){
    ui->label_hill->setVisible(status);
    ui->hillMatrixTextEdit->setVisible(status);
}

void MainWindow::set_Permutation_Form(bool status){
    ui->label_permutation->setVisible(status);
    ui->permutation_KeyLineEdit->setVisible(status);
}

void MainWindow::set_Playfair_Form(bool status){
    ui->label_playfair->setVisible(status);
    ui->playfair_KeyLineEdit->setVisible(status);
}

void MainWindow::set_RailFence_Form(bool status){
    ui->label_railFence->setVisible(status);
    ui->railFence_SpinBox->setVisible(status);
}

void MainWindow::on_encryptionTypeComboBox_currentIndexChanged() {
    if (ui->encryptionTypeComboBox->currentText() == "Caesar Cipher") {
        set_Caesar_Form(true);
        set_Affine_Form(false);
        set_Vigenere_Form(false);
        set_Hill_Form(false);
        set_Permutation_Form(false);
        set_Playfair_Form(false);
        set_RailFence_Form(false);
    } else if (ui->encryptionTypeComboBox->currentText() == "Substitution Cipher") {
        set_Caesar_Form(false);
        set_Affine_Form(false);
        set_Vigenere_Form(false);
        set_Hill_Form(false);
        set_Permutation_Form(false);
        set_Playfair_Form(false);
        set_RailFence_Form(false);
    } else if (ui->encryptionTypeComboBox->currentText() == "Affine Cipher") {
        set_Caesar_Form(false);
        set_Affine_Form(true);
        set_Vigenere_Form(false);
        set_Hill_Form(false);
        set_Permutation_Form(false);
        set_Playfair_Form(false);
        set_RailFence_Form(false);
    } else if (ui->encryptionTypeComboBox->currentText() == "Vigenère Cipher") {
        set_Caesar_Form(false);
        set_Affine_Form(false);
        set_Vigenere_Form(true);
        set_Hill_Form(false);
        set_Permutation_Form(false);
        set_Playfair_Form(false);
    } else if (ui->encryptionTypeComboBox->currentText() == "Hill Cipher") {
        set_Caesar_Form(false);
        set_Affine_Form(false);
        set_Vigenere_Form(false);
        set_Hill_Form(true);
        set_Permutation_Form(false);
        set_Playfair_Form(false);
        set_RailFence_Form(false);
    } else if (ui->encryptionTypeComboBox->currentText() == "Permutation Cipher"){
        set_Caesar_Form(false);
        set_Affine_Form(false);
        set_Vigenere_Form(false);
        set_Hill_Form(false);
        set_Permutation_Form(true);
        set_Playfair_Form(false);
    } else if (ui->encryptionTypeComboBox->currentText() == "Playfair Cipher"){
        set_Caesar_Form(false);
        set_Affine_Form(false);
        set_Vigenere_Form(false);
        set_Hill_Form(false);
        set_Permutation_Form(false);
        set_Playfair_Form(true);
        set_RailFence_Form(false);
    } else if (ui->encryptionTypeComboBox->currentText() == "Rail Fence Cipher"){
        set_Caesar_Form(false);
        set_Affine_Form(false);
        set_Vigenere_Form(false);
        set_Hill_Form(false);
        set_Permutation_Form(false);
        set_Playfair_Form(false);
        set_RailFence_Form(true);
    }
}

// Hàm đọc file
QString MainWindow::readFile(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cannot open file: " + fileName);
        return "";
    }
    QTextStream in(&file);
    return in.readAll();
}

// Hàm ghi file
void MainWindow::writeFile(const QString &fileName, const QString &text) {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cannot write to file: " + fileName);
        return;
    }
    QTextStream out(&file);
    out << text;
}

// Hàm lấy đường dẫn thực thi
QString MainWindow::getExecutablePath() {
    return QCoreApplication::applicationDirPath();
}
