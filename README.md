# OptionLib

OptionLib is a C++ library designed for options pricing and financial modelling, with support for various pricing models. This project is a **work in progress**, with more features planned for future release.

## Features

- **Pricing models**
  - Black-Scholes
  - Monte-Carlo
  - Binomial model
  - More soon...!

- **Portfolio management**
  - Create and manage a portfolio of options
  - Greek sensitivity analysis
 
## Installation

Make sure you have a C++20 compatible compiler. Clone the repo and build using CMake:

```bash
git clone https://github.com/James-Wirth/OptionLib.git
cd OptionLib
mkdir build && cd build
cmake ..
make
```

## Usage

Here is a simple usage that demonstrates setting up a portfolio with call and put options:

```cpp
// Black-Scholes model
auto defaultModel = std::make_shared<BlackScholes>(spotPrice=100.0,
                                                   riskFreeRate=0.05,
                                                   volatility=0.2);
Portfolio portfolio(defaultModel);

auto callOption = std::make_shared<Option>(strikePrice=100,
                                           timeToMaturity=1.0,
                                           type=OptionType::Call);

portfolio.addOption(callOption);
```

Calculate the Greeks for the portfolio:

```cpp
auto greekVector = portfolio.greekVector(GreekType::Delta)
```

Calculate the portfolio value:

```cpp
double portfolioValue = portfolio.totalValue()
```




