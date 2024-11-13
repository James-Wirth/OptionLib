//
// Created by James Wirth on 13/11/2024.
//

#include <OptionLib/models/MonteCarlo.h>
#include <cmath>
#include <future>
#include <random>
#include <vector>

namespace OptionLib::Models {

    double MonteCarlo::price(const Option& option) const {

        double K = option.getStrikePrice();
        double T = option.getTimeToExpiry();
        double discountFactor = std::exp(-riskFreeRate * T);

        unsigned int numThreads = std::thread::hardware_concurrency();
        if (numThreads == 0) numThreads = 2; // Use at least 2 threads if hardware concurrency isn't specified

        int simulationsPerThread = numSimulations / numThreads;
        int remainingSimulations = numSimulations % numThreads;

        auto monteCarloWorker = [&](int simulations) {
            std::mt19937 rng(std::random_device{}());
            std::normal_distribution<> dist(0.0, 1.0);

            double payoffSum = 0.0;
            for (int i = 0; i < simulations; ++i) {

                double Z = dist(rng);
                double ST = spotPrice * std::exp((riskFreeRate - 0.5 * volatility * volatility) * T + volatility * std::sqrt(T) * Z);

                double payoff = 0.0;
                if (option.getType() == OptionType::Call) {
                    payoff = std::max(ST - K, 0.0); // Call payoff
                } else if (option.getType() == OptionType::Put) {
                    payoff = std::max(K - ST, 0.0); // Put payoff
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

        double averagePayoff = totalPayoffSum / numSimulations;
        return averagePayoff * discountFactor;
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

    double MonteCarlo::calculateDelta(const Option& option) const {return 0.0;}
    double MonteCarlo::calculateGamma(const Option& option) const {return 0.0;}
    double MonteCarlo::calculateVega(const Option& option) const {return 0.0;}
    double MonteCarlo::calculateTheta(const Option& option) const {return 0.0;}
    double MonteCarlo::calculateRho(const Option& option) const {return 0.0;}



} // namespace OptionLib::Models