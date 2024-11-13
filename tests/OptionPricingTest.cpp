//
// Created by James Wirth on 13/11/2024.
//

#include <gtest/gtest.h>
#include <cmath>
#include <OptionLib/OptionLib.h>

constexpr double TOLERANCE = 1e-2;

TEST(OptionPricingTest, ModelComparison) {
    using namespace OptionLib;
    using namespace OptionLib::Models;

    double spotPrice = 100.0;
    double strikePrice = 100.0;
    double timeToExpiry = 1.0;
    double riskFreeRate = 0.05;
    double volatility = 0.2;

    Option callOption(strikePrice, timeToExpiry, OptionType::Call);
    Option putOption(strikePrice, timeToExpiry, OptionType::Put);

    double expectedCallPrice = 10.45;
    double expectedPutPrice = 5.57;

    std::vector<std::unique_ptr<Model>> models;
    models.emplace_back(std::make_unique<BlackScholes>(spotPrice, riskFreeRate, volatility));
    models.emplace_back(std::make_unique<Binomial>(spotPrice, riskFreeRate, volatility, pow(10, 4)));
    models.emplace_back(std::make_unique<MonteCarlo>(spotPrice, riskFreeRate, volatility, pow(10, 7)));

    for (const auto& model : models) {
        double callPrice = model->price(callOption);
        double putPrice = model->price(putOption);

        EXPECT_NEAR(callPrice, expectedCallPrice, TOLERANCE) << "Call price mismatch for model";
        EXPECT_NEAR(putPrice, expectedPutPrice, TOLERANCE) << "Put price mismatch for model";
    }
}