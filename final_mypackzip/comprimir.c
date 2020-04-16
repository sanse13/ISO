#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include "zlib.h"
#include "util.h"

#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

#define CHUNK 16384
int Error_RW = 0; // Read error(1)   Write error(2)


int def(int  fd_source, int fd_dest, int level, char * fich_origen, char * dest_file)
{

    int ret, ret2, flush;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];

    struct s_header header;
    struct stat statVar;
    struct stat statVar2;
    char buf[50];
    char res[50];
    char buffer[TAM_BUFFER];
    int tam, n;
    
    strcpy(header.InfoF.FileName, fich_origen);
    lstat(fich_origen, &statVar);
    lstat(dest_file, &statVar2);
    if (S_ISLNK(statVar.st_mode)){
        header.InfoF.Tipo = 'S';
        tam = readlink(fich_origen, buf, 50);
        strncpy(res, buf, tam);
        strcpy(header.InfoF.OriginalName, res);

    } else {
        header.InfoF.Tipo = 'Z';
        strcpy(header.InfoF.OriginalName, "");
    }

    header.InfoF.Compri = 'Y';
    header.InfoF.TamOri = statVar.st_size;
    header.InfoF.TamComp = statVar2.st_size;
    write(fd_dest, &header, sizeof(header));
    n = read(fd_source, buffer, TAM_BUFFER);
    write(fd_dest, buffer, n);

    /* allocate deflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, level);
    if (ret != Z_OK)
        return ret;

    /* compress until end of file */
    do {
        // strm.avail_in = fread(in, 1, CHUNK, source);
        ret2 = read(fd_source, in, CHUNK);
        strm.avail_in = ret2;
        // if (ferror(source)) {
        if (ret2 == -1) {
            (void)deflateEnd(&strm);
            Error_RW = 1 ; // Read error
            return Z_ERRNO;
        }
        // flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
        flush = (ret2== 0) ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = in;

        /* run deflate() on input until output buffer not full, finish
           compression if all of source has been read in */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = deflate(&strm, flush);    /* no bad return value */
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            have = CHUNK - strm.avail_out;
            //if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
            if (write(fd_dest, out, have ) != have ) {
                (void)deflateEnd(&strm);
                 Error_RW = 2 ; // Write error
                 return Z_ERRNO;
            }
        } while (strm.avail_out == 0);
        assert(strm.avail_in == 0);     /* all input will be used */

        /* done when last data in file processed */
    } while (flush != Z_FINISH);
    assert(ret == Z_STREAM_END);        /* stream will be complete */

    /* clean up and return */
    (void)deflateEnd(&strm);
    return Z_OK;
}

void zerr(int ret)
{
    fputs("zpipe: ", stderr);
    switch (ret) {
    case Z_ERRNO:
        // if (ferror(stdin))
        if ( Error_RW == 1 ) // Read error
            fputs("error reading stdin\n", stderr);
        // if (ferror(stdout))
        if ( Error_RW == 2 ) // Write error
            fputs("error writing stdout\n", stderr);
        break;
    case Z_STREAM_ERROR:
        fputs("invalid compression level\n", stderr);
        break;
    case Z_DATA_ERROR:
        fputs("invalid or incomplete deflate data\n", stderr);
        break;
    case Z_MEM_ERROR:
        fputs("out of memory\n", stderr);
        break;
    case Z_VERSION_ERROR:
        fputs("zlib version mismatch!\n", stderr);
    }
}