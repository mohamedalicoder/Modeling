#include "../../include/tests/randomness_tests.hpp"
#include <cmath>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <iomanip>

namespace rng
{

    // Helper function to calculate critical values
    double get_critical_value(double significance_level, size_t df)
    {
        // Simplified critical values for chi-square test
        // In a real implementation, this would use a proper statistical library
        if (significance_level == 0.01)
            return df * 1.5;
        if (significance_level == 0.05)
            return df * 1.3;
        return df * 1.2; // For 0.10
    }

    // Helper function to calculate normal distribution p-value
    double normal_cdf(double z)
    {
        // Approximation of the normal CDF
        return 0.5 * (1 + std::erf(z / std::sqrt(2)));
    }

    // Chi-Square Test Implementation
    ChiSquareTest::ChiSquareTest(size_t num_bins)
        : num_bins_(num_bins), chi_square_value_(0.0), p_value_(0.0) {}

    bool ChiSquareTest::run_test(const std::vector<double> &numbers, double significance_level)
    {
        std::vector<size_t> observed(num_bins_, 0);
        size_t n = numbers.size();
        double expected = static_cast<double>(n) / num_bins_;

        // Count observations in each bin
        for (double number : numbers)
        {
            size_t bin = static_cast<size_t>(number * num_bins_);
            if (bin == num_bins_)
                bin--; // Handle edge case of 1.0
            observed[bin]++;
        }

        // Calculate chi-square statistic
        chi_square_value_ = 0.0;
        for (size_t count : observed)
        {
            double diff = count - expected;
            chi_square_value_ += (diff * diff) / expected;
        }

        // Degrees of freedom
        size_t df = num_bins_ - 1;
        double critical_value = get_critical_value(significance_level, df);
        bool passed = chi_square_value_ <= critical_value;

        std::stringstream ss;
        ss << "Chi-square value: " << std::fixed << std::setprecision(4) << chi_square_value_
           << "\nCritical value: " << critical_value
           << "\nDegrees of freedom: " << df
           << "\nSignificance level: " << significance_level
           << "\nTest " << (passed ? "PASSED" : "FAILED");
        result_message_ = ss.str();

        return passed;
    }

    std::string ChiSquareTest::get_test_name() const
    {
        return "Chi-Square Test for Uniformity";
    }

    std::string ChiSquareTest::get_test_result() const
    {
        return result_message_;
    }

    // Runs Test Implementation
    bool RunsTest::run_test(const std::vector<double> &numbers, double significance_level)
    {
        if (numbers.size() < 2)
            return false;

        size_t runs = 1;
        bool increasing = numbers[1] > numbers[0];

        // Count runs
        for (size_t i = 1; i < numbers.size(); ++i)
        {
            if ((numbers[i] > numbers[i - 1]) != increasing)
            {
                increasing = !increasing;
                runs++;
            }
        }

        // Calculate test statistic
        double n = static_cast<double>(numbers.size());
        double expected_runs = (2.0 * n - 1.0) / 3.0;
        double variance = (16.0 * n - 29.0) / 90.0;
        z_statistic_ = (runs - expected_runs) / std::sqrt(variance);
        p_value_ = 2.0 * (1.0 - normal_cdf(std::abs(z_statistic_)));

        bool passed = p_value_ > significance_level;

        std::stringstream ss;
        ss << "Number of runs: " << runs
           << "\nZ-statistic: " << std::fixed << std::setprecision(4) << z_statistic_
           << "\nP-value: " << p_value_
           << "\nSignificance level: " << significance_level
           << "\nTest " << (passed ? "PASSED" : "FAILED");
        result_message_ = ss.str();

        return passed;
    }

    std::string RunsTest::get_test_name() const
    {
        return "Runs Test for Independence";
    }

    std::string RunsTest::get_test_result() const
    {
        return result_message_;
    }

    // Serial Correlation Test Implementation
    bool SerialCorrelationTest::run_test(const std::vector<double> &numbers, double significance_level)
    {
        if (numbers.size() < 2)
            return false;

        size_t n = numbers.size() - 1;
        std::vector<double> x(n), y(n);

        // Prepare pairs of consecutive numbers
        for (size_t i = 0; i < n; ++i)
        {
            x[i] = numbers[i];
            y[i] = numbers[i + 1];
        }

        // Calculate means
        double mean_x = std::accumulate(x.begin(), x.end(), 0.0) / n;
        double mean_y = std::accumulate(y.begin(), y.end(), 0.0) / n;

        // Calculate correlation coefficient
        double numerator = 0.0, denom_x = 0.0, denom_y = 0.0;
        for (size_t i = 0; i < n; ++i)
        {
            double dx = x[i] - mean_x;
            double dy = y[i] - mean_y;
            numerator += dx * dy;
            denom_x += dx * dx;
            denom_y += dy * dy;
        }

        correlation_coefficient_ = numerator / std::sqrt(denom_x * denom_y);
        z_statistic_ = correlation_coefficient_ * std::sqrt(n);
        p_value_ = 2.0 * (1.0 - normal_cdf(std::abs(z_statistic_)));

        bool passed = p_value_ > significance_level;

        std::stringstream ss;
        ss << "Serial correlation coefficient: " << std::fixed << std::setprecision(4)
           << correlation_coefficient_
           << "\nZ-statistic: " << z_statistic_
           << "\nP-value: " << p_value_
           << "\nSignificance level: " << significance_level
           << "\nTest " << (passed ? "PASSED" : "FAILED");
        result_message_ = ss.str();

        return passed;
    }

    std::string SerialCorrelationTest::get_test_name() const
    {
        return "Serial Correlation Test";
    }

    std::string SerialCorrelationTest::get_test_result() const
    {
        return result_message_;
    }

    // Factory function implementation
    std::vector<std::unique_ptr<RandomnessTest>> create_test_suite()
    {
        std::vector<std::unique_ptr<RandomnessTest>> tests;
        tests.push_back(std::make_unique<ChiSquareTest>());
        tests.push_back(std::make_unique<RunsTest>());
        tests.push_back(std::make_unique<SerialCorrelationTest>());
        return tests;
    }

} // namespace rng