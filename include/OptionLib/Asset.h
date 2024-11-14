//
// Created by James Wirth on 14/11/2024.
//

#ifndef ASSET_H
#define ASSET_H

namespace OptionLib {

    class Asset {
    public:
        Asset(std::string id, double spotPrice, double volatility, double riskFreeRate);

        // Getters
        [[nodiscard]] std::string getId() const;
        [[nodiscard]] double getSpotPrice() const;
        [[nodiscard]] double getVolatility() const;
        [[nodiscard]] double getRiskFreeRate() const;

        // Setters (for updating values if needed)
        void setSpotPrice(double newPrice);
        void setVolatility(double newVolatility);
        void setRiskFreeRate(double newRate);

    private:
        std::string id;          // Unique identifier for the asset (e.g., ticker symbol)
        double spotPrice;
        double volatility;
        double riskFreeRate;
    };

} // namespace OptionLib

#endif //ASSET_H
