#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setAcceptDrops(true);

    connect(ui->importFileButton, &QPushButton::clicked, this, &MainWindow::showImportOptions);
    connect(ui->enterTextButton, &QPushButton::clicked, this, &MainWindow::showTextEntry);
    connect(ui->importEncryptButton, &QPushButton::clicked, this, &MainWindow::showFileOptions);
    connect(ui->importDecryptButton, &QPushButton::clicked, this, &MainWindow::showFileOptions);
    connect(ui->importEncryptButton, &QPushButton::clicked, this, &MainWindow::importEncryptFile);
    connect(ui->importDecryptButton, &QPushButton::clicked, this, &MainWindow::importDecryptFile);
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::startProcess);
    connect(ui->text_backHomeButton, &QPushButton::clicked, this, &MainWindow::backToHome);
    connect(ui->import_backHomeButton, &QPushButton::clicked, this, &MainWindow::backToHome);
    connect(ui->file_backImportButton, &QPushButton::clicked, this, &MainWindow::backToImport);

    QVector<QString> items = {"Caesar Cipher", "Substitution Cipher", "Affine Cipher", "Vigenère Cipher",
                              "Hill Cipher", "Permutation Cipher", "Playfair Cipher", "Rail Fence Cipher",
                              "DES Cipher", "AES Cipher"};

    for(const auto& item : items){
        ui->encryptionTypeComboBox->addItem(item);
    }

    ui->encryptedTextEdit->setReadOnly(true);
    ui->decryptedTextEdit->setReadOnly(true);
    ui->saveEncryptedButton->setVisible(false);
    ui->saveDecryptedButton->setVisible(false);
    ui->reScanButton->setVisible(false);

    connect(ui->inputTextEdit, &QTextEdit::textChanged, this, &MainWindow::checkTextEdits);
    connect(ui->encryptedTextEdit, &QTextEdit::textChanged, this, &MainWindow::checkTextEdits);
    connect(ui->decryptedTextEdit, &QTextEdit::textChanged, this, &MainWindow::checkTextEdits);

    ui->algorithmComboBox->addItem("DES Cipher");
    ui->algorithmComboBox->addItem("AES Cipher");
}

MainWindow::~MainWindow() {
    delete ui;
}

QString byteArrayToString(const QByteArray &byteArray) {
    return QString::fromUtf8(byteArray);
}

void MainWindow::showImportOptions()
{
    ui->stackedWidget->setCurrentWidget(ui->pageImport);
}

void MainWindow::showTextEntry()
{
    ui->stackedWidget->setCurrentWidget(ui->pageText);
}

void MainWindow::showFileOptions()
{
    ui->stackedWidget->setCurrentWidget(ui->pageFile);
}

void MainWindow::browseFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("All Files (*)"));
    if (!filePath.isEmpty()) {
        ui->inputPathLineEdit->setText(filePath);
        ui->stackedWidget->setCurrentWidget(ui->pageFile);
    }
}

void MainWindow::on_file_browseButton_clicked() {
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("All Files (*)"));
    if (!filePath.isEmpty()) {
        QString mode = ui->startButton->property("mode").toString();
        QFileInfo fileInfo(ui->inputPathLineEdit->text());
        QString inputSuffix = "." + fileInfo.suffix();

        if (mode == "encrypt") {
            ui->outputPathLineEdit->setText(filePath + inputSuffix + ".dat");
        } else if (mode == "decrypt") {
            if (filePath.endsWith(".dat")) {
                filePath.chop(4); // Remove ".dat"
            }
            ui->outputPathLineEdit->setText(filePath + inputSuffix);
        }
    }
}

void MainWindow::importEncryptFile()
{
    browseFile();
    QString inputFilePath = ui->inputPathLineEdit->text();
    ui->outputPathLineEdit->setText(inputFilePath + ".dat");
    ui->startButton->setProperty("mode", "encrypt");
}

void MainWindow::importDecryptFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Data Files (*.dat)"));
    if (!filePath.isEmpty()) {
        ui->inputPathLineEdit->setText(filePath);
        QString outputFilePath = filePath;
        outputFilePath.chop(4);
        ui->outputPathLineEdit->setText(outputFilePath);
        ui->startButton->setProperty("mode", "decrypt");
    }
}

