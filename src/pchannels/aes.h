#ifndef AES_H
#define AES_H

#include <QObject>

#include <openssl/evp.h>

class AES : public QObject
{
    Q_OBJECT
public:
    QByteArray key;
    QByteArray iv;

    explicit AES(QByteArray salt = 0);

    QByteArray encrypt(QByteArray plaintext);
    QByteArray decrypt(QByteArray ciphertext);

private:
    EVP_CIPHER_CTX encCtx;
    EVP_CIPHER_CTX decCtx;
};

#endif // AES_H
