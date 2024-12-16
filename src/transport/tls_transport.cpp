#include "tls_transport.hpp"
#include "../common/logging.hpp"
#include <unistd.h> // for close()

namespace c2me2 {
    namespace transport {

        TLSTransport::TLSTransport(SSL_CTX* ctx, int clientFd)
            : m_ssl(nullptr), m_clientFd(clientFd) {
            m_ssl = SSL_new(ctx);
            if (!m_ssl) {
                c2me2::common::logError("Failed to create SSL structure.");
                return;
            }

            // Associate the SSL object with the client file descriptor
            SSL_set_fd(m_ssl, clientFd);
        }

        TLSTransport::~TLSTransport() {
            if (m_ssl) {
                // Initiate SSL shutdown handshake
                SSL_shutdown(m_ssl);
                SSL_free(m_ssl);
            }
            if (m_clientFd >= 0) {
                close(m_clientFd);
            }
        }

        bool TLSTransport::handshake() {
            // Perform SSL/TLS handshake with the client
            int ret = SSL_accept(m_ssl);
            if (ret <= 0) {
                int err = SSL_get_error(m_ssl, ret);
                c2me2::common::logError("TLS handshake failed. SSL_get_error: " + std::to_string(err));
                ERR_print_errors_fp(stderr);
                return false;
            }
            c2me2::common::logInfo("TLS handshake successful.");
            return true;
        }

        int TLSTransport::readData(char* buf, int bufSize) {
            int ret = SSL_read(m_ssl, buf, bufSize);
            if (ret <= 0) {
                int err = SSL_get_error(m_ssl, ret);
                if (err == SSL_ERROR_ZERO_RETURN) {
                    // Connection closed cleanly by peer
                    c2me2::common::logInfo("TLS connection closed by peer.");
                } else {
                    // Some other error
                    c2me2::common::logError("TLS read error. SSL_get_error: " + std::to_string(err));
                    ERR_print_errors_fp(stderr);
                }
            }
            return ret;
        }

        int TLSTransport::writeData(const char* buf, int size) {
            int ret = SSL_write(m_ssl, buf, size);
            if (ret <= 0) {
                int err = SSL_get_error(m_ssl, ret);
                c2me2::common::logError("TLS write error. SSL_get_error: " + std::to_string(err));
                ERR_print_errors_fp(stderr);
            }
            return ret;
        }

    } // namespace transport
} // namespace c2me2
