#ifndef RCS_IP_TCP_HPP
#define RCS_IP_TCP_HPP

#include <sys/socket.h> // IWYU pragma: keep

#include <rcs/execution/executor.hpp>

namespace rcs::ip {

template <typename TCommunicationProtocol,
          rcs::execution::executor TExecutorType>
class socket;

/// @brief Encapsulates the flags identifying TCP/IP.
template <typename TInternetProtocol>
class tcp final {
  public:
    tcp(const tcp &)                     = default;
    tcp(tcp &&)                          = default;
    auto operator=(const tcp &) -> tcp & = default;
    auto operator=(tcp &&) -> tcp      & = default;

  public:
    /// @brief Internet protocol type.
    using ip_t = TInternetProtocol;

  public:
    /// @brief Get the protocol address family
    [[nodiscard]] static constexpr auto family() noexcept
        -> decltype(TInternetProtocol::family()) { return TInternetProtocol::family(); }

    /// @brief Get the protocol type.
    [[nodiscard]] static constexpr auto type() noexcept
        -> int { return SOCK_STREAM; }

  public:
    template <typename TExecutorType>
    using socket = rcs::ip::socket<tcp, TExecutorType>;

  public:
    /// @brief Default constructor.
    tcp() = default;

    /// @brief Default destructor.
    ~tcp() = default;
};

} // namespace rcs::ip

#endif
