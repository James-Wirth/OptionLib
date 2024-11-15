//
// Created by James Wirth on 14/11/2024.
//

#ifndef ASSET_H
#define ASSET_H

#include <unordered_map>

namespace OptionLib {

    // Define possible parameter keys as an enum
    enum class Param {
        volatility,
        riskFreeRate,
        meanReversion,
        volOfVol,
        longTermVariance,
        hestonCorrelation,
    };

    class Asset {
    public:
        Asset(std::string id, double spotPrice);

        // Getters
        [[nodiscard]] std::string getId() const;
        [[nodiscard]] double getSpotPrice() const;

        // Optional parameters setters and getters
        void set(Param param, double value);
        [[nodiscard]] double get(Param param) const;

    private:
        std::string id;          // Unique identifier for the asset (e.g., ticker symbol)
        double spotPrice;
        std::unordered_map<Param, double> parameters;
    };

} // namespace OptionLib

#endif //ASSET_H
