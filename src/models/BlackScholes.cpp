//
// Created by James Wirth on 13/11/2024.
//

#include <OptionLib/models/BlackScholes.h>
#include <cmath>
#include <stdexcept>
#include <limits>
#include <algorithm>

double approxErfInv(double x) {
    const double a = 0.147;  // Constant for the approximation
    double ln_term = std::log(1 - x * x);
    double pi_term = (2 / (M_PI * a)) + (ln_term / 2);

    return std::copysign(std::sqrt(std::sqrt(pi_term * pi_term - ln_term / a) - pi_term), x);
}

namespace OptionLib::Models {

    double normalCDF(double value) {
        return 0.5 * std::erfc(-value / std::sqrt(2));
    }

    double normalPDF(double value) {
        return std::exp(-0.5 * value * value) / std::sqrt(2 * M_PI);
    }

    double BlackScholes::price(const Option& option) const {

        auto asset = option.getAsset();
        double K = option.getStrikePrice();
        double T = option.getTimeToExpiry();

        double d1 = (std::log(asset->getSpotPrice() / K) + (asset->get(Param::riskFreeRate) + 0.5 * asset->get(Param::volatility) * asset->get(Param::volatility)) * T) / (asset->get(Param::volatility) * std::sqrt(T));
        double d2 = d1 - asset->get(Param::volatility) * std::sqrt(T);

        if (option.getType() == OptionType::Call) {
            return asset->getSpotPrice() * normalCDF(d1) - K * std::exp(-asset->get(Param::riskFreeRate) * T) * normalCDF(d2);
        } else if (option.getType() == OptionType::Put) {
            return K * std::exp(-asset->get(Param::riskFreeRate) * T) * normalCDF(-d2) - asset->getSpotPrice() * normalCDF(-d1);
        } else {
            throw std::invalid_argument("Unknown option type.");
        }
    }

    double BlackScholes::computeGreek(const Option& option, GreekType greekType) const {
        switch (greekType) {
            case GreekType::Delta: return calculateDelta(option);
            case GreekType::Gamma: return calculateGamma(option);
            case GreekType::Vega: return calculateVega(option);
            case GreekType::Theta: return calculateTheta(option);
            case GreekType::Rho: return calculateRho(option);
            default:
                throw std::invalid_argument("Invalid Greek type");
        }
    }

    double BlackScholes::calculateDelta(const Option& option) {
        auto asset = option.getAsset();
        double K = option.getStrikePrice();
        double T = option.getTimeToExpiry();
        double d1 = (std::log(asset->getSpotPrice() / K) + (asset->get(Param::riskFreeRate) + 0.5 * asset->get(Param::volatility) * asset->get(Param::volatility)) * T) /
                    (asset->get(Param::volatility) * std::sqrt(T));
        if (option.getType() == OptionType::Call) {
            return normalCDF(d1);
        } else {
            return normalCDF(d1) - 1;
        }
    }

    double BlackScholes::calculateGamma(const Option& option) {
        auto asset = option.getAsset();
        double K = option.getStrikePrice();
        double T = option.getTimeToExpiry();
        double d1 = (std::log(asset->getSpotPrice() / K) + (asset->get(Param::riskFreeRate) + 0.5 * asset->get(Param::volatility) * asset->get(Param::volatility)) * T) /
                    (asset->get(Param::volatility) * std::sqrt(T));
        return normalPDF(d1) / (asset->getSpotPrice() * asset->get(Param::volatility) * std::sqrt(T));
    }

    double BlackScholes::calculateVega(const Option& option) {
        auto asset = option.getAsset();
        double K = option.getStrikePrice();
        double T = option.getTimeToExpiry();
        double d1 = (std::log(asset->getSpotPrice() / K) + (asset->get(Param::riskFreeRate) + 0.5 * asset->get(Param::volatility) * asset->get(Param::volatility)) * T) /
                    (asset->get(Param::volatility) * std::sqrt(T));
        return asset->getSpotPrice() * normalPDF(d1) * std::sqrt(T);
    }

    double BlackScholes::calculateTheta(const Option& option) {
        auto asset = option.getAsset();
        double K = option.getStrikePrice();
        double T = option.getTimeToExpiry();
        double d1 = (std::log(asset->getSpotPrice() / K) + (asset->get(Param::riskFreeRate) + 0.5 * asset->get(Param::volatility) * asset->get(Param::volatility)) * T) /
                    (asset->get(Param::volatility) * std::sqrt(T));
        double d2 = d1 - asset->get(Param::volatility) * std::sqrt(T);
        if (option.getType() == OptionType::Call) {
            return -asset->getSpotPrice() * normalPDF(d1) * asset->get(Param::volatility) / (2 * std::sqrt(T)) - asset->get(Param::riskFreeRate) * K * std::exp(-asset->get(Param::riskFreeRate) * T) * normalCDF(d2);
        } else {
            return -asset->getSpotPrice() * normalPDF(d1) * asset->get(Param::volatility) / (2 * std::sqrt(T)) + asset->get(Param::riskFreeRate) * K * std::exp(-asset->get(Param::riskFreeRate) * T) * normalCDF(-d2);
        }
    }

    double BlackScholes::calculateRho(const Option& option) {
        auto asset = option.getAsset();
        double K = option.getStrikePrice();
        double T = option.getTimeToExpiry();
        double d2 = (std::log(asset->getSpotPrice() / K) + (asset->get(Param::riskFreeRate) - 0.5 * asset->get(Param::volatility) * asset->get(Param::volatility)) * T) /
                    (asset->get(Param::volatility) * std::sqrt(T));
        if (option.getType() == OptionType::Call) {
            return K * T * std::exp(-asset->get(Param::riskFreeRate) * T) * normalCDF(d2);
        } else {
            return -K * T * std::exp(-asset->get(Param::riskFreeRate) * T) * normalCDF(-d2);
        }
    }

    double BlackScholes::VaR(const Option& option, double confidenceLevel, double holdingPeriod) const {
        auto asset = option.getAsset();
        double optionPrice = price(option);
        double adjustedVolatility = asset->get(Param::volatility) * std::sqrt(holdingPeriod);

        // Calculate the Z-score for the specified confidence level
        double zScore = approxErfInv(2 * confidenceLevel - 1) * std::sqrt(2);  // Using Boost's erf_inv

        // Calculate VaR as the expected loss at the confidence level
        return optionPrice * (1 - std::exp(-zScore * adjustedVolatility));
    }

    double BlackScholes::ExpectedShortfall(const Option& option, double confidenceLevel, double holdingPeriod) const {
        double VaR = this->VaR(option, confidenceLevel, holdingPeriod);

        auto asset = option.getAsset();
        // Mean Excess Loss beyond VaR
        double optionPrice = price(option);
        double adjustedVolatility = asset->get(Param::volatility) * std::sqrt(holdingPeriod);

        // Expected Shortfall calculation (adjusted for Black-Scholes assumptions)
        double meanExcessLoss = optionPrice * adjustedVolatility * approxErfInv(2 * confidenceLevel - 1) / std::sqrt(M_PI);
        return VaR + meanExcessLoss;
    }


} // namespace OptionLib::Models