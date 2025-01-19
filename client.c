#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MIDA_PAQUET 1024

int socket_fd;
struct sockaddr_in contacte_servidor;

// Prototipos de funciones

/**
 * @brief Permite el acceso al sistema como usuario invitado.
 * 
 * Esta función configura el entorno y los permisos necesarios para
 * que un usuario pueda acceder como invitado, sin necesidad de registro.
 */
void accederComoInvitado();

/**
 * @brief Permite el acceso al sistema como usuario registrado.
 * 
 * Esta función configura el entorno y los permisos necesarios para
 * que un usuario acceda al sistema con sus credenciales de registro.
 */
void accederComoRegistrado();

/**
 * @brief Envía un paquete de datos al sistema o a otro dispositivo.
 * 
 * @param paquet Puntero a un arreglo de caracteres que contiene los datos del paquete a enviar.
 * 
 * Esta función toma un paquete en forma de cadena de caracteres y lo envía
 * al destinatario correspondiente, gestionando el proceso de transmisión.
 */
void enviarPaquet(char *paquet);

/**
 * @brief Recibe un paquete de datos del sistema o de otro dispositivo.
 * 
 * @param paquet Puntero a un arreglo de caracteres donde se almacenarán los datos recibidos.
 * 
 * Esta función recibe un paquete de datos y lo almacena en la ubicación
 * especificada por el puntero `paquet`, permitiendo el acceso a la información recibida.
 */
void recibirPaquet(char *paquet);

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("¡El nombre de parámetros no es el correcto!\n");
        return 1;
    }

    // Configuración del socket UDP

    // Crea el socket UDP (SOCK_DGRAM) usando IPv4 (AF_INET)
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0); 


    contacte_servidor.sin_family = AF_INET; // Establece familia de direcciones IPv4
    contacte_servidor.sin_addr.s_addr = inet_addr(argv[1]);  // Asigna dirección IP desde el argumento argv[1]
    contacte_servidor.sin_port = htons(atoi(argv[2])); // Asigna puerto convertido desde argv[2]

    int opcion;
    printf("Bienvenido al Zoo de Tarragona!\n");
    printf("Seleccione como quiere acceder:\n");
    printf("\t[1] Acceder como invitado.\n");
    printf("\t[2] Acceder como registrado.\n");
    printf("\n>>> ");
    scanf("%d", &opcion);

    do {
    switch (opcion) {
        case 1:
            accederComoInvitado();
            break;
        case 2:
            accederComoRegistrado();
            break;
        default:
            printf("Opción no válida.\n");
            break;
    }
    } while(1);

    // Cerrar el socket
    close(socket_fd);

    return 0;
}


/**
 * @brief Permite al usuario invitado interactuar con el sistema.
 * 
 * Esta función simula el acceso de un usuario como "Invitado". Al conectarse, envía y recibe paquetes 
 * de datos con el servidor para obtener información sobre diversas opciones de consulta relacionadas 
 * con especies y animales. Incluye un menú interactivo para que el usuario seleccione una acción:
 * 
 */

