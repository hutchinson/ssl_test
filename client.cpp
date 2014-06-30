/* OpenSSL headers */

#include "openssl/bio.h"
#include "openssl/ssl.h"
#include "openssl/err.h"

#include <iostream>

const char *HOST = "localhost:15900";
const char *MESSAGE = "Hello World Using OpenSSL!\n";

// Generating a self signed cert
// 1). First create a private key.
// 


int main(int argc, char **argv)
{
  // Initialise ssl
  SSL_load_error_strings();
  ERR_load_BIO_strings();
  OpenSSL_add_all_algorithms();

  // Create the SSL context
  SSL_CTX *ctx = SSL_CTX_new(SSLv23_client_method());
  SSL *ssl;

  BIO *bio = BIO_new_connect(const_cast<char*>(HOST));

  if(bio == NULL)
  {
    std::cerr << "Couldn't connect to localhost:9999" << std::endl;
    return -1;
  }

  if(BIO_do_connect(bio) <= 0)
  {
    std::cerr << "Couldn't connect!" << std::endl;
    return -1;
  }

  std::cout << "Writing connection " << MESSAGE << std::endl;

  /* Try to write to the connection */
  while( BIO_write(bio, MESSAGE, strlen(MESSAGE)) <= 0)
  {
    if(!BIO_should_retry(bio))
    {
      std::cerr << "Error occurred writing message" << std::endl;
      break;
    }
  }

  /* Free up resources */
  BIO_free_all(bio);

  return 0;
}
