//
// Created by James Wirth on 13/11/2024.
//

#ifndef MODEL_H
#define MODEL_H

#include <OptionLib/Option.h>

namespace OptionLib::Models {

    enum class GreekType {
        Delta,
        Gamma,
        Vega,
        Theta,
        Rho
    };

    class Model {
    public:
        Model(double spotPrice, double riskFreeRate, double volatility)
            : spotPrice(spotPrice), riskFreeRate(riskFreeRate), volatility(volatility) {}

        virtual ~Model();

        virtual double VaR(const Option& option, double confidenceLevel, double holdingPeriod) const = 0;
        virtual double ExpectedShortfall(const Option& option, double confidenceLevel, double holdingPeriod) const = 0;

        virtual double price(const Option& option) const = 0;
        virtual double computeGreek(const Option& option, GreekType type) const = 0;

    protected:
        double spotPrice;
        double riskFreeRate;
        double volatility;
    };

} // namespace OptionLib::Models

#endif //MODEL_H
