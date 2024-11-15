//
// Created by James Wirth on 15/11/2024.
//

#include <gtest/gtest.h>
#include <complex>
#include <cmath>
#include <limits>
#include <OptionLib/OptionLib.h>

using namespace OptionLib;
using namespace OptionLib::Models;

class HestonTestHelper {
public:
    AssetSP asset;
    OptionSP option;
    HestonTestHelper() {
        asset = Factory::makeSharedAsset("AAPL", 100.0);
        asset->set(Param::volatility, 0.2);
        asset->set(Param::riskFreeRate, 0.03);
        asset->set(Param::meanReversion, 2.0);
        asset->set(Param::volOfVol, 0.5);
        asset->set(Param::longTermVariance, 0.04);
        asset->set(Param::hestonCorrelation, -0.7);
        option = Factory::makeSharedOption(asset, 100, 1.0, OptionType::Call);
    }
    [[nodiscard]] AssetSP getAsset() const {
        return asset;
    }
    [[nodiscard]] OptionSP getOption() const {
        return option;
    }
};

TEST(HestonCharacteristicFunction, ZeroImaginaryPart) {
    HestonTestHelper helper;

    // Test u = 0 (real part of the characteristic function, should equal 1)
    std::complex<double> u(0.0, 0.0);
    auto result = Heston::characteristicFunction(u, *helper.getOption(), *helper.getAsset());
    EXPECT_NEAR(result.real(), 1.0, 1e-6);
    EXPECT_NEAR(result.imag(), 0.0, 1e-6);
}

TEST(HestonCharacteristicFunction, ExpectedSymmetry) {
    HestonTestHelper helper;

    // Test conjugate symmetry: characteristicFunction(-u*) = conj(characteristicFunction(u))
    std::complex<double> u(0.5, 0.5);
    auto result = Heston::characteristicFunction(u, *helper.getOption(), *helper.getAsset());
    auto conjugate_result = Heston::characteristicFunction(-std::conj(u), *helper.getOption(), *helper.getAsset());

    EXPECT_NEAR(result.real(), conjugate_result.real(), 1e-6);
    EXPECT_NEAR(result.imag(), -conjugate_result.imag(), 1e-6);
}

TEST(HestonCharacteristicFunction, KnownParametersValidation) {
    HestonTestHelper helper;

    // Test against known parameters with pre-computed result
    std::complex<double> u(0.1, 0.2);

    // Expected computed result
    std::complex<double> expected_result(0.356143, 0.176719);

    auto result = Heston::characteristicFunction(u, *helper.getOption(), *helper.getAsset());

    EXPECT_NEAR(result.real(), expected_result.real(), 1e-4);
    EXPECT_NEAR(result.imag(), expected_result.imag(), 1e-4);
}

TEST(HestonCharacteristicFunction, NumericalStability) {
    HestonTestHelper helper;

    // Test for large |u| to ensure numerical stability
    std::complex<double> u(10.0, 10.0);
    auto result = Heston::characteristicFunction(u, *helper.getOption(), *helper.getAsset());

    // Ensure the function does not return NaN or Inf
    EXPECT_FALSE(std::isnan(result.real()));
    EXPECT_FALSE(std::isnan(result.imag()));
    EXPECT_FALSE(std::isinf(result.real()));
    EXPECT_FALSE(std::isinf(result.imag()));
}