#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/rand.h>
#include <openssl/err.h>

#define PORT 8080
#define BUFFER_SIZE 8192
#define AES_KEY_SIZE 32
#define IV_SIZE 12
#define TAG_SIZE 16

EVP_PKEY *local_priv_key = NULL, *peer_public_key = NULL;
unsigned char session_aes_key[AES_KEY_SIZE];

void handle_errors() {
    ERR_print_errors_fp(stderr);
    exit(1);
}

// Стриктно четене на точно N байта
int recv_all(int sock, void *buf, int len) {
    int total = 0;
    unsigned char *p = (unsigned char *)buf;
    while (total < len) {
        int r = recv(sock, p + total, len - total, 0);
        if (r <= 0) return -1;
        total += r;
    }
    return total;
}

void send_packet(int sock, void *data, uint32_t len) {
    uint32_t net_len = htonl(len);
    send(sock, &net_len, sizeof(net_len), 0);
    send(sock, data, len, 0);
}

int recv_packet(int sock, void *buf, uint32_t max_len) {
    uint32_t net_len = 0;
    if (recv_all(sock, &net_len, sizeof(net_len)) <= 0) return -1;
    uint32_t len = ntohl(net_len);
    if (len > max_len) return -1;
    memset(buf, 0, max_len); // Чистим буфера преди запис
    return recv_all(sock, buf, len);
}

int encrypt_aes_gcm(unsigned char *p, int p_len, unsigned char *key, unsigned char *iv, unsigned char *tag, unsigned char *c) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int len, c_len;
    EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL);
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, IV_SIZE, NULL);
    EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv);
    EVP_EncryptUpdate(ctx, c, &len, p, p_len);
    c_len = len;
    EVP_EncryptFinal_ex(ctx, c + len, &len);
    c_len += len;
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, TAG_SIZE, tag);
    EVP_CIPHER_CTX_free(ctx);
    return c_len;
}

int decrypt_aes_gcm(unsigned char *c, int c_len, unsigned char *key, unsigned char *iv, unsigned char *tag, unsigned char *p) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int len, p_len;
    EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL);
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, IV_SIZE, NULL);
    EVP_DecryptInit_ex(ctx, NULL, NULL, key, iv);
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, TAG_SIZE, tag);
    if (EVP_DecryptUpdate(ctx, p, &len, c, c_len) != 1) { EVP_CIPHER_CTX_free(ctx); return -1; }
    p_len = len;
    int res = EVP_DecryptFinal_ex(ctx, p + len, &len);
    EVP_CIPHER_CTX_free(ctx);
    return (res > 0) ? (p_len + len) : -1;
}

void generate_rsa_keys() {
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    EVP_PKEY_keygen_init(ctx);
    EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048);
    EVP_PKEY_keygen(ctx, &local_priv_key);
    EVP_PKEY_CTX_free(ctx);
}

int main() {
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr = { .sin_family = AF_INET, .sin_port = htons(PORT) };
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    printf("Ти ли започваш (y/n)? ");
    char choice; scanf(" %c", &choice); while (getchar() != '\n'); 

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) exit(1);

    generate_rsa_keys();
    
    // Exchange Keys
    BIO *bio = BIO_new(BIO_s_mem());
    PEM_write_bio_PUBKEY(bio, local_priv_key);
    char pem[2048];
    int pem_len = BIO_read(bio, pem, sizeof(pem));
    send_packet(sock, pem, pem_len);
    BIO_free(bio);

    unsigned char buffer[BUFFER_SIZE];
    int in_pem_len = recv_packet(sock, buffer, BUFFER_SIZE);
    BIO *bio_p = BIO_new_mem_buf(buffer, in_pem_len);
    peer_public_key = PEM_read_bio_PUBKEY(bio_p, NULL, NULL, NULL);
    BIO_free(bio_p);
    
    printf("RSA Exchange OK.\n");
    int rsa_size = EVP_PKEY_get_size(local_priv_key);

    if (choice == 'y') {
        printf("Чакаме READY...\n");
        char ready_buf[64];
        recv_packet(sock, ready_buf, 64); // Четем "READY" пакета

        RAND_bytes(session_aes_key, AES_KEY_SIZE);
        unsigned char *enc_key = malloc(rsa_size);
        size_t outlen = rsa_size;
        
        EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(peer_public_key, NULL);
        EVP_PKEY_encrypt_init(ctx);
        EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING);
        
        if (EVP_PKEY_encrypt(ctx, enc_key, &outlen, session_aes_key, AES_KEY_SIZE) <= 0) handle_errors();
        
        send_packet(sock, enc_key, outlen);
        EVP_PKEY_CTX_free(ctx);
        free(enc_key);
    } else {
        printf("Пращаме READY...\n");
        send_packet(sock, "READY", 5); 

        unsigned char *enc_key = malloc(rsa_size);
        int received_len = recv_packet(sock, enc_key, rsa_size);
        
        EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(local_priv_key, NULL);
        EVP_PKEY_decrypt_init(ctx);
        EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING);
        
        size_t outlen = rsa_size; 
        if (EVP_PKEY_decrypt(ctx, session_aes_key, &outlen, enc_key, received_len) <= 0) {
            handle_errors();
        }
        EVP_PKEY_CTX_free(ctx);
        free(enc_key);
    }

    printf("AES Key READY!\n");

    fd_set fds;
    while(1) {
        FD_ZERO(&fds); FD_SET(0, &fds); FD_SET(sock, &fds);
        select(sock + 1, &fds, NULL, NULL, NULL);

        if (FD_ISSET(sock, &fds)) {
            uint32_t net_c_len;
            if (recv_all(sock, &net_c_len, sizeof(uint32_t)) <= 0) break;
            uint32_t c_len = ntohl(net_c_len);
            
            unsigned char iv[IV_SIZE], tag[TAG_SIZE], ciph[BUFFER_SIZE];
            recv_all(sock, iv, IV_SIZE);
            recv_all(sock, tag, TAG_SIZE);
            recv_all(sock, ciph, c_len);

            unsigned char plain[BUFFER_SIZE];
            int p_len = decrypt_aes_gcm(ciph, c_len, session_aes_key, iv, tag, plain);
            if (p_len > 0) { 
                plain[p_len] = 0; 
                printf("\nДругият: %s\nТи: ", plain); 
                fflush(stdout); 
            }
        }

        if (FD_ISSET(0, &fds)) {
            char in[BUFFER_SIZE];
            if(!fgets(in, BUFFER_SIZE, stdin)) break;
            in[strcspn(in, "\n")] = 0;
            if (strlen(in) == 0) continue;

            unsigned char iv[IV_SIZE], tag[TAG_SIZE], ciph[BUFFER_SIZE];
            RAND_bytes(iv, IV_SIZE); 
            int c_len = encrypt_aes_gcm((unsigned char*)in, strlen(in), session_aes_key, iv, tag, ciph);
            
            uint32_t net_c_len = htonl(c_len);
            send(sock, &net_c_len, sizeof(uint32_t), 0);
            send(sock, iv, IV_SIZE, 0);
            send(sock, tag, TAG_SIZE, 0);
            send(sock, ciph, c_len, 0);
            printf("Ти: "); fflush(stdout);
        }
    }
    return 0;
}
