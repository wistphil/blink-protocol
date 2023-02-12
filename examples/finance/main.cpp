#include "generated/ExampleSchema.hpp"

#include <array>
#include <iostream>

void handle_messages(std::span<std::uint8_t> data)
{
    while (!data.empty()) {
        blink::DynamicGroupPreamble preamble(data);

        switch (preamble.get_type_id()) {
            case examples::NewOrder::type_id: {
                examples::NewOrder msg(data);
                std::cout << "handle " << msg << '\n';
                break;
            }
            case examples::MassQuote::type_id: {
                examples::MassQuote msg(data);
                std::cout << "handle " << msg << '\n';
                break;
            }
            default:
                std::cout << "Unexpected message type [" << preamble.get_type_id() << "]";
                return;
        }

        data = data.subspan(preamble.get_size() + sizeof(std::uint32_t));
    }
}

auto append_mass_quote(std::vector<std::uint8_t> & buffer) -> void
{
    std::array<std::uint8_t, 256> data{};

    examples::MassQuote mass_quote(data);
    mass_quote.set_seq_num(2);
    mass_quote.set_sender_id("j.doe");
    mass_quote.set_quote_id(12345);
    auto quote_entries = mass_quote.init_quote_entries(3);

    std::uint32_t entry_id{1};
    int strike_price{100};

    for (std::size_t i{0}; i < quote_entries.size(); ++i) {
        const std::string symbol = "AAPL230210C" + std::to_string(strike_price);
        auto quote_entry = quote_entries[i];
        quote_entry.set_symbol(symbol);
        quote_entry.set_quote_entry_id(entry_id++);
        quote_entry.set_bid_price(10.0 + static_cast<double>(i));
        quote_entry.set_bid_size(100);
        quote_entry.set_ask_price(11.0 + static_cast<double>(i));
        quote_entry.set_ask_size(100);
        strike_price += 25;
    }

    mass_quote.set_preamble_size();

    std::copy(mass_quote.data(), mass_quote.data() + mass_quote.size(), std::back_inserter(buffer));
}

auto append_new_order(std::vector<std::uint8_t> & buffer) -> void
{
    std::array<std::uint8_t, 256> data{};

    examples::NewOrder order(data);
    order.set_seq_num(1);
    order.set_sender_id("j.doe");
    order.set_symbol("AAPL");
    order.set_side(static_cast<std::uint8_t>(1));
    order.set_volume(100);
    order.set_client_order_id("ABC123");
    order.set_preamble_size();

    std::copy(order.data(), order.data() + order.size(), std::back_inserter(buffer));
}

auto main() -> int
{
    std::vector<std::uint8_t> data;

    append_new_order(data);
    append_mass_quote(data);

    handle_messages(data);

    return 0;
}
