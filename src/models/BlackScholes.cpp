//
// Created by James Wirth on 13/11/2024.
//

#include <OptionLib/models/BlackScholes.h>
#include <cmath>
#include <stdexcept>

namespace OptionLib::Models {

    double cumulativeNormal(double x) {
        return 0.5 * (1.0 + std::erf(x / std::sqrt(2.0)));
    }

    double BlackScholes::price(const Option& option) const {
        double K = option.getStrikePrice();
        double T = option.getTimeToExpiry();

        double d1 = (std::log(spotPrice / K) + (riskFreeRate + 0.5 * volatility * volatility) * T) / (volatility * std::sqrt(T));
        double d2 = d1 - volatility * std::sqrt(T);

        if (option.getType() == OptionType::Call) {
            return spotPrice * cumulativeNormal(d1) - K * std::exp(-riskFreeRate * T) * cumulativeNormal(d2);
        } else if (option.getType() == OptionType::Put) {
            return K * std::exp(-riskFreeRate * T) * cumulativeNormal(-d2) - spotPrice * cumulativeNormal(-d1);
        } else {
            throw std::invalid_argument("Unknown option type.");
        }
    }

} // namespace OptionLib::Models