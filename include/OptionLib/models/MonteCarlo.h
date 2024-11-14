//
// Created by James Wirth on 13/11/2024.
//

#ifndef MONTECARLO_H
#define MONTECARLO_H

#include "Model.h"
#include <random>
#include <cmath>

namespace OptionLib::Models {

    class MonteCarlo : public Model {
    public:
        MonteCarlo(double spotPrice, double riskFreeRate, double volatility, int numSimulations = std::pow(10, 7))
            : Model(spotPrice, riskFreeRate, volatility), numSimulations(numSimulations) {}

        double price(const Option& option) const override;
        double computeGreek(const Option& option, GreekType greekType) const override;

        // Implement VaR and Expected Shortfall with Monte Carlo
        double VaR(const Option& option, double confidenceLevel, double holdingPeriod) const override;
        double ExpectedShortfall(const Option& option, double confidenceLevel, double holdingPeriod) const override;


    private:
        int numSimulations;
        double calculateDelta(const Option& option) const;
        double calculateGamma(const Option& option) const;
        double calculateVega(const Option& option) const;
        double calculateTheta(const Option& option) const;
        double calculateRho(const Option& option) const;

        static double priceWrapper(const Option& option, double spotPrice, double riskFreeRate, double volatility,
                                   double strikePrice, double timeToMaturity, int numSimulations);
    };

} // namespace OptionLib::Models

#endif //MONTECARLO_H
