//
// Created by James Wirth on 13/11/2024.
//

#ifndef BLACKSCHOLES_H
#define BLACKSCHOLES_H

#include "Model.h"

namespace OptionLib::Models {

    class BlackScholes : public Model {
    public:
        BlackScholes(double spotPrice, double riskFreeRate, double volatility)
            : Model(spotPrice, riskFreeRate, volatility) {}

        double price(const Option& option) const override;
    };

} // namespace OptionLib::Models

#endif //BLACKSCHOLES_H
