#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QFile>
#include <algorithm>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <qnamespace.h>
#include <QRandomGenerator>
#include <QCoreApplication>
#include <QRegularExpression>

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

    ui->encryptionTypeComboBox->addItem("Caesar Cipher");
    ui->encryptionTypeComboBox->addItem("Substitution Cipher");
    ui->encryptionTypeComboBox->addItem("Affine Cipher");
    ui->encryptionTypeComboBox->addItem("Vigenère Cipher");
    ui->encryptionTypeComboBox->addItem("Hill Cipher");
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
        encryptedText = caesarEncrypt(text, key);
    } else if (ui->encryptionTypeComboBox->currentText() == "Substitution Cipher") {
        QString keyFilePath = getExecutablePath() + "/substitution_key.txt";
        QString substitutionKey;
        if (QFile::exists(keyFilePath)) {
            substitutionKey = readSubstitutionKey(keyFilePath);
        } else {
            substitutionKey = generateSubstitutionKey();
            saveSubstitutionKey(substitutionKey, keyFilePath);
        }
        encryptedText = SubstitutionEncryption(text, substitutionKey);
    } else if (ui->encryptionTypeComboBox->currentText() == "Affine Cipher") {
        int a = ui->affine_a_SpinBox->value();
        int b = ui->affine_b_SpinBox->value();
        encryptedText = affineEncrypt(text, a, b);
    } else if (ui->encryptionTypeComboBox->currentText() == "Vigenère Cipher") {
        QString key = ui->vigen_keyLineEdit->text();
        encryptedText = vigenereEncrypt(text, key);
    } else if (ui->encryptionTypeComboBox->currentText() == "Hill Cipher") {
        QString key = ui->hillMatrixTextEdit->toPlainText();
        QVector<QVector<int>> keyMatrix = parseHillKey(key);
        encryptedText = hillEncrypt(text, keyMatrix);
    }

    ui->encryptedTextEdit->setPlainText(encryptedText);
}

