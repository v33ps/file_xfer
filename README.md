# file_xfer

Coming in future branches mbedtls will be required. To build with this, make sure to install mbedtls 
Then build with the command
```
gcc -g -I./include src/*.c -lmbedtls -lmbedx509 -lmbedcrypto
```

## Install mbedtls
```
# make sure this is done in ANOTHER branch 
git clone https://github.com/ARMmbed/mbedtls.git 
cd mbedtls 
cmake . 
make 
sudo make install
```
