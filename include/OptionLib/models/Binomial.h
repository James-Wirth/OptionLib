//
// Created by James Wirth on 13/11/2024.
//

#ifndef BINOMIAL_H
#define BINOMIAL_H

#include "Model.h"

namespace OptionLib::Models {

    class Binomial : public Model {
    public:
        Binomial(double spotPrice, double riskFreeRate, double volatility, int numSteps = 100)
            : Model(spotPrice, riskFreeRate, volatility), numSteps(numSteps) {}

        double price(const Option& option) const override;
        double computeGreek(const Option& option, GreekType greekType) const override;

    private:
        int numSteps;
        double calculateDelta(const Option& option) const;
        double calculateGamma(const Option& option) const;
        double calculateVega(const Option& option) const;
        double calculateTheta(const Option& option) const;
        double calculateRho(const Option& option) const;
    };

} // namespace OptionLib::Models

#endif //BINOMIAL_H
