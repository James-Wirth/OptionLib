//
// Created by James Wirth on 14/11/2024.
//

#include <string>
#include <OptionLib/Asset.h>

namespace OptionLib {

    Asset::Asset(std::string id, double spotPrice, double volatility, double riskFreeRate)
        : id(std::move(id)), spotPrice(spotPrice), volatility(volatility), riskFreeRate(riskFreeRate) {}

    std::string Asset::getId() const {
        return id;
    }

    double Asset::getSpotPrice() const {
        return spotPrice;
    }

    double Asset::getVolatility() const {
        return volatility;
    }

    double Asset::getRiskFreeRate() const {
        return riskFreeRate;
    }

    void Asset::setSpotPrice(double newPrice) {
        spotPrice = newPrice;
    }

    void Asset::setVolatility(double newVolatility) {
        volatility = newVolatility;
    }

    void Asset::setRiskFreeRate(double newRate) {
        riskFreeRate = newRate;
    }

} // namespace OptionLib
