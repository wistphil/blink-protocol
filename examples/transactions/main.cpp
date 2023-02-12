#include "generated/ExampleSchema.hpp"

#include <iostream>
#include <vector>

auto main() -> int
{
    std::vector<std::uint8_t> data(256);
    examples::Transactions transactions(data);
    transactions.set_owner("j.doe");
    transactions.set_account("Chase");

    auto sequence = transactions.init_transactions(3);
    for (std::size_t i{0}; i < sequence.size(); ++i) {
        auto transaction = sequence[i];
        transaction.set_description("ABC");
        transaction.set_price(5.25);
        transaction.set_timestamp(20230212);
    }

    std::cout << transactions << '\n';
    return 0;
}
