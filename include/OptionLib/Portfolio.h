//
// Created by James Wirth on 13/11/2024.
//

#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include "OptionLib/Option.h"
#include "OptionLib/models/Model.h" // Include the complete Model definition
#include <vector>
#include <map>
#include <memory>

namespace OptionLib {

    class Portfolio {
    public:
        explicit Portfolio(std::shared_ptr<Models::Model> defaultModel = nullptr);

        void addOption(std::shared_ptr<Option> option, std::shared_ptr<Models::Model> model = nullptr);

        double totalValue() const;
        double totalGreek(Models::GreekType greekType) const;
        std::vector<double> greekVector(Models::GreekType greekType) const;

        // std::map<std::string, double> sensitivityAnalysis(double spotChange, double volatilityChange) const;
        std::map<std::string, double> concentrationMeasures() const;

        double VaR(double confidenceLevel, double holdingPeriod) const;
        double ExpectedShortfall(double confidenceLevel, double holdingPeriod) const;

    private:
        struct PortfolioItem {
            std::shared_ptr<Option> option;
            std::shared_ptr<Models::Model> model;

            PortfolioItem(std::shared_ptr<Option> opt, std::shared_ptr<Models::Model> mod)
                : option(std::move(opt)), model(std::move(mod)) {}
        };

        std::vector<PortfolioItem> items;
        std::shared_ptr<Models::Model> defaultModel;
    };

} // namespace OptionLib

#endif //PORTFOLIO_H
