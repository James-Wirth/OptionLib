//
// Created by James Wirth on 15/11/2024.
//

#include "OptionLib/models/Heston.h"
#include <cmath>
#include <complex>
#include <stdexcept>
#include <iostream>

namespace OptionLib::Models {

    using namespace std;
    using namespace std::complex_literals;

    // Convenience function for necessary Heston parameters
    auto getHestonParameters(const Asset& asset, const Option& option) {
        return std::make_tuple(
            asset.get(Param::meanReversion),                                // kappa
            asset.get(Param::longTermVariance),                             // theta
            asset.get(Param::volOfVol),                                     // zeta
            asset.get(Param::hestonCorrelation),                            // rho
            asset.get(Param::volatility) * asset.get(Param::volatility),    // v0
            asset.get(Param::riskFreeRate),                                 // r
            option.getTimeToExpiry(),                                       // T
            option.getStrikePrice(),                                        // K
            asset.getSpotPrice()                                            // S
        );
    }

    std::complex<double> Heston::characteristicFunction(const std::complex<double>& u, const Option& option, const Asset& asset) {
        using namespace std::complex_literals;
        auto [kappa, theta, zeta, rho, v0, r, T, K, S] = getHestonParameters(asset, option);
        double m = log(S) + r * T;
        complex<double> D = sqrt(pow(rho * zeta * 1i * u - kappa, 2) + pow(zeta, 2) * (1i * u + pow(u, 2)));
        complex<double> C = (kappa - rho*zeta*1i*u - D)/(kappa-rho*zeta*1i*u + D);
        complex<double> beta = ((kappa - rho*zeta*1i*u - D)*(1.0-exp(-D*T))) / (pow(zeta,2)*(1.0-C*exp(-D*T)));
        complex<double> alpha = ((kappa*theta)/(pow(zeta,2))) * ((kappa-rho*zeta*1i*u-D)*T - 2.0*log((1.0-C*exp(-D*T))/(1.0-C)));
        return exp(1i*u*m + alpha + beta*v0);
    }

    // Fourier implementation of Heston price
    double Heston::price(const Option& option) const {
        double z = 24;
        double N = 1021;
        auto [kappa, theta, zeta, rho, v0, r, T, K, S] = getHestonParameters(*option.getAsset(), option);
        double c1 = log(S) + r * T - 0.5 * theta * T;
        double c2 = theta / (8 * pow(kappa, 3)) *
                    (-pow(zeta, 2) * exp(-2 * kappa * T)
                     + 4 * zeta * exp(-kappa * T) * (zeta - 2 * kappa * rho)
                     + 2 * kappa * T * (4 * pow(kappa, 2) + pow(zeta, 2) - 4 * kappa * zeta * rho)
                     + zeta * (8 * kappa * rho - 3 * zeta));

        double a = c1 - z * sqrt(abs(c2));
        double b = c1 + z * sqrt(abs(c2));
        auto h = [&](int n) { return (n * M_PI) / (b - a); };
        auto g_n = [&](int n) {
            double h_n = h(n);
            return (exp(a) - (K / h_n) * sin(h_n * (a - log(K))) - K * cos(h_n * (a - log(K)))) / (1 + pow(h_n, 2));
        };
        double g0 = K * (log(K) - a - 1) + exp(a);

        complex F = g0;
        for (int n = 1; n <= N; ++n) {
            double h_n = h(n);
            F += 2.0 * characteristicFunction(h_n, option, *option.getAsset()) * exp(complex<double>(0, -1) * a * h_n) * g_n(n);
        }
        double F_real = exp(-r * T) / (b - a) * real(F);

        if (option.getType() == OptionType::Call) {
            // Put-call parity
            F_real += S - K * exp(-r * T);
        }
        return max(0.0, F_real);
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
