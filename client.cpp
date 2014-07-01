// Standard headers
#include <iostream>

// OpenSSL headers
#include "openssl/bio.h"
#include "openssl/ssl.h"
#include "openssl/err.h"

const char *HOST = "localhost:15900";
const char *PATH_TO_TRUSTED_CERT = "certificate/digital_certificate.cer";

const char *MESSAGE = "\n**** Hello World Using OpenSSL!\n\n" \
                      "This text could potentially be very secret and we wouldn't want\n" \
                      "anyone to know what we were saying, hence the 2048 bit encryption.\n";

// RAII Scoper for SSL Context and BIO objects
struct ssl_ctx_scoper
{
  ssl_ctx_scoper()
    : ptr( SSL_CTX_new(SSLv23_client_method()) )
  {

  }

  ~ssl_ctx_scoper()
  {
    SSL_CTX_free(ptr);
  }

  SSL_CTX *ptr;
};

struct bio_scoper
{
  bio_scoper(SSL_CTX *ctx)
    : ptr( BIO_new_ssl_connect(ctx) )
  {

  }

  ~bio_scoper()
  {
    BIO_free_all(ptr);
  }

  BIO *ptr;
};

// This is a very simple client that connects to a secure host listening on
// port 15900 (provided by running the supplied test server in secure_server).
int main(int argc, char **argv)
{
  // Initialise ssl
  SSL_library_init();

  SSL_load_error_strings();
  ERR_load_BIO_strings();
  OpenSSL_add_all_algorithms();

  // Create the SSL context
  ssl_ctx_scoper ctx;

  // Load the certificate we trust.
  if(!SSL_CTX_load_verify_locations(ctx.ptr, const_cast<char*>(PATH_TO_TRUSTED_CERT), NULL))
  {
    std::cerr << "Couldn't load certificate" << std::endl;
    ERR_print_errors_fp(stderr);
    return -1;
  }

  // Set up the connection to our server.
  SSL *ssl;
  bio_scoper bio(ctx.ptr);
  BIO_get_ssl(bio.ptr, &ssl);
  SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

  BIO_set_conn_hostname(bio.ptr, const_cast<char*>(HOST));

  if(BIO_do_connect(bio.ptr) <= 0)
  {
    std::cerr << "Couldn't connect!" << std::endl;
    ERR_print_errors_fp(stderr);
    return -1;
  }

  // Verify the certificate we got from the server was OK.
  // Test this code by running ./sercure_sever untrusted
  if(SSL_get_verify_result(ssl) != X509_V_OK)
  {
    std::cerr << "Certificate validation error: "
              << SSL_get_verify_result(ssl)
              << std::endl;
    return -1;
  }

  // Send our secure data.
  std::cout << "Writing connection " << MESSAGE << std::endl;

  // Try to write to the connection
  while( BIO_write(bio.ptr, MESSAGE, strlen(MESSAGE)) <= 0)
  {
    if(!BIO_should_retry(bio.ptr))
    {
      std::cerr << "Error occurred writing message" << std::endl;
      break;
    }
  }

  return 0;
}
