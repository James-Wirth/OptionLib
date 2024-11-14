//
// Created by James Wirth on 13/11/2024.
//

#include "OptionLib/Portfolio.h"
#include "OptionLib/models/Model.h" // Include Model to access price method
#include <random>
#include <thread>
#include <future>

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

    double Portfolio::VaR(double confidenceLevel, double holdingPeriod) const {
        double portfolioVaR = 0.0;
        for (const auto& item : items) {
            portfolioVaR += item.model->VaR(*item.option, confidenceLevel, holdingPeriod);
        }
        return portfolioVaR;
    }

    double Portfolio::ExpectedShortfall(double confidenceLevel, double holdingPeriod) const {
        double portfolioES = 0.0;
        for (const auto& item : items) {
            portfolioES += item.model->ExpectedShortfall(*item.option, confidenceLevel, holdingPeriod);
        }
        return portfolioES;
    }

    // std::map<std::string, double> Portfolio::sensitivityAnalysis(double spotChange, double volatilityChange) const {
    //     std::map<std::string, double> sensitivities;
    //
    //     for (const auto& item : items) {
    //         double originalValue = item.model->price(*item.option);
    //
    //         // Sensitivity to spot change
    //         item.model->setSpotPrice(item.model->getSpotPrice() * (1 + spotChange));
    //         double newSpotValue = item.model->price(*item.option);
    //         sensitivities[item.option->getDescription() + "_spotSensitivity"] = (newSpotValue - originalValue) / originalValue;
    //
    //         // Sensitivity to volatility change
    //         item.model->setVolatility(item.model->getVolatility() * (1 + volatilityChange));
    //         double newVolValue = item.model->price(*item.option);
    //         sensitivities[item.option->getDescription() + "_volatilitySensitivity"] = (newVolValue - originalValue) / originalValue;
    //
    //         // Reset
    //         item.model->resetParams();
    //     }
    //
    //     return sensitivities;
    // }

    std::map<std::string, double> Portfolio::concentrationMeasures() const {
        std::map<std::string, double> concentrations;
        double totalValue = this->totalValue();

        int index = 1;
        for (const auto& item : items) {
            double itemValue = item.model->price(*item.option);
            concentrations["Option_" + std::to_string(index++)] = itemValue / totalValue;
        }

        return concentrations;
    }

} // namespace OptionLib
