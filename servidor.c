#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MIDA_BUFFER 2048
#define MAX_ANIMALES 100
#define ANIMALS_FILE "animales.txt"

char usuaris[3][10] = {"admin", "Oscar_4", "Rogelio_6"};
int contra[3] = {123, 20052, 20053};

typedef enum {
    TIGRE,
    ELEFANTE,
    LEON,
    OSO,
} Especie;

typedef enum {
    SELVA,
    SABANA,
    BOSQUE,
} Habitat;

typedef struct {
    Especie especie;
    char nombre[50]; //Nom de l'animal concret
    Habitat habitat;
    char descripcion[100]; //Petit text
    char sexo;  //Resumim posant M per mascle i F per famella
    int edad;
    char pais[50]; //País de procedencia
} Animal;

Animal animales[MAX_ANIMALES];
int num_animales = 0;

// Function to load animals from file
void cargar_animales() {
    FILE *file = fopen(ANIMALS_FILE, "r");
    if (file == NULL) {
        perror("Error al abrir el archivo de animales.");
        exit(1);
    }
    
    num_animales = 0;
    char buffer[MIDA_BUFFER];

    // Leer cada línea del archivo
    while (fgets(buffer, MIDA_BUFFER, file) != NULL) {
        Animal temp;
        char descripcion[100];

        // Extraer datos desde la línea, capturando la descripción completa entre comillas
        int matched = sscanf(buffer, "%d %49s %d \"%99[^\"]\" %c %d %49s",
                             (int*)&temp.especie,
                             temp.nombre,
                             (int*)&temp.habitat,
                             descripcion,
                             &temp.sexo,
                             &temp.edad,
                             temp.pais);

        if (matched == 7) {  // Asegurarse de que se leyeron todos los campos correctamente
            strncpy(temp.descripcion, descripcion, sizeof(temp.descripcion) - 1);
            temp.descripcion[sizeof(temp.descripcion) - 1] = '\0';  // Asegura el terminador nulo

            // Agregar el animal leído al arreglo de animales
            animales[num_animales++] = temp;
        }
    }

    printf("Número de animales cargados: %d\n", num_animales);
    fclose(file);
}


// Function to save animals to file
void guardar_animales() {
    FILE *file = fopen(ANIMALS_FILE, "w");
    if (file == NULL) {
        perror("Error al abrir el archivo para guardar animales.");
        exit(1);
    }
    //imprimirà linea a linea els animals al arxiu
    for (int i = 0; i < num_animales; i++) {
        fprintf(file, "%d %s %d \"%s\" %c %d %s\n",
                animales[i].especie,
                animales[i].nombre,
                animales[i].habitat,
                animales[i].descripcion,
                animales[i].sexo,
                animales[i].edad,
                animales[i].pais);
    }
    fclose(file);
}



/**
 * @brief Agrega un nuevo animal al registro de animales.
 * 
 * Esta función crea un nuevo animal y lo agrega a la lista de animales registrados, 
 * almacenando sus datos si aún no se ha alcanzado el límite máximo de animales.
 * 
 * @param especie Tipo de especie al que pertenece el animal (tipo `Especie`).
 * @param nombre Nombre del animal como cadena de caracteres.
 * @param habitat Hábitat del animal (tipo `Habitat`).
 * @param descripcion Descripción del animal como cadena de caracteres.
 * @param sexo Sexo del animal ('M' para macho o 'F' para hembra).
 * @param edad Edad del animal en años.
 * @param pais País de origen del animal como cadena de caracteres.
 * 
 * Detalles:
 *  - Si el número actual de animales (`num_animales`) es menor que el máximo permitido (`MAX_ANIMALES`),
 *    se asignan los valores especificados al nuevo registro de tipo `Animal`.
 */

/**
 * @brief Busca el índice de un animal por su nombre.
 *
 * @param nombre Nombre del animal a buscar.
 * @return Índice del animal si existe, -1 si no se encuentra.
 */
int buscarIndiceAnimal(const char *nombre) {
    for (int i = 0; i < num_animales; i++) {
        if (strcmp(animales[i].nombre, nombre) == 0) {
            return i;
        }
    }
    return -1; // comprovació per veur si l'animal ja existeix
}

//----------------------------------------------
//Funcion agregar animal al final de la lista