void accederComoInvitado() {
    char paquet[MIDA_PAQUET];

    int opcion, edad, especie, habitat;
    char sexo;

    do {
        printf("\n");
        printf("[1] Consultar todas las descripciones de una especie.\n");
        printf("[2] Mostrar nombres de animales de una especie con su edad y sexo.\n");
        printf("[3] Mostrar animales de una edad concreta.\n");
        printf("[4] Mostrar animales de un hábitat determinado.\n");
        printf("[5] Mostrar animales de un sexo concreto.\n");
        printf("[6] Accedar como registrado.\n");
        printf("[0] Salir.\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                sprintf(paquet, "%d", opcion);  // Corrección de %s a %d
                enviarPaquet(paquet);
                recibirPaquet(paquet);
                printf("Escriba el número de la especie que desea consultar: %s\n", paquet);
                scanf("%d", &especie);
                sprintf(paquet, "%d", especie);
                enviarPaquet(paquet);
                recibirPaquet(paquet);
                printf("Descripciones de los especímenes:\n%s", paquet);
                break;

            case 2:
                sprintf(paquet, "%d", opcion);  // Corrección de %s a %d
                enviarPaquet(paquet);
                recibirPaquet(paquet);
                printf("Escriba el número de la especie que quieres consultar: %s\n", paquet);
                scanf("%d", &especie);
                sprintf(paquet, "%d", especie);
                enviarPaquet(paquet);
                recibirPaquet(paquet);
                printf("Lista de animales:\n%s", paquet);
                break;

            case 3:
                sprintf(paquet, "%d", opcion);  // Corrección de %s a %d
                enviarPaquet(paquet);
                printf("Ingrese la edad: ");
                scanf("%d", &edad);
                sprintf(paquet, "%d", edad);
                enviarPaquet(paquet);
                recibirPaquet(paquet);
                printf("Lista de animales:\n%s", paquet);
                break;

            case 4:
                sprintf(paquet, "%d", opcion);  // Corrección de %s a %d
                enviarPaquet(paquet);
                recibirPaquet(paquet);
                printf("Escriba el número de habitat que desea consultar: %s\n", paquet);
                scanf("%d", &habitat);
                sprintf(paquet, "%d", habitat);
                enviarPaquet(paquet);
                recibirPaquet(paquet);
                printf("Lista de animales:\n%s", paquet);
                break;

            case 5:
                sprintf(paquet, "%d", opcion);  // Corrección de %s a %d
                enviarPaquet(paquet);
                printf("Ingrese el sexo (M/F): ");
                scanf(" %c", &sexo);  // Espacio antes del %c
                sprintf(paquet, "%c", sexo);
                enviarPaquet(paquet);
                recibirPaquet(paquet);
                printf("Lista de animales:\n%s", paquet);
                break;

            case 6:
                accederComoRegistrado();
                break;

            case 0:
                printf("Saliendo...\n");
                break;

            default:
                printf("Opción no válida.\n");
        }
    } while (opcion != 0);

}


/**
 * @brief Permite al usuario registrado interactuar con el sistema.
 * 
 * La función permite al usuario acceder al sistema con un nombre de usuario y una contraseña.
 * Después de la autenticación exitosa, el usuario tiene acceso a opciones avanzadas de consulta
 * y edición de datos sobre especies y animales.
 * 
 * Proceso:
 *  - Se envía una solicitud de autenticación ("Registrado").
 *  - Solicita nombre de usuario y contraseña en un bucle hasta que la respuesta del servidor
 *    indique "Acces completat".
 */

