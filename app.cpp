// NMSHacking tarafından yazıldı.
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <cstdio>

#ifdef _WIN32
#include <windows.h>
#define CHECK_COMMAND_GPP "g++ --version"
#define CHECK_COMMAND_CS "csc -help"
#define CHECK_COMMAND_CC "gcc --version"
#define CHECK_COMMAND_PY "python --version"
#define CHECK_COMMAND_GO "go version"
#define INSTALL_COMMAND "choco install mingw -y && choco install python -y && choco install dotnet-sdk -y && choco install golang -y"
#elif __linux__
#include <unistd.h>
#define CHECK_COMMAND_GPP "g++ --version"
#define CHECK_COMMAND_CS "mcs -help"
#define CHECK_COMMAND_CC "gcc --version"
#define CHECK_COMMAND_PY "python3 --version"
#define CHECK_COMMAND_GO "go version"
#define INSTALL_COMMAND "sudo apt-get install -y g++ gcc mono-mcs python3 python3-pip dotnet-sdk-5.0 golang"
#elif __APPLE__
#include <unistd.h>
#define CHECK_COMMAND_GPP "clang++ --version"
#define CHECK_COMMAND_CS "mcs -help"
#define CHECK_COMMAND_CC "gcc --version"
#define CHECK_COMMAND_PY "python3 --version"
#define CHECK_COMMAND_GO "go version"
#define INSTALL_COMMAND "brew install gcc python dotnet go mono"
#endif

void xorEncryptDecrypt(const std::string &key, std::vector<char> &data) {
    size_t keyLen = key.size();
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] ^= key[i % keyLen];
    }
}

std::string generateRandomKey(size_t length) {
    std::string key;
    static const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    for (size_t i = 0; i < length; ++i) {
        key += charset[rand() % (sizeof(charset) - 1)];
    }
    return key;
}