// Hàm giải mã
void MainWindow::on_decryptButton_clicked() {
    QString text = ui->inputTextEdit->toPlainText();
    QString decryptedText;
    if (ui->encryptionTypeComboBox->currentText() == "Caesar Cipher") {
        int key = ui->caesar_keySpinBox->value();
        decryptedText = caesarDecrypt(text, key);
    } else if (ui->encryptionTypeComboBox->currentText() == "Substitution Cipher") {
        QString keyFilePath = getExecutablePath() + "/substitution_key.txt";
        QString substitutionKey = readSubstitutionKey(keyFilePath);
        decryptedText = SubstitutionDecryption(text, substitutionKey);
    } else if (ui->encryptionTypeComboBox->currentText() == "Affine Cipher") {
        int a = ui->affine_a_SpinBox->value();
        int b = ui->affine_b_SpinBox->value();
        decryptedText = affineDecrypt(text, a, b);
    } else if (ui->encryptionTypeComboBox->currentText() == "Vigenère Cipher") {
        QString key = ui->vigen_keyLineEdit->text();
        decryptedText = vigenereDecrypt(text, key);
    } else if (ui->encryptionTypeComboBox->currentText() == "Hill Cipher") {
        QString key = ui->hillMatrixTextEdit->toPlainText();
        QVector<QVector<int>> keyMatrix = parseHillKey(key);
        decryptedText = hillDecrypt(text, keyMatrix);
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

// Hàm xử lý sự kiện khi thay đổi loại mã hóa
void MainWindow::on_encryptionTypeComboBox_currentIndexChanged() {
    if (ui->encryptionTypeComboBox->currentText() == "Caesar Cipher") {
        ui->label_caesar->setVisible(true);
        ui->caesar_keySpinBox->setVisible(true);
        ui->label_affine_a->setVisible(false);
        ui->label_affine_b->setVisible(false);
        ui->affine_a_SpinBox->setVisible(false);
        ui->affine_b_SpinBox->setVisible(false);
        ui->label_vigen->setVisible(false);
        ui->vigen_keyLineEdit->setVisible(false);
        ui->label_hill->setVisible(false);
        ui->hillMatrixTextEdit->setVisible(false);
    } else if (ui->encryptionTypeComboBox->currentText() == "Substitution Cipher") {
        ui->label_caesar->setVisible(false);
        ui->caesar_keySpinBox->setVisible(false);
        ui->label_affine_a->setVisible(false);
        ui->label_affine_b->setVisible(false);
        ui->affine_a_SpinBox->setVisible(false);
        ui->affine_b_SpinBox->setVisible(false);
        ui->label_vigen->setVisible(false);
        ui->vigen_keyLineEdit->setVisible(false);
        ui->label_hill->setVisible(false);
        ui->hillMatrixTextEdit->setVisible(false);
    } else if (ui->encryptionTypeComboBox->currentText() == "Affine Cipher") {
        ui->label_caesar->setVisible(false);
        ui->caesar_keySpinBox->setVisible(false);
        ui->label_affine_a->setVisible(true);
        ui->label_affine_b->setVisible(true);
        ui->affine_a_SpinBox->setVisible(true);
        ui->affine_b_SpinBox->setVisible(true);
        ui->label_vigen->setVisible(false);
        ui->vigen_keyLineEdit->setVisible(false);
        ui->label_hill->setVisible(false);
        ui->hillMatrixTextEdit->setVisible(false);
    } else if (ui->encryptionTypeComboBox->currentText() == "Vigenère Cipher") {
        ui->label_caesar->setVisible(false);
        ui->caesar_keySpinBox->setVisible(false);
        ui->label_affine_a->setVisible(false);
        ui->label_affine_b->setVisible(false);
        ui->affine_a_SpinBox->setVisible(false);
        ui->affine_b_SpinBox->setVisible(false);
        ui->label_vigen->setVisible(true);
        ui->vigen_keyLineEdit->setVisible(true);
        ui->label_hill->setVisible(false);
        ui->hillMatrixTextEdit->setVisible(false);
    } else if (ui->encryptionTypeComboBox->currentText() == "Hill Cipher") {
        ui->label_caesar->setVisible(false);
        ui->caesar_keySpinBox->setVisible(false);
        ui->label_affine_a->setVisible(false);
        ui->label_affine_b->setVisible(false);
        ui->affine_a_SpinBox->setVisible(false);
        ui->affine_b_SpinBox->setVisible(false);
        ui->label_vigen->setVisible(false);
        ui->vigen_keyLineEdit->setVisible(false);
        ui->label_hill->setVisible(true);
        ui->hillMatrixTextEdit->setVisible(true);
    }
}

/*
******************************************
Các hàm mã hóa và giải mã
******************************************
*/

/*
******************************************
Mã hóa và giải mã Caesar
******************************************
*/

// Hàm mã hóa Caesar
QString MainWindow::caesarEncrypt(const QString &input, int key) {
    QString result;
    for (QChar c : input) {
        if (c.isLetter()) {
            QChar base = c.isLower() ? 'a' : 'A';
            c = QChar((c.unicode() - base.unicode() + key) % 26 + base.unicode());
        }
        result += c;
    }
    return result;
}

// Hàm giải mã Caesar
QString MainWindow::caesarDecrypt(const QString &input, int key) {
    return caesarEncrypt(input, 26 - key);
}

/*
******************************************
Mã hóa và giải mã Substitution
******************************************
*/

// Hàm tạo khóa thay thế
QString MainWindow::generateSubstitutionKey() {
    QString alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    QString key = alphabet;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(key.begin(), key.end(), g);
    return key;
}

// Hàm lưu khóa thay thế
void MainWindow::saveSubstitutionKey(const QString &key, const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cannot write to file: " + fileName);
        return;
    }
    QTextStream out(&file);
    out << key;
}

// Hàm đọc khóa thay thế
QString MainWindow::readSubstitutionKey(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cannot open file: " + fileName);
        return "";
    }
    QTextStream in(&file);
    return in.readAll();
}

// Hàm mã hóa Substitution
QString MainWindow::SubstitutionEncryption(const QString &input, const QString &key) {
    QString result;
    QString alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (QChar c : input) {
        if (c.isLetter()) {
            int index = alphabet.indexOf(c.toUpper());
            if (index != -1) {
                QChar encryptedChar = key[index];
                if (c.isLower()) {
                    encryptedChar = encryptedChar.toLower();
                }
                result += encryptedChar;
            }
        } else {
            result += c;
        }
    }
    return result;
}

// Hàm giải mã Substitution
QString MainWindow::SubstitutionDecryption(const QString &input, const QString &key) {
    QString result;
    QString alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (QChar c : input) {
        if (c.isLetter()) {
            int index = key.indexOf(c.toUpper());
            if (index != -1) {
                QChar decryptedChar = alphabet[index];
                if (c.isLower()) {
                    decryptedChar = decryptedChar.toLower();
                }
                result += decryptedChar;
            }
        } else {
            result += c;
        }
    }
    return result;
}

/*
******************************************
Mã hóa và giải mã Affine
******************************************
*/

