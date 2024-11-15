//
// Created by James Wirth on 15/11/2024.
//

#include "OptionLib/models/Heston.h"
#include <cmath>
#include <complex>
#include <vector>
#include <stdexcept>
#include <iostream>

namespace OptionLib::Models {

    using namespace std::complex_literals;

    std::complex<double> Heston::characteristicFunction(const std::complex<double>& u, const Option& option, const Asset& asset) {
        using namespace std::complex_literals;

        // Retrieve parameters
        double kappa = asset.get(Param::meanReversion);
        double theta = asset.get(Param::longTermVariance);
        double zeta = asset.get(Param::volOfVol);
        double rho = asset.get(Param::hestonCorrelation);
        double v0 = asset.get(Param::volatility) * asset.get(Param::volatility);
        double r = asset.get(Param::riskFreeRate);
        double T = option.getTimeToExpiry();
        double S = asset.getSpotPrice();
        std::complex i(0.0, 1.0);

        double m = std::log(S) + r * T;
        std::complex<double> D = std::sqrt(
            std::pow(rho * zeta * 1i * u - kappa, 2) + std::pow(zeta, 2) * (1i * u + std::pow(u, 2)));
        std::complex<double> C = (kappa - rho*zeta*1i*u - D)/(kappa-rho*zeta*1i*u + D);

        std::complex<double> beta = ((kappa - rho*zeta*1i*u - D)*(1.0-std::exp(-D*T))) / (std::pow(zeta,2)*(1.0-C*std::exp(-D*T)));
        std::complex<double> alpha = ((kappa*theta)/(std::pow(zeta,2))) * ((kappa-rho*zeta*1i*u-D)*T - 2.0*std::log((1.0-C*std::exp(-D*T))/(1.0-C)));

        return std::exp(1i*u*m + alpha + beta*v0);
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
