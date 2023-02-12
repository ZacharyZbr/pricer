# Pricer

Developpers of the library :

* Zachary Zuber, Grégoire Rabusson and Julien Bensoussan for the multi-currency pricing and protfolio hedger. 
* Zachary Zuber and Gaspard Moizo for the pricing library of the american options with the Longstoff algorithm. 

Implementation of multi currency pricer.

## User's guide :

### How to install this pricing library

This project requires the use of the pnl-lib scientific library (https://pnlnum.github.io/pnl/) and the JSON library nlohmann (https://github.com/nlohmann/json)

#### On Mac OS or linux : 

Install pnl-lib and nlohmann from gitHub repos (you can clone, fork or simply download a zip).
Then copy and paste the CMakeLists.txt from \CMake\linux to the root of the project directory and run the following command :
```
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH="path to pnl-lib library"; "path to nlohmann library" ..
```
Then ```make``` and the multi-currency pricing library will build two executables called **hedging_pricer** and **am_pricer**.

#### On Windows
I suggest to use the IDE visual studio and the cmake-gui. Firstly, copy and paste the CMakeLists.txt from \CMake\windows to the root of the project directory. Then : 

1. Download the pnl-lib here https://github.com/pnlnum/pnl/releases and open cmake-gui. 
* In the "Where is the source file" section, be sure to enter the path to the root of the project where there is the CmakeLists.txt.
* Click on "+ Add Entry" and a window will pop up and enter in the sections 
    - **Name**>CMAKE_PREFIX_PATH 
    - **Type**>PATH
    - **Value**>click on the '...' and choose the path to where the is the file PnlConfig.cmake in the pnl-lib directory. Then click OK.
* Click on Configure, then Generate, then Open Project 
* Once visual studio is opened, you need to install nlohmann. Right click on the solution explorer > "Nuggets packages", find and install nlohmann package for the solution. 

### Running the pricing tool

#### Multi-currency pricer and option hedger

One can find some examples of the input parameter in the \data directory.

```
./hedging_portfolio "path to option_params.json" "path_to market.csv" "path to portfolio.json"
```

```
.\hedging_portfolio.exe "path to option_params.json" "path_to market.csv" "path to portfolio.json"
```

#### American option pricer 

One can find some examples of the input parameter in the \data directory.
```
./pricer_am "path to option_params.json" 
```

```
.\pricer_am.exe "path to option_params.json" 
```
## Delta hedging

Few words about delta hedging. 

## Multi currency

Let's consier a multicurrency market that contains 1 domestic currency and $\hat{n} \in \mathbb{N}$ foreign currencies. Each market contains : 
* $n_{i}$ risky assets $(S^{i, 1}, ..., S^{i, n_{i}})$ and an exchange rate $X^{i}$ with the domestic market $(\forall 1 \leq i \leq \hat{n})$. 
* The domestic market contains $n_{0}$ risky assets $(S^{0, 1}, ..., S^{i, n_{0}})$ 
* One risk free rate $r^{i} \ \forall 1 \leq i \leq \hat{n}$

The mapping between the all the markets and the number of risky assets they have is represented by a vector $N = (n_{0}, n_{1}, ..., n_{\hat{n}})$. The dynamics of the different risky assets are the followings : 
$$
dS_{t}^{i, l} = S^{i,l} \times (\mu^{i,l}dt + \sigma^{i,l}dW_{t}^{i,l}) \ i \in {0, ..., \hat{n}}, \ l \in {0, ..., n_{i}}
$$
$$
dX_{t}^{i} = X_{t}^{i} \times (\mu^{i,X}dt + \sigma^{i,X}dW_{t}^{i,X}) \ i \in {1, ..., \hat{n}}
$$

## Longstoff algorithm
