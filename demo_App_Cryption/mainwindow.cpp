#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Cho phép kéo và thả
    setAcceptDrops(true);

    // Kết nối các tín hiệu nút với các khe tương ứng (chức năng)
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

    // Tạo danh sách các loại mã hóa
    QVector<QString> items = {"Caesar Cipher", "Substitution Cipher", "Affine Cipher", "Vigenère Cipher",
                              "Hill Cipher", "Permutation Cipher", "Playfair Cipher", "Rail Fence Cipher",
                              "DES Cipher", "AES Cipher"};

    // Thêm các mục vào hộp kết hợp cho các loại mã hóa
    for(const auto& item : items){
        ui->encryptionTypeComboBox->addItem(item);
    }

    // Đặt chỉnh sửa văn bản được mã hóa và giải mã thành chỉ đọc
    ui->encryptedTextEdit->setReadOnly(true);
    ui->decryptedTextEdit->setReadOnly(true);

    // Ẩn các nút cụ thể ban đầu
    ui->saveEncryptedButton->setVisible(false);
    ui->saveDecryptedButton->setVisible(false);
    ui->reScanButton->setVisible(false);

    // Kết nối tín hiệu để theo dõi các thay đổi văn bản trong các bản chỉnh sửa văn bản
    connect(ui->inputTextEdit, &QTextEdit::textChanged, this, &MainWindow::checkTextEdits);
    connect(ui->encryptedTextEdit, &QTextEdit::textChanged, this, &MainWindow::checkTextEdits);
    connect(ui->decryptedTextEdit, &QTextEdit::textChanged, this, &MainWindow::checkTextEdits);

    // Thêm thuật toán vào hộp kết hợp cho thuật toán mã hóa/giải mã
    ui->algorithmComboBox->addItem("DES Cipher");
    ui->algorithmComboBox->addItem("AES Cipher");
}

MainWindow::~MainWindow() {
    delete ui;
}

/*
* ********************************************
* Các hàm chuyển đổi Widget
* ********************************************
*/

// Chuyển đổi sang Page Import
void MainWindow::showImportOptions()
{
    ui->stackedWidget->setCurrentWidget(ui->pageImport);
}

// Chuyển đổi sang Page Text
void MainWindow::showTextEntry()
{
    ui->stackedWidget->setCurrentWidget(ui->pageText);
}

// Chuyển đổi sang Page File
void MainWindow::showFileOptions()
{
    ui->stackedWidget->setCurrentWidget(ui->pageFile);
}

// Chuyển đổi về Page Home
void MainWindow::backToHome()
{
    ui->stackedWidget->setCurrentWidget(ui->pageHome);
}

// Chuyển đổi về Page Import
void MainWindow::backToImport()
{
    ui->stackedWidget->setCurrentWidget(ui->pageImport);
}

/*
* ********************************************
* Các hàm xử lý đến file
* ********************************************
*/

// Hàm duyệt file - dùng để chọn file để mã hóa và giải mã
void MainWindow::browseFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("All Files (*)"));
    if (!filePath.isEmpty()) {
        ui->inputPathLineEdit->setText(filePath);
        ui->stackedWidget->setCurrentWidget(ui->pageFile);
    }
}

void MainWindow::on_file_browseInputButton_clicked(){
    QString filePath = QFileDialog::getOpenFileName(this, tr("Save File"), "", tr("All Files (*)"));
    if (!filePath.isEmpty()) {
        ui->inputPathLineEdit->setText(filePath);
    }
}