void MainWindow::backToHome()
{
    ui->stackedWidget->setCurrentWidget(ui->pageHome);
}

void MainWindow::backToImport()
{
    ui->stackedWidget->setCurrentWidget(ui->pageImport);
}

void MainWindow::startProcess()
{
    QString inputFilePath = ui->inputPathLineEdit->text();
    QString outputFilePath = ui->outputPathLineEdit->text();
    QString mode = ui->startButton->property("mode").toString();
    QString algorithm = ui->algorithmComboBox->currentText();

    QFile inputFile(inputFilePath);
    if (inputFile.open(QIODevice::ReadOnly)) {
        QByteArray fileContent = inputFile.readAll();
        inputFile.close();

        QByteArray processedContent;

        if (algorithm == "AES Cipher") {
            if (mode == "encrypt") {
                processedContent = Cipher::encryptAES(fileContent);
            } else if (mode == "decrypt") {
                processedContent = Cipher::decryptAES(fileContent);
            }
        } else if (algorithm == "DES Cipher") {
            if (mode == "encrypt") {
                processedContent = Cipher::encryptDES(fileContent);
            } else if (mode == "decrypt") {
                processedContent = Cipher::decryptDES(fileContent);
            }
        }

        if (!processedContent.isEmpty()) {
            QFile outputFile(outputFilePath);
            if (outputFile.open(QIODevice::WriteOnly)) {
                outputFile.write(processedContent);
                outputFile.close();
                QMessageBox::information(this, tr("Success"), tr("Process completed successfully."));
            } else {
                QMessageBox::warning(this, tr("Error"), tr("Failed to open output file."));
            }
        } else {
            QMessageBox::warning(this, tr("Error"), tr("Failed to process content."));
        }

        ui->stackedWidget->setCurrentWidget(ui->pageImport);
    } else {
        QMessageBox::warning(this, tr("Error"), tr("Failed to open input file."));
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    foreach (const QUrl &url, event->mimeData()->urls()) {
        QString filePath = url.toLocalFile();
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString fileContent = in.readAll();
            ui->inputTextEdit->setPlainText(fileContent);
            file.close();
        }
    }
}

/*
Các hàm xử lý sự kiện
*/

// Hàm tìm UCLN
int gcd(int a, int b) {
    if (b == 0) {
        return a;
    }
    return gcd(b, a % b);
}

void MainWindow::check_Exception_Key(const QString& key){
    if (key.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please provide cipherkey.");
    }
}

QString MainWindow::encryptText(const QString& text, const QString& type) {
    if (text.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please provide plaintext.");
        return QString();
    }

    if (type == "Caesar Cipher") {
        int key = ui->keySpinBox->value();
        return cipher.caesarEncrypt(text, key);
    }

    else if (type == "Substitution Cipher") {
        QString keyFilePath = getExecutablePath() + "/substitution_key.txt";
        QString substitutionKey;
        if (QFile::exists(keyFilePath)) {
            substitutionKey = cipher.readSubstitutionKey(keyFilePath);
        } else {
            substitutionKey = cipher.generateSubstitutionKey();
            cipher.saveSubstitutionKey(substitutionKey, keyFilePath);
        }
        return cipher.SubstitutionEncryption(text, substitutionKey);
    }

    else if (type == "Affine Cipher") {
        int a = ui->affine_a_SpinBox->value();
        if (gcd(a, 26) != 1) {
            QMessageBox::warning(this, "Input Error", "Key 'a' must be coprime with 26.");
            return QString();
        }
        int b = ui->affine_b_SpinBox->value();
        return cipher.affineEncrypt(text, a, b);
    }

    else if (type == "Vigenère Cipher") {
        QString key = ui->keyLineEdit->text();
        check_Exception_Key(key);
        return cipher.vigenereEncrypt(text, key);
    }

    else if (type == "Hill Cipher") {
        QString key = ui->hillMatrixTextEdit->toPlainText();     
        check_Exception_Key(key);
        QVector<QVector<int>> keyMatrix = cipher.parseHillKey(key);
        return cipher.hillEncrypt(text, keyMatrix);
    }

    else if (type == "Permutation Cipher") {
        QString key = ui->keyLineEdit->text();
        check_Exception_Key(key);
        return cipher.encryptPermutationCipher(text, key);
    }

    else if (type == "Playfair Cipher") {
        QString keyText = ui->keyLineEdit->text();
        check_Exception_Key(keyText);
        QVector<QVector<QChar>> key = cipher.generatePlayfairTable(keyText);
        return cipher.playfairEncrypt(text, key);
    }

    else if (type == "Rail Fence Cipher") {
        int key = ui->keySpinBox->value();
        return cipher.railFenceEncrypt(text, key);
    }

    else if (type == "DES Cipher") {
        QString key = ui->keyLineEdit->text();
        check_Exception_Key(key);
        return cipher.desEncrypt(text, key);
    }

    else if (type == "AES Cipher") {
        QString key = ui->keyLineEdit->text();
        check_Exception_Key(key);
        return cipher.aesEncrypt(text, key);
    }

    return QString();
}

