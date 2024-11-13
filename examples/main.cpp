//
// Created by James Wirth on 13/11/2024.
//

#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>
#include "OptionLib/OptionLib.h"

void calculateAndDisplayPrices(const OptionLib::Option& callOption, const OptionLib::Option& putOption,
                               const std::string& modelName, const OptionLib::Models::Model& model) {
    double callPrice = model.price(callOption);
    double putPrice = model.price(putOption);
    std::cout << modelName << " | " << callPrice << "       | " << putPrice << '\n';
}

int main() {
    using namespace OptionLib;
    using namespace OptionLib::Models;

    std::cout << std::fixed << std::setprecision(2);

    double spotPrice = 100.0;
    double strikePrice = 100.0;
    double timeToExpiry = 1.0;
    double riskFreeRate = 0.05;
    double volatility = 0.2;

    Option callOption(strikePrice, timeToExpiry, OptionType::Call);
    Option putOption(strikePrice, timeToExpiry, OptionType::Put);

    std::vector<std::pair<std::string, std::unique_ptr<Model>>> models;
    models.emplace_back("Black-Scholes     ", std::make_unique<BlackScholes>(spotPrice, riskFreeRate, volatility));
    models.emplace_back("Binomial (10^4)   ", std::make_unique<Binomial>(spotPrice, riskFreeRate, volatility, pow(10, 4)));
    models.emplace_back("Monte Carlo (10^7)", std::make_unique<MonteCarlo>(spotPrice, riskFreeRate, volatility, pow(10, 7)));

    std::cout << "\nOption Pricing Comparison:\n";
    std::cout << "==========================\n";
    std::cout << "Model             | Call Price | Put Price\n";
    std::cout << "------------------|------------|-----------\n";

    for (const auto& [modelName, model] : models) {
        calculateAndDisplayPrices(callOption, putOption, modelName, *model);
    }

    return 0;
}