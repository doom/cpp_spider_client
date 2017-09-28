//
// Created by doom on 27/09/17.
//

#ifndef SPIDER_SERVER_SSLCONNECTION_HPP
#define SPIDER_SERVER_SSLCONNECTION_HPP

#include <boost/asio/ssl.hpp>
#include <boost/asio.hpp>
#include <Network/SSLContext.hpp>
#include <Network/IOManager.hpp>

namespace asio = boost::asio;

namespace spi
{
    class SSLConnection
    {
    public:
        enum HandshakeType
        {
            Client,
            Server,
        };

        using Socket = asio::ssl::stream<asio::ip::tcp::socket>;

        SSLConnection(net::IOService &service, net::SSLContext &ctx) noexcept : _socket(service.get(), ctx.get())
        {}

        template <typename CallBackT>
        void asyncHandshake(HandshakeType type, CallBackT &&cb) noexcept
        {
            _socket.async_handshake(static_cast<asio::ssl::stream_base::handshake_type>(type),
                                    std::forward<CallBackT>(cb));
        }

        template <typename BufferT, typename CallBackT>
        void asyncReadSome(const BufferT &buff, CallBackT &&cb) noexcept
        {
            _socket.async_read_some(boost::asio::buffer(buff.data(), buff.size()), std::forward<CallBackT>(cb));
        }

        template <typename BufferT, typename CallBackT>
        void asyncWriteSome(const BufferT &buff, CallBackT &&cb) noexcept
        {
            _socket.async_write_some(boost::asio::buffer(buff.data(), buff.size()), std::forward<CallBackT>(cb));
        }

        Socket::lowest_layer_type &rawSocket() noexcept
        {
            return _socket.lowest_layer();
        }

        Socket &socket() noexcept
        {
            return _socket;
        }

    private:
        Socket _socket;
    };
}

#endif //SPIDER_SERVER_SSLCONNECTION_HPP