// Hàm mã hóa affine
QString MainWindow::affineEncrypt(const QString &text, int a, int b) {
    QString result;
    for (QChar c : text) {
        if (c.isLetter()) {
            QChar base = c.isUpper() ? 'A' : 'a';
            result.append(QChar((a * (c.unicode() - base.unicode()) + b) % 26 + base.unicode()));
        } else {
            result.append(c);
        }
    }
    return result;
}

// Hàm giải mã affine
QString MainWindow::affineDecrypt(const QString &text, int a, int b) {
    QString result;
    int a_inv = 0;
    int flag = 0;

    for (int i = 0; i < 26; i++) {
        flag = (a * i) % 26;
        if (flag == 1) {
            a_inv = i;
            break;
        }
    }

    for (QChar c : text) {
        if (c.isLetter()) {
            QChar base = c.isUpper() ? 'A' : 'a';
            result.append(QChar((a_inv * ((c.unicode() - base.unicode() - b + 26)) % 26) + base.unicode()));
        } else {
            result.append(c);
        }
    }
    return result;
}

/*
******************************************
Mã hóa và giải mã Vigenère
******************************************
*/

// Hàm mã hóa Vigenère
QString MainWindow::vigenereEncrypt(const QString &text, const QString &key) {
    QString result;
    int keyIndex = 0;
    QList<int> numericKey;

    if (key.contains(QRegularExpression("[0-9]"))) {
        QStringList keyParts = key.split(QRegularExpression("[,\\s]+"), Qt::SkipEmptyParts);
        for (const QString &part : keyParts) {
            numericKey.append(part.toInt());
        }
    }

    int n = key.length();
    QString paddedText = text;
    while (paddedText.length() % n != 0) {
        paddedText.append('Z');
    }

    for (QChar c : text) {
        if (c.isLetter()) {
            QChar base = c.isUpper() ? 'A' : 'a';
            int shift;
            if (!numericKey.isEmpty()) {
                shift = numericKey[keyIndex % numericKey.length()];
            } else {
                shift = key[keyIndex % key.length()].toUpper().unicode() - 'A';
            }
            result.append(QChar((c.unicode() - base.unicode() + shift) % 26 + base.unicode()));
            keyIndex++;
        } else {
            result.append(c);
        }
    }
    return result;
}

// Hàm giải mã Vigenère
QString MainWindow::vigenereDecrypt(const QString &text, const QString &key) {
    QString result;
    int keyIndex = 0;
    QList<int> numericKey;

    if (key.contains(QRegularExpression("[0-9]"))) {
        QStringList keyParts = key.split(QRegularExpression("[,\\s]+"), Qt::SkipEmptyParts);
        for (const QString &part : keyParts) {
            numericKey.append(part.toInt());
        }
    }

    for (QChar c : text) {
        if (c.isLetter()) {
            QChar base = c.isUpper() ? 'A' : 'a';
            int shift;
            if (!numericKey.isEmpty()) {
                shift = numericKey[keyIndex % numericKey.length()];
            } else {
                shift = key[keyIndex % key.length()].toUpper().unicode() - 'A';
            }
            result.append(QChar((c.unicode() - base.unicode() - shift + 26) % 26 + base.unicode()));
            keyIndex++;
        } else {
            result.append(c);
        }
    }
    return result;
}

/*
******************************************
Mã hóa và giải mã Hill
******************************************
*/

// Hàm phân tích khóa Hill
QVector<QVector<int>> MainWindow::parseHillKey(const QString &key) {
    QVector<QVector<int>> keyMatrix;
    QStringList rows = key.split(QRegularExpression("[;\\n]+"), Qt::SkipEmptyParts);
    for (const QString &row : rows) {
        QStringList elements = row.split(QRegularExpression("[,\\s]+"), Qt::SkipEmptyParts);
        QVector<int> rowVector;
        for (const QString &element : elements) {
            rowVector.append(element.toInt());
        }
        keyMatrix.append(rowVector);
    }
    return keyMatrix;
}

// Hàm mã hóa Hill
QString MainWindow::hillEncrypt(const QString &text, const QVector<QVector<int>> &keyMatrix) {
    QString result;
    int n = keyMatrix.size();
    QVector<int> textVector(n);

    QString paddedText = "";
    for(QChar c : text) {
        if (c.isLetter()) {
            paddedText.append(c);
        }
    }
    
    while (paddedText.length() % n != 0) {
        paddedText.append('Z');
    }

    for (int i = 0; i < paddedText.length(); i += n) {
        for (int j = 0; j < n; ++j) {
            QChar base = paddedText[i + j].isUpper() ? 'A' : 'a';
            textVector[j] = (paddedText[i + j].unicode() - base.unicode()) % 26;
        }

        for (int j = 0; j < n; ++j) {
            int sum = 0;
            for (int k = 0; k < n; ++k) {
                sum += keyMatrix[k][j] * textVector[k];
            }
            result.append(QChar((sum % 26 + 26) % 26 + 'A'));
        }
    }

    return result;
}

