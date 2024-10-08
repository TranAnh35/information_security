#include "Cipher.h"

/*
-------------------------------------------------------------------------------------------------------
* *****************************************************************************************************
* Mã hóa và giải mã Page Text
* *****************************************************************************************************
-------------------------------------------------------------------------------------------------------
*/

/*
* ****************************************
* Mã hóa và giải mã Caesar
* ****************************************
*/

// Hàm mã hóa Caesar
QString Cipher::caesarEncrypt(const QString &input, int key) {
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
QString Cipher::caesarDecrypt(const QString &input, int key) {
    return caesarEncrypt(input, 26 - key);
}

/*
* ****************************************
* Mã hóa và giải mã Substitution
* ****************************************
*/

// Hàm tạo khóa thay thế
QString Cipher::generateSubstitutionKey() {
    QString alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    QString key = alphabet;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(key.begin(), key.end(), g);
    return key;
}

QString Cipher::checkSubstitutionKey(){
    QString keyFilePath = QCoreApplication::applicationDirPath() + "/substitution_key.txt";
    QString key;
    if (QFile::exists(keyFilePath)) {
        key = QString::fromUtf8(loadKeyFromFile(keyFilePath));
    } else {
        key = generateSubstitutionKey();
        saveKeyToFile(keyFilePath, key.toUtf8());
    }
    return key;
}

// Hàm mã hóa Substitution
QString Cipher::SubstitutionEncryption(const QString &input) {
    QString result;
    QString alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    QString key = checkSubstitutionKey();

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
QString Cipher::SubstitutionDecryption(const QString &input) {
    QString result;
    QString alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    QString key = checkSubstitutionKey();

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
* ****************************************
* Mã hóa và giải mã Affine
* ****************************************
*/

// Hàm mã hóa affine
QString Cipher::affineEncrypt(const QString &text, int a, int b) {
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
QString Cipher::affineDecrypt(const QString &text, int a, int b) {
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
* ****************************************
* Mã hóa và giải mã Vigenère
* ****************************************
*/

// Hàm mã hóa Vigenère
QString Cipher::vigenereEncrypt(const QString &text, const QString &key) {
    QString result;
    int keyIndex = 0;
    QList<int> numericKey;

    static const QRegularExpression digitPattern("[0-9]");
    static const QRegularExpression splitPattern("[,\\s]+");

    if (key.contains(digitPattern)) {
        QStringList keyParts = key.split(splitPattern, Qt::SkipEmptyParts);
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
QString Cipher::vigenereDecrypt(const QString &text, const QString &key) {
    QString result;
    int keyIndex = 0;
    QList<int> numericKey;

    static const QRegularExpression digitPattern("[0-9]");
    static const QRegularExpression splitPattern("[,\\s]+");

    if (key.contains(digitPattern)) {
        QStringList keyParts = key.split(splitPattern, Qt::SkipEmptyParts);
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
* ****************************************
* Mã hóa và giải mã Hill
* ****************************************
*/

// Hàm phân tích khóa Hill
QVector<QVector<int>> Cipher::parseHillKey(const QString &key) {
    QVector<QVector<int>> keyMatrix;
    static const QRegularExpression rowPattern("[;\\n]+");
    static const QRegularExpression elementPattern("[,\\s]+");

    QStringList rows = key.split(rowPattern, Qt::SkipEmptyParts);
    for (const QString &row : rows) {
        QStringList elements = row.split(elementPattern, Qt::SkipEmptyParts);
        QVector<int> rowVector;
        for (const QString &element : elements) {
            rowVector.append(element.toInt());
        }
        keyMatrix.append(rowVector);
    }
    return keyMatrix;
}

// Hàm mã hóa Hill
QString Cipher::hillEncrypt(const QString &text, const QVector<QVector<int>> &keyMatrix) {
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

// Hàm lấy ma trận con
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

// Hàm tạo ma trận phụ hợp
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

// Hàm chuyển vị ma trận
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
QString Cipher::hillDecrypt(const QString &text, const QVector<QVector<int>> &keyMatrix) {
    QString result;
    int n = keyMatrix.size();
    QVector<int> textVector(n);

    QVector<QVector<int>> invKeyMatrix = inverseMatrix(keyMatrix, n);

    if (invKeyMatrix.isEmpty()) {
        QMessageBox::warning(nullptr, "Error", "The key matrix is not invertible.");
        // qDebug() << "The key matrix is not invertible.";
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

/*
* ****************************************
* Mã hóa và giải mã Permutation
* ****************************************
*/

// Hàm mã hóa Permutation
QString Cipher::encryptPermutationCipher(const QString &text, const QString &key) {
    QString result;
    int keyLength = key.length();

    // Kiểm tra nếu khóa không hợp lệ (không chứa các chữ số duy nhất từ 1 đến keyLength)
    QVector<bool> seen(keyLength, false);
    for (int i = 0; i < keyLength; ++i) {
        int digit = key[i].digitValue();
        if (digit < 1 || digit > keyLength || seen[digit - 1]) {
            return "Invalid key!";  // Trả về lỗi nếu khóa không hợp lệ
        }
        seen[digit - 1] = true;
    }

    QString paddedText = text;
    // Thêm đệm nếu độ dài văn bản không phải là bội số của độ dài khóa
    while (paddedText.length() % keyLength != 0) {
        paddedText.append('Z');  // Thêm 'X' làm ký tự đệm
    }

    for (int i = 0; i < paddedText.length(); i += keyLength) {
        QString chunk = paddedText.mid(i, keyLength);  // Lấy một đoạn văn bản
        QString encryptedChunk(keyLength, ' ');  // Khởi tạo một đoạn trống
        for (int j = 0; j < chunk.length(); ++j) {
            encryptedChunk[j] = chunk[key[j].digitValue() - 1];  // Sắp xếp lại các ký tự
        }
        result += encryptedChunk;
    }
    return result;
}

// Hàm giải mã Permutation
QString Cipher::decryptPermutationCipher(const QString &text, const QString &key) {
    QString result;
    int keyLength = key.length();

    // Kiểm tra nếu khóa không hợp lệ
    QVector<bool> seen(keyLength, false);
    for (int i = 0; i < keyLength; ++i) {
        int digit = key[i].digitValue();
        if (digit < 1 || digit > keyLength || seen[digit - 1]) {
            return "Invalid key!";  // Trả về lỗi nếu khóa không hợp lệ
        }
        seen[digit - 1] = true;
    }

    for (int i = 0; i < text.length(); i += keyLength) {
        QString chunk = text.mid(i, keyLength);
        QString decryptedChunk(keyLength, ' ');
        for (int j = 0; j < chunk.length(); ++j) {
            decryptedChunk[key[j].digitValue() - 1] = chunk[j];  // Đưa về vị trí ban đầu
        }
        result += decryptedChunk;
    }

    // Loại bỏ các ký tự đệm ('Z') đã thêm trong quá trình mã hóa
    result = result.trimmed();
    result = result.remove('Z');

    return result;
}

/*
* ****************************************
* Mã hóa và giải mã Playfair
* ****************************************
*/

// Hàm sinh ma trận khóa mã hóa
QVector<QVector<QChar>> Cipher::generatePlayfairTable(const QString &key) {
    QString alphabet = "ABCDEFGHIKLMNOPQRSTUVWXYZ";
    QString processedKey = key.toUpper();
    static const QRegularExpression nonAlphaPattern("[^A-Z]");
    processedKey = processedKey.remove(nonAlphaPattern);
    processedKey = processedKey.remove('J');

    QVector<QVector<QChar>> table(5, QVector<QChar>(5));
    QSet<QChar> usedChars;

    int index = 0;
    for (const QChar& c : processedKey) {
        if (!usedChars.contains(c)) {
            table[index / 5][index % 5] = c;
            usedChars.insert(c);
            index++;
        }
    }

    for (QChar c : alphabet) {
        if (!usedChars.contains(c)) {
            table[index / 5][index % 5] = c;
            usedChars.insert(c);
            index++;
        }
    }

    return table;
}

// Hàm mã hóa Playfair
QString Cipher::playfairEncrypt(const QString &text, const QVector<QVector<QChar>> &table) {
    QString input = text.toUpper();
    static const QRegularExpression nonAlphaPattern("[^A-Z]");
    input = input.replace("J", "I").remove(nonAlphaPattern);

    QString result;
    for (int i = 0; i < input.length(); i += 2) {
        if (i + 1 == input.length() || input[i] == input[i + 1]) {
            input.insert(i + 1, 'Z'); // Add padding if needed
        }

        QChar first = input[i];
        QChar second = input[i + 1];

        QPoint firstPos, secondPos;
        for (int row = 0; row < 5; ++row) {
            for (int col = 0; col < 5; ++col) {
                if (table[row][col] == first) {
                    firstPos = {row, col};
                }
                if (table[row][col] == second) {
                    secondPos = {row, col};
                }
            }
        }

        if (firstPos.x() == secondPos.x()) {
            // Same row
            result += table[firstPos.x()][(firstPos.y() + 1) % 5];
            result += table[secondPos.x()][(secondPos.y() + 1) % 5];
        } else if (firstPos.y() == secondPos.y()) {
            // Same column
            result += table[(firstPos.x() + 1) % 5][firstPos.y()];
            result += table[(secondPos.x() + 1) % 5][secondPos.y()];
        } else {
            // Rectangle swap
            result += table[firstPos.x()][secondPos.y()];
            result += table[secondPos.x()][firstPos.y()];
        }
    }

    return result;
}

// Hàm giải mã Playfair
QString Cipher::playfairDecrypt(const QString &text, const QVector<QVector<QChar>> &table) {
    QString input = text.toUpper();

    QString result;
    for (int i = 0; i < input.length(); i += 2) {
        QChar first = input[i];
        QChar second = input[i + 1];

        QPoint firstPos, secondPos;
        for (int row = 0; row < 5; ++row) {
            for (int col = 0; col < 5; ++col) {
                if (table[row][col] == first) {
                    firstPos = {row, col};
                }
                if (table[row][col] == second) {
                    secondPos = {row, col};
                }
            }
        }

        if (firstPos.x() == secondPos.x()) {
            // Same row
            result += table[firstPos.x()][(firstPos.y() + 4) % 5];
            result += table[secondPos.x()][(secondPos.y() + 4) % 5];
        } else if (firstPos.y() == secondPos.y()) {
            // Same column
            result += table[(firstPos.x() + 4) % 5][firstPos.y()];
            result += table[(secondPos.x() + 4) % 5][secondPos.y()];
        } else {
            // Rectangle swap
            result += table[firstPos.x()][secondPos.y()];
            result += table[secondPos.x()][firstPos.y()];
        }
    }
    result = result.trimmed();
    result = result.remove('Z');
    return result;
}

/*
* ****************************************
* Mã hóa và giải mã Rail Fence
* ****************************************
*/

// Hàm mã hóa Rail Fence
QString Cipher::railFenceEncrypt(const QString &text, int key) {
    if (key <= 1) return text; // Không cần mã hóa nếu chỉ có 1 hàng

    QVector<QString> rails(key);
    bool directionDown = false;
    int row = 0;

    for (const QChar &c : text) {
        rails[row] += c;

        if (row == 0 || row == key - 1) {
            directionDown = !directionDown;
        }

        row += directionDown ? 1 : -1;
    }

    QString result;
    for (const QString &rail : rails) {
        result += rail;
    }

    return result;
}

// Hàm giải mã Rail Fence
QString Cipher::railFenceDecrypt(const QString &cipherText, int key) {
    if (key <= 1) return cipherText; // Không cần giải mã nếu chỉ có 1 hàng

    QVector<QString> rails(key);
    QVector<int> railLengths(key, 0);
    bool directionDown = false;
    int row = 0;

    // Đếm số ký tự trên mỗi hàng
    for (const QChar &c : cipherText) {
        railLengths[row]++;
        if (row == 0 || row == key - 1) {
            directionDown = !directionDown;
        }
        row += directionDown ? 1 : -1;
    }

    // Gán ký tự cho mỗi hàng
    int index = 0;
    for (int i = 0; i < key; ++i) {
        rails[i] = cipherText.mid(index, railLengths[i]);
        index += railLengths[i];
    }

    // Ghép các ký tự theo thứ tự zigzag
    QString result;
    row = 0;
    directionDown = false;
    QVector<int> railIndex(key, 0);

    for (int i = 0; i < cipherText.length(); ++i) {
        result += rails[row][railIndex[row]++];

        if (row == 0 || row == key - 1) {
            directionDown = !directionDown;
        }
        row += directionDown ? 1 : -1;
    }

    return result;
}

/*
* ****************************************
* Mã hóa và giải mã DES
* ****************************************
*/

// Hàm mã hóa DES
QString Cipher::desEncrypt(const QString &plaintext, const QString &key) {
    gcry_cipher_hd_t handle;
    gcry_error_t err;

    QByteArray keyBytes = key.toUtf8();
    QByteArray plainBytes = plaintext.toUtf8();

    // Add PKCS7 padding
    int paddingSize = 8 - (plainBytes.size() % 8);
    plainBytes.append(paddingSize, static_cast<char>(paddingSize));

    QByteArray cipherBytes(plainBytes.size(), 0);

    // Ensure the key is 8 bytes long
    if (keyBytes.size() < 8) {
        keyBytes.resize(8, '0');
    }

    // Open cipher
    err = gcry_cipher_open(&handle, GCRY_CIPHER_DES, GCRY_CIPHER_MODE_ECB, 0);
    if (err) {
        return QString("Encryption Error: %1").arg(gcry_strerror(err));
    }

    // Set key
    err = gcry_cipher_setkey(handle, keyBytes.data(), keyBytes.size());
    if (err) {
        gcry_cipher_close(handle);
        return QString("Encryption Error: %1").arg(gcry_strerror(err));
    }

    // Encrypt
    err = gcry_cipher_encrypt(handle, cipherBytes.data(), cipherBytes.size(), plainBytes.data(), plainBytes.size());
    if (err) {
        gcry_cipher_close(handle);
        return QString("Encryption Error: %1").arg(gcry_strerror(err));
    }

    gcry_cipher_close(handle);

    return QString(cipherBytes.toHex());
}

// Hàm giải mã DES
QString Cipher::desDecrypt(const QString &ciphertext, const QString &key) {
    gcry_cipher_hd_t handle;
    gcry_error_t err;

    QByteArray keyBytes = key.toUtf8();
    QByteArray cipherBytes = QByteArray::fromHex(ciphertext.toUtf8());
    QByteArray plainBytes(cipherBytes.size(), 0);

    if (keyBytes.size() < 8) {
        keyBytes.resize(8, '0');
    }

    err = gcry_cipher_open(&handle, GCRY_CIPHER_DES, GCRY_CIPHER_MODE_ECB, 0);
    if (err) {
        return QString("Decryption Error: %1").arg(gcry_strerror(err));
    }

    // Set key
    err = gcry_cipher_setkey(handle, keyBytes.data(), keyBytes.size());
    if (err) {
        gcry_cipher_close(handle);
        return QString("Decryption Error: %1").arg(gcry_strerror(err));
    }

    // Decrypt
    err = gcry_cipher_decrypt(handle, plainBytes.data(), plainBytes.size(), cipherBytes.data(), cipherBytes.size());
    if (err) {
        gcry_cipher_close(handle);
        return QString("Decryption Error: %1").arg(gcry_strerror(err));
    }

    gcry_cipher_close(handle);

    int paddingSize = static_cast<int>(plainBytes[plainBytes.size() - 1]);
    if (paddingSize > 0 && paddingSize <= 8) {
        plainBytes.chop(paddingSize);
    }

    return QString(plainBytes);
}

/*
* ****************************************
* Mã hóa và giải mã AES
* ****************************************
*/

// Hàm mã hóa AES
QString Cipher::aesEncrypt(const QString &plaintext, const QString &key) {
    gcry_cipher_hd_t handle;
    QByteArray keyBytes = key.toUtf8();
    QByteArray plainBytes = plaintext.toUtf8();

    if (keyBytes.size() < 16) {
        keyBytes.resize(16, '0');
    }

    int padding = 16 - (plainBytes.size() % 16);
    plainBytes.append(padding, static_cast<char>(padding));

    QByteArray cipherBytes(plainBytes.size(), 0);

    if (gcry_cipher_open(&handle, GCRY_CIPHER_AES128, GCRY_CIPHER_MODE_ECB, 0) != 0) {
        qDebug() << "Failed to open cipher";
        return QString();
    }

    if (gcry_cipher_setkey(handle, keyBytes.data(), keyBytes.size()) != 0) {
        qDebug() << "Failed to set key";
        gcry_cipher_close(handle);
        return QString();
    }

    if (gcry_cipher_encrypt(handle, cipherBytes.data(), cipherBytes.size(), plainBytes.data(), plainBytes.size()) != 0) {
        qDebug() << "Failed to encrypt";
        gcry_cipher_close(handle);
        return QString();
    }

    gcry_cipher_close(handle);

    return QString(cipherBytes.toHex());
}

// Hàm giải mã AES
QString Cipher::aesDecrypt(const QString &ciphertext, const QString &key) {
    gcry_cipher_hd_t handle;
    QByteArray keyBytes = key.toUtf8();
    QByteArray cipherBytes = QByteArray::fromHex(ciphertext.toUtf8());
    QByteArray plainBytes(cipherBytes.size(), 0);

    if (keyBytes.size() < 16) {
        keyBytes.resize(16, '0');
    }

    if (gcry_cipher_open(&handle, GCRY_CIPHER_AES128, GCRY_CIPHER_MODE_ECB, 0) != 0) {
        qDebug() << "Failed to open cipher";
        return QString();
    }

    if (gcry_cipher_setkey(handle, keyBytes.data(), keyBytes.size()) != 0) {
        qDebug() << "Failed to set key";
        gcry_cipher_close(handle);
        return QString();
    }

    if (gcry_cipher_decrypt(handle, plainBytes.data(), plainBytes.size(), cipherBytes.data(), cipherBytes.size()) != 0) {
        qDebug() << "Failed to decrypt";
        gcry_cipher_close(handle);
        return QString();
    }

    gcry_cipher_close(handle);

    int padding = static_cast<int>(plainBytes[plainBytes.size() - 1]);
    if (padding > 0 && padding <= 16) {
        plainBytes.chop(padding);
    } else {
        qDebug() << "Invalid padding";
        return QString();
    }

    return QString::fromUtf8(plainBytes);
}

/*
-------------------------------------------------------------------------------------------------------
* *****************************************************************************************************
* Mã hóa và giải mã Page Import
* *****************************************************************************************************
-------------------------------------------------------------------------------------------------------
*/

/*
* **************************************************
* Mã hóa và Giải mã DES
* **************************************************
*/

// Hàm sinh khóa DES
QByteArray Cipher::generateDESKey() {
    QByteArray key(8, 0);
    gcry_randomize(key.data(), key.size(), GCRY_STRONG_RANDOM);
    return key;
}

// Hàm kiểm tra khóa DES
QByteArray Cipher::checkDESKey() {
    QString keyFilePath = QCoreApplication::applicationDirPath() + "/DES_key.txt";
    QByteArray key;
    if (QFile::exists(keyFilePath)) {
        key = loadKeyFromFile(keyFilePath);
    } else {
        key = generateDESKey();
        saveKeyToFile(keyFilePath, key);
    }
    return key;
}

// Hàm mã hóa DES
QByteArray Cipher::encryptDES(const QByteArray &data) {
    gcry_cipher_hd_t handle;
    QByteArray key = checkDESKey();

    if (key.size() < 8) {
        key.resize(8, '0');
    }

    int padding = 8 - (data.size() % 8);
    QByteArray paddedData = data;
    paddedData.append(padding, static_cast<char>(padding));

    QByteArray cipherBytes(paddedData.size(), 0);

    if (gcry_cipher_open(&handle, GCRY_CIPHER_DES, GCRY_CIPHER_MODE_ECB, 0) != 0) {
        qDebug() << "Failed to open DES cipher in ECB mode";
        return QByteArray();
    }

    if (gcry_cipher_setkey(handle, key.data(), key.size()) != 0) {
        qDebug() << "Failed to set DES key";
        gcry_cipher_close(handle);
        return QByteArray();
    }

    if (gcry_cipher_encrypt(handle, cipherBytes.data(), cipherBytes.size(), paddedData.data(), paddedData.size()) != 0) {
        qDebug() << "Failed to encrypt data using DES";
        gcry_cipher_close(handle);
        return QByteArray();
    }

    gcry_cipher_close(handle);
    return cipherBytes;
}

// Hàm giải mã DES
QByteArray Cipher::decryptDES(const QByteArray &data) {
    gcry_cipher_hd_t handle;
    QByteArray key = checkDESKey();

    if (key.size() < 8) {
        key.resize(8, '0');
    }

    QByteArray plainBytes(data.size(), 0);

    if (gcry_cipher_open(&handle, GCRY_CIPHER_DES, GCRY_CIPHER_MODE_ECB, 0) != 0) {
        qDebug() << "Failed to open DES cipher in ECB mode";
        return QByteArray();
    }

    if (gcry_cipher_setkey(handle, key.data(), key.size()) != 0) {
        qDebug() << "Failed to set DES key";
        gcry_cipher_close(handle);
        return QByteArray();
    }

    if (gcry_cipher_decrypt(handle, plainBytes.data(), plainBytes.size(), data.data(), data.size()) != 0) {
        qDebug() << "Failed to decrypt data using DES";
        gcry_cipher_close(handle);
        return QByteArray();
    }

    gcry_cipher_close(handle);

    int padding = static_cast<int>(plainBytes.at(plainBytes.size() - 1));
    if (padding < 1 || padding > 8) {
        qDebug() << "Invalid padding value:" << padding;
        return QByteArray();
    }
    plainBytes.chop(padding);

    return plainBytes;
}

/*
* **************************************************
* Mã hóa và Giải mã AES
* **************************************************
*/

// Hàm sinh khóa AES
QByteArray Cipher::generateAESKey() {
    QByteArray key(16, 0);
    gcry_randomize(key.data(), key.size(), GCRY_STRONG_RANDOM);
    return key;
}

// Hàm kiểm tra khóa AES
QByteArray Cipher::checkAESKey() {
    QString keyFilePath = QCoreApplication::applicationDirPath() + "/AES_key.txt";
    QByteArray key;
    if (QFile::exists(keyFilePath)) {
        key = loadKeyFromFile(keyFilePath);
    } else {
        key = generateAESKey();
        saveKeyToFile(keyFilePath, key);
    }
    return key;
}

// Hàm mã hóa AES
QByteArray Cipher::encryptAES(const QByteArray &data) {
    gcry_cipher_hd_t handle;
    QByteArray key = checkAESKey();
    QByteArray iv(16, 0);

    gcry_randomize(iv.data(), iv.size(), GCRY_STRONG_RANDOM);

    QByteArray paddedData = data;
    int padding = 16 - (data.size() % 16);
    paddedData.append(padding, static_cast<char>(padding));

    QByteArray cipherBytes(paddedData.size(), 0);

    if (gcry_cipher_open(&handle, GCRY_CIPHER_AES128, GCRY_CIPHER_MODE_CBC, 0) != 0) {
        qDebug() << "Failed to open cipher";
        return QByteArray();
    }

    if (gcry_cipher_setkey(handle, key.data(), key.size()) != 0) {
        qDebug() << "Failed to set key";
        gcry_cipher_close(handle);
        return QByteArray();
    }

    if (gcry_cipher_setiv(handle, iv.data(), iv.size()) != 0) {
        qDebug() << "Failed to set IV";
        gcry_cipher_close(handle);
        return QByteArray();
    }

    if (gcry_cipher_encrypt(handle, cipherBytes.data(), cipherBytes.size(), paddedData.data(), paddedData.size()) != 0) {
        qDebug() << "Failed to encrypt";
        gcry_cipher_close(handle);
        return QByteArray();
    }

    gcry_cipher_close(handle);

    return iv + cipherBytes;
}

// Hàm giải mã AES
QByteArray Cipher::decryptAES(const QByteArray &data) {
    if (data.size() < 16) {
        qDebug() << "Invalid data size";
        return QByteArray();
    }

    gcry_cipher_hd_t handle;
    QByteArray key = checkAESKey();
    QByteArray iv = data.left(16);
    QByteArray cipherBytes = data.mid(16);

    QByteArray plainBytes(cipherBytes.size(), 0);

    if (gcry_cipher_open(&handle, GCRY_CIPHER_AES128, GCRY_CIPHER_MODE_CBC, 0) != 0) {
        qDebug() << "Failed to open cipher";
        return QByteArray();
    }

    if (gcry_cipher_setkey(handle, key.data(), key.size()) != 0) {
        qDebug() << "Failed to set key";
        gcry_cipher_close(handle);
        return QByteArray();
    }

    if (gcry_cipher_setiv(handle, iv.data(), iv.size()) != 0) {
        qDebug() << "Failed to set IV";
        gcry_cipher_close(handle);
        return QByteArray();
    }

    if (gcry_cipher_decrypt(handle, plainBytes.data(), plainBytes.size(), cipherBytes.data(), cipherBytes.size()) != 0) {
        qDebug() << "Failed to decrypt";
        gcry_cipher_close(handle);
        return QByteArray();
    }

    gcry_cipher_close(handle);

    // Remove PKCS#7 padding
    int padding = static_cast<int>(plainBytes.at(plainBytes.size() - 1));
    if (padding < 1 || padding > 16) {
        qDebug() << "Invalid padding";
        return QByteArray();
    }
    plainBytes.chop(padding);

    return plainBytes;
}

/*
* **************************************************
* Lưu và đọc file
* **************************************************
*/

// Hàm lưu file
void Cipher::saveKeyToFile(const QString &filePath, const QByteArray &key) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for writing";
    }

    QTextStream out(&file);
    out << key.toHex();
    file.close();
}

// Hàm đọc file
QByteArray Cipher::loadKeyFromFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for reading";
        return QByteArray();
    }

    QTextStream in(&file);
    QString keyHex = in.readAll();
    file.close();
    return QByteArray::fromHex(keyHex.toUtf8());
}
