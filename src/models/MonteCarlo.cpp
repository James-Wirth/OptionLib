//
// Created by James Wirth on 13/11/2024.
//

#include <OptionLib/models/MonteCarlo.h>
#include <cmath>
#include <future>
#include <random>
#include <vector>

namespace OptionLib::Models {

    double MonteCarlo::priceWrapper(const Option& _option, double _spotPrice, double _riskFreeRate, double _volatility,
                                    double strikePrice, double timeToMaturity, int _numSimulations) {

        double discountFactor = std::exp(-_riskFreeRate * timeToMaturity);

        unsigned int numThreads = std::thread::hardware_concurrency();
        if (numThreads == 0) numThreads = 2;

        int simulationsPerThread = _numSimulations / numThreads;
        int remainingSimulations = _numSimulations % numThreads;

        auto monteCarloWorker = [&](int simulations) {
            std::mt19937 rng(std::random_device{}());
            std::normal_distribution<> dist(0.0, 1.0);

            double payoffSum = 0.0;
            for (int i = 0; i < simulations; ++i) {

                double Z = dist(rng);
                double ST = _spotPrice * std::exp((_riskFreeRate - 0.5 * _volatility * _volatility) * timeToMaturity + _volatility * std::sqrt(timeToMaturity) * Z);

                double payoff = 0.0;
                if (_option.getType() == OptionType::Call) {
                    payoff = std::max(ST - strikePrice, 0.0); // Call payoff
                } else if (_option.getType() == OptionType::Put) {
                    payoff = std::max(strikePrice - ST, 0.0); // Put payoff
                }

                payoffSum += payoff;
            }
            return payoffSum;
        };

        std::vector<std::future<double>> futures;
        for (unsigned int i = 0; i < numThreads; ++i) {
            int simulations = (i < remainingSimulations) ? simulationsPerThread + 1 : simulationsPerThread;
            futures.push_back(std::async(std::launch::async, monteCarloWorker, simulations));
        }

        double totalPayoffSum = 0.0;
        for (auto& future : futures) {
            totalPayoffSum += future.get();
        }

        double averagePayoff = totalPayoffSum / _numSimulations;
        return averagePayoff * discountFactor;
    }

    double MonteCarlo::price(const Option& option) const {
        double K = option.getStrikePrice();
        double T = option.getTimeToExpiry();
        return priceWrapper(option, spotPrice, riskFreeRate, volatility, K, T, numSimulations);
    }

    double MonteCarlo::computeGreek(const Option& option, GreekType greekType) const {
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

    double MonteCarlo::calculateDelta(const Option& option) const {
        double epsilon = 0.01 * spotPrice;
        double pricePlus = priceWrapper(option, spotPrice + epsilon, riskFreeRate, volatility, option.getStrikePrice(), option.getTimeToExpiry(), numSimulations);
        double priceMinus = priceWrapper(option, spotPrice - epsilon, riskFreeRate, volatility, option.getStrikePrice(), option.getTimeToExpiry(), numSimulations);
        return (pricePlus - priceMinus) / (2 * epsilon);
    }

    double MonteCarlo::calculateGamma(const Option& option) const {
        double epsilon = 0.01 * spotPrice;
        double pricePlus = priceWrapper(option, spotPrice + epsilon, riskFreeRate, volatility, option.getStrikePrice(), option.getTimeToExpiry(), numSimulations);
        double priceCenter = priceWrapper(option, spotPrice, riskFreeRate, volatility, option.getStrikePrice(),option.getTimeToExpiry(), numSimulations);
        double priceMinus = priceWrapper(option, spotPrice - epsilon, riskFreeRate, volatility, option.getStrikePrice(), option.getTimeToExpiry(), numSimulations);
        return (pricePlus - 2 * priceCenter + priceMinus) / (epsilon * epsilon);
    }

    double MonteCarlo::calculateVega(const Option& option) const {
        double epsilon = 0.01;
        double pricePlus = priceWrapper(option, spotPrice, riskFreeRate, volatility + epsilon, option.getStrikePrice(), option.getTimeToExpiry(), numSimulations);
        double priceMinus = priceWrapper(option, spotPrice, riskFreeRate, volatility - epsilon, option.getStrikePrice(), option.getTimeToExpiry(), numSimulations);
        return (pricePlus - priceMinus) / (2 * epsilon);
    }

    double MonteCarlo::calculateTheta(const Option& option) const {
        double epsilon = 1.0 / 365;
        double priceNow = priceWrapper(option, spotPrice, riskFreeRate, volatility, option.getStrikePrice(), option.getTimeToExpiry(), numSimulations);
        double priceLater = priceWrapper(option, spotPrice, riskFreeRate, volatility, option.getStrikePrice(), option.getTimeToExpiry() - epsilon, numSimulations);
        return (priceLater - priceNow) / epsilon;
    }

    double MonteCarlo::calculateRho(const Option& option) const {
        double epsilon = 0.0001;
        double pricePlus = priceWrapper(option, spotPrice, riskFreeRate + epsilon, volatility, option.getStrikePrice(), option.getTimeToExpiry(), numSimulations);
        double priceMinus = priceWrapper(option, spotPrice, riskFreeRate - epsilon, volatility, option.getStrikePrice(), option.getTimeToExpiry(), numSimulations);
        return (pricePlus - priceMinus) / (2 * epsilon);
    }

} // namespace OptionLib::Models