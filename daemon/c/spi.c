#include "spi.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdlib.h>
#include <stdio.h>

#include <string.h>

void loadAppKey(char *fname, SPI_application_key *key){
    int fd = open(fname, O_RDONLY);

    if(fd < 0){
        // Error!
        return;
    }

    // Get the filesize
    struct stat s;
    fstat(fd, &s);
    key->len = s.st_size;

    key->data = (char *) malloc(key->len);
    int nread = read(fd, key->data, key->len);

    close(fd);
    
    if(nread < key->len){
        // ERROR!
    }
}

// CREATE and fill a sp_session_config struct with mostly default options,
// but use the application key from the file {keyfile},
// and dont set specific things like callbacks
struct sp_session_config *spi_init_session_config(const char *keyfile){
    // Create the struct
    struct sp_session_config *config = malloc(sizeof(sp_session_config));
    if(config == NULL){
        printf("error getting memory for sp_session_config *config\n");
        exit(1);
    }
    memset(config, 0, sizeof(sp_session_config));

    // Start to fill it with data
    config->api_version = SPOTIFY_API_VERSION;
    config->user_agent = "spot-beans c0.1";
    
    // Get the application key from {keyfile}
    SPI_application_key *key = malloc(sizeof(SPI_application_key));
    memset(key, 0, sizeof(SPI_application_key));

    config->application_key = key->data;
    config->application_key_size = key->len;

    free(key); // key->data has been allocated separatly

    return config;
}
