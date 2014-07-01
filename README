Overview
========
- Very simple OpenSSL demo that illustrates a C++ client loading a trusted
certificate, connecting to a server and then communicating some data to it.

Steps
=====
- build.sh will create a self-signed digital certificate in the folder
./certificate if it doesn't already exist. (See comments in build.sh for more
info)

- It will then build the C++ code into a binary called ssl_client.

- To test:
	- Open a new terminal and run ./secure_server to create an ssl server listening
	on port 15900.
	- Run ./ssl_client to connect and send data to the server.

- You should see ./secure_server output relevant information about the connection.

Certificate Validation
======================
- To test certificate validation you can create another certificate and run
the server in 'untrusted mode'. In practicate this basically means that:

1). We create an new cert an put it in a folder 'untrusted_cert'
2). We point the server to load cert from the 'untrusted_cert' folder cert
3). The ssl_client continues to only trust the original certificate in the
	'certificate' folder.

Todo this, do the following:

1). mkdir untrusted_cert
2). cd untrusted_cert
3). openssl req -new -x509 -nodes -keyout key_pair.pem -out untrusted_cert.pem
4). cd ../
5). ./secure_server untrusted

- Command (3) basically creates a key_pair, a certificate signing request and
signs it using the key pair all in one line. The steps taken in ./build.sh are
more for educational purposes.

Notes
=====
- This is a 'demo', not suitable for production.
- ssl_client is sensitive to where it's run (i.e. it expects to find the
trusted certificate folder in the same directory as itself).
- Compiles with:
	- clang++ - Apple LLVM version 5.1
	- OpenSSL - OpenSSL 0.9.8y 5 Feb 2013
