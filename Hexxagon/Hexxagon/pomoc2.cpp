#include <iostream>
#include <fstream>

int main() {
    // Tworzenie pliku tekstowego
    std::ofstream file("liczby.txt");

    // Sprawdzenie, czy plik został poprawnie otwarty
    if (!file) {
        std::cout << "Nie można otworzyć pliku." << std::endl;
        return 1;
    }

    // Wpisywanie liczb do pliku
    int liczba1 = 10;
    int liczba2 = 20;

    file << liczba1 << " ";
    file << liczba2;

    // Zamknięcie pliku
    file.close();

    // Odczytywanie liczb z pliku
    std::ifstream inFile("liczby.txt");

    if (!inFile) {
        std::cout << "Nie można otworzyć pliku." << std::endl;
        return 1;
    }

    int wczytanaLiczba;
    while (inFile >> wczytanaLiczba) {
        std::cout << "Wczytano liczbę: " << wczytanaLiczba << std::endl;
    }

    // Zamknięcie pliku
    inFile.close();

    return 0;
}
