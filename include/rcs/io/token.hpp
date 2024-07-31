#ifndef RCS_IO_TOKEN_HPP
#define RCS_IO_TOKEN_HPP

namespace rcs::io {

/// @brief Asynchronous completion token.
template <typename TResultType, typename TContinuation>
struct token {
    TResultType   result;
    TContinuation continuation;
};

} // namespace rcs::io

#endif
