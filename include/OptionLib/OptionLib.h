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

namespace OptionLib {
    using Models::Binomial;
    using Models::BlackScholes;
    using Models::MonteCarlo;
    using Models::GreekType;

    class Factory {
    public:
        // Factory methods for creating shared pointers to Model objects
        template<typename ModelType, typename... Args>
        static std::shared_ptr<Models::Model> createModel(Args&&... args) {
            return std::make_shared<ModelType>(std::forward<Args>(args)...);
        }

        // Factory method for creating shared pointers to Option objects
        static std::shared_ptr<Option> createOption(double strike, double expiry, OptionType type) {
            return std::make_shared<Option>(strike, expiry, type);
        }
    };
    using OptionLib::Factory;
}

#endif //OPTIONLIB_H
