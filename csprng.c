//! csprng.c generation function
//! written by barrystyle sometime during 2018..

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>

#define BLOCKSIZE 64
#define CHUNKSIZE 4096
#define FILELIMIT 1073741824

int generateFile()
{
    const char *filename = "heavy3a.bin";
    FILE *ft = fopen(filename, "wb");

    unsigned char *buffer;
    char seed[] = "Bismuth is a chemical element with symbol Bi and atomic number 83. It is a pentavalent post-transition metal and one of the pnictogens with chemical properties resembling its lighter homologs arsenic and antimony.";

    char xs[CHUNKSIZE];
    char key[BLOCKSIZE];
    char val[BLOCKSIZE];
    char mixbuf[CHUNKSIZE];
    memset(mixbuf,'\0',CHUNKSIZE);
    memset(key,'\0',BLOCKSIZE);
    memset(val,'\1',BLOCKSIZE);

    memcpy(mixbuf,val,strlen(val));
    memset(mixbuf+64,'\0',1);
    memcpy(mixbuf+65,seed,strlen(seed));
    buffer = HMAC(EVP_sha512(),key,BLOCKSIZE,(uint8_t*)mixbuf,278,NULL,NULL);
    memcpy(key,buffer,BLOCKSIZE);
    memcpy(mixbuf,val,strlen(val));
    buffer = HMAC(EVP_sha512(),key,BLOCKSIZE,(uint8_t*)mixbuf,BLOCKSIZE,NULL,NULL);
    memcpy(val,buffer,BLOCKSIZE);
    memcpy(mixbuf,val,strlen(val));
    memset(mixbuf+64,'\1',1);
    memcpy(mixbuf+65,seed,strlen(seed));
    buffer = HMAC(EVP_sha512(),key,BLOCKSIZE,(uint8_t*)mixbuf,278,NULL,NULL);
    memcpy(key,buffer,BLOCKSIZE);
    memcpy(mixbuf,val,strlen(val));
    buffer = HMAC(EVP_sha512(),key,BLOCKSIZE,(uint8_t*)mixbuf,BLOCKSIZE,NULL,NULL);

    for (int filefill=0; filefill < 262144; filefill++)
    {
		int xslen = 0;
		while(xslen < CHUNKSIZE) {
			buffer = HMAC(EVP_sha512(),key,BLOCKSIZE,(uint8_t*)buffer,BLOCKSIZE,NULL,NULL);
			memcpy(xs+xslen,buffer,BLOCKSIZE);
			xslen += BLOCKSIZE;
		}

		memcpy(mixbuf,buffer,BLOCKSIZE);
		memset(mixbuf+BLOCKSIZE,'\0',1);
		buffer = HMAC(EVP_sha512(),key,BLOCKSIZE,(uint8_t*)mixbuf,BLOCKSIZE+1,NULL,NULL);
                memcpy(key,buffer,BLOCKSIZE);
		buffer = HMAC(EVP_sha512(),key,BLOCKSIZE,(uint8_t*)mixbuf,BLOCKSIZE,NULL,NULL);
		memcpy(val,buffer,BLOCKSIZE);
		fwrite(xs,1,CHUNKSIZE,ft);
		printf("%dMb\r",((filefill*4)/1024));
    }

    fclose(ft);
    return(0);
}

int verifyFile()
{
    const char *filename = "heavy3a.bin";
    FILE *ft = fopen(filename, "rb");

    // csprng file hash is 42dd7e60...
    SHA256_CTX ctx;
    unsigned char singlebyte[1024];
    memset(singlebyte,'\0',1024);
    SHA256_Init(&ctx);
    for (int filetest=0; filetest < 131072; filetest++)
    {
      fseek(ft, (filetest*8192), SEEK_SET);
      fread(singlebyte, 1, 1024, ft);
      SHA256_Update(&ctx,(unsigned char*)singlebyte,1024);
    }
    SHA256_Final(singlebyte,&ctx);
    fclose(ft);

    // test checksum
    if (singlebyte[0] == 0x42 && singlebyte[1] == 0xdd &&
        singlebyte[2] == 0x7e && singlebyte[3] == 0x60)
    {
	printf("checksum matches\n");
        return(0);
    } else {
        printf("checksum fails\n");
        return(1);
    }
}