int agregarAnimal(Especie especie, const char *nombre, Habitat habitat, const char *descripcion, char sexo, int edad, const char *pais) {
    if (num_animales >= MAX_ANIMALES) {
        printf("No se pueden añadir más animales.");
        return 1;
    }
    animales[num_animales].especie = especie;
    strncpy(animales[num_animales].nombre, nombre, sizeof(animales[num_animales].nombre) - 1);
    animales[num_animales].habitat = habitat;
    strncpy(animales[num_animales].descripcion, descripcion, sizeof(animales[num_animales].descripcion) - 1);
    animales[num_animales].sexo = sexo;
    animales[num_animales].edad = edad;
    strncpy(animales[num_animales].pais, pais, sizeof(animales[num_animales].pais) - 1);
    num_animales++;
    guardar_animales();
    return 0;
}

/**
 * @brief Borra un animal de la lista y reorganiza los datos.
 * 
 * @param nombre Nombre del animal a borrar.
 * @return int Retorna 1 si el animal fue borrado, 0 si no se encontró.
 */
int borrarAnimal(const char* nombre) {
    int i, j;
    int encontrado = 0;

    for (i = 0; i < num_animales; i++) {
        if (strcmp(animales[i].nombre, nombre) == 0) {
            encontrado = 1;
            // Mover todos los animales siguientes una posición hacia arriba
            for (j = i; j < num_animales - 1; j++) {
                animales[j] = animales[j + 1];
            }
            num_animales--;
            break;
        }
    }

    if (encontrado) {
        // Guardar los cambios en el archivo
        guardar_animales();
        return 1;
    }

    return 0;
}



/**
 * @brief Devuelve el nombre en texto de una especie.
 * 
 * Esta función toma un valor de tipo `Especie` y devuelve una cadena de caracteres
 * que representa el nombre correspondiente de la especie en texto.
 * 
 * @param especie Valor de tipo `Especie` que representa la especie del animal.
 * buscarIndiceAnimal
 * @return Un puntero a una cadena de caracteres con el nombre de la especie:
 *  - "Tigre" para `TIGRE`
 *  - "Elefante" para `ELEFANTE`
 *  - "Leon" para `LEON`
 *  - "Oso" para `OSO`
 *  - "Desconocido" si el valor de `especie` no coincide con ninguno de los casos especificados.
 */

char *nombreEspecie(Especie especie) {
    switch (especie) {
        case TIGRE: return "Tigre";
        case ELEFANTE: return "Elefante";
        case LEON: return "Leon";
        case OSO: return "Oso";
        default: return "Desconocido";
    }
}


/**
 * @brief Consulta las descripciones de animales de una especie específica.
 * 
 * Esta función busca en el registro de animales aquellos que coinciden con la especie
 * proporcionada y concatena sus descripciones en una cadena de caracteres.
 * Si no se encuentran animales de la especie indicada, se devuelve un mensaje informando
 * que no se han encontrado resultados.
 * 
 * @param especie Valor de tipo `Especie` que representa la especie a consultar.
 * 
 * @return Un puntero a una cadena de caracteres con las descripciones de los animales encontrados
 *         o un mensaje indicando que no se han encontrado resultados.
 * 
 * Detalles:
 *  - Se utiliza `malloc` para asignar memoria dinámica para la cadena de resultados.
 *  - La cadena resultante se debe liberar después de su uso para evitar fugas de memoria.
 */

char *consultarDescripcionEspecie(Especie especie) {
    char *resultado = malloc(1024);
    int encontrados = 0;
    resultado[0] = '\0';

    for (int i = 0; i < num_animales; i++) {
        if (animales[i].especie == especie) {
            
            strcat(resultado, animales[i].descripcion);
            strcat(resultado, "\n");
            encontrados++;
        }
    }
    if (encontrados == 0) {
        strcpy(resultado, "No se han encontrado resultados.\n");
    }
    return resultado;
}


