//
// Created by James Wirth on 13/11/2024.
//

#include <OptionLib/models/Binomial.h>
#include <cmath>
#include <future>
#include <vector>
#include <algorithm>

namespace OptionLib::Models {

    double Binomial::price(const Option& option) const {
        double K = option.getStrikePrice();
        double T = option.getTimeToExpiry();

        double dt = T / numSteps;
        double u = std::exp(volatility * std::sqrt(dt));
        double d = 1.0 / u;
        double p = (std::exp(riskFreeRate * dt) - d) / (u - d);
        double discountFactor = std::exp(-riskFreeRate * dt);

        unsigned int numThreads = std::thread::hardware_concurrency();
        if (numThreads == 0) numThreads = 2;

        int nodesPerThread = (numSteps + 1) / numThreads;
        int remainingNodes = (numSteps + 1) % numThreads;

        std::vector<double> optionValues(numSteps + 1);
        std::vector<double> assetPrices(numSteps + 1);

        auto payoffCalculator = [&](int start, int end) {
            for (int i = start; i < end; ++i) {
                double assetPrice = spotPrice * std::pow(u, numSteps - i) * std::pow(d, i);
                assetPrices[i] = assetPrice;
                optionValues[i] = (option.getType() == OptionType::Call) ?
                                   std::max(assetPrice - K, 0.0) :
                                   std::max(K - assetPrice, 0.0);
            }
        };

        std::vector<std::future<void>> futures;
        int start = 0;
        for (unsigned int i = 0; i < numThreads; ++i) {
            int end = start + nodesPerThread + (i < remainingNodes ? 1 : 0);
            futures.push_back(std::async(std::launch::async, payoffCalculator, start, end));
            start = end;
        }

        for (auto& future : futures) {
            future.get();
        }

        for (int step = numSteps - 1; step >= 0; --step) {
            for (int i = 0; i <= step; ++i) {
                optionValues[i] = discountFactor * (p * optionValues[i] + (1.0 - p) * optionValues[i + 1]);
            }
            optionValues.resize(step + 1);
        }

        return optionValues[0];
    }

} // namespace OptionLib::Models
