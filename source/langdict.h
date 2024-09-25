typedef struct {
    char* MANUALMODE;
    char* ERRORUSER;
    char* DIRNOTIFY;
    char* DIRSCANNING;
    char* TEMPFILESCREATED;
    char* DELTEMPQUEST;
    char* DELTEMPNO;
    char* DELTEMPYES;
    char* DELTEMPERROR;
    char* NONPRINTABLE;
    char* YAMLPROCESSING;
    char* FOPENERROR;
    char* TEMPFERROR;
    char* FILEPROC;
    char* VARVALUE;
    char* VARFOUNDED;
    char* TFILECREATED;
    char* DIROPENERROR;
    char* PROCFILEQUEST;
    char* PROCFILENO;
    char* TDIROPERROR;
    char* FILEDELETED;
    char* DELFILEERROR;
    char* TEMPDIRDEL;
    char* TEMPDIRDELERROR;
    char* UNSUPPORTEDLANG;
    char* DIRCREATFAIL;
    char* ALLOCATIONERROR;
    char* UNSUPPORTEDMODE;
} Dictionary;

Dictionary spa = {
    .MANUALMODE = "Ejecutando en modo manual.\n",
    .ERRORUSER = "Error: Este programa NO debe ejecutarse como root.\n",
    .DIRNOTIFY = "Directorio TEMP creado.\n",
    .DIRSCANNING = "Escaneando directorio actual y subdirectorios...\n",
    .TEMPFILESCREATED = "Todos los archivos temporales se han creado en el directorio TEMP.\n",
    .DELTEMPQUEST = "¿Desea eliminar los archivos temporales y el directorio TEMP? (yes/no): ",
    .DELTEMPNO = "Los archivos temporales no se eliminarán.\n",
    .DELTEMPYES = "Archivo temporal eliminado: %s \n",
    .DELTEMPERROR = "Error al eliminar archivo temporal \n",
    .NONPRINTABLE = "Carácter no imprimible (ASCII): %d",
    .YAMLPROCESSING = "Procesando yaml \n",
    .FOPENERROR = "Error al abrir el archivo %s ",
    .TEMPFERROR = "Error al crear el archivo temporal \n",
    .FILEPROC = "Procesando archivo: ",
    .VARVALUE = "Ingrese valor para: ",
    .VARFOUNDED = "Variable encontrada en yaml: VAR-",
    .TFILECREATED = "Archivo temporal creado en TEMP: %s\n",
    .DIROPENERROR = "Error al abrir el directorio \n",
    .PROCFILEQUEST = "¿Desea procesar el archivo %s? (yes/no): ",
    .PROCFILENO = "Archivo %s no procesado.\n",
    .TDIROPERROR = "Error al abrir el directorio TEMP \n",
    .FILEDELETED = "Archivo eliminado: %s\n",
    .DELFILEERROR = "Error al eliminar archivo \n",
    .TEMPDIRDEL = "Directorio TEMP eliminado.\n",
    .TEMPDIRDELERROR = "Error al eliminar el directorio TEMP \n",
    .UNSUPPORTEDLANG =  "Idioma no soportado: %s\n",
    .DIRCREATFAIL = "Error al crear el directorio %s\n",
    .ALLOCATIONERROR = "Error en la reserva de memoria\n",
    .UNSUPPORTEDMODE = "Modo: %s no soportado\n"
};

Dictionary eng = {
    .MANUALMODE = "Running at manual mode.\n",
    .ERRORUSER = "This program cant run as root.\n",
    .DIRNOTIFY = "Temp dir created.\n",
    .DIRSCANNING = "Scanning actual path recursively...\n",
    .TEMPFILESCREATED = "All temp files has been created at TEMP dir.\n",
    .DELTEMPQUEST = "Remove temp files? (yes/no): ",
    .DELTEMPNO = "Temp files will not be deleted.\n",
    .DELTEMPYES = "Temp file deleted: %s \n",
    .DELTEMPERROR = "Error deleting temp file \n",
    .NONPRINTABLE = "Not printable character (ASCII): %d",
    .YAMLPROCESSING = "Processing yaml \n",
    .FOPENERROR = "Error oppening file  %s \n",
    .TEMPFERROR = "Error creating temp file \n",
    .FILEPROC = "Processing file: ",
    .VARVALUE = "Input value for %s: \n",
    .VARFOUNDED = "Variable founded: VAR-%s\n",
    .TFILECREATED = "Temp file created at TEMP: %s\n",
    .DIROPENERROR = "Error oppening path \n",
    .PROCFILEQUEST = "Process file: %s? (yes/no): ",
    .PROCFILENO = "File %s not processed.\n",
    .TDIROPERROR = "Error oppening TEMP dir \n",
    .FILEDELETED = "File deleted: %s\n",
    .DELFILEERROR = "Error deleting file \n",
    .TEMPDIRDEL = "Temp dir deleted.\n",
    .TEMPDIRDELERROR = "Error deleting TEMP dir \n",
    .UNSUPPORTEDLANG =  "Unsupported lang: %s\n",
    .DIRCREATFAIL = "Error creating dir %s\n",
    .ALLOCATIONERROR = "Error allocating memory\n",
    .UNSUPPORTEDMODE = "Unsupported: %s mode\n"
};

typedef struct{
    char* USAGETITTLE;
    char* USAGEVERNOSE;
    char* USAGELANGUAJE;
    char* USAGEPATH;
    char* USAGECONFIG;
} UsageDict;

UsageDict usspa = {
    .USAGETITTLE = "Uso: deployer [opciones]\n",
    .USAGEVERNOSE = "  -v, --verbose       Modo detallado\n",
    .USAGELANGUAJE = "  -l, --language=LANG Seleccionar idioma (esp o eng)\n",
    .USAGEPATH = "  -p, --path=PATH     Directorio de trabajo\n",
    .USAGECONFIG = "  -c, --config=FILE   Archivo de configuración\n"
};

UsageDict useng = {
    .USAGETITTLE = "Usage: deployer [options]\n",
    .USAGEVERNOSE = "  -v, --verbose       detailed mode\n",
    .USAGELANGUAJE = "  -l, --language=LANG select languaje (esp or eng)\n",
    .USAGEPATH = "  -p, --path=PATH     Workdir path\n",
    .USAGECONFIG = "  -c, --config=FILE   Config file path\n"
};

static void print_usage() {
    printf("%s \n", usspa.USAGETITTLE   );
    printf("%s \n", usspa.USAGECONFIG   );
    printf("%s \n", usspa.USAGELANGUAJE   );
    printf("%s \n", usspa.USAGEPATH   );
    printf("%s \n", usspa.USAGEVERNOSE   );
    printf("%s \n", useng.USAGETITTLE   );
    printf("%s \n", useng.USAGECONFIG   );
    printf("%s \n", useng.USAGELANGUAJE   );
    printf("%s \n", useng.USAGEPATH   );
    printf("%s \n", useng.USAGEVERNOSE   );
}