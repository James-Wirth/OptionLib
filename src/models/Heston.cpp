//
// Created by James Wirth on 15/11/2024.
//

#include "OptionLib/models/Heston.h"
#include <cmath>
#include <complex>
#include <vector>
#include <stdexcept>
#include <boost/math/quadrature/trapezoidal.hpp>
#include <boost/math/constants/constants.hpp>
#include <Eigen/Dense>
#include <iostream>

namespace OptionLib::Models {

    using namespace Eigen;
    using namespace std::complex_literals;

    std::complex<double> Heston::characteristicFunction(const std::complex<double>& u, const Option& option, const Asset& asset) {
        using namespace std::complex_literals;

        // Retrieve parameters
        double kappa = asset.get(Param::meanReversion);
        double theta = asset.get(Param::longTermVariance);
        double sigma = asset.get(Param::volOfVol);
        double rho = asset.get(Param::hestonCorrelation);
        double v0 = asset.get(Param::volatility) * asset.get(Param::volatility);
        double r = asset.get(Param::riskFreeRate);
        double T = option.getTimeToExpiry();
        double S = asset.getSpotPrice();

        std::complex<double> i = 1i;
        std::complex<double> alpha = -u * u * 0.5 - i * u * 0.5;
        std::complex<double> beta = kappa - rho * sigma * i * u;
        std::complex<double> gamma = sigma * sigma * 0.5;

        std::complex<double> D = std::sqrt(beta * beta - 4.0 * alpha * gamma);
        std::complex<double> G = (beta - D) / (beta + D);

        std::complex<double> C = r * i * u * T + (kappa * theta / (sigma * sigma)) * ((beta - D) * T - 2.0 * std::log((1.0 - G * std::exp(-D * T)) / (1.0 - G)));
        std::complex<double> D_term = ((beta - D) / (sigma * sigma)) * ((1.0 - std::exp(-D * T)) / (1.0 - G * std::exp(-D * T)));

        return std::exp(C + D_term * v0 + i * u * std::log(S));
    }


    double Heston::price(const Option& option) const {
        return 0.0;
    }


    double Heston::computeGreek(const Option& option, GreekType greekType) const {
        switch (greekType) {
            case GreekType::Delta:
                return calculateDelta(option);
            case GreekType::Gamma:
                return calculateGamma(option);
            case GreekType::Vega:
                return calculateVega(option);
            case GreekType::Theta:
                return calculateTheta(option);
            case GreekType::Rho:
                return calculateRho(option);
            default:
                throw std::invalid_argument("Unsupported Greek type");
        }
    }

    double Heston::VaR(const Option& option, double confidenceLevel, double holdingPeriod) const {
        // Simulate paths and calculate VaR based on option payoff
        return 0.0; // Placeholder
    }

    double Heston::ExpectedShortfall(const Option& option, double confidenceLevel, double holdingPeriod) const {
        // Simulate paths and calculate Expected Shortfall based on option payoff
        return 0.0; // Placeholder
    }

    double Heston::calculateDelta(const Option& option) {
        // Approximate or calculate delta using finite differences or formula
        return 0.0; // Placeholder
    }

    double Heston::calculateGamma(const Option& option) {
        // Approximate or calculate gamma using finite differences or formula
        return 0.0; // Placeholder
    }

    double Heston::calculateVega(const Option& option) {
        // Approximate or calculate vega using finite differences or formula
        return 0.0; // Placeholder
    }

    double Heston::calculateTheta(const Option& option) {
        // Approximate or calculate theta using finite differences or formula
        return 0.0; // Placeholder
    }

    double Heston::calculateRho(const Option& option) {
        // Approximate or calculate rho using finite differences or formula
        return 0.0; // Placeholder
    }

} // namespace OptionLib::Models
