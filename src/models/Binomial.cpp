//
// Created by James Wirth on 13/11/2024.
//

#include <OptionLib/models/Binomial.h>
#include <cmath>
#include <future>
#include <vector>
#include <algorithm>
#include <numeric>

namespace OptionLib::Models {

    double Binomial::priceWrapper(const Option &_option, double _spotPrice, double _riskFreeRate, double _volatility, double _strikePrice, double _timeToMaturity, int _numSteps) {
        double dt = _timeToMaturity / _numSteps;
        double u = std::exp(_volatility * std::sqrt(dt));
        double d = 1.0 / u;
        double p = (std::exp(_riskFreeRate * dt) - d) / (u - d);
        double discountFactor = std::exp(-_riskFreeRate * dt);

        unsigned int numThreads = std::thread::hardware_concurrency();
        if (numThreads == 0) numThreads = 2;

        int nodesPerThread = (_numSteps + 1) / numThreads;
        int remainingNodes = (_numSteps + 1) % numThreads;

        std::vector<double> optionValues(_numSteps + 1);
        std::vector<double> assetPrices(_numSteps + 1);

        auto payoffCalculator = [&](int start, int end) {
            for (int i = start; i < end; ++i) {
                double assetPrice = _spotPrice * std::pow(u, _numSteps - i) * std::pow(d, i);
                assetPrices[i] = assetPrice;
                optionValues[i] = (_option.getType() == OptionType::Call) ?
                                   std::max(assetPrice - _strikePrice, 0.0) :
                                   std::max(_strikePrice - assetPrice, 0.0);
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

        for (int step = _numSteps - 1; step >= 0; --step) {
            for (int i = 0; i <= step; ++i) {
                optionValues[i] = discountFactor * (p * optionValues[i] + (1.0 - p) * optionValues[i + 1]);
            }
            optionValues.resize(step + 1);
        }

        return optionValues[0];
    }

    double Binomial::price(const Option& option) const {
        double K = option.getStrikePrice();
        double T = option.getTimeToExpiry();
        return priceWrapper(option, spotPrice, riskFreeRate, volatility, K, T, numSteps);
    }

    double Binomial::computeGreek(const Option& option, GreekType greekType) const {
        switch (greekType) {
            case GreekType::Delta: return calculateDelta(option);
            case GreekType::Gamma: return calculateGamma(option);
            case GreekType::Vega: return calculateVega(option);
            case GreekType::Theta: return calculateTheta(option);
            case GreekType::Rho: return calculateRho(option);
            default:
                throw std::invalid_argument("Invalid Greek type");
        }
    }

    // Need to complete
    double Binomial::calculateDelta(const Option& option) const {
        double epsilon = 0.01 * spotPrice;
        double pricePlus = priceWrapper(option, spotPrice + epsilon, riskFreeRate, volatility, option.getStrikePrice(), option.getTimeToExpiry(), numSteps);
        double priceMinus = priceWrapper(option, spotPrice - epsilon, riskFreeRate, volatility, option.getStrikePrice(), option.getTimeToExpiry(), numSteps);
        return (pricePlus - priceMinus) / (2 * epsilon);
    }

    double Binomial::calculateGamma(const Option& option) const {
        double epsilon = 0.01 * spotPrice;
        double pricePlus = priceWrapper(option, spotPrice + epsilon, riskFreeRate, volatility, option.getStrikePrice(), option.getTimeToExpiry(), numSteps);
        double priceCenter = priceWrapper(option, spotPrice, riskFreeRate, volatility, option.getStrikePrice(),option.getTimeToExpiry(), numSteps);
        double priceMinus = priceWrapper(option, spotPrice - epsilon, riskFreeRate, volatility, option.getStrikePrice(), option.getTimeToExpiry(), numSteps);
        return (pricePlus - 2 * priceCenter + priceMinus) / (epsilon * epsilon);
    }

    double Binomial::calculateVega(const Option& option) const {
        double epsilon = 0.01;
        double pricePlus = priceWrapper(option, spotPrice, riskFreeRate, volatility + epsilon, option.getStrikePrice(), option.getTimeToExpiry(), numSteps);
        double priceMinus = priceWrapper(option, spotPrice, riskFreeRate, volatility - epsilon, option.getStrikePrice(), option.getTimeToExpiry(), numSteps);
        return (pricePlus - priceMinus) / (2 * epsilon);
    }

    double Binomial::calculateTheta(const Option& option) const {
        double epsilon = 1.0 / 365;
        double priceNow = priceWrapper(option, spotPrice, riskFreeRate, volatility, option.getStrikePrice(), option.getTimeToExpiry(), numSteps);
        double priceLater = priceWrapper(option, spotPrice, riskFreeRate, volatility, option.getStrikePrice(), option.getTimeToExpiry() - epsilon, numSteps);
        return (priceLater - priceNow) / epsilon;
    }

    double Binomial::calculateRho(const Option& option) const {
        double epsilon = 0.0001;
        double pricePlus = priceWrapper(option, spotPrice, riskFreeRate + epsilon, volatility, option.getStrikePrice(), option.getTimeToExpiry(), numSteps);
        double priceMinus = priceWrapper(option, spotPrice, riskFreeRate - epsilon, volatility, option.getStrikePrice(), option.getTimeToExpiry(), numSteps);
        return (pricePlus - priceMinus) / (2 * epsilon);
    }

    double Binomial::VaR(const Option& option, double confidenceLevel, double holdingPeriod) const {
        return 0.0;
    }

    double Binomial::ExpectedShortfall(const Option& option, double confidenceLevel, double holdingPeriod) const {
        return 0.0;
    }

} // namespace OptionLib::Models
