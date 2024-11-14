//
// Created by James Wirth on 13/11/2024.
//

#ifndef BINOMIAL_H
#define BINOMIAL_H

#include "Model.h"
#include <cmath>

namespace OptionLib::Models {

    class Binomial : public Model {
    public:
        explicit Binomial() = default;

        [[nodiscard]] double price(const Option& option) const override;
        [[nodiscard]] double computeGreek(const Option& option, GreekType greekType) const override;

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

#endif //BINOMIAL_H
