#ifndef LFG_HPP
#define LFG_HPP

#include "../rng.hpp"
#include <deque>

namespace rng
{

    class LFG : public RandomGenerator
    {
    public:
        // j and k are the lag values where j < k
        LFG(uint64_t seed, size_t j, size_t k, char operation = '+');

        double generate() override;
        std::vector<double> generate_sequence(size_t count) override;
        std::string get_name() const override;
        void set_seed(uint64_t seed) override;

    private:
        std::deque<uint64_t> state_;
        const size_t j_;                           // first lag
        const size_t k_;                           // second lag
        const char operation_;                     // operation to use ('+', '-', '*', or '^')
        static constexpr uint64_t m_ = 1ULL << 32; // modulus (2^32)

        void initialize_state(uint64_t seed);
        uint64_t combine_values(uint64_t a, uint64_t b) const;
        void validate_parameters() const;
    };

} // namespace rng

#endif // LFG_HPP