#define MAX_FILE_NAME   256
#define RESTO_HEADER (TAM_S_HEADER - TAM_S_INFO)
#define TAM_S_HEADER    512
#define TAM_S_INFO sizeof(struct s_info )

#define TAM_BUFFER      512

#define ERR1 1
#define ERR2 2
#define ERR3 3
#define ERR4 4

#define ERR_MSG1 "Error en los argumentos.\nPuedes usar <./mypackzip help>\n"
#define ERR_MSG2 "No existe el fichero de origen\n"
#define ERR_MSG3 "No se dispone de permisos para esta operación. No se dispone de permisos para crear o modificar fich_destino\n"
#define ERR_MSG4 "Otro tipo de errores\n"


struct s_info {
           char Tipo;                       // ='Z'
           char Compri;                     // ='N' (por defecto) o 'Y'. Indica si los datos del  
                                            // fichero están almacenados de forma comprimida
           unsigned long TamOri;            // Tamanio original del fichero (sin comprimir)
           unsigned long TamComp;           // Tamanio del fichero comprimido. Si los datos no estan
                                            // comprimidos, su valor es el mismo de TamOri-
           char FileName[MAX_FILE_NAME];    // Nombre del fichero origen 
};

struct  s_header {
           struct s_info InfoF;                 // Información del fichero
           char RestoHeader[RESTO_HEADER];       // ToDo...  
};

int extraer_fichero(char * file_mypackzip, unsigned long Posicion);
int insertar_fichero(char * fich_origen, long Posicion, char * file_mypackzip);
int insertar_directorio(char *dir_fuente,  char *file_mypackzip);
int extraer_directorio(char *dir_destino,  char *file_mypackzip);
void quitar_primero (char* dir_fuente);
