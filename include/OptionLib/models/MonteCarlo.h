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
        explicit MonteCarlo() = default;

        [[nodiscard]] double price(const Option& option) const override;
        [[nodiscard]] double computeGreek(const Option& option, GreekType greekType) const override;

        // Implement VaR and Expected Shortfall with Monte Carlo
        [[nodiscard]] double VaR(const Option& option, double confidenceLevel, double holdingPeriod) const override;
        [[nodiscard]] double ExpectedShortfall(const Option& option, double confidenceLevel, double holdingPeriod) const override;


    private:
        static double calculateDelta(const Option& option);
        static double calculateGamma(const Option& option);
        static double calculateVega(const Option& option);
        static double calculateTheta(const Option& option);
        static double calculateRho(const Option& option);

        static double priceWrapper(const Option& option, double spotPrice, double riskFreeRate, double volatility,
                                   double strikePrice, double timeToMaturity, int numSimulations);
    };

} // namespace OptionLib::Models

#endif //MONTECARLO_H
