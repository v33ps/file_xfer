#include <stdio.h>
#include <string.h>
#include "crypto.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"


int create_aes_key()
{
    // variables
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_entropy_context entropy;
    unsigned char key[32]; // holds the 256bit AES key

    // make this unique to your application
    char *pers = "aes generate key";
    int ret;

    mbedtls_entropy_init( &entropy );
    mbedtls_ctr_drbg_init( &ctr_drbg );


    if( ( ret = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy,
        (unsigned char *) pers, strlen( pers ) ) ) != 0 )
    {
        printf( " failed\n ! mbedtls_ctr_drbg_init returned -0x%04x\n", -ret );
        return -1;
    }

    if( ( ret = mbedtls_ctr_drbg_random( &ctr_drbg, key, 32 ) ) != 0 )
    {
        printf( " failed\n ! mbedtls_ctr_drbg_random returned -0x%04x\n", -ret );
        return -1;
    }

    printf("key: %s\n", key);
    return 0;
}

