// signature C++ example

#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

// liboqs C++ wrapper
#include "oqs_cpp.hpp"

using namespace std;

int main() {
    
    // std::cout << std::boolalpha;
    // std::cout << "liboqs version: " << oqs::oqs_version() << '\n';
    // std::cout << "liboqs-cpp version: " << oqs::oqs_cpp_version() << '\n';
    // std::cout << "Enabled signatures:\n" << oqs::Sigs::get_enabled_sigs();

    oqs::bytes message = "This is the message to sign"_bytes;
    // std::string sig_name = "SPHINCS+-SHA2-192f-simple";
    
    std::vector<string> lista = {"SPHINCS+-SHA2-192f-simple", "ML-DSA-65"};
    

    for(string sig_name : lista){

        std::ofstream outputFile(sig_name+".csv");

        oqs::Signature signer{sig_name};
        // std::cout << "\n\nSignature details:\n" << signer.get_details();

        oqs::Timer<std::chrono::microseconds> t;
        oqs::bytes signer_public_key = signer.generate_keypair();
        // t.toc();
        // std::cout << "\n\nSigner public key:\n" << oqs::hex_chop(signer_public_key);
        // std::cout << "\n\nIt took " << t << " microsecs to generate the key pair";

        oqs::Signature verifier{sig_name};
        
        std::cout << sig_name << endl;

        for(int i = 0; i < 1000; i++){
            oqs::bytes signature = signer.sign(message);
            
            bool is_valid = verifier.verify(message, signature, signer_public_key);
        }

        for(int i = 0; i < 10000; i++){
            t.tic();
            oqs::bytes signature = signer.sign(message);
            t.toc();
            outputFile << t << ", ";
            // std::cout << "\n\nSignature:\n" << oqs::hex_chop(signature);
            
            t.tic();
            bool is_valid = verifier.verify(message, signature, signer_public_key);
            t.toc();
            outputFile << t << std::endl;
        }
        // std::cout << "\n\nValid signature? " << is_valid << '\n';

        outputFile.close();
    }

    return 0;
    // is_valid ? oqs::OQS_STATUS::OQS_SUCCESS : oqs::OQS_STATUS::OQS_ERROR;
}
