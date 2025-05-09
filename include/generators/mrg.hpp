#ifndef MRG_HPP
#define MRG_HPP

#include "../rng.hpp"
#include <deque>

namespace rng
{

    class MRG : public RandomGenerator
    {
    public:
        // k is the order of recursion, a contains the multipliers, m is the modulus
        MRG(const std::vector<uint64_t> &initial_values,
            const std::vector<uint64_t> &multipliers,
            uint64_t m = 2147483647);

        double generate() override;
        std::vector<double> generate_sequence(size_t count) override;
        std::string get_name() const override;
        void set_seed(uint64_t seed) override;

    private:
        std::deque<uint64_t> state_;        // Stores the k most recent values
        std::vector<uint64_t> multipliers_; // Coefficients a_i
        const uint64_t m_;                  // modulus
        const size_t k_;                    // order of recursion

        void initialize_state(uint64_t seed);
        void validate_parameters() const;
    };

} // namespace rng

#endif // MRG_HPP