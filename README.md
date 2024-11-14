# OptionLib

[![CMake Build and Test](https://github.com/James-Wirth/OptionLib/actions/workflows/ci.yml/badge.svg)](https://github.com/James-Wirth/OptionLib/actions/workflows/ci.yml)

OptionLib is a C++ library designed for options pricing and financial modelling, with support for various pricing models. This project is a **work in progress**, with more features planned for future release.
 
## Installation

Make sure you have a C++20 compatible compiler. Clone the repo and build using CMake:

```bash
git clone https://github.com/James-Wirth/OptionLib.git
cd OptionLib
mkdir build && cd build
cmake ..
make
```

To install the library and the executables, run:

```bash
make install
```

Run the unit tests with:

```bash
ctest
```

## Usage

Here is a simple usage that demonstrates setting up a portfolio with call and put options:

```cpp
using namespace OptionLib;

spotPrice = 100.0;
riskFreeRate = 0.05;
volatility = 0.2;
AssetSP asset = Factory::makeSharedAsset("AAPL", spotPrice, riskFreeRate, volatility);

strikePrice = 100.0;
timeToMaturity = 1.0;
OptionSP callOption = Factory::createOption(asset, strikePrice, timeToMaturity, OptionType::Call);

ModelSP model = Factory::createModel<BlackScholes>(spotPrice, riskFreeRate, timeToMaturity);
Portfolio portfolio(model);
portfolio.addOption(callOption);
```

### Greeks Calculation:

Calculate the value of the Greeks for each option in the portfolio. For example, for the $\Delta$,

```cpp
auto greekVector = portfolio.greekVector(GreekType::Delta)
```

### Portfolio Value and Concentration:

Calculate the total value of the portfolio and its concentration among the held options:

```cpp
double portfolioValue = portfolio.totalValue()
auto concentrations = portfolio.concentrationMeasures();
```

### Risk Analysis:

Calculate the value at risk (VaR) and the expected shortfall (ES).

```cpp
confidenceLevel = 0.95;
holdingTime = 1.0/52;

double portfolioVaR = portfolio.VaR(confidenceLevel, holdingPeriod);
double portfolioES = portfolio.ExpectedShortfall(confidenceLevel, holdingPeriod);;
```