// Hàm tìm nghịch đảo modulo
int modInverse(int a, int mod) {
    a = a % mod;
    for (int x = 1; x < mod; x++) {
        if ((a * x) % mod == 1) {
            return x;
        }
    }
    return 1; // Trường hợp không tìm thấy nghịch đảo, trả về 1
}

// Hàm tính định thức ma trận
int determinant(const QVector<QVector<int>> &matrix, int n) {
    int det = 0;
    if (n == 1) {
        return matrix[0][0];
    }
    if (n == 2) {
        return (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]) % 26;
    }
    QVector<QVector<int>> submatrix(n, QVector<int>(n));
    for (int x = 0; x < n; x++) {
        int subi = 0;
        for (int i = 1; i < n; i++) {
            int subj = 0;
            for (int j = 0; j < n; j++) {
                if (j == x) {
                    continue;
                }
                submatrix[subi][subj] = matrix[i][j];
                subj++;
            }
            subi++;
        }
        int sign = (x % 2 == 0) ? 1 : -1;
        det = (det + sign * matrix[0][x] * determinant(submatrix, n -1)) % 26;
    }
    return (det + 26) % 26;
}

QVector<QVector<int>> getMinorMatrix(const QVector<QVector<int>>& matrix, int p, int q, int n){
    QVector<QVector<int>> submatrix(n - 1, QVector<int>(n - 1));
    int subi = 0;
    for(int i = 0; i < n; i++){
        if(i == p) continue;
        int subj = 0;
        for(int j = 0; j < n; j++){
            if(j == q) continue;
            submatrix[subi][subj] = matrix[i][j];
            subj++;
        }
        subi++;
    }
    return submatrix;
}

QVector<QVector<int>> cofactorMatrix(const QVector<QVector<int>>& matrix, int n){
    QVector<QVector<int>> cofactor(n, QVector<int>(n));
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            QVector<QVector<int>> minor = getMinorMatrix(matrix, i, j, n);
            int sign = ((i + j) % 2 == 0) ? 1 : -1;
            cofactor[i][j] = sign * determinant(minor, n - 1);
            cofactor[i][j] = (cofactor[i][j] % 26 + 26) % 26;
        }
    }
    return cofactor;
}

QVector<QVector<int>> transpose(const QVector<QVector<int>>& matrix, int n){
    QVector<QVector<int>> transposed(n, QVector<int>(n));
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            transposed[i][j] = matrix[j][i];
        }
    }
    return transposed;
}

// Hàm tìm ma trận nghịch đảo
QVector<QVector<int>> inverseMatrix(const QVector<QVector<int>> &matrix, int n) {
    int det = determinant(matrix, n);
    int detInverse = modInverse(det, 26);
    if (detInverse == -1){
        return {{}};
    }

    QVector<QVector<int>> cofactor = cofactorMatrix(matrix, n);
    QVector<QVector<int>> adjugate = transpose(cofactor, n);

    QVector<QVector<int>> inverse(n, QVector<int>(n));
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            inverse[i][j] = (adjugate[i][j] * detInverse) % 26;
            if (inverse[i][j] < 0)
                inverse[i][j] += 26;
        }
    }
    return inverse;
}

// Hàm giải mã Hill
QString MainWindow::hillDecrypt(const QString &text, const QVector<QVector<int>> &keyMatrix) {
    QString result;
    int n = keyMatrix.size();
    QVector<int> textVector(n);

    QVector<QVector<int>> invKeyMatrix = inverseMatrix(keyMatrix, n);

    if (invKeyMatrix.isEmpty()) {
        QMessageBox::warning(this, "Error", "The key matrix is not invertible.");
        return "";
    }

    for (int i = 0; i < text.length(); i += n) {
        for (int j = 0; j < n; ++j) {
            QChar base = text[i + j].isUpper() ? 'A' : 'a';
            textVector[j] = (text[i + j].unicode() - base.unicode()) % 26;
        }

        for (int j = 0; j < n; ++j) {
            int sum = 0;
            for (int k = 0; k < n; ++k) {
                sum += invKeyMatrix[k][j] * textVector[k];
            }
            result.append(QChar((sum % 26 + 26) % 26 + 'A'));
        }
    }

    return result;
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
