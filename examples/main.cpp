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
    AssetSP asset = Factory::makeSharedAsset("AAPL", 100.0);

    asset->set(Param::volatility, 0.2);
    asset->set(Param::riskFreeRate, 0.05);
    asset->set(Param::meanReversion, 1.0);
    asset->set(Param::volOfVol, 0.3);
    asset->set(Param::longTermVariance, 0.04);
    asset->set(Param::hestonCorrelation, -0.4);

    OptionSP callOption = Factory::makeSharedOption(asset, 100, 1.0, OptionType::Call);
    OptionSP putOption = Factory::makeSharedOption(asset, 100, 1.0, OptionType::Put);

    ModelSP defaultModel = Factory::makeSharedModel<Heston>();

    Portfolio portfolio(defaultModel);
    portfolio.addOption(callOption);
    portfolio.addOption(putOption);

    // Output the total portfolio value
    double portfolioValue = portfolio.totalValue();
    std::cout << "\nTotal Portfolio Value: " << portfolioValue << std::endl;

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
        std::cout << "\n" << greekTypeName(greek) << " Vector: ";
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
    std::cout << "\nPortfolio Concentration:\n";
    for (const auto& [optionId, concentration] : concentrations) {
        std::cout << optionId << ": " << concentration * 100 << "%\n";
    }

    double confidenceLevel = 0.95;
    double holdingPeriod = 1.0 / 52; // Weekly holding period

    // Output portfolio VaR and Expected Shortfall
    double portfolioVaR = portfolio.VaR(confidenceLevel, holdingPeriod);
    double portfolioES = portfolio.ExpectedShortfall(confidenceLevel, holdingPeriod);
    std::cout << "\nPortfolio Risk Metrics (95% Confidence, Weekly Holding Period):\n";
    std::cout << "VaR: " << portfolioVaR << "\n";
    std::cout << "Expected Shortfall: " << portfolioES << "\n";
}