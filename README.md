# EasyCrypt
EasyCrypt is a desktop C++ application encrypting text into BMP images (Steganography). It includes two ciphers and a GUI (implemented with Qt) which runs on Linux, Mac OSX, and Windows.
The code has been designed so that one can implement new ciphers easily, by creating a new class inheriting from MethodeDeCryptage (which stands for "cipher" in French).

Disclaimer: The point of this application is to hide data into images, where people would not look for secrets. It does not implement strong ciphers. As a result, the first cipher is not secure against Frequency Analysis. The second cipher protects the data with a password, but it is not secure against Known Plaintext Attack. The password is not stored, but once an attacker recovers the plaintext, he can recover the password as well.
