//
// Created by James Wirth on 13/11/2024.
//

#ifndef MONTECARLO_H
#define MONTECARLO_H

#include "Model.h"
#include <random>

namespace OptionLib::Models {

    class MonteCarlo : public Model {
    public:
        MonteCarlo(double spotPrice, double riskFreeRate, double volatility, int numSimulations = 10000)
            : Model(spotPrice, riskFreeRate, volatility), numSimulations(numSimulations) {}

        double price(const Option& option) const override;

    private:
        int numSimulations;
    };

} // namespace OptionLib::Models

#endif //MONTECARLO_H
