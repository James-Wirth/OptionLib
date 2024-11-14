//
// Created by James Wirth on 13/11/2024.
//

#ifndef OPTION_H
#define OPTION_H

#include <string>
#include <memory>
#include "Asset.h"

namespace OptionLib {

    enum class OptionType {
        Call,
        Put
    };

    class Option {
    public:
        Option(std::shared_ptr<Asset> asset, double strikePrice, double timeToExpiry, OptionType type);

        std::shared_ptr<Asset> getAsset() const;
        double getStrikePrice() const;
        double getTimeToExpiry() const;
        OptionType getType() const;

        void setStrikePrice(double newStrikePrice);
        void setTimeToExpiry(double newTimeToExpiry);
        void setType(OptionType newType);

        std::string typeToString() const;

    private:
        std::shared_ptr<Asset> asset;
        double strikePrice;
        double timeToExpiry;
        OptionType type;
    };

} // namespace OptionLib

#endif //OPTION_H
