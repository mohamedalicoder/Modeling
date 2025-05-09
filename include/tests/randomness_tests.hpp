#ifndef RANDOMNESS_TESTS_HPP
#define RANDOMNESS_TESTS_HPP

#include "../rng.hpp"
#include <string>
#include <memory>

namespace rng
{

    class ChiSquareTest : public RandomnessTest
    {
    public:
        ChiSquareTest(size_t num_bins = 10);
        bool run_test(const std::vector<double> &numbers, double significance_level) override;
        std::string get_test_name() const override;
        std::string get_test_result() const override;

    private:
        const size_t num_bins_;
        double chi_square_value_;
        double p_value_;
        std::string result_message_;
    };

    class RunsTest : public RandomnessTest
    {
    public:
        bool run_test(const std::vector<double> &numbers, double significance_level) override;
        std::string get_test_name() const override;
        std::string get_test_result() const override;

    private:
        double z_statistic_;
        double p_value_;
        std::string result_message_;
    };

    class SerialCorrelationTest : public RandomnessTest
    {
    public:
        bool run_test(const std::vector<double> &numbers, double significance_level) override;
        std::string get_test_name() const override;
        std::string get_test_result() const override;

    private:
        double correlation_coefficient_;
        double z_statistic_;
        double p_value_;
        std::string result_message_;
    };

    // Factory function to create all available tests
    std::vector<std::unique_ptr<RandomnessTest>> create_test_suite();

} // namespace rng

#endif // RANDOMNESS_TESTS_HPP