// Hàm duyệt file - dùng để chọn nơi lưu file sau khi mã hóa hoặc giải mã.
void MainWindow::on_file_browseOutputButton_clicked() {
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

// Hàm duyệt file cần mã hóa
void MainWindow::importEncryptFile()
{
    browseFile();
    QString inputFilePath = ui->inputPathLineEdit->text();
    ui->outputPathLineEdit->setText(inputFilePath + ".dat");
    ui->startButton->setProperty("mode", "encrypt");
}

// Hàm duyệt các file đã mã hóa
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

// Hàm xử lý mã hóa và giải mã
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

// Hàm xử lý kéo file
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

// Hàm xử lý thả file
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

// Hàm xử lý sự kiện khi nhấn nút - Dùng trong Page Text
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

// Hàm đọc lại file - Dùng trong Page Text - Dùng sau khi thực hiện lưu file
void MainWindow::on_reScanButton_clicked() {
    QString text = readFile(currentFilePath);
    ui->inputTextEdit->setPlainText(text);
}

// Hàm lưu kết quả mã hóa vào file - Dùng trong Page Text
void MainWindow::on_saveEncryptedButton_clicked() {
    if (currentFilePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No file selected!");
        return;
    }
    QString encryptedText = ui->encryptedTextEdit->toPlainText();
    writeFile(currentFilePath, encryptedText);
    QMessageBox::information(this, "Success", "Encrypted text saved to file.");
}

// Hàm lưu kết quả giải mã vào file - Dùng trong Page Text
void MainWindow::on_saveDecryptedButton_clicked() {
    if (currentFilePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No file selected!");
        return;
    }
    QString decryptedText = ui->decryptedTextEdit->toPlainText();
    writeFile(currentFilePath, decryptedText);
    QMessageBox::information(this, "Success", "Decrypted text saved to file.");
}

/*
* ********************************************
* Các hàm xử lý sự kiện
* ********************************************
*/

// Hàm kiểm tra ngoại lệ của khóa
void MainWindow::check_Exception_Key(const QString& key){
    if (key.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please provide cipherkey.");
    }
}

// Hàm tìm UCLN - Hỗ trợ Affine Cipher
int gcd(int a, int b) {
    if (b == 0) {
        return a;
    }
    return gcd(b, a % b);
}

// Hàm mã hóa text chung
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
        return cipher.SubstitutionEncryption(text);
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

// Hàm giải mã text chung
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
        return cipher.SubstitutionDecryption(text);
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

// Hàm xử lý sự kiện khi nhấn nút mã hóa
void MainWindow::on_encryptButton_clicked() {
    QString text = ui->inputTextEdit->toPlainText();
    QString type = ui->encryptionTypeComboBox->currentText();
    QString encryptedText = encryptText(text, type);
    ui->encryptedTextEdit->setPlainText(encryptedText);
}

// Hàm xử lý sự kiện khi nhấn nút giải mã
void MainWindow::on_decryptButton_clicked() {
    // QString text = ui->encryptedTextEdit->toPlainText();
    QString text = ui->inputTextEdit->toPlainText();
    QString type = ui->encryptionTypeComboBox->currentText();
    QString decryptedText = decryptText(text, type);
    ui->decryptedTextEdit->setPlainText(decryptedText);
}

// Hàm kiểm tra các ô nhập liệu - Dùng trong Page Text
void MainWindow::checkTextEdits() {
    bool enableSave = !ui->inputTextEdit->toPlainText().isEmpty() &&
                      (!ui->encryptedTextEdit->toPlainText().isEmpty() ||
                       !ui->decryptedTextEdit->toPlainText().isEmpty());
    ui->saveEncryptedButton->setEnabled(enableSave);
    ui->saveDecryptedButton->setEnabled(enableSave);
}

/*
* ********************************************
* Các hàm xử lý sự kiện khi thay đổi loại mã hóa
* ********************************************
*/

// Hàm phụ trách Spin Box
void MainWindow::set_SpinBox(bool status){
    ui->keySpinBox->setVisible(status);
}

// Hàm phụ trách Line Edit
void MainWindow::set_LineEdit(bool status){
    ui->keyLineEdit->setVisible(status);
}

// Hàm phụ trách các đối tượng liên quan đến Affine Cipher
void MainWindow::set_Affine_Form(bool status){
    ui->label_affine_a->setVisible(status);
    ui->label_affine_b->setVisible(status);
    ui->affine_a_SpinBox->setVisible(status);
    ui->affine_b_SpinBox->setVisible(status);
}

// Hàm phụ trách các đối tượng liên quan đến Hill Cipher
void MainWindow::set_Hill_Form(bool status){
    ui->label_hill->setVisible(status);
    ui->hillMatrixTextEdit->setVisible(status);
}

// Hàm set visible khi thay đổi phương thức mã hóa
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