void accederComoRegistrado() {
    char usuario[10];
    char contrasena[10];
    char paquet[MIDA_PAQUET];
    
    do {
        sprintf(paquet, "-1");
        enviarPaquet(paquet);

        printf("Introduce tu nombre de usuario: ");
        scanf("%s", usuario);
        sprintf(paquet, "%s", usuario);
        enviarPaquet(paquet);

        printf("Introduce la contraseña: ");
        scanf("%s", contrasena);
        sprintf(paquet, "%s", contrasena);
        enviarPaquet(paquet);

        recibirPaquet(paquet);
        printf("Respuesta del servidor: %s\n", paquet);

    } while (strcmp(paquet, "Acceso concedido.") != 0);

    int opcion, edad, especie, habitat;
    char sexo;

    do {
        
        printf("\n");
        printf("[1] Consultar todas las descripciones de una especie.\n");
        printf("[2] Mostrar nombres de animales de una especie con su edad y sexo.\n");
        printf("[3] Mostrar animales de una edad concreta.\n");
        printf("[4] Mostrar animales de un hábitat determinado.\n");
        printf("[5] Mostrar animales de un sexo concreto.\n");
        printf("[6] Editar el nombre de un animal.\n");
        printf("[7] Cambiar el hábitat de un animal.\n");
        printf("[8] Sumar un año (a todos los animales).\n");
        printf("[9] Restar un año (a todos los animales).\n");
        printf("[10] Sumar un año (a un animal concreto).\n");
        printf("[11] Restar un año (a un animal concreto).\n");
        printf("[12] Cambiar la descripción.\n");
        printf("[13] Borrar animal (muerte o traslado).\n");
        printf("[14] Añadir animal (nacimiento o traslado).\n");
        printf("[15] Ver como invitado.\n");
        printf("[0] Salir.\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                sprintf(paquet, "%d", opcion);
                enviarPaquet(paquet);
                recibirPaquet(paquet);
                printf("Escriba el numero de especie que desea consultar: %s\n", paquet);
                scanf("%d", &especie);
                sprintf(paquet, "%d", especie);
                enviarPaquet(paquet);
                recibirPaquet(paquet);
                printf("Descripciones de los especímenes:\n%s", paquet);
                break;

            case 2:
                sprintf(paquet, "%d", opcion);
                enviarPaquet(paquet);
                recibirPaquet(paquet);
                printf("Escriba el numero de la especie que quieres consultar: %s\n", paquet);
                scanf("%d", &especie);
                sprintf(paquet, "%d", especie);
                enviarPaquet(paquet);
                recibirPaquet(paquet);
                printf("Lista de animales:\n%s", paquet);
                break;

            case 3:
                sprintf(paquet, "%d", opcion);
                enviarPaquet(paquet);
                printf("Ingrese la edad: ");
                scanf("%d", &edad);
                sprintf(paquet, "%d", edad);
                enviarPaquet(paquet);
                recibirPaquet(paquet);
                printf("Lista de animales:\n%s", paquet);
                break;

            case 4:
                sprintf(paquet, "%d", opcion);
                enviarPaquet(paquet);
                recibirPaquet(paquet);
                printf("Escriba el numero del habitat que desea consultar: %s\n", paquet);
                scanf("%d", &habitat);
                sprintf(paquet, "%d", habitat);
                enviarPaquet(paquet);
                recibirPaquet(paquet);
                printf("Lista de animales:\n%s", paquet);
                break;

            case 5:
                sprintf(paquet, "%d", opcion);
                enviarPaquet(paquet);
                printf("Ingrese el sexo (M/F): ");
                scanf(" %c", &sexo);
                sprintf(paquet, "%c", sexo);
                enviarPaquet(paquet);
                recibirPaquet(paquet);
                printf("Lista de animales:\n%s", paquet);
                break;

            case 6:
                sprintf(paquet, "%d", opcion);
                enviarPaquet(paquet);
                char nomoriginal[50];
                char nomnou[50];

                printf("Ingrese el nombre original del animal: ");
                scanf("%49s", nomoriginal);
                sprintf(paquet, "%s", nomoriginal);
                enviarPaquet(paquet);

                printf("Ingrese el nuevo nombre: ");
                scanf("%49s", nomnou);
                sprintf(paquet, "%s", nomnou);
                enviarPaquet(paquet);

                recibirPaquet(paquet);
                printf("Resultado del servidor:\n%s", paquet);
                break;

            case 7:
                sprintf(paquet, "%d", opcion);
                enviarPaquet(paquet);
                char nom[50];

                printf("Ingrese el nombre original del animal: ");
                scanf("%49s", nom);
                sprintf(paquet, "%s", nom);
                enviarPaquet(paquet);
                recibirPaquet(paquet);

                printf("Escribe el numero de habitat que desea consultar: %s\n", paquet);
                scanf("%d", &habitat);
                sprintf(paquet, "%d", habitat);
                enviarPaquet(paquet);
                
                recibirPaquet(paquet);
                printf("Lista de animales:\n%s", paquet);
                break;

            case 8:
                sprintf(paquet, "%d", opcion);
                enviarPaquet(paquet);
                recibirPaquet(paquet);
                printf("Resultado del servidor:\n%s", paquet);
                break;

            case 9:
                sprintf(paquet, "%d", opcion);
                enviarPaquet(paquet);
                recibirPaquet(paquet);
                printf("Resultado del servidor:\n%s", paquet);
                break;

            case 10:
                sprintf(paquet, "%d", opcion);
                enviarPaquet(paquet);
                printf("Ingrese el nombre del animal al que desea sumar un año: ");
                scanf("%49s", nom);
                sprintf(paquet, "%s", nom);
                enviarPaquet(paquet);
                recibirPaquet(paquet);
                printf("Resultado del servidor:\n%s", paquet);
                break;

            case 11:
                sprintf(paquet, "%d", opcion);
                enviarPaquet(paquet);
                printf("Ingrese el nombre del animal al que desea restar: ");
                scanf("%49s", nom);
                sprintf(paquet, "%s", nom);
                enviarPaquet(paquet);
                recibirPaquet(paquet);
                printf("Resultado del servidor:\n%s", paquet);
                break;

            case 12:
                sprintf(paquet, "%d", opcion);
                enviarPaquet(paquet);
                char nomanimal[50];
                char novadescripcio[99];

                printf("Ingrese el nombre del animal al que desea cambiarle la descripción: ");
                scanf("%49s", nomanimal);
                sprintf(paquet, "%s", nomanimal);
                enviarPaquet(paquet);

                printf("Ingrese la nueva descripción (sin espacios): ");
                scanf("%99s", novadescripcio);
                sprintf(paquet, "%s", novadescripcio);
                enviarPaquet(paquet);

                recibirPaquet(paquet);
                printf("Resultado del servidor:\n%s", paquet);
                break;

            case 13:
                sprintf(paquet, "%d", opcion);
                enviarPaquet(paquet);
                char nomanimal2[50];

                printf("Ingrese el nombre del animal fallecido o trasladado: ");
                scanf("%49s", nomanimal2);
                sprintf(paquet, "%s", nomanimal2);
                enviarPaquet(paquet);

                recibirPaquet(paquet);
                printf("Resultado del servidor:\n%s", paquet);
                break;

            case 14:
                sprintf(paquet, "%d", opcion);
                enviarPaquet(paquet);
                int Especie;
                char nomnouanimal[50];
                int Habitat;
                char novadescripcioanimal[99];
                char Sexo;
                int Edad;
                char Pais[50];

                printf("Ingrese el número de especie del nuevo animal: ");
                scanf("%d", &Especie);
                sprintf(paquet, "%d", Especie);
                enviarPaquet(paquet);

                printf("\nIngrese el nombre del nuevo animal (sin espacios): ");
                scanf("%49s", nomnouanimal);
                sprintf(paquet, "%s", nomnouanimal);
                enviarPaquet(paquet);

                printf("\nIngrese el número de habitat del nuevo animal: ");
                scanf("%d", &Habitat);
                sprintf(paquet, "%d", Habitat);
                enviarPaquet(paquet);

                printf("\nIngrese la descripción del nuevo animal (sin espacios): ");
                scanf("%99s", novadescripcioanimal);
                sprintf(paquet, "%s", novadescripcioanimal);
                enviarPaquet(paquet);

                printf("\nIngrese el sexo (M/F) del nuevo animal: ");
                scanf(" %c", &Sexo);
                sprintf(paquet, "%c", Sexo);
                enviarPaquet(paquet);

                printf("\nIngrese la edad del nuevo animal: ");
                scanf("%d", &Edad);
                sprintf(paquet, "%d", Edad);
                enviarPaquet(paquet);

                printf("\nIngrese el pais del nuevo animal (sin espacios): ");
                scanf("%49s", Pais);
                sprintf(paquet, "%s", Pais);
                enviarPaquet(paquet);

                recibirPaquet(paquet);
                printf("Resultado del servidor:\n%s", paquet);
                break;

            case 15:
            accederComoInvitado();
            break;

            case 0:
                printf("Saliendo...\n");
                break;

            default:
                printf("Opción no válida\n");
        }
    } while (opcion != 0);
}



