#include <iostream>
#include <ctime>
#include <iomanip>


int main() {
    int count = 15;

    std::cin >> count;
    std::cout << count << std::endl;

    std::srand(std::time(nullptr));

    for (int i = 0; i < count; ++i) {
        for (int j = 0; j < count; ++j) {
            double val = (static_cast<double>(std::rand() % 10000)) / 18000.0;
            std::cout << std::setw(5) << val << " ";
        }

        std::cout << std::endl;
    }
}