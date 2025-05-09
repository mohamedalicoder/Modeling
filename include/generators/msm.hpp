#ifndef MSM_HPP
#define MSM_HPP

#include "../rng.hpp"

namespace rng
{

    class MSM : public RandomGenerator
    {
    public:
        explicit MSM(uint64_t seed);

        double generate() override;
        std::vector<double> generate_sequence(size_t count) override;
        std::string get_name() const override;
        void set_seed(uint64_t seed) override;

    private:
        uint64_t current_;
        static constexpr size_t DIGIT_COUNT = 16;                 // Using 16 digits for better period
        static constexpr uint64_t MAX_SEED = 9999999999999999ULL; // 16 digits max

        uint64_t extract_middle(uint64_t square) const;
        void validate_seed(uint64_t seed) const;
    };

} // namespace rng

#endif // MSM_HPP