/**
 * @brief Muestra la lista de animales de una especie específica.
 * 
 * Esta función recorre el registro de animales y construye una cadena de caracteres que
 * contiene información sobre los animales que pertenecen a la especie indicada. Para cada 
 * animal encontrado, se incluye su número, nombre, edad y sexo en la cadena resultante.
 * Si no se encuentran animales de la especie indicada, se devuelve un mensaje informando
 * que no se han encontrado resultados.
 * 
 * @param especie Valor de tipo `Especie` que representa la especie a consultar.
 * 
 * @return Un puntero a una cadena de caracteres con la información de los animales encontrados
 *         o un mensaje indicando que no se han encontrado resultados.
 * 
 * Detalles:
 *  - Se utiliza `malloc` para asignar memoria dinámica para la cadena de resultados.
 *  - La cadena resultante debe liberarse después de su uso para evitar fugas de memoria.
 */

char *mostrarAnimalesEspecie(Especie especie) {
    char *resultado = malloc(1024);
    int encontrados = 0;
    resultado[0] = '\0';

    for (int i = 0; i < num_animales; i++) {
        if (animales[i].especie == especie) {
            char buffer[100];
            sprintf(buffer, "[%d] Nombre: %s, Edad: %d, Sexo: %c\n", (encontrados+1), animales[i].nombre, animales[i].edad, animales[i].sexo);
            strcat(resultado, buffer);
            encontrados++;
        }
    }
    if (encontrados == 0) {
        strcpy(resultado, "No se han encontrado resultados.\n");
    }
    return resultado;
}


/**
 * @brief Muestra la lista de animales que tienen una edad específica.
 * 
 * Esta función recorre el registro de animalebuscarIndiceAnimals y construye una cadena de caracteres que
 * contiene información sobre los animales con la edad indicada. Para cada animal encontrado,
 * se incluye su número, nombre y especie en la cadena resultante. Si no se encuentran animales
 * con la edad indicada, se devuelve un mensaje informando que no se han encontrado resultados.
 * 
 * @param edad Edad específica a consultar para mostrar animales.
 * 
 * @return Un puntero a una cadena de caracteres con la información de los animales encontrados
 *         o un mensaje indicando que no se han encontrado resultados.
 * 
 * Detalles:
 *  - Se utiliza `malloc` para asignar memoria dinámica para la cadena de resultados.
 *  - La cadena resultante debe liberarse después de su uso para evitar fugas de memoria.
 */

char *mostrarAnimalesEdad(int edad) {
    char *resultado = malloc(1024);
    int encontrados = 0;
    resultado[0] = '\0';

    for (int i = 0; i < num_animales; i++) {
        if (animales[i].edad == edad) {
            char buffer[100];
            sprintf(buffer, "[%d] Nombre: %s, Especie: %s\n", (encontrados+1), animales[i].nombre, nombreEspecie(animales[i].especie));
            strcat(resultado, buffer);
            encontrados++;
        }
    }
    if (encontrados == 0) {
        strcpy(resultado, "No se han encontrado animales de esta edad.\n");
    }
    return resultado;
}


/**
 * @brief Muestra la lista de animales que habitan en un hábitat específico.
 * 
 * Esta función recorre el registro de animales y construye una cadena de caracteres que
 * contiene información sobre los animales que pertenecen al hábitat especificado. Para cada
 * animal encontrado, se incluye su número, nombre y especie en la cadena resultante.
 * Si no se encuentran animales en el hábitat indicado, se devuelve un mensaje informando
 * que no se han encontrado resultados.
 * 
 * @param habitat Tipo de hábitat a consultar (de tipo `Habitat`).
 * 
 * @return Un puntero a una cadena de caracteres con la información de los animales encontrados
 *         o un mensaje indicando que no se han encontrado resultados.
 * 
 * Detalles:
 *  - Se utiliza `malloc` para asignar memoria dinámica para la cadena de resultados.
 *  - La cadena resultante debe liberarse después de su uso para evitar fugas de memoria.
 */

char *mostrarAnimalesHabitat(Habitat habitat) {
    char *resultado = malloc(1024);
    int encontrados = 0;
    resultado[0] = '\0';

    for (int i = 0; i < num_animales; i++) {
        if (animales[i].habitat == habitat) {
            char buffer[100];
            sprintf(buffer, "[%d] Nombre: %s, Especie: %s\n", (encontrados+1), animales[i].nombre, nombreEspecie(animales[i].especie));
            strcat(resultado, buffer);
            encontrados++;
        }
    }
    if (encontrados == 0) {
        strcpy(resultado, "No se han encontrado animales que vivan en este habitat.\n");
    }
    return resultado;
}


