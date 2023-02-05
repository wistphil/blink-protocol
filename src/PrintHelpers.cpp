#include "blink/PrintHelpers.hpp"

namespace blink {

const int Indenter::s_xalloc = std::ios_base::xalloc();

Indenter::Indenter(std::ostream & os)
    : os_(os)
    , prev_indent_(os.iword(s_xalloc))
{
    auto new_indent = prev_indent_ + 4;
    os.iword(s_xalloc) = new_indent;
    indent_.assign(static_cast<std::size_t>(new_indent), ' ');
}

Indenter::~Indenter()
{
    os_.iword(s_xalloc) = prev_indent_;
}


} // namespace blink {