// Hàm giải mã chung
QString MainWindow::decryptText(const QString& text, const QString& type) {
    if (text.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please provide ciphertext.");
        return QString();
    }

    if (type == "Caesar Cipher") {
        int key = ui->keySpinBox->value();
        return cipher.caesarDecrypt(text, key);
    }

    else if (type == "Substitution Cipher") {
        QString keyFilePath = getExecutablePath() + "/substitution_key.txt";
        QString substitutionKey = cipher.readSubstitutionKey(keyFilePath);
        return cipher.SubstitutionDecryption(text, substitutionKey);
    }

    else if (type == "Affine Cipher") {
        int a = ui->affine_a_SpinBox->value();
        if (gcd(a, 26) != 1) {
            QMessageBox::warning(this, "Input Error", "Key 'a' must be coprime with 26.");
            return QString();
        }
        int b = ui->affine_b_SpinBox->value();
        return cipher.affineDecrypt(text, a, b);
    }

    else if (type == "Vigenère Cipher") {
        QString key = ui->keyLineEdit->text();
        check_Exception_Key(key);
        return cipher.vigenereDecrypt(text, key);
    }

    else if (type == "Hill Cipher") {
        QString key = ui->hillMatrixTextEdit->toPlainText();
        check_Exception_Key(key);
        QVector<QVector<int>> keyMatrix = cipher.parseHillKey(key);
        return cipher.hillDecrypt(text, keyMatrix);
    }

    else if (type == "Permutation Cipher") {
        QString key = ui->keyLineEdit->text();
        check_Exception_Key(key);
        return cipher.decryptPermutationCipher(text, key);
    }

    else if (type == "Playfair Cipher") {
        QString keyText = ui->keyLineEdit->text();
        check_Exception_Key(keyText);
        QVector<QVector<QChar>> key = cipher.generatePlayfairTable(keyText);
        return cipher.playfairDecrypt(text, key);
    }

    else if (type == "Rail Fence Cipher") {
        int key = ui->keySpinBox->value();
        return cipher.railFenceDecrypt(text, key);
    }

    else if (type == "DES Cipher") {
        QString key = ui->keyLineEdit->text();
        check_Exception_Key(key);
        return cipher.desDecrypt(text, key);
    }

    else if (type == "AES Cipher") {
        QString key = ui->keyLineEdit->text();
        check_Exception_Key(key);
        return cipher.aesDecrypt(text, key);
    }

    return QString();
}

// Hàm mã hóa
void MainWindow::on_encryptButton_clicked() {
    QString text = ui->inputTextEdit->toPlainText();
    QString type = ui->encryptionTypeComboBox->currentText();
    QString encryptedText = encryptText(text, type);
    ui->encryptedTextEdit->setPlainText(encryptedText);
}

// Hàm giải mã
void MainWindow::on_decryptButton_clicked() {
    // QString text = ui->encryptedTextEdit->toPlainText();
    QString text = ui->inputTextEdit->toPlainText();
    QString type = ui->encryptionTypeComboBox->currentText();
    QString decryptedText = decryptText(text, type);
    ui->decryptedTextEdit->setPlainText(decryptedText);
}