/**
 * @brief Muestra la lista de animales de un sexo específico.
 * 
 * Esta función recorre el registro de animales y construye una cadena de caracteres que
 * contiene información sobre los animales cuyo sexo coincide con el especificado. Para cada
 * animal encontrado, se incluye su número, nombre, especie y edad en la cadena resultante.
 * Si no se encuentran animales con el sexo indicado, se devuelve un mensaje informando
 * que no se han encontrado resultados.
 * 
 * @param sexo Sexo específico a consultar ('M' para macho o 'F' para hembra).
 * 
 * @return Un puntero a una cadena de caracteres con la información de los animales encontrados
 *         o un mensaje indicando que no se han encontrado resultados.
 * 
 * Detalles:
 *  - Se utiliza `malloc` para asignar memoria dinámica para la cadena de resultados.
 *  - La cadena resultante debe liberarse después de su uso para evitar fugas de memoria.
 */

char *mostrarAnimalesSexo(char sexo) {
    char *resultado = malloc(1024);
    int encontrados = 0;
    resultado[0] = '\0';

    for (int i = 0; i < num_animales; i++) {
        if (animales[i].sexo == sexo) {
            char buffer[100];
            sprintf(buffer, "[%d] Nombre: %s, Especie: %s, Edad: %d\n", (encontrados+1), animales[i].nombre, nombreEspecie(animales[i].especie), animales[i].edad);
            strcat(resultado, buffer);
            encontrados++;
        }
    }
    if (encontrados == 0) {
        strcpy(resultado, "No se han encontrado animales de este sexo.\n");
    }
    return resultado;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ANIMALES 100

// Estructura y enums previamente definidos...

/**
 * @brief Verifica si existe un animal con un nombre específico.
 * 
 * Esta función recorre el registro de animales y compara el nombre de cada uno
 * con el nombre proporcionado. Si se encuentra un animal con el nombre indicado,
 * la función devuelve 1 para indicar que el nombre ya existe. De lo contrario,
 * devuelve 0.
 * 
 * @param nombre Cadena de caracteres que representa el nombre a verificar.
 * 
 * @return Un entero:
 *         - 1 si el nombre ya existe en el registro de animales.
 *         - 0 si no se encuentra el nombre en el registro.
 */

int existeNombre(const char *nombre) {
    for (int i = 0; i < num_animales; i++) {
        if (strcmp(animales[i].nombre, nombre) == 0) {
            return 1; // Nombre ya existe
        }
    }
    return 0;
}


/**
 * @brief Edita el nombre de un animal en una posición específica.
 * 
 * Esta función cambia el nombre de un animal ubicado en el índice especificado por
 * `index`, verificando antes que el nuevo nombre no esté en uso por otro animal.
 * Si el nombre ya existe, muestra un mensaje de error y no realiza el cambio.
 * 
 * @param index Índice en el arreglo `animales` que identifica al animal cuyo nombre se desea cambiar.
 * @param nuevoNombre Cadena de caracteres que representa el nuevo nombre a asignar.
 * 
 * @return Un entero:
 *         - 1 si el nombre fue actualizado exitosamente.
 *         - 0 si el nombre ya existe y no se realizó la modificación.
 */

int editarNombreAnimal(const char *nombre, const char *nuevoNombre) {
    int index = buscarIndiceAnimal(nombre);
    
    if (index == -1) {
        printf("Animal no encontrado.\n");
        return -1;
    }
    else {
    
    if (existeNombre(nuevoNombre)) {
        printf("Este nombre ya existe y no puede duplicarse.\n");
        return 0;
    }
    strcpy(animales[index].nombre, nuevoNombre);
    guardar_animales();
    return 1;
    }
}


/**
 * @brief Cambia el hábitat de un animal en una posición específica.
 * 
 * Esta función actualiza el hábitat de un animal en el índice especificado por `index`.
 * Antes de realizar el cambio, verifica si el animal es de la especie `OSO`, en cuyo
 * caso solo se permite cambiar su hábitat a `BOSQUE`. Si la condición no se cumple,
 * muestra un mensaje de error y no realiza el cambio.
 * 
 * @param index Índice en el arreglo `animales` que identifica al animal cuyo hábitat se desea cambiar.
 * @param nuevoHabitat Nuevo valor de tipo `Habitat` que representa el hábitat a asignar al animal.
 * 
 * @return Un entero:
 *         - 1 si el hábitat fue actualizado exitosamente.
 *         - 0 si el cambio de hábitat no se realizó debido a restricciones.
 */

int cambiarHabitatAnimal(const char *nombre, Habitat nuevoHabitat) {
    int index = buscarIndiceAnimal(nombre);
    if (index == -1) {
        printf("Animal no encontrado.\n");
        return -1;
    }
    else {
    if (animales[index].especie == OSO && nuevoHabitat != BOSQUE) {
        printf("Se permite que los osos habiten únicamente en el bosque.\n");
        return 0;
    }
    animales[index].habitat = nuevoHabitat;
    guardar_animales();
    return 1;
    }
}


/**
 * @brief Incrementa en un año la edad de todos los animales, con un límite de edad máxima.
 * 
 * Esta función recorre el registro de animales y aumenta en uno la edad de cada animal, 
 * siempre que su edad sea menor a 100. Este límite evita que la edad de los animales
 * sobrepase un valor razonable.
 */

int sumarAnyTots() {
    int r = 1;
    for (int i = 0; i < num_animales; i++) {
        if (animales[i].edad < 100) {
            animales[i].edad++;
        }
        else {r= 0; }
    }
    guardar_animales();
    return r;
}

int restarAnyTots() {
    int r = 0;
    for (int i = 0; i < num_animales; i++) {
        if (animales[i].edad > 0) {
            animales[i].edad--;
            r= 1;
        }
    }
    guardar_animales();
    return r;
}



/**
 * @brief Incrementa en un año la edad de un animal específico, con un límite de edad máxima.
 * 
 * Esta función aumenta en uno la edad del animal ubicado en el índice especificado por `index`,
 * siempre que su edad actual sea menor a 100. Si el animal ya tiene 100 años, se muestra
 * un mensaje indicando que ha alcanzado el límite de edad y no se realiza el incremento.
 * 
 * @param index Índice en el arreglo `animales` que identifica al animal cuya edad se desea incrementar.
 */

int sumarAnyAnimal(const char *nombre) {
    int index = buscarIndiceAnimal(nombre);
    if (index == -1) {
        printf("Animal no encontrado.\n");
        return -1;
    }
    else{
    if (animales[index].edad < 100) {
        animales[index].edad++;
        guardar_animales();
        return 0;
        } else {
        printf("Este animal ha alcanzado el límite de edad de 100 años.\n");
        return 1;
    }
    }
}

int restarAnyAnimal(const char *nombre) {
    int index = buscarIndiceAnimal(nombre);
    if (index == -1) {
        printf("Animal no encontrado.\n");
        return -1;
    }
    else{
    if (animales[index].edad > 0) {
        animales[index].edad--;
        guardar_animales();
        return 0;
    } else {
        printf("Este animal tiene la edad mínima de 0 años.\n");
        return 1;
    }
    }
}

/**
 * @brief Actualiza la descripción de un animal específico.
 * 
 * Esta función reemplaza la descripción actual de un animal en la posición
 * indicada por `index` con una nueva descripción proporcionada.
 * 
 * @param index Índice en el arreglo `animales` que identifica al animal cuya descripción se desea actualizar.
 * @param nuevaDescripcion Cadena de caracteres que representa la nueva descripción a asignar al animal.
 */

int cambiarDescripcionAnimal(const char *nombre, const char *nuevaDescripcion) {
    int index = buscarIndiceAnimal(nombre);
    if (index == -1) {
        printf("Animal no encontrado.\n");
        return 1;
    }
    else{
    strcpy(animales[index].descripcion, nuevaDescripcion);
    printf("Edición completada con éxito.\n");
    guardar_animales();
    return 0;
    }

}

/**
 * @brief Muestra un mensaje para solicitar la selección de una especie.
 * 
 * Esta función imprime un texto en la consola que indica al usuario que debe
 * ingresar el número correspondiente a la especie deseada. Las opciones disponibles
 * son: Tigre (0), Elefante (1), León (2) y Oso (3).
 */
 
void mostrarTextoSeleccionEspecie() {
    printf("Seleccione la especie deseada ingresando el número correspondiente (0: Tigre, 1: Elefante, 2: León, 3: Oso):\n");
}


/**
 * @brief Verifica las credenciales de un usuario.
 * 
 * Esta función comprueba si un nombre de usuario y una contraseña coinciden con
 * alguna de las credenciales almacenadas en los arreglos `usuaris` y `contra`.
 * Si se encuentra una coincidencia, devuelve `true`; de lo contrario, devuelve `false`.
 * 
 * @param usuari Cadena de caracteres que representa el nombre de usuario a verificar.
 * @param contrasena Entero que representa la contraseña a verificar.
 * 
 * @return `true` si las credenciales son válidas; `false` en caso contrario.
 */

bool verificarCredenciales(char *usuari, int contrasena) {
    for (int i = 0; i < 3; i++) {
        if (strcmp(usuaris[i], usuari) == 0 && contra[i] == contrasena) {
            return true;
        }
    }
    return false;
}

int main() {
    cargar_animales();
    int s;
    struct sockaddr_in serv_adr, client_adr;
    socklen_t client_adr_len = sizeof(client_adr);
    char buffer[MIDA_BUFFER];
    char ip_client[16];
    int port_client;
    socklen_t mida;
    int n;
    bool ha_accedit = false;
    int accionRecibida = 0;
    int opcion, edad, especie, habitat;
    char sexo;

     // Configuración del socket
    s = socket(AF_INET, SOCK_DGRAM, 0);
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = INADDR_ANY;
    serv_adr.sin_port = htons(44444);

    // Enlazar el socket
    n = bind(s, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
    if (n < 0) {
        printf("No se ha podido establecer conexión con el socket.\n");
        return 1;
    }

    printf("¡Servidor operativo!\n");
    accionRecibida = 0;
        while (1) {
            // Espera recibir la acción solicitada
            if (accionRecibida == 0) {
                recvfrom(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, &client_adr_len);
                sscanf(buffer, "%d", &accionRecibida);
            } else {
                int especie, edad, habitat;
                char sexo;

                switch (accionRecibida) {
                    case -1:
                    char usuari[10];
                    int contrasena;
                    recvfrom(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, &(socklen_t){sizeof(client_adr)});
                    sscanf(buffer, "%s", usuari);
                    printf("Introduce el nombre de usuario: %s\n", usuari);
                    recvfrom(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, &(socklen_t){sizeof(client_adr)});
                    sscanf(buffer, "%d", &contrasena);
                    printf("Introduce la contraseña: %d\n", contrasena);
                    if (verificarCredenciales(usuari, contrasena)) {
                        printf("Acceso concedido.\n");
                        sprintf(buffer, "Acceso concedido.");
                        ha_accedit = true;
                    } else {
                        printf("Usuario o contraseña incorrectos.\n");
                        sprintf(buffer, "Usuario o contraseña incorrectos.");
                    }
                        accionRecibida = 0;
                        sendto(s, buffer, MIDA_BUFFER + 1, 0, (struct sockaddr*)&client_adr, sizeof(client_adr));
                        break;
                    case 1:
                        sprintf(buffer, "(0: Tigre, 1: Elefante, 2: Leon, 3: Oso)");
                        sendto(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, client_adr_len);

                        // Recibir el número de especie
                        recvfrom(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, &client_adr_len);
                        sscanf(buffer, "%d", &especie);

                        char *descripcion = consultarDescripcionEspecie(especie);
                        if (descripcion) {
                            snprintf(buffer, MIDA_BUFFER, "%s", descripcion);
                            free(descripcion);
                        } else {
                            snprintf(buffer, MIDA_BUFFER, "No se han encontrado resultdos.");
                        }

                        sendto(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, client_adr_len);
                        accionRecibida = 0;
                        break;

                    case 2:
                        sprintf(buffer, "(0: Tigre, 1: Elefante, 2: Leon, 3: Oso)");
                        sendto(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, client_adr_len);

                        // Recibir el número de especie
                        recvfrom(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, &client_adr_len);
                        sscanf(buffer, "%d", &especie);

                        char *nombresEspecie = mostrarAnimalesEspecie(especie);
                        if (nombresEspecie) {
                            snprintf(buffer, MIDA_BUFFER, "%s", nombresEspecie);
                            free(nombresEspecie);
                        } else {
                            snprintf(buffer, MIDA_BUFFER, "No se han encontrado resultdos.");
                        }

                        sendto(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, client_adr_len);
                        accionRecibida = 0;
                        break;

                    case 3:
                        // Recibir la edad
                        recvfrom(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, &client_adr_len);
                        sscanf(buffer, "%d", &edad);

                        char *animalesEdad = mostrarAnimalesEdad(edad);
                        if (animalesEdad) {
                            snprintf(buffer, MIDA_BUFFER, "%s", animalesEdad);
                            free(animalesEdad);
                        } else {
                            snprintf(buffer, MIDA_BUFFER, "No se han encontrado resultdos.");
                        }

                        sendto(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, client_adr_len);
                        accionRecibida = 0;
                        break;

                    case 4:
                        sprintf(buffer, "(0: Selva, 1: Sabana, 2: Bosque)");
                        sendto(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, client_adr_len);

                        // Recibir el número de hábitat
                        recvfrom(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, &client_adr_len);
                        sscanf(buffer, "%d", &habitat);

                        char *animalesHabitat = mostrarAnimalesHabitat(habitat);
                        if (animalesHabitat) {
                            snprintf(buffer, MIDA_BUFFER, "%s", animalesHabitat);
                            free(animalesHabitat);
                        } else {
                            snprintf(buffer, MIDA_BUFFER, "No se han encontrado resultdos.");
                        }

                        sendto(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, client_adr_len);
                        accionRecibida = 0;
                        break;

                    case 5:
                        // Recibir el sexo
                        recvfrom(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, &client_adr_len);
                        sscanf(buffer, " %c", &sexo);

                        char *animalesSexo = mostrarAnimalesSexo(sexo);
                        if (animalesSexo) {
                            snprintf(buffer, MIDA_BUFFER, "%s", animalesSexo);
                            free(animalesSexo);
                        } else {
                            snprintf(buffer, MIDA_BUFFER, "No se han encontrado resultdos.");
                        }

                        sendto(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, client_adr_len);
                        accionRecibida = 0;
                        break;

                    case 6:
                        // Recibir el nombre
                        recvfrom(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, &client_adr_len);
                        char nomoriginal[50];
                        sscanf(buffer, "%s", &nomoriginal);

                        // Recibir el nombre
                        recvfrom(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, &client_adr_len);
                        char nomnou[50];
                        sscanf(buffer, "%s", &nomnou);

                        int control = editarNombreAnimal(nomoriginal, nomnou);

                        if (control == -1) {
                            snprintf(buffer, MIDA_BUFFER, "El animal no existe.");
                        } else if (control == 0){
                            snprintf(buffer, MIDA_BUFFER, "El nombre del animal ya existe.");
                        } else {
                            snprintf(buffer, MIDA_BUFFER, "Edición completada con éxito.");
                        }

                        sendto(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, client_adr_len);
                        accionRecibida = 0;
                        break;

                    case 7:
                        // Recibir el nombre
                        recvfrom(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, &client_adr_len);
                        char nom[50];
                        sscanf(buffer, "%s", &nom);

                        sprintf(buffer, "(0: Tigre, 1: Elefante, 2: Leon, 3: Oso)");
                        sendto(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, client_adr_len);

                        // Recibir el número de hábitat
                        recvfrom(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, &client_adr_len);
                        sscanf(buffer, "%d", &habitat);

                        int control2 = cambiarHabitatAnimal(nom, habitat);

                        if (control2 == -1) {
                            snprintf(buffer, MIDA_BUFFER, "El animal no existe");
                        } else if (control2 == 0){
                            snprintf(buffer, MIDA_BUFFER, "El hábitat no es adecuado para este animal.");
                        } else {
                            snprintf(buffer, MIDA_BUFFER, "Edición completada con éxito.");
                        }

                        sendto(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, client_adr_len);
                        accionRecibida = 0;
                        break;

                    case 8:

                        if (sumarAnyTots() == 0) {
                            snprintf(buffer, MIDA_BUFFER, "La edad supera el límite establecido.");
                        } else  {
                            snprintf(buffer, MIDA_BUFFER, "Edición completada con éxito.");
                        }

                        sendto(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, client_adr_len);
                        accionRecibida = 0;
                    break;

                    case 9:

                        if (restarAnyTots() == 0) {
                            snprintf(buffer, MIDA_BUFFER, "La edad es inferior al límite.");
                        } else  {
                            snprintf(buffer, MIDA_BUFFER, "Edición completada con éxito.");
                        }

                        sendto(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, client_adr_len);
                        accionRecibida = 0;
                    break;

                    case 10:
                        recvfrom(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, &client_adr_len);
                        char nom2[50];
                        sscanf(buffer, "%s", &nom2);
                        if (sumarAnyAnimal(nom2) == 0) {
                            snprintf(buffer, MIDA_BUFFER, "Edición completada con éxito.");
                        } else  {
                            snprintf(buffer, MIDA_BUFFER, "La edad está fuera del rango o el animal no existe.");
                        }

                        sendto(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, client_adr_len);
                        accionRecibida = 0;
                    break;

                    case 11:
                        recvfrom(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, &client_adr_len);
                        char nom3[50];
                        sscanf(buffer, "%s", &nom3);
                        if (restarAnyAnimal(nom3) == 0) {
                            snprintf(buffer, MIDA_BUFFER, "Edición completada con éxito.");
                        } else  {
                            snprintf(buffer, MIDA_BUFFER, "Edat fora de rang o animal no existeix");
                        }

                        sendto(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, client_adr_len);
                        accionRecibida = 0;
                    break;

                    case 12:
                        // Recibir el nombre
                        recvfrom(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, &client_adr_len);
                        char nom4[50];
                        sscanf(buffer, "%s", &nom4);

                        // Recibir nueva descripción
                        recvfrom(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, &client_adr_len);
                        char descripcionova[99];
                        sscanf(buffer, "%s", &descripcionova);

                        if (cambiarDescripcionAnimal(nom4, descripcionova) == 0){
                            snprintf(buffer, MIDA_BUFFER, "Edición completada con éxito.");
                        } else {
                            snprintf(buffer, MIDA_BUFFER, "El animal no existe.");
                        }

                        sendto(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, client_adr_len);
                        accionRecibida = 0;
                        break;

                    case 13: // Borrar animal
                        recvfrom(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, &client_adr_len);
                        char nombreAnimalBorrar[50];
                        sscanf(buffer, "%s", nombreAnimalBorrar);
    
                        if (borrarAnimal(nombreAnimalBorrar)) {
                            sprintf(buffer, "El animal '%s' ha sido eliminado con éxito.", nombreAnimalBorrar);
                        } else {
                            sprintf(buffer, "No se encontró el animal '%s'.", nombreAnimalBorrar);
                        }
                        sendto(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, client_adr_len);
                        accionRecibida = 0;
                        break;

                    case 14:

                        // Recibir el nombre
                        char nomnouanimal[50];
                        char novadescripcioanimal[99];
                        char Sexo;
                        int Edad;
                        char pais2[50];
                        
                        recvfrom(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, &client_adr_len);
                        sscanf(buffer, "%d", &especie);

                        recvfrom(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, &client_adr_len);
                        sscanf(buffer, "%s", &nomnouanimal);

                        recvfrom(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, &client_adr_len);
                        sscanf(buffer, "%d", &habitat);

                        recvfrom(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, &client_adr_len);
                        sscanf(buffer, "%s", &novadescripcioanimal);

                        recvfrom(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, &client_adr_len);
                        sscanf(buffer, "%c", &sexo);

                        recvfrom(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, &client_adr_len);
                        sscanf(buffer, "%d", &edad);

                        recvfrom(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, &client_adr_len);
                        sscanf(buffer, "%s", &pais2);

                        if (agregarAnimal(especie, nomnouanimal, habitat, novadescripcioanimal, sexo, edad, pais2) == 0){
                            snprintf(buffer, MIDA_BUFFER, "El animal ha sido añadido con éxito.");
                        } else {
                            snprintf(buffer, MIDA_BUFFER, "No se pueden añadir más animales.");
                        }

                        sendto(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, client_adr_len);
                        accionRecibida = 0;
                        break;

                    default:
                        snprintf(buffer, MIDA_BUFFER, "Opción no válida.");
                        sendto(s, buffer, MIDA_BUFFER, 0, (struct sockaddr*)&client_adr, client_adr_len);
                        accionRecibida = 0;
                        break;
                }
            }
        }
    

    close(s);
    return 0;
}