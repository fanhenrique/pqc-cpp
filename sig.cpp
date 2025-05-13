#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

// liboqs C++ wrapper
#include "oqs_cpp.hpp"

using namespace std;

int main() {
    
    oqs::bytes message = "This is the message to sign"_bytes;
    
    std::vector<string> lista = {"SPHINCS+-SHA2-192f-simple", "ML-DSA-65"};

    for(string sig_name : lista){

        std::ofstream outputFile(sig_name+".csv");

        oqs::Signature signer{sig_name};
    
        oqs::Timer<std::chrono::microseconds> t;
        oqs::bytes signer_public_key = signer.generate_keypair();

        oqs::Signature verifier{sig_name};
        
        std::cout << sig_name << endl;
        
        // warm up executions
        for(int i = 0; i < 1000; i++){

            oqs::bytes signature = signer.sign(message);
            
            bool is_valid = verifier.verify(message, signature, signer_public_key);
        }

        for(int i = 0; i < 10000; i++){
            
            t.tic();
            oqs::bytes signature = signer.sign(message);
            t.toc();
            outputFile << t << ", ";
               
            t.tic();
            bool is_valid = verifier.verify(message, signature, signer_public_key);
            t.toc();
            outputFile << t << std::endl;
        }

        outputFile.close();
    }

    return 0;
}
