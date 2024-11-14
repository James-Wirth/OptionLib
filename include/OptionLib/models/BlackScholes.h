//
// Created by James Wirth on 13/11/2024.
//

#ifndef BLACKSCHOLES_H
#define BLACKSCHOLES_H

#include "Model.h"

namespace OptionLib::Models {

    class BlackScholes : public Model {
    public:
        BlackScholes() = default;

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
    };

} // namespace OptionLib::Models

#endif //BLACKSCHOLES_H
