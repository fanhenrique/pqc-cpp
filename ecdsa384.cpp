#include <iostream>
#include <gcrypt.h>
#include <cstring>
#include <chrono>

// Inicializa o libgcrypt corretamente
void init_libgcrypt() {
    if (!gcry_check_version(GCRYPT_VERSION)) {
        std::cerr << "Versão incompatível do libgcrypt!\n";
        exit(1);
    }

    gcry_control(GCRYCTL_DISABLE_SECMEM, 0);
    gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);
}

int main() {
    init_libgcrypt();

    // warm up executions
    for(int i = 0; i < 1000; i++){
        // ========================================================
        // Gerar chave ECDSA com curva NIST P-384 ou NIST P-256
        // ========================================================
        gcry_sexp_t key_spec, keypair;

        const char* key_str = "(genkey (ecdsa (curve \"NIST P-384\")))";
        gcry_sexp_new(&key_spec, key_str, 0, 1);
        gcry_pk_genkey(&keypair, key_spec);
        gcry_sexp_release(key_spec);

        gcry_sexp_t priv_key = gcry_sexp_find_token(keypair, "private-key", 0);
        gcry_sexp_t pub_key  = gcry_sexp_find_token(keypair, "public-key", 0);

        // ========================================================
        // Mensagem crua
        // ========================================================
        const char* message = "This is the message to sign";
        size_t message_len = strlen(message);

        gcry_sexp_t data;
        gcry_sexp_build(&data, nullptr, "(data (flags raw) (value %b))", message_len, message);

        // ========================================================
        // Medir tempo da assinatura
        // ========================================================
        gcry_sexp_t signature;

        auto t1 = std::chrono::high_resolution_clock::now();
        gcry_error_t err = gcry_pk_sign(&signature, data, priv_key);
        auto t2 = std::chrono::high_resolution_clock::now();

        if (err) {
            std::cerr << "Erro ao assinar: " << gcry_strerror(err) << "\n";
            return 1;
        }

        std::chrono::duration<double, std::micro> sign_duration = t2 - t1;
        // std::cout << "Assinatura criada com sucesso!\n";
        // std::cout << "Tempo de assinatura: " << sign_duration.count() << " microssegundos\n";

        // ========================================================
        // Medir tempo da verificação
        // ========================================================
        auto t3 = std::chrono::high_resolution_clock::now();
        err = gcry_pk_verify(signature, data, pub_key);
        auto t4 = std::chrono::high_resolution_clock::now();

        if (err) {
            std::cerr << "❌ Assinatura inválida: " << gcry_strerror(err) << "\n";
            return 1;
        }

        std::chrono::duration<double, std::micro> verify_duration = t4 - t3;

        std::cout << sign_duration.count() << ", " << verify_duration.count() << std::endl;
        // std::cout << "Assinatura verificada com sucesso!\n";
        // std::cout << "Tempo de verificação: " << verify_duration.count() << " microssegundos\n";

        // ========================================================
        // Liberação de recursos
        // ========================================================
        gcry_sexp_release(priv_key);
        gcry_sexp_release(pub_key);
        gcry_sexp_release(signature);
        gcry_sexp_release(data);
        gcry_sexp_release(keypair);
    }


    for(int i = 0; i < 10000; i++){
        // ========================================================
        // Gerar chave ECDSA com curva NIST P-384 ou NIST P-256
        // ========================================================
        gcry_sexp_t key_spec, keypair;

        const char* key_str = "(genkey (ecdsa (curve \"NIST P-384\")))";
        gcry_sexp_new(&key_spec, key_str, 0, 1);
        gcry_pk_genkey(&keypair, key_spec);
        gcry_sexp_release(key_spec);

        gcry_sexp_t priv_key = gcry_sexp_find_token(keypair, "private-key", 0);
        gcry_sexp_t pub_key  = gcry_sexp_find_token(keypair, "public-key", 0);

        // ========================================================
        // Mensagem crua
        // ========================================================
        const char* message = "This is the message to sign";
        size_t message_len = strlen(message);

        gcry_sexp_t data;
        gcry_sexp_build(&data, nullptr, "(data (flags raw) (value %b))", message_len, message);

        // ========================================================
        // Medir tempo da assinatura
        // ========================================================
        gcry_sexp_t signature;

        auto t1 = std::chrono::high_resolution_clock::now();
        gcry_error_t err = gcry_pk_sign(&signature, data, priv_key);
        auto t2 = std::chrono::high_resolution_clock::now();

        if (err) {
            std::cerr << "Erro ao assinar: " << gcry_strerror(err) << "\n";
            return 1;
        }

        std::chrono::duration<double, std::micro> sign_duration = t2 - t1;
        // std::cout << "Assinatura criada com sucesso!\n";
        // std::cout << "Tempo de assinatura: " << sign_duration.count() << " microssegundos\n";

        // ========================================================
        // Medir tempo da verificação
        // ========================================================
        auto t3 = std::chrono::high_resolution_clock::now();
        err = gcry_pk_verify(signature, data, pub_key);
        auto t4 = std::chrono::high_resolution_clock::now();

        if (err) {
            std::cerr << "❌ Assinatura inválida: " << gcry_strerror(err) << "\n";
            return 1;
        }

        std::chrono::duration<double, std::micro> verify_duration = t4 - t3;

        std::cout << sign_duration.count() << ", " << verify_duration.count() << std::endl;
        // std::cout << "Assinatura verificada com sucesso!\n";
        // std::cout << "Tempo de verificação: " << verify_duration.count() << " microssegundos\n";

        // ========================================================
        // Liberação de recursos
        // ========================================================
        gcry_sexp_release(priv_key);
        gcry_sexp_release(pub_key);
        gcry_sexp_release(signature);
        gcry_sexp_release(data);
        gcry_sexp_release(keypair);
    }

    return 0;
}
