#ifndef SPI_H_INCLUDED
#define SPI_H_INCLUDED
// Spotify aPi Interface

#include "api.h"


// Struct to hold the application key and its length.
// mainly used locally.
struct SPI_application_key {
    int len;
    char *data;
};

typedef struct SPI_application_key SPI_application_key;



struct sp_session_config *spi_init_session_config(const char *keyfile);



#endif /* ifndef SPI_H_INCLUDED */
