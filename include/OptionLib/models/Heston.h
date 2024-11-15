//
// Created by James Wirth on 15/11/2024.
//

#ifndef HESTON_H
#define HESTON_H

#include "Model.h"
#include <complex>

namespace OptionLib::Models {

    class Heston : public Model {
    public:
        Heston() = default;

        static std::complex<double> characteristicFunction(const std::complex<double>& u, const Option& option, const Asset& asset) ;

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

#endif //HESTON_H
