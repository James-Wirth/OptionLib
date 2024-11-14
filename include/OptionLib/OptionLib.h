//
// Created by James Wirth on 13/11/2024.
//

#ifndef OPTIONLIB_H
#define OPTIONLIB_H

#include <OptionLib/Option.h>

#include <OptionLib/models/Model.h>
#include <OptionLib/models/BlackScholes.h>
#include <OptionLib/models/MonteCarlo.h>
#include <OptionLib/models/Binomial.h>
#include <OptionLib/Portfolio.h>

// Type aliases for shared pointer
using OptionSP = std::shared_ptr<OptionLib::Option>;
using ModelSP = std::shared_ptr<OptionLib::Models::Model>;
using AssetSP = std::shared_ptr<OptionLib::Asset>;

namespace OptionLib {
    using Models::Binomial;
    using Models::BlackScholes;
    using Models::MonteCarlo;
    using Models::GreekType;

    class Factory {
    public:
        // Factory methods for creating shared pointers to Model objects
        template<typename ModelType, typename... Args>
        static std::shared_ptr<Models::Model> makeSharedModel(Args&&... args) {
            return std::make_shared<ModelType>(std::forward<Args>(args)...);
        }

        // Factory method for creating shared pointers to Option objects
        static std::shared_ptr<Option> makeSharedOption(std::shared_ptr<Asset> asset, double strike, double expiry, OptionType type) {
            return std::make_shared<Option>(asset, strike, expiry, type);
        }

        static std::shared_ptr<Asset> makeSharedAsset(std::string id, double spotPrice, double volatility, double riskFreeRate) {
            return std::make_shared<Asset>(id, spotPrice, volatility, riskFreeRate);
        }
    };
    using OptionLib::Factory;
}

#endif //OPTIONLIB_H
