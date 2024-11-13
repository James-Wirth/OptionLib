//
// Created by James Wirth on 13/11/2024.
//

#include "OptionLib/Portfolio.h"
#include "OptionLib/models/Model.h" // Include Model to access price method

namespace OptionLib {

    // Constructor with an optional default model
    Portfolio::Portfolio(std::shared_ptr<Models::Model> defaultModel)
        : defaultModel(std::move(defaultModel)) {}

    // Add an option with an optional model override
    void Portfolio::addOption(std::shared_ptr<Option> option, std::shared_ptr<Models::Model> model) {
        // Use the provided model if available, otherwise fall back to the default model
        if (!model) {
            if (!defaultModel) {
                throw std::runtime_error("No model specified for option and no default model set.");
            }
            model = defaultModel;
        }
        items.emplace_back(std::move(option), std::move(model));
    }

    // Calculate the total value of the portfolio
    double Portfolio::totalValue() const {
        double total = 0.0;
        for (const auto& item : items) {
            total += item.model->price(*item.option); // Calculate option price using its model
        }
        return total;
    }

} // namespace OptionLib