/**
 * @brief Envía un paquete de datos al servidor.
 * 
 * Esta función envía un paquete de datos al servidor usando el socket UDP previamente configurado.
 * Utiliza `sendto` para enviar el contenido del paquete especificado a la dirección y puerto
 * definidos en `contacte_servidor`.
 * 
 * @param paquet Puntero a un arreglo de caracteres que contiene los datos a enviar.
 * 
 * Detalles:
 *  - `socket_fd` es el descriptor del socket usado para la transmisión.
 *  - `MIDA_PAQUET` define el tamaño del paquete enviado.
 *  - Al finalizar el envío, la función imprime un mensaje indicando que el paquete fue enviado.
 */

void enviarPaquet(char *paquet) {
    sendto(socket_fd, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&contacte_servidor, sizeof(contacte_servidor));
    printf("Paquete enviado!\n");
}



/**
 * @brief Recibe un paquete de datos desde el servidor.
 * 
 * Esta función utiliza `recvfrom` para recibir un paquete de datos desde el servidor a través del 
 * socket UDP. Los datos recibidos se almacenan en el buffer apuntado por `paquet`.
 * 
 * @param paquet Puntero a un arreglo de caracteres donde se almacenarán los datos recibidos.
 * 
 * Detalles:
 *  - `socket_fd` es el descriptor del socket configurado para recibir la transmisión.
 *  - `MIDA_PAQUET` define el tamaño máximo de datos a recibir.
 */

void recibirPaquet(char *paquet) {
    recvfrom(socket_fd, paquet, MIDA_PAQUET, 0, NULL, NULL);
}
