//
// Created by James Wirth on 13/11/2024.
//

#include <iostream>
#include "OptionLib/OptionLib.h"

int main() {
    using namespace OptionLib;
    using namespace OptionLib::Models;

    double spotPrice = 100.0;
    double riskFreeRate = 0.05;
    double volatility = 0.2;

    auto defaultModel = std::make_shared<BlackScholes>(spotPrice, riskFreeRate, volatility);
    Portfolio portfolio(defaultModel);

    auto callOption = std::make_shared<Option>(100, 1.0, OptionType::Call);
    auto putOption = std::make_shared<Option>(100, 1.0, OptionType::Put);

    portfolio.addOption(callOption);
    portfolio.addOption(putOption);

    auto binomialModel = std::make_shared<Binomial>(spotPrice, riskFreeRate, volatility);
    auto callOption2 = std::make_shared<Option>(100, 1.0, OptionType::Call);

    portfolio.addOption(callOption2, binomialModel); // Uses Binomial model specifically for this option

    std::cout << "Total Portfolio Value: " << portfolio.totalValue() << '\n';

    return 0;
}