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

using namespace OptionLib;

int main() {
    double spotPrice = 100.0;
    double riskFreeRate = 0.05;
    double volatility = 0.2;

    auto defaultModel = Factory::createModel<MonteCarlo>(spotPrice, riskFreeRate, volatility);
    Portfolio portfolio(defaultModel);

    auto callOption = Factory::createOption(100, 1.0, OptionType::Call);
    auto putOption = Factory::createOption(100, 1.0, OptionType::Put);

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

    // Calculate concentration measures
    auto concentrations = portfolio.concentrationMeasures();

    // Output the concentration of each option in the portfolio
    std::cout << "Portfolio Concentration Measures:\n";
    for (const auto& [optionId, concentration] : concentrations) {
        std::cout << "Option " << optionId << ": " << concentration * 100 << "%\n";
    }

    double confidenceLevel = 0.95;
    double holdingPeriod = 1.0 / 52; // Weekly holding period

    // Output portfolio VaR and Expected Shortfall
    double portfolioVaR = portfolio.VaR(confidenceLevel, holdingPeriod);
    double portfolioES = portfolio.ExpectedShortfall(confidenceLevel, holdingPeriod);
    std::cout << "\nPortfolio Risk Metrics (95% Confidence, Weekly Holding Period):\n";
    std::cout << "  VaR: " << portfolioVaR << "\n";
    std::cout << "  Expected Shortfall: " << portfolioES << "\n";


    // auto sensitivities = portfolio.sensitivityAnalysis(0.01, 0.02);  // 1% spot change, 2% volatility change
    // for (const auto& [option, sensitivity] : sensitivities) {
    //     std::cout << option << " sensitivity: " << sensitivity << std::endl;
    // }
}