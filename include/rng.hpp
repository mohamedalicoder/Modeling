#ifndef RNG_HPP
#define RNG_HPP

#include <vector>
#include <cstdint>
#include <memory>
#include <string>

namespace rng {

class RandomGenerator {
public:
    virtual ~RandomGenerator() = default;
    virtual double generate() = 0;
    virtual std::vector<double> generate_sequence(size_t count) = 0;
    virtual std::string get_name() const = 0;
    virtual void set_seed(uint64_t seed) = 0;
};

class RandomnessTest {
public:
    virtual ~RandomnessTest() = default;
    virtual bool run_test(const std::vector<double>& numbers, double significance_level) = 0;
    virtual std::string get_test_name() const = 0;
    virtual std::string get_test_result() const = 0;
};

// Significance levels
constexpr double ALPHA_0_01 = 0.01;
constexpr double ALPHA_0_05 = 0.05;
constexpr double ALPHA_0_10 = 0.10;

} // namespace rng

#endif // RNG_HPP