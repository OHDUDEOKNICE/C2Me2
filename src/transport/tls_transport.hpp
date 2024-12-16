#ifndef C2ME2_TRANSPORT_TLS_TRANSPORT_HPP
#define C2ME2_TRANSPORT_TLS_TRANSPORT_HPP

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <string>

namespace c2me2 {
    namespace transport {

        class TLSTransport {
        public:
            // Constructor takes an existing SSL_CTX and a client file descriptor.
            // The client_fd should be the accepted socket from accept().
            TLSTransport(SSL_CTX* ctx, int clientFd);

            // Destructor performs an SSL shutdown and frees the SSL structure,
            // also closes the underlying client file descriptor.
            ~TLSTransport();

            // Perform the TLS handshake with the client. Returns true if successful.
            bool handshake();

            // Read data from the TLS connection into buf. Returns number of bytes read,
            // or <=0 on error/connection closed.
            int readData(char* buf, int bufSize);

            // Write data to the TLS connection from buf. Returns number of bytes written,
            // or <=0 on error/connection closed.
            int writeData(const char* buf, int size);

        private:
            SSL* m_ssl;
            int m_clientFd;
        };

    } // namespace transport
} // namespace c2me2

#endif // C2ME2_TRANSPORT_TLS_TRANSPORT_HPP
