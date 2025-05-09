#include "../../include/generators/mrg.hpp"
#include <stdexcept>
#include <sstream>

namespace rng
{

    MRG::MRG(const std::vector<uint64_t> &initial_values,
             const std::vector<uint64_t> &multipliers,
             uint64_t m)
        : multipliers_(multipliers), m_(m), k_(multipliers.size())
    {

        if (initial_values.size() != k_)
        {
            throw std::invalid_argument("Number of initial values must match the number of multipliers");
        }

        validate_parameters();

        // Initialize state with initial values
        for (const auto &value : initial_values)
        {
            if (value >= m_)
            {
                throw std::invalid_argument("Initial values must be less than modulus");
            }
            state_.push_back(value);
        }
    }

    void MRG::validate_parameters() const
    {
        if (k_ == 0)
        {
            throw std::invalid_argument("At least one multiplier is required");
        }
        if (m_ <= 0)
        {
            throw std::invalid_argument("Modulus must be positive");
        }

        for (const auto &a : multipliers_)
        {
            if (a >= m_)
            {
                throw std::invalid_argument("Multipliers must be less than modulus");
            }
        }
    }

    double MRG::generate()
    {
        // Calculate next value using the recurrence relation:
        // x_n = (a_1 * x_(n-1) + a_2 * x_(n-2) + ... + a_k * x_(n-k)) mod m
        uint64_t next = 0;

        for (size_t i = 0; i < k_; ++i)
        {
            // Use modular arithmetic to prevent overflow
            uint64_t term = (multipliers_[i] * state_[k_ - 1 - i]) % m_;
            next = (next + term) % m_;
        }

        // Update state
        state_.pop_front();
        state_.push_back(next);

        // Normalize to [0,1)
        return static_cast<double>(next) / m_;
    }

    std::vector<double> MRG::generate_sequence(size_t count)
    {
        std::vector<double> sequence;
        sequence.reserve(count);
        for (size_t i = 0; i < count; ++i)
        {
            sequence.push_back(generate());
        }
        return sequence;
    }

    std::string MRG::get_name() const
    {
        return "Multiple Recursive Generator";
    }

    void MRG::set_seed(uint64_t seed)
    {
        initialize_state(seed);
    }

    void MRG::initialize_state(uint64_t seed)
    {
        state_.clear();

        // Initialize state using the seed and simple transformations
        uint64_t current = seed;
        for (size_t i = 0; i < k_; ++i)
        {
            current = (multipliers_[i] * current + i + 1) % m_;
            state_.push_back(current);
        }
    }

} // namespace rng