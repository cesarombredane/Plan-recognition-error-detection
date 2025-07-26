# Plan Recognition and Anomaly Detection System

A C++ implementation of anomaly and abnormal behavior detection algorithms based on plan recognition using particle filters and cognitive distress management.

## 📋 Table of Contents

- [Overview](#overview)
- [Architecture](#architecture)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Algorithms](#algorithms)
- [Results](#results)
- [Documentation](#documentation)
- [Contributing](#contributing)
- [Authors](#authors)

## 🎯 Overview

This project implements algorithms for detecting anomalies and abnormal behaviors in plan execution using plan recognition techniques. The system employs three different methods to compute the probability of anomaly at time t: **Min**, **Sum**, and **Support** methods.

The core principle is based on the cognitive distress management formula:
```
P(At) = P(At-1) + P(At|¬At-1) × (1 - P(At-1))
```

Where:
- `P(At)` is the probability of anomaly at time t
- `P(At-1)` is the probability of anomaly at time t-1
- `P(At|¬At-1)` is the conditional probability of anomaly given no previous anomaly

## 🏗️ Architecture

### Core Components

#### 1. Plan Recognition System (`solver.h/cpp`)
- **Particle Filter**: Uses 1000 particles by default for plan recognition
- **Tree Structure**: Represents plan hierarchies with treeNode and tree classes
- **Goal Tracking**: Maintains probability distributions over possible goals

#### 2. Cognitive Distress Management (`cognitive_distress_management.h/cpp`)
Three implementations for anomaly detection:

- **CDMmin**: Implements the 'Min' method for computing P(At|¬At-1)
- **CDMsum**: Implements the 'Sum' method with precision-based thresholding
- **CDMsupport**: Implements the 'Support' method using particle support ratios

#### 3. Extended Plan Library (`extended_plan_library.h/cpp`)
- **Noise Modeling**: Represents observation noise in plan execution
- **Probability Distributions**: Models rule probabilities for non-terminals
- **Action Mapping**: Converts between action numbers and names

#### 4. Rule System (`rule.h/cpp`)
- **Grammar Rules**: Defines plan structure with children and constraints
- **Constraint Handling**: Manages ordering constraints between actions

## ✨ Features

- **Multiple Detection Methods**: Three different approaches to anomaly detection
- **Particle Filter-based Plan Recognition**: Robust probabilistic inference
- **Noise Handling**: Configurable noise models for realistic scenarios
- **Comprehensive Testing**: Automated test suite with performance metrics
- **Result Analysis**: Detailed statistics on detection performance
- **Precision Optimization**: Tools for finding optimal precision parameters

## 🚀 Installation

### Prerequisites

- **Compiler**: GCC with C++11 support
- **Make**: GNU Make utility
- **Git**: For version control

### Build Instructions

1. **Clone the repository**:
   ```bash
   git clone https://github.com/cesarombredane/Plan-recognition-error-detection.git
   cd Plan-recognition-error-detection
   ```

2. **Build the project**:
   ```bash
   make clean && make
   ```

3. **Run tests** (optional):
   ```bash
   make run
   ```

### Build Targets

- `make` or `make all`: Build the project
- `make debug`: Build with debug symbols
- `make clean`: Remove build artifacts
- `make run`: Build and run the program
- `make install`: Install to system (requires sudo)

## 💻 Usage

### Basic Execution

```bash
./parc
```

The program runs predefined tests by default. You can modify the main function to customize:

### Test Configuration

```cpp
// Full test suite
FullTestCdm(true, 100, 10, 10, "results_distress.txt");
FullTestCdm(false, 100, 10, 10, "results_normal.txt");

// Precision optimization
testPrecSum(5, 10, "precision_sum_results.txt");
testPrecSupport(5, 10, "precision_support_results.txt");
```

### Parameters

- **distress**: `true` for tests with anomalies, `false` for normal behavior
- **nbLib**: Number of different plan libraries to test
- **nbIt**: Number of iterations per plan library
- **nbAction**: Size/complexity of plan library
- **resultPath**: Output file for results

## 🧮 Algorithms

### 1. Min Method (CDMmin)

The simplest approach that computes the minimum probability threshold for anomaly detection.

**Characteristics**:
- No precision parameter required
- Fast computation
- Basic threshold-based detection

### 2. Sum Method (CDMsum)

Computes anomaly probability using summation over all possible actions and plans:

```
P(At|¬At-1) = ΣΣ(δa∉e × P(a) × P(e))
```

**Parameters**:
- **precision**: Controls sensitivity (1 - threshold)
- **A**: Set of all possible next actions
- **E**: Set of all possible plans

**Characteristics**:
- Precision-tunable
- Comprehensive probability calculation
- Good balance between accuracy and performance

### 3. Support Method (CDMsupport)

Uses particle filter support ratios for anomaly detection:

```
P(At|¬At-1) = P(nbPs/nbP < s)
```

**Parameters**:
- **nbP**: Total number of particles
- **nbPs**: Number of particles supporting the observation
- **s**: Precision threshold

**Characteristics**:
- Specifically designed for particle filters
- Direct support measurement
- Intuitive probabilistic interpretation

## 📊 Results

The system generates comprehensive performance metrics:

### Output Metrics

- **P(effective)**: Overall detection effectiveness
- **P(falsePositive)**: False positive rate
- **P(falseNegative)**: False negative rate  
- **P(GoalFound)**: Goal recognition accuracy
- **Latency**: Average detection delay

### Test Scenarios

1. **Noise Variation**: Tests from 0.0 to 0.4 noise levels
2. **Library Size**: Different plan complexity levels (10, 45, 100 actions)
3. **Goal Variation**: Different numbers of goals (5, 10, 20)
4. **Precision Optimization**: Finding optimal precision parameters

## 📚 Documentation

Detailed documentation is available in the `documentation/` folder:

- **Research Papers**: Academic publications and references
- **Technical Reports**: Implementation details and analysis
- **Presentation Materials**: Project presentations and findings

### Code Documentation

The codebase includes comprehensive Doxygen-style documentation:

```cpp
/*!
 *  \brief Method description
 *  \param parameter Description of parameter
 *  \return Description of return value
 */
```

## 👥 Authors

- **César Ombredane** - Main implementation and anomaly detection algorithms
- **Jean Massardi** - Plan recognition system and particle filter implementation

### Version History

- **v2.0** - Cognitive Distress Management implementation
- **v1.1** - Enhanced testing framework  
- **v1.0** - Initial particle filter-based plan recognition

## 📄 License

This project is part of academic research. Please refer to the documentation folder for publication and usage guidelines.

## 🔗 Related Work

This implementation is based on research in:
- Plan Recognition using Particle Filters
- Cognitive Distress Management in AI Systems
- Anomaly Detection in Sequential Data
- Probabilistic Plan Execution Monitoring

For academic references, see the `documentation/articles/` folder.
