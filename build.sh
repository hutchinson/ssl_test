#!/usr/bin/env bash

mkdir -pv certificate
cd certificate

###############################################################################
# PKI Setup
# The following steps create an RSA key-pair, then create a certificate signing
# request, finally the certificate signing request is signed (by the same
# certificate) therefore creating a self signed certificated.
#
# Note: This could be done in one step using the OpenSSL command line utility
# however it's done this way to demonstrate the concept! For completeness the
# one liner would be:
#
# openssl req -new -x509 -nodes -keyout key_pair.pem -out signed_cert.pem
###############################################################################

# First, generate a 2048 bit RSA keypair. Usually, keys come in pairs - public,
# and private. Because we're using OpenSSL the private key actually contains
# the public key information also. So only one file is actually produced.
# See https://www.openssl.org/docs/HOWTO/keys.txt for more info.
# Also see: https://www.openssl.org/docs/HOWTO/certificates.txt
# and https://www.openssl.org/docs/apps/openssl.html
if [ ! -f key_pair.pem ]; then
	echo "Creating server RSA keypair"
	openssl genrsa -out key_pair.pem 2048
else
	echo "key_pair already exists."
fi

# Second we create a certificate signing request, it contains a good deal of
# information about us such as our country, name, email address etc.
#
# It also contains our public key (which is provided in key_pair.pem). When
# a Certificate Authority recieves this, they sign it with their private key,
# that's right! They sign the certificate request to turn it into a
# certificate.
#
# It's then sent back to you as your certificate.
if [ ! -f certificate_signing_request.csr ]; then
	echo "Generating certificate signing request, prepare to provide some"
	echo "information!"
	openssl req -new -key key_pair.pem -out certificate_signing_request.csr
else
	echo "certificate_signing_request already exists."
fi

# In this final stage, we take our key pair produced in step one, and the signing
# request we created in step 2 and we sign the request with the key pair. This is
# called self-signing!
if [ ! -f digital_certificate.cer ]; then
	echo "Self-signing certificate_signing_request.csr with key_pair.pem to produce"
	echo "a self-signed digital certificate."
	openssl x509 -req -days 365 -in certificate_signing_request.csr\
		 -signkey key_pair.pem -out digital_certificate.cer
else
	echo "Digital certificate already exists."
fi

cd ../

rm -f ssl_client
clang++ client.cpp -Wall -Werror -Wno-deprecated-declarations -o ssl_client -lssl -lcrypto
