#include <iostream>
#include <cmath>

int main(int argc, char* argv[])
{
    if (argc != 7) {
        std::cout << "Usage: calc -a <amount> -i <interest%> -n <months>\n";
        return 1;
    }

    long double principal = 0;
    long double annualInterest = 0;
    int months = 0;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-a") {
            principal = std::stold(argv[++i]);
        } else if (arg == "-i") {
            annualInterest = std::stold(argv[++i]);
        } else if (arg == "-n") {
            months = std::stoi(argv[++i]);
        }
    }

    if (principal <= 0 || annualInterest <= 0 || months <= 0) {
        std::cout << "Invalid input. All values must be greater than zero.\n";
        return 1;
    }

    long double monthlyInterest = annualInterest / 100.0L / 12.0L;
    long double emi = (principal * monthlyInterest) /
                      (1.0L - std::pow(1.0L + monthlyInterest, -months));

    std::cout << "For principal = " << principal
              << ", rate = " << annualInterest
              << "%, months = " << months << "\n";
    std::cout << "Monthly EMI = " << emi << "\n";

    return 0;
}

