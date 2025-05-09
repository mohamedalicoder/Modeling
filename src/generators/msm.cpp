#include "../../include/generators/msm.hpp"
#include <stdexcept>
#include <cmath>
#include <string>

namespace rng
{

    MSM::MSM(uint64_t seed) : current_(seed)
    {
        validate_seed(seed);
    }

    void MSM::validate_seed(uint64_t seed) const
    {
        if (seed == 0)
        {
            throw std::invalid_argument("Seed cannot be zero");
        }
        if (seed > MAX_SEED)
        {
            throw std::invalid_argument("Seed must not exceed " + std::to_string(MAX_SEED));
        }
    }

    uint64_t MSM::extract_middle(uint64_t square) const
    {
        // Convert to string to handle the middle extraction more easily
        std::string square_str = std::to_string(square);

        // Pad with leading zeros if necessary to ensure proper length
        while (square_str.length() < 2 * DIGIT_COUNT)
        {
            square_str = "0" + square_str;
        }

        // Extract middle digits
        size_t start = (square_str.length() - DIGIT_COUNT) / 2;
        std::string middle = square_str.substr(start, DIGIT_COUNT);

        // Convert back to integer
        return std::stoull(middle);
    }

    double MSM::generate()
    {
        // Square the current value
        uint64_t square = current_ * current_;

        // Extract the middle digits
        current_ = extract_middle(square);

        // If we get a zero, reset to a non-zero value
        if (current_ == 0)
        {
            current_ = 1234567890123456ULL;
        }

        // Normalize to [0,1)
        return static_cast<double>(current_) / (MAX_SEED + 1.0);
    }

    std::vector<double> MSM::generate_sequence(size_t count)
    {
        std::vector<double> sequence;
        sequence.reserve(count);
        for (size_t i = 0; i < count; ++i)
        {
            sequence.push_back(generate());
        }
        return sequence;
    }

    std::string MSM::get_name() const
    {
        return "Mid-Square Method";
    }

    void MSM::set_seed(uint64_t seed)
    {
        validate_seed(seed);
        current_ = seed;
    }

} // namespace rng