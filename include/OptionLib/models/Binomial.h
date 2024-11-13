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

    private:
        int numSteps;
    };

} // namespace OptionLib::Models

#endif //BINOMIAL_H
