#include "../../include/menu/menu_handler.hpp"
#include "../../include/generators/icg.hpp"
#include "../../include/generators/mrg.hpp"
#include "../../include/generators/lfg.hpp"
#include "../../include/generators/msm.hpp"
#include "../../include/generators/lcg.hpp"
#include "../../include/generators/mcg.hpp"
#include "../../include/tests/randomness_tests.hpp"
#include <iostream>
#include <limits>
#include <cstdlib>

namespace rng
{

    MenuHandler::MenuHandler()
    {
        initialize_generators();
        tests_ = create_test_suite();
    }

    void MenuHandler::initialize_generators()
    {
        generators_.push_back(std::make_unique<ICG>());
        generators_.push_back(std::make_unique<MRG>(std::vector<uint64_t>{1, 2, 3}, std::vector<uint64_t>{1, 1, 1}));
        generators_.push_back(std::make_unique<LFG>(1, 3, 7));
        generators_.push_back(std::make_unique<MSM>(12345));
        generators_.push_back(std::make_unique<LCG>());
        generators_.push_back(std::make_unique<MCG>());
    }

    void MenuHandler::display_main_menu() const
    {
        std::cout << "\nRandom Number Generator Suite\n";
        std::cout << "============================\n\n";
        std::cout << "Available Generators:\n";

        for (size_t i = 0; i < generators_.size(); ++i)
        {
            std::cout << i + 1 << ". " << generators_[i]->get_name() << '\n';
        }

        std::cout << "0. Exit\n\n";
        std::cout << "Select a generator (0-" << generators_.size() << "): ";
    }

    void MenuHandler::run()
    {
        while (true)
        {
            clear_screen();
            display_main_menu();

            size_t choice;
            std::cin >> choice;

            if (choice == 0)
                break;

            if (choice > 0 && choice <= generators_.size())
            {
                handle_generator_selection();
            }
            else
            {
                std::cout << "\nInvalid choice. Press Enter to continue...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
            }
        }
    }

    void MenuHandler::handle_generator_selection()
    {
        clear_screen();
        std::cout << "Generator Parameters\n";
        std::cout << "===================\n\n";

        uint64_t seed = get_valid_seed();
        size_t sequence_length = get_valid_sequence_length();

        RandomGenerator *generator = generators_[0].get(); // Using first generator as example
        generator->set_seed(seed);

        std::vector<double> numbers = generator->generate_sequence(sequence_length);

        std::cout << "\nGenerated " << sequence_length << " numbers.\n";
        std::cout << "Would you like to run randomness tests? (y/n): ";

        char choice;
        std::cin >> choice;

        if (choice == 'y' || choice == 'Y')
        {
            handle_test_selection(numbers);
        }
    }

    void MenuHandler::handle_test_selection(const std::vector<double> &numbers)
    {
        clear_screen();
        std::cout << "Randomness Tests\n";
        std::cout << "================\n\n";

        double significance_level = get_valid_significance_level();
        display_test_results(numbers, significance_level);
    }

    void MenuHandler::display_test_results(const std::vector<double> &numbers, double significance_level)
    {
        clear_screen();
        std::cout << "Test Results\n";
        std::cout << "============\n\n";

        for (const auto &test : tests_)
        {
            std::cout << "Running " << test->get_test_name() << "...\n";
            bool passed = test->run_test(numbers, significance_level);
            std::cout << test->get_test_result() << "\n\n";
        }

        pause();
    }

    uint64_t MenuHandler::get_valid_seed() const
    {
        uint64_t seed;
        std::cout << "Enter seed value: ";
        std::cin >> seed;
        return seed;
    }

    size_t MenuHandler::get_valid_sequence_length() const
    {
        size_t length;
        std::cout << "Enter sequence length: ";
        std::cin >> length;
        return length;
    }

    double MenuHandler::get_valid_significance_level() const
    {
        std::cout << "Select significance level:\n";
        std::cout << "1. 0.01\n";
        std::cout << "2. 0.05\n";
        std::cout << "3. 0.10\n";
        std::cout << "Choice (1-3): ";

        int choice;
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            return 0.01;
        case 2:
            return 0.05;
        case 3:
            return 0.10;
        default:
            return 0.05; // Default to 0.05
        }
    }

    void MenuHandler::clear_screen() const
    {
#ifdef _WIN32
        std::system("cls");
#else
        std::system("clear");
#endif
    }

    void MenuHandler::pause() const
    {
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }

} // namespace rng