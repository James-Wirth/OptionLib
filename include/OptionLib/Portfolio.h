//
// Created by James Wirth on 13/11/2024.
//

#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include "OptionLib/Option.h"
#include "OptionLib/models/Model.h" // Include the complete Model definition
#include <vector>
#include <memory>

namespace OptionLib {

    class Portfolio {
    public:
        // Constructor with an optional default model
        explicit Portfolio(std::shared_ptr<Models::Model> defaultModel = nullptr);

        // Add an option with an optional model override; if no model is provided, use the default model
        void addOption(std::shared_ptr<Option> option, std::shared_ptr<Models::Model> model = nullptr);

        // Calculate the total value of the portfolio
        double totalValue() const;

    private:
        // Internal structure to store each option with its model
        struct PortfolioItem {
            std::shared_ptr<Option> option;
            std::shared_ptr<Models::Model> model;

            PortfolioItem(std::shared_ptr<Option> opt, std::shared_ptr<Models::Model> mod)
                : option(std::move(opt)), model(std::move(mod)) {}
        };

        std::vector<PortfolioItem> items;               // List of options and their models
        std::shared_ptr<Models::Model> defaultModel;    // Default model for the portfolio
    };

} // namespace OptionLib

#endif //PORTFOLIO_H
