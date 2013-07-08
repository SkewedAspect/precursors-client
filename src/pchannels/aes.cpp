#include <QDebug>
#include <openssl/aes.h>
#include <openssl/rand.h>

#include "aes.h"

/**********************************************************************************************************************/
/* Public API                                                                                                         */
/**********************************************************************************************************************/

AES::AES(QByteArray salt) :
    QObject()
{
    int bytes, nrounds = 5;
    unsigned char _key[16], _iv[16];

    unsigned char key_data[40];
    int key_data_len = 40;

    // Generate our random key_data
    RAND_bytes(key_data, key_data_len);

    // Generate a key and iv for AES 128 CBC. An SHA1 digest is used to hash the supplied key material `nrounds` is the
    // number of times we hash the material. More rounds are more secure, but slower.
    bytes = EVP_BytesToKey(EVP_aes_128_cbc(), EVP_sha1(), (unsigned char*) salt.constData(), key_data, key_data_len, nrounds, _key, _iv);

    // Something's go wrong here
    if (bytes != 16)
    {
        printf("Key size is %d bits - should be 256 bits\n", bytes);
        return;
    }

    // Setup our encryption and decryption contexts
    EVP_CIPHER_CTX_init(&encCtx);
    EVP_EncryptInit_ex(&encCtx, EVP_aes_128_cbc(), NULL, _key, _iv);
    EVP_CIPHER_CTX_init(&decCtx);
    EVP_DecryptInit_ex(&decCtx, EVP_aes_128_cbc(), NULL, _key, _iv);

    // Set key and iv
    this->key = QByteArray((char*) _key, 16);
    this->iv = QByteArray((char*) _iv, 16);
} // end AES

QByteArray AES::encrypt(QByteArray plaintext)
{
    unsigned char* _plaintext = (unsigned char*) plaintext.constData();
    int len = plaintext.length();

    // Max ciphertext len for a n bytes of plaintext is n + AES_BLOCK_SIZE -1 bytes
    int c_len = len + AES_BLOCK_SIZE, f_len = 0;
    unsigned char *ciphertext = new unsigned char[c_len];

    // Allows reusing of 'encCtx' for multiple encryption cycles
    EVP_EncryptInit_ex(&encCtx, NULL, NULL, NULL, NULL);

    // Update ciphertext, c_len is filled with the length of ciphertext generated,
    // *len is the size of plaintext in bytes
    EVP_EncryptUpdate(&encCtx, ciphertext, &c_len, _plaintext, len);

    // Update ciphertext with the final remaining bytes
    EVP_EncryptFinal_ex(&encCtx, ciphertext + c_len, &f_len);

    len = c_len + f_len;

    return QByteArray((char*) ciphertext, len);
} // end encrypt

QByteArray AES::decrypt(QByteArray ciphertext)
{
    unsigned char* _ciphertext = (unsigned char*) ciphertext.constData();
    int len = ciphertext.length();

    // because we have padding ON, we must allocate an extra cipher block size of memory
    int p_len = len, f_len = 0;
    unsigned char *plaintext = new unsigned char[p_len + AES_BLOCK_SIZE];

    // Allows reusing of 'decCtx' for multiple encryption cycles
    EVP_DecryptInit_ex(&decCtx, NULL, NULL, NULL, NULL);

    EVP_DecryptUpdate(&decCtx, plaintext, &p_len, _ciphertext, len);
    EVP_DecryptFinal_ex(&decCtx, plaintext + p_len, &f_len);

    len = p_len + f_len;

    return QByteArray((char*) plaintext, len);
} // end decrypt

