#include "../../include/generators/lcg.hpp"
#include <stdexcept>

namespace rng
{

    LCG::LCG(uint64_t seed, uint64_t a, uint64_t c, uint64_t m)
        : current_(seed), a_(a), c_(c), m_(m)
    {
        validate_parameters();
    }

    void LCG::validate_parameters() const
    {
        if (m_ == 0)
        {
            throw std::invalid_argument("Modulus cannot be zero");
        }
        if (a_ >= m_)
        {
            throw std::invalid_argument("Multiplier must be less than modulus");
        }
        if (c_ >= m_)
        {
            throw std::invalid_argument("Increment must be less than modulus");
        }
        if (current_ >= m_)
        {
            throw std::invalid_argument("Seed must be less than modulus");
        }

        // Check if parameters satisfy Hull-Dobell theorem
        if (m_ > 0)
        {
            // 1. c and m are relatively prime
            if (c_ != 0)
            {
                uint64_t c_temp = c_;
                uint64_t m_temp = m_;
                while (m_temp != 0)
                {
                    uint64_t t = c_temp % m_temp;
                    c_temp = m_temp;
                    m_temp = t;
                }
                if (c_temp != 1)
                {
                    throw std::invalid_argument("Increment and modulus must be relatively prime");
                }
            }

            // 2. a-1 is divisible by all prime factors of m
            // 3. a-1 is divisible by 4 if m is divisible by 4
            // These conditions are assumed to be satisfied by the default parameters
        }
    }

    double LCG::generate()
    {
        // Apply the LCG formula: x_(n+1) = (a * x_n + c) mod m
        current_ = (a_ * current_ + c_) % m_;

        // Normalize to [0,1)
        return static_cast<double>(current_) / m_;
    }

    std::vector<double> LCG::generate_sequence(size_t count)
    {
        std::vector<double> sequence;
        sequence.reserve(count);
        for (size_t i = 0; i < count; ++i)
        {
            sequence.push_back(generate());
        }
        return sequence;
    }

    std::string LCG::get_name() const
    {
        return "Linear Congruential Generator";
    }

    void LCG::set_seed(uint64_t seed)
    {
        if (seed >= m_)
        {
            throw std::invalid_argument("Seed must be less than modulus");
        }
        current_ = seed;
    }

} // namespace rng