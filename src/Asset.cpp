//
// Created by James Wirth on 14/11/2024.
//

#include <string>
#include <OptionLib/Asset.h>

namespace OptionLib {

    Asset::Asset(std::string id, double spotPrice)
        : id(std::move(id)), spotPrice(spotPrice) {}

    std::string Asset::getId() const {
        return id;
    }

    double Asset::getSpotPrice() const {
        return spotPrice;
    }

    // Optional parameter setters and getters
    void Asset::set(Param param, double value) {
        parameters[param] = value;
    }

    double Asset::get(Param param) const {
        auto it = parameters.find(param);
        if (it != parameters.end()) {
            return it->second;
        } else {
            throw std::runtime_error("Parameter not found");
        }
    }



} // namespace OptionLib
