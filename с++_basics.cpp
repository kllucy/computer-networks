#include <iostream>
#include <vector>
#include <string>
#include <cstdlib> 
#include <ctime>

int main()
{

    setlocale(LC_ALL, "Russian");
    std::srand(std::time(0));

    std::vector<std::string> radioStations = {
       "Europa Plus",
       "Radio Energy",
       "DFM",
       "Retro FM",
       "Maximum",
       "Record"
    };

    double targetFrequency;
    std::cout << "Введите частоту для поиска (от 85.0 до 105.0): ";
    std::cin >> targetFrequency; 

    std::cout << "Поиск частоты " << targetFrequency << " MHz..." << std::endl;


    std::cout << "Поиск частоты " << targetFrequency << std::endl;

    int attempts = 0; 
    bool found = false;
    for (double currentFreq = 85.0; currentFreq <= 105.05; currentFreq += 0.1) {
        attempts++;

        if (std::abs(currentFreq - targetFrequency) < 0.05) {
            found = true;
            break;
        }

    }
    if (found) {
        std::cout << "Частота найдена!" << std::endl;
        std::cout << "Номер попытки: " << attempts << std::endl;

        int randomIndex = std::rand() % radioStations.size();
        std::cout << "Сейчас играет: " << radioStations[randomIndex] << std::endl;
    }
    else {
        std::cout << "Частота не найдена в диапазоне от 85.0 до 105.0" << std::endl;
    }

    return 0;
}

