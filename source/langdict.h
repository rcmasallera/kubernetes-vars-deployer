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
    .FOPENERROR = "Error al abrir el archivo \n",
    .TEMPFERROR = "Error al crear el archivo temporal \n",
    .FILEPROC = "Procesando archivo: %s\n",
    .VARVALUE = "Ingrese valor para %s: \n",
    .VARFOUNDED = "Variable encontrada: VAR-%s\n",
    .TFILECREATED = "Archivo temporal creado en TEMP: %s\n",
    .DIROPENERROR = "Error al abrir el directorio \n",
    .PROCFILEQUEST = "¿Desea procesar el archivo %s? (yes/no): \n",
    .PROCFILENO = "Archivo %s no procesado.\n",
    .TDIROPERROR = "Error al abrir el directorio TEMP \n",
    .FILEDELETED = "Archivo eliminado: %s\n",
    .DELFILEERROR = "Error al eliminar archivo \n",
    .TEMPDIRDEL = "Directorio TEMP eliminado.\n",
    .TEMPDIRDELERROR = "Error al eliminar el directorio TEMP \n",
    .UNSUPPORTEDLANG =  "Idioma no soportado: %s\n"
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
    .FOPENERROR = "Error oppening file \n",
    .TEMPFERROR = "Error creating temp file \n",
    .FILEPROC = "Processing file: %s\n",
    .VARVALUE = "Input value for %s: \n",
    .VARFOUNDED = "Variable founded: VAR-%s\n",
    .TFILECREATED = "Temp file created at TEMP: %s\n",
    .DIROPENERROR = "Error oppening path \n",
    .PROCFILEQUEST = "Process file: %s? (yes/no): \n",
    .PROCFILENO = "File %s not processed.\n",
    .TDIROPERROR = "Error oppening TEMP dir \n",
    .FILEDELETED = "File deleted: %s\n",
    .DELFILEERROR = "Error deleting file \n",
    .TEMPDIRDEL = "Temp dir deleted.\n",
    .TEMPDIRDELERROR = "Error deleting TEMP dir \n",
    .UNSUPPORTEDLANG =  "Unsupported lang: %s\n"
};
