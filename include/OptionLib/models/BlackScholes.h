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
        double computeGreek(const Option& option, GreekType greekType) const override;

        double VaR(const Option& option, double confidenceLevel, double holdingPeriod) const override;
        double ExpectedShortfall(const Option& option, double confidenceLevel, double holdingPeriod) const override;


    private:
        double calculateDelta(const Option& option) const;
        double calculateGamma(const Option& option) const;
        double calculateVega(const Option& option) const;
        double calculateTheta(const Option& option) const;
        double calculateRho(const Option& option) const;
    };

} // namespace OptionLib::Models

#endif //BLACKSCHOLES_H
