#ifndef MENU_HANDLER_HPP
#define MENU_HANDLER_HPP

#include "../rng.hpp"
#include <memory>
#include <vector>

namespace rng
{

    class MenuHandler
    {
    public:
        MenuHandler();
        void run();

    private:
        std::vector<std::unique_ptr<RandomGenerator>> generators_;
        std::vector<std::unique_ptr<RandomnessTest>> tests_;

        void initialize_generators();
        void display_main_menu() const;
        void handle_generator_selection();
        void handle_generator_parameters(RandomGenerator *generator);
        void handle_sequence_generation(RandomGenerator *generator);
        void handle_test_selection(const std::vector<double> &numbers);
        void display_test_results(const std::vector<double> &numbers, double significance_level);

        // Helper functions
        uint64_t get_valid_seed() const;
        size_t get_valid_sequence_length() const;
        double get_valid_significance_level() const;
        void clear_screen() const;
        void pause() const;
    };

} // namespace rng

#endif // MENU_HANDLER_HPP