//
// Created by James Wirth on 13/11/2024.
//

#ifndef OPTION_H
#define OPTION_H

#include <string>

namespace OptionLib {

    enum class OptionType {
        Call,
        Put
    };

    class Option {
    public:
        Option(double strikePrice, double timeToExpiry, OptionType type);

        double getStrikePrice() const;
        double getTimeToExpiry() const;
        OptionType getType() const;

        void setStrikePrice(double newStrikePrice);
        void setTimeToExpiry(double newTimeToExpiry);
        void setType(OptionType newType);

        std::string typeToString() const;

    private:
        double strikePrice;
        double timeToExpiry;
        OptionType type;
    };

} // namespace OptionLib

#endif //OPTION_H
