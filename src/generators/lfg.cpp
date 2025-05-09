#include "../../include/generators/lfg.hpp"
#include <stdexcept>
#include <ctime>

namespace rng
{

    LFG::LFG(uint64_t seed, size_t j, size_t k, char operation)
        : j_(j), k_(k), operation_(operation)
    {

        validate_parameters();
        initialize_state(seed);
    }

    void LFG::validate_parameters() const
    {
        if (j_ >= k_)
        {
            throw std::invalid_argument("j must be less than k");
        }
        if (k_ == 0)
        {
            throw std::invalid_argument("k must be positive");
        }
        if (operation_ != '+' && operation_ != '-' &&
            operation_ != '*' && operation_ != '^')
        {
            throw std::invalid_argument("Invalid operation. Use '+', '-', '*', or '^'");
        }
    }

    uint64_t LFG::combine_values(uint64_t a, uint64_t b) const
    {
        switch (operation_)
        {
        case '+':
            return (a + b) % m_;
        case '-':
            return (a >= b) ? (a - b) : (m_ - (b - a));
        case '*':
            return (a * b) % m_;
        case '^':
            return a ^ b;
        default:
            return (a + b) % m_; // Default to addition
        }
    }

    double LFG::generate()
    {
        // Get the lagged values
        uint64_t x_j = state_[state_.size() - j_ - 1];
        uint64_t x_k = state_[state_.size() - k_ - 1];

        // Calculate next value using the chosen operation
        uint64_t next = combine_values(x_j, x_k);

        // Update state
        state_.pop_front();
        state_.push_back(next);

        // Normalize to [0,1)
        return static_cast<double>(next) / m_;
    }

    std::vector<double> LFG::generate_sequence(size_t count)
    {
        std::vector<double> sequence;
        sequence.reserve(count);
        for (size_t i = 0; i < count; ++i)
        {
            sequence.push_back(generate());
        }
        return sequence;
    }

    std::string LFG::get_name() const
    {
        return "Lagged Fibonacci Generator";
    }

    void LFG::set_seed(uint64_t seed)
    {
        initialize_state(seed);
    }

    void LFG::initialize_state(uint64_t seed)
    {
        state_.clear();

        // Initialize state using linear congruential generator
        uint64_t a = 1664525;
        uint64_t c = 1013904223;
        uint64_t current = seed;

        // Generate k initial values
        for (size_t i = 0; i < k_; ++i)
        {
            current = (a * current + c) % m_;
            state_.push_back(current);
        }
    }

} // namespace rng