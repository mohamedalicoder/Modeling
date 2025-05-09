#ifndef ICG_HPP
#define ICG_HPP

#include "../rng.hpp"

namespace rng
{

    class ICG : public RandomGenerator
    {
    public:
        ICG(uint64_t seed = 1, uint64_t a = 1, uint64_t b = 0, uint64_t m = 2147483647);

        double generate() override;
        std::vector<double> generate_sequence(size_t count) override;
        std::string get_name() const override;
        void set_seed(uint64_t seed) override;

    private:
        uint64_t current_;
        const uint64_t a_; // multiplier
        const uint64_t b_; // increment
        const uint64_t m_; // modulus

        uint64_t multiplicative_inverse(uint64_t x) const;
    };

} // namespace rng

#endif // ICG_HPP