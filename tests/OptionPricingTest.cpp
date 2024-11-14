//
// Created by James Wirth on 13/11/2024.
//

#include <gtest/gtest.h>
#include <cmath>
#include <OptionLib/OptionLib.h>

constexpr double TOLERANCE = 2e-2;

TEST(OptionPricingTest, ModelComparison) {
    using namespace OptionLib;
    using namespace OptionLib::Models;

    double strikePrice = 100.0;
    double timeToExpiry = 1.0;

    auto asset = std::make_shared<Asset>("AAPL", 100.0);
    asset->set(Param::volatility, 0.2);
    asset->set(Param::riskFreeRate, 0.05);

    Option callOption = Option(asset, strikePrice, timeToExpiry, OptionType::Call);
    Option putOption = Option(asset, strikePrice, timeToExpiry, OptionType::Put);

    double expectedCallPrice = 10.45;
    double expectedPutPrice = 5.57;

    std::vector<std::unique_ptr<Model>> models;
    models.emplace_back(std::make_unique<BlackScholes>());
    models.emplace_back(std::make_unique<Binomial>());
    models.emplace_back(std::make_unique<MonteCarlo>());

    for (const auto& model : models) {
        double callPrice = model->price(callOption);
        double putPrice = model->price(putOption);

        EXPECT_NEAR(callPrice, expectedCallPrice, TOLERANCE) << "Call price mismatch for model";
        EXPECT_NEAR(putPrice, expectedPutPrice, TOLERANCE) << "Put price mismatch for model";
    }
}