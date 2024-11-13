//
// Created by James Wirth on 13/11/2024.
//

#include <OptionLib/models/BlackScholes.h>
#include <cmath>
#include <stdexcept>

namespace OptionLib::Models {

    double normalCDF(double value) {
        return 0.5 * std::erfc(-value / std::sqrt(2));
    }

    double normalPDF(double value) {
        return std::exp(-0.5 * value * value) / std::sqrt(2 * M_PI);
    }

    double BlackScholes::price(const Option& option) const {
        double K = option.getStrikePrice();
        double T = option.getTimeToExpiry();

        double d1 = (std::log(spotPrice / K) + (riskFreeRate + 0.5 * volatility * volatility) * T) / (volatility * std::sqrt(T));
        double d2 = d1 - volatility * std::sqrt(T);

        if (option.getType() == OptionType::Call) {
            return spotPrice * normalCDF(d1) - K * std::exp(-riskFreeRate * T) * normalCDF(d2);
        } else if (option.getType() == OptionType::Put) {
            return K * std::exp(-riskFreeRate * T) * normalCDF(-d2) - spotPrice * normalCDF(-d1);
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

    double BlackScholes::calculateDelta(const Option& option) const {
        double K = option.getStrikePrice();
        double T = option.getTimeToExpiry();
        double d1 = (std::log(spotPrice / K) + (riskFreeRate + 0.5 * volatility * volatility) * T) /
                    (volatility * std::sqrt(T));
        if (option.getType() == OptionType::Call) {
            return normalCDF(d1);
        } else {
            return normalCDF(d1) - 1;
        }
    }

    double BlackScholes::calculateGamma(const Option& option) const {
        double K = option.getStrikePrice();
        double T = option.getTimeToExpiry();
        double d1 = (std::log(spotPrice / K) + (riskFreeRate + 0.5 * volatility * volatility) * T) /
                    (volatility * std::sqrt(T));
        return normalPDF(d1) / (spotPrice * volatility * std::sqrt(T));
    }

    double BlackScholes::calculateVega(const Option& option) const {
        double K = option.getStrikePrice();
        double T = option.getTimeToExpiry();
        double d1 = (std::log(spotPrice / K) + (riskFreeRate + 0.5 * volatility * volatility) * T) /
                    (volatility * std::sqrt(T));
        return spotPrice * normalPDF(d1) * std::sqrt(T);
    }

    double BlackScholes::calculateTheta(const Option& option) const {
        double K = option.getStrikePrice();
        double T = option.getTimeToExpiry();
        double d1 = (std::log(spotPrice / K) + (riskFreeRate + 0.5 * volatility * volatility) * T) /
                    (volatility * std::sqrt(T));
        double d2 = d1 - volatility * std::sqrt(T);
        if (option.getType() == OptionType::Call) {
            return -spotPrice * normalPDF(d1) * volatility / (2 * std::sqrt(T)) - riskFreeRate * K * std::exp(-riskFreeRate * T) * normalCDF(d2);
        } else {
            return -spotPrice * normalPDF(d1) * volatility / (2 * std::sqrt(T)) + riskFreeRate * K * std::exp(-riskFreeRate * T) * normalCDF(-d2);
        }
    }

    double BlackScholes::calculateRho(const Option& option) const {
        double K = option.getStrikePrice();
        double T = option.getTimeToExpiry();
        double d2 = (std::log(spotPrice / K) + (riskFreeRate - 0.5 * volatility * volatility) * T) /
                    (volatility * std::sqrt(T));
        if (option.getType() == OptionType::Call) {
            return K * T * std::exp(-riskFreeRate * T) * normalCDF(d2);
        } else {
            return -K * T * std::exp(-riskFreeRate * T) * normalCDF(-d2);
        }
    }


} // namespace OptionLib::Models