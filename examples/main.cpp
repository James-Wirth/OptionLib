//
// Created by James Wirth on 13/11/2024.
//

#include <iostream>
#include "OptionLib/OptionLib.h"

// Inline function for converting GreekType to a string
using namespace OptionLib::Models;
inline std::string greekTypeName(GreekType greekType) {
    switch (greekType) {
        case GreekType::Delta: return "Delta";
        case GreekType::Gamma: return "Gamma";
        case GreekType::Vega:  return "Vega";
        case GreekType::Theta: return "Theta";
        case GreekType::Rho:   return "Rho";
        default: return "Unknown";
    }
}

int main() {
    using namespace OptionLib;
    using namespace OptionLib::Models;

    double spotPrice = 100.0;
    double riskFreeRate = 0.05;
    double volatility = 0.2;

    auto defaultModel = std::make_shared<MonteCarlo>(spotPrice, riskFreeRate, volatility);
    Portfolio portfolio(defaultModel);

    auto callOption = std::make_shared<Option>(100, 1.0, OptionType::Call);
    auto putOption = std::make_shared<Option>(100, 1.0, OptionType::Put);

    portfolio.addOption(callOption);
    portfolio.addOption(putOption);

    // auto binomialModel = std::make_shared<MonteCarlo>(spotPrice, riskFreeRate, volatility);
    // auto callOption2 = std::make_shared<Option>(100, 1.0, OptionType::Call);
    // portfolio.addOption(callOption2, binomialModel);

    // Output the total portfolio value
    double portfolioValue = portfolio.totalValue();
    std::cout << "Total Portfolio Value: " << portfolioValue << std::endl;

    // Output Greek vectors
    const std::vector greekTypes = {
        GreekType::Delta,
        GreekType::Gamma,
        GreekType::Vega,
        GreekType::Theta,
        GreekType::Rho
    };

    for (const auto& greek : greekTypes) {
        auto greekVector = portfolio.greekVector(greek);
        std::cout << "Greek Vector (" << greekTypeName(greek) << "): ";
        for (double value : greekVector) {
            std::cout << value << " ";
        }
        std::cout << '\n';

        double totalGreekValue = portfolio.totalGreek(greek);
        std::cout << "Total " << greekTypeName(greek) << ": " << totalGreekValue << std::endl;
    }
}