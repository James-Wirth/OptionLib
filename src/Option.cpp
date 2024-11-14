//
// Created by James Wirth on 13/11/2024.
//

#include <OptionLib/Option.h>
#include <stdexcept>

namespace OptionLib {

    Option::Option(std::shared_ptr<Asset> asset, double strikePrice, double timeToExpiry, OptionType type)
        : asset(std::move(asset)), strikePrice(strikePrice), timeToExpiry(timeToExpiry), type(type) {
        if (strikePrice <= 0) {
            throw std::invalid_argument("Strike price must be positive.");
        }
        if (timeToExpiry <= 0) {
            throw std::invalid_argument("Time to expiry must be positive.");
        }
    }

    std::shared_ptr<Asset> Option::getAsset() const {
        return asset;
    }

    double Option::getStrikePrice() const {
        return strikePrice;
    }

    double Option::getTimeToExpiry() const {
        return timeToExpiry;
    }

    OptionType Option::getType() const {
        return type;
    }

    void Option::setStrikePrice(double newStrikePrice) {
        if (newStrikePrice <= 0) {
            throw std::invalid_argument("Strike price must be positive.");
        }
        strikePrice = newStrikePrice;
    }

    void Option::setTimeToExpiry(double newTimeToExpiry) {
        if (newTimeToExpiry <= 0) {
            throw std::invalid_argument("Time to expiry must be positive.");
        }
        timeToExpiry = newTimeToExpiry;
    }

    void Option::setType(OptionType newType) {
        type = newType;
    }

    std::string Option::typeToString() const {
        return (type == OptionType::Call) ? "Call" : "Put";
    }

} // namespace OptionLib