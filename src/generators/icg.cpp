#include "../../include/generators/icg.hpp"
#include <stdexcept>
#include <numeric>

namespace rng
{

    ICG::ICG(uint64_t seed, uint64_t a, uint64_t b, uint64_t m)
        : current_(seed), a_(a), b_(b), m_(m)
    {
        if (m_ <= 0)
        {
            throw std::invalid_argument("Modulus must be positive");
        }
        if (seed >= m_)
        {
            throw std::invalid_argument("Seed must be less than modulus");
        }
        if (a_ >= m_)
        {
            throw std::invalid_argument("Multiplier must be less than modulus");
        }
        if (b_ >= m_)
        {
            throw std::invalid_argument("Increment must be less than modulus");
        }
    }

    double ICG::generate()
    {
        if (current_ == 0)
        {
            current_ = 1; // Avoid division by zero
        }

        // Calculate inverse modulo m
        uint64_t inverse = multiplicative_inverse(current_);

        // Apply ICG formula: x_(n+1) = (a * inverse(x_n) + b) mod m
        current_ = (a_ * inverse + b_) % m_;

        // Normalize to [0,1)
        return static_cast<double>(current_) / m_;
    }

    std::vector<double> ICG::generate_sequence(size_t count)
    {
        std::vector<double> sequence;
        sequence.reserve(count);
        for (size_t i = 0; i < count; ++i)
        {
            sequence.push_back(generate());
        }
        return sequence;
    }

    std::string ICG::get_name() const
    {
        return "Inversive Congruential Generator";
    }

    void ICG::set_seed(uint64_t seed)
    {
        if (seed >= m_)
        {
            throw std::invalid_argument("Seed must be less than modulus");
        }
        current_ = seed;
    }

    uint64_t ICG::multiplicative_inverse(uint64_t x) const
    {
        // Extended Euclidean Algorithm to find multiplicative inverse
        int64_t t = 0, newt = 1;
        int64_t r = m_, newr = x;

        while (newr != 0)
        {
            int64_t quotient = r / newr;
            int64_t temp = t;
            t = newt;
            newt = temp - quotient * newt;
            temp = r;
            r = newr;
            newr = temp - quotient * newr;
        }

        if (r > 1)
        {
            throw std::runtime_error("Multiplicative inverse does not exist");
        }
        if (t < 0)
        {
            t += m_;
        }

        return static_cast<uint64_t>(t);
    }

} // namespace rng