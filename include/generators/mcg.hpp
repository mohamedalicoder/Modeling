#ifndef MCG_HPP
#define MCG_HPP

#include "../rng.hpp"

namespace rng
{

    class MCG : public RandomGenerator
    {
    public:
        // MCG is a special case of LCG where c = 0
        MCG(uint64_t seed = 1,
            uint64_t a = 48271,       // Popular multiplier from L'Ecuyer
            uint64_t m = 2147483647); // 2^31 - 1 (Mersenne prime)

        double generate() override;
        std::vector<double> generate_sequence(size_t count) override;
        std::string get_name() const override;
        void set_seed(uint64_t seed) override;

    private:
        uint64_t current_;
        const uint64_t a_; // multiplier
        const uint64_t m_; // modulus

        void validate_parameters() const;
    };

} // namespace rng

#endif // MCG_HPP