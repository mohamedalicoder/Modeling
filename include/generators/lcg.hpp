#ifndef LCG_HPP
#define LCG_HPP

#include "../rng.hpp"

namespace rng
{

    class LCG : public RandomGenerator
    {
    public:
        // Using parameters similar to glibc's rand()
        LCG(uint64_t seed = 1,
            uint64_t a = 1103515245,
            uint64_t c = 12345,
            uint64_t m = 2147483648);

        double generate() override;
        std::vector<double> generate_sequence(size_t count) override;
        std::string get_name() const override;
        void set_seed(uint64_t seed) override;

    private:
        uint64_t current_;
        const uint64_t a_; // multiplier
        const uint64_t c_; // increment
        const uint64_t m_; // modulus

        void validate_parameters() const;
    };

} // namespace rng

#endif // LCG_HPP