void encryptFile(const std::string &inputFileName, const std::string &outputFileName, std::string &key) {
    std::ifstream inputFile(inputFileName, std::ios::binary);
    std::vector<char> data((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close();

    key = generateRandomKey(16); // Generate a random key of length 16
    xorEncryptDecrypt(key, data);

    std::ofstream outputFile(outputFileName, std::ios::binary);
    outputFile.write(data.data(), data.size());
    outputFile.close();
}

void decryptFile(const std::string &inputFileName, const std::string &outputFileName, const std::string &key) {
    std::ifstream inputFile(inputFileName, std::ios::binary);
    std::vector<char> data((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close();

    xorEncryptDecrypt(key, data);

    std::ofstream outputFile(outputFileName, std::ios::binary);
    outputFile.write(data.data(), data.size());
    outputFile.close();
}

bool checkCompiler(const std::string& command) {
    return (std::system(command.c_str()) == 0);
}

void compileFile(const std::string &fileName, const std::string &language) {
    std::string command;
    if (language == "C++") {
#ifdef _WIN32
        command = "g++ -o output.exe " + fileName;
#else
        command = "g++ -o output " + fileName;
#endif
    } else if (language == "C") {
#ifdef _WIN32
        command = "gcc -o output.exe " + fileName;
#else
        command = "gcc -o output " + fileName;
#endif
    } else if (language == "C#") {
#ifdef _WIN32
        command = "csc -out:output.exe " + fileName;
#else
        command = "mcs -out:output.exe " + fileName;
#endif
    } else if (language == "Python") {
        command = "pyinstaller --onefile " + fileName;
    } else if (language == "Go") {
        command = "go build -o output " + fileName;
    }
    std::system(command.c_str());
}

void installCompilers() {
    std::system(INSTALL_COMMAND);
}

int main() {
    srand(time(0)); // Seed the random number generator

    std::cout << R"(
███╗   ██╗███╗   ███╗███████╗ ██████╗ ██████╗ ██████╗ ███████╗███████╗███████╗ ██████╗██╗   ██╗██████╗ ███████╗
████╗  ██║████╗ ████║██╔════╝██╔════╝██╔═══██╗██╔══██╗██╔════╝██╔════╝██╔════╝██╔════╝██║   ██║██╔══██╗██╔════╝
██╔██╗ ██║██╔████╔██║███████╗██║     ██║   ██║██║  ██║█████╗  ███████╗█████╗  ██║     ██║   ██║██████╔╝█████╗  
██║╚██╗██║██║╚██╔╝██║╚════██║██║     ██║   ██║██║  ██║██╔══╝  ╚════██║██╔══╝  ██║     ██║   ██║██╔══██╗██╔══╝  
██║ ╚████║██║ ╚═╝ ██║███████║╚██████╗╚██████╔╝██████╔╝███████╗███████║███████╗╚██████╗╚██████╔╝██║  ██║███████╗
╚═╝  ╚═══╝╚═╝     ╚═╝╚══════╝ ╚═════╝ ╚═════╝ ╚═════╝ ╚══════╝╚══════╝╚══════╝ ╚═════╝ ╚═════╝ ╚═╝  ╚═╝╚══════╝
                                                                                                               
)" << std::endl;

    std::cout << "Derleyiciler yükleniyor...\n";
    installCompilers();
    std::cout << "Derleyiciler yüklendi.\n";

    int choice;
    std::string inputFileName, outputFileName, key, language;

    while (true) {
        std::cout << "Seçenekler:\n";
        std::cout << "1. Kod Şifrele\n";
        std::cout << "2. Kod Çöz\n";
        std::cout << "3. Şifreli Kodu Derle\n";
        std::cout << "4. Derleyicileri Kontrol Et\n";
        std::cout << "Seçiminizi girin: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "Girdi dosyasının adını girin: ";
                std::cin >> inputFileName;
                std::cout << "Çıktı dosyasının adını girin: ";
                std::cin >> outputFileName;

                encryptFile(inputFileName, outputFileName, key);

                std::cout << "Dosya başarıyla şifrelendi.\n";
                std::cout << "Şifreleme anahtarı: " << key << "\n";
                break;

            case 2:
                std::cout << "Girdi dosyasının adını girin: ";
                std::cin >> inputFileName;
                std::cout << "Çıktı dosyasının adını girin: ";
                std::cin >> outputFileName;
                std::cout << "Şifreleme anahtarını girin: ";
                std::cin >> key;

                decryptFile(inputFileName, outputFileName, key);

                std::cout << "Dosya başarıyla çözüldü.\n";
                break;

            case 3:
                std::cout << "Derlenecek şifreli dosyanın adını girin: ";
                std::cin >> inputFileName;

                std::cout << "Şifreleme anahtarını girin: ";
                std::cin >> key;

                decryptFile(inputFileName, "decrypted_temp.cpp", key);

                std::cout << "Derleme dili seçin (C++, C, C#, Python, Go [Go Stabil Değil]): ";
                std::cin >> language;

                compileFile("decrypted_temp.cpp", language);
                std::remove("decrypted_temp.cpp");

                std::cout << "Şifreli dosya başarıyla derlendi.\n";
                break;

            case 4:
                std::cout << "Derleyiciler kontrol ediliyor...\n";
                if (checkCompiler(CHECK_COMMAND_GPP) &&
                    checkCompiler(CHECK_COMMAND_CS) &&
                    checkCompiler(CHECK_COMMAND_CC) &&
                    checkCompiler(CHECK_COMMAND_PY) &&
                    checkCompiler(CHECK_COMMAND_GO)) {
                    std::cout << "Tüm derleyiciler mevcut.\n";
                } else {
                    std::cerr << "Derleyiciler bulunamadı veya kurulumda sorun var.\n";
                }
                break;

            default:
                std::cerr << "Geçersiz seçenek.\n";
                break;
        }
    }

    return 0;
}