// Hàm mở file
void MainWindow::on_browseFileTxtButton_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Text File", "", "Text Files (*.txt)");
    if (!fileName.isEmpty()) {
        QString text = readFile(fileName);
        ui->inputTextEdit->setPlainText(text);
        ui->filePathLabel->setText(fileName);
        currentFilePath = fileName;
        ui->saveEncryptedButton->setVisible(true);
        ui->saveDecryptedButton->setVisible(true);
        ui->reScanButton->setVisible(true);
    }
}

// Hàm đọc lại file
void MainWindow::on_reScanButton_clicked() {
    QString text = readFile(currentFilePath);
    ui->inputTextEdit->setPlainText(text);
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

void MainWindow::set_SpinBox(bool status){
    ui->keySpinBox->setVisible(status);
}

void MainWindow::set_LineEdit(bool status){
    ui->keyLineEdit->setVisible(status);
}

void MainWindow::set_Affine_Form(bool status){
    ui->label_affine_a->setVisible(status);
    ui->label_affine_b->setVisible(status);
    ui->affine_a_SpinBox->setVisible(status);
    ui->affine_b_SpinBox->setVisible(status);
}

void MainWindow::set_Hill_Form(bool status){
    ui->label_hill->setVisible(status);
    ui->hillMatrixTextEdit->setVisible(status);
}

void MainWindow::on_encryptionTypeComboBox_currentIndexChanged() {
    if (ui->encryptionTypeComboBox->currentText() == "Caesar Cipher") {
        ui->label_key->setVisible(true);
        set_SpinBox(true);
        set_LineEdit(false);
        set_Affine_Form(false);
        set_Hill_Form(false);
    } else if (ui->encryptionTypeComboBox->currentText() == "Substitution Cipher") {
        ui->label_key->setVisible(false);
        set_SpinBox(false);
        set_LineEdit(false);
        set_Affine_Form(false);
        set_Hill_Form(false);
    } else if (ui->encryptionTypeComboBox->currentText() == "Affine Cipher") {
        ui->label_key->setVisible(false);
        set_SpinBox(false);
        set_LineEdit(false);
        set_Affine_Form(true);
        set_Hill_Form(false);
    } else if (ui->encryptionTypeComboBox->currentText() == "Vigenère Cipher") {
        ui->label_key->setVisible(true);
        set_SpinBox(false);
        set_LineEdit(true);
        set_Affine_Form(false);
        set_Hill_Form(false);
    } else if (ui->encryptionTypeComboBox->currentText() == "Hill Cipher") {
        ui->label_key->setVisible(false);
        set_SpinBox(false);
        set_LineEdit(false);
        set_Affine_Form(false);
        set_Hill_Form(true);
    } else if (ui->encryptionTypeComboBox->currentText() == "Permutation Cipher"){
        ui->label_key->setVisible(true);
        set_SpinBox(false);
        set_LineEdit(true);
        set_Affine_Form(false);
        set_Hill_Form(false);
    } else if (ui->encryptionTypeComboBox->currentText() == "Playfair Cipher"){
        ui->label_key->setVisible(true);
        set_SpinBox(false);
        set_LineEdit(true);
        set_Affine_Form(false);
        set_Hill_Form(false);
    } else if (ui->encryptionTypeComboBox->currentText() == "Rail Fence Cipher"){
        ui->label_key->setVisible(true);
        set_SpinBox(true);
        set_LineEdit(false);
        set_Affine_Form(false);
        set_Hill_Form(false);
    } else if (ui->encryptionTypeComboBox->currentText() == "DES Cipher"){
        ui->label_key->setVisible(true);
        set_SpinBox(false);
        set_LineEdit(true);
        set_Affine_Form(false);
        set_Hill_Form(false);
    } else if (ui->encryptionTypeComboBox->currentText() == "AES Cipher"){
        ui->label_key->setVisible(true);
        set_SpinBox(false);
        set_LineEdit(true);
        set_Affine_Form(false);
        set_Hill_Form(false);
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
