//
// Created by James Wirth on 13/11/2024.
//

#include "OptionLib/Portfolio.h"
#include "OptionLib/models/Model.h" // Include Model to access price method

namespace OptionLib {

    Portfolio::Portfolio(std::shared_ptr<Models::Model> defaultModel)
        : defaultModel(std::move(defaultModel)) {}

    void Portfolio::addOption(std::shared_ptr<Option> option, std::shared_ptr<Models::Model> model) {
        // Use the provided model or fall back to the default model if none is provided
        if (!model) {
            model = defaultModel;
        }

        // Check if a model is provided (either directly or as the default model)
        if (!model) {
            throw std::invalid_argument("No model provided for option and no default model set.");
        }

        items.emplace_back(std::move(option), std::move(model));
    }

    double Portfolio::totalValue() const {
        double total = 0.0;
        for (const auto& item : items) {
            total += item.model->price(*item.option);
        }
        return total;
    }

    double Portfolio::totalGreek(Models::GreekType greekType) const {
        double total = 0.0;
        for (const auto& item : items) {
            total += item.model->computeGreek(*item.option, greekType);
        }
        return total;
    }

    std::vector<double> Portfolio::greekVector(Models::GreekType greekType) const {
        std::vector<double> values;
        values.reserve(items.size()); // Reserve memory for efficiency

        for (const auto& item : items) {
            values.push_back(item.model->computeGreek(*item.option, greekType));
        }
        return values;
    }

} // namespace OptionLib