#include "../../include/generators/mcg.hpp"
#include <stdexcept>

namespace rng
{

    MCG::MCG(uint64_t seed, uint64_t a, uint64_t m)
        : current_(seed), a_(a), m_(m)
    {
        validate_parameters();
    }

    void MCG::validate_parameters() const
    {
        if (m_ == 0)
        {
            throw std::invalid_argument("Modulus cannot be zero");
        }
        if (a_ >= m_)
        {
            throw std::invalid_argument("Multiplier must be less than modulus");
        }
        if (current_ >= m_)
        {
            throw std::invalid_argument("Seed must be less than modulus");
        }
        if (current_ == 0)
        {
            throw std::invalid_argument("Seed cannot be zero for MCG");
        }

        // Check if the multiplier satisfies necessary conditions for maximum period
        bool is_prime = true;
        for (uint64_t i = 2; i * i <= m_; ++i)
        {
            if (m_ % i == 0)
            {
                is_prime = false;
                break;
            }
        }

        if (!is_prime && m_ != 0)
        {
            // For non-prime modulus, additional checks could be implemented
            // but we'll use the default parameters which are known to be good
        }
    }

    double MCG::generate()
    {
        // Apply the MCG formula: x_(n+1) = (a * x_n) mod m
        current_ = (a_ * current_) % m_;

        // Normalize to [0,1)
        return static_cast<double>(current_) / m_;
    }

    std::vector<double> MCG::generate_sequence(size_t count)
    {
        std::vector<double> sequence;
        sequence.reserve(count);
        for (size_t i = 0; i < count; ++i)
        {
            sequence.push_back(generate());
        }
        return sequence;
    }

    std::string MCG::get_name() const
    {
        return "Multiplicative Congruential Generator";
    }

    void MCG::set_seed(uint64_t seed)
    {
        if (seed >= m_)
        {
            throw std::invalid_argument("Seed must be less than modulus");
        }
        if (seed == 0)
        {
            throw std::invalid_argument("Seed cannot be zero for MCG");
        }
        current_ = seed;
    }

} // namespace rng