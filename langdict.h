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
    .TEMPDIRDELERROR = "Error al eliminar el directorio TEMP \n"
};

Dictionary eng = {
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
    .TEMPDIRDELERROR = "Error al eliminar el directorio TEMP \n"
};
