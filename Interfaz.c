/*
En el caso de la interfaz, se deben considerar los siguientes aspectos:
1.- El usuario debe iniciar sesion y registrarse al final si viene en el pdf xdd
2.- Posteriormente entrar en un menu switch case en donde se vean las distintas opciones a explorar, desde ver amigos hasta publicar en el muro, por lo que debe llevar varios cases dependiendo de lo que se quiera hacer.
3.- La segunda parte del diagrama es bastante explícita en como debería funcionar el muro como tal, intenten seguir la estructura de lo que se ve en el diagrama y lo que se pide como tal, pero dejen en blanco o vacíos espacios como "fechas" o "hora" de posteo, porque aun no sabemos si es posible hacerlo sin una libreria en especifico, el orden puede ser cualquiera pero recuerden que cada interfaz es o la parte que RECIBE un dato o que MANDA un dato a la siguiente funcion del CODIGO, por lo que tienen que ser constantes en lo que deciden MANDAR o RECIBIR en el codigo de la interfaz para adaptarlo en el CODIGO.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 10
#define MAX_NAME_LENGTH 15
#define MAX_CLAVE_LENGTH 10


//NO SON NECESARIAS LAS FUNCIONES DE INTERFAZ, SOLO PARA EL CODIGO DE PRUEBA
//LAS FUNCIONES SE ESTAN CREANDO EN EL ARCHIVO MAIN

//Funcion principal
/*
int main(){
  //Arreglo para almacenar usuarios y variablas relacionadas
  Usuario usuarios[MAX_USERS];
  int numUsuarios = 0;
  int loggedIn = 0; //Esta variable indica si ha iniciado sesion un usuario

  //Bucle principal del programa
  while(1){
    //Menu principal
    printf("\n\t\t\tRED SOCIAL UV 2023\n");
    printf("\t\t==================================\n");
    printf("\t\t[1]. Registrar usuario nuevo\n");
    printf("\t\t[2]. Iniciar sesion\n");
    printf("\t\t[3]. Ver usuarios registrados\n");
    printf("\t\t[4]. Salir\n");
    printf("\t\t==================================\n");

    //Solicita al usuario seleccionar una opcion
    int opcion;
    printf("\n\t\tSelecciona una opcion: ");
    scanf("%d", &opcion);

    //Realiza acciones segun la opcion seleccionada
    switch(opcion){
      case 1:
        //Llama la funcion para registrar un usuario
        mainRegistrar(usuarios, &numUsuarios);
        break;
      case 2:
        //Llama la funcion para inniciar sesion y actualiza el estado de la variable loggedIn
        mainIngresar(usuarios, numUsuarios);
        loggedIn = 1;
        break;
      case 3:
        //Llama la funcion para ver los usuarios registrados
        mainVerUsuarios(usuarios, numUsuarios);
        break;
      case 4:
        printf("\t\t==================================\n");
        printf("\t\t\tSaliendo del programa...\n");
        printf("\t\t==================================\n");
        exit(0);
        break;
      default:
        printf("\t\t==================================\n");
        printf("\t\tOpcion invalida, intente de nuevo\n");
        printf("\t\t==================================\n");
    }

    //Si un usuario ha iniciado sesion, muestra un segundo menu
    if(loggedIn){
      while(1){
        //Menu para usuarios autenticados
        printf("\t\t==================================\n");
        printf("\n\t\tBienvenido a la red social de Luzio UV\n");
        printf("\t\t==================================\n");
        printf("\t\tElige una opcion, polluelo.\n");
        printf("\t\t[1]. Ver amigos\n");
        printf("\t\t[2]. Notificaciones\n");
        printf("\t\t[3]. Mandar solicitud\n");
        printf("\t\t[4]. Ver muro\n");
        printf("\t\t[5]. Publicar en el muro\n");
        printf("\t\t[6]. Cerrar Sesion\n");
        printf("\t\t==================================\n");

        //Solicita al usuario seleccionar una opcion
        printf("\t\tSelecciona una opcion: ");
        scanf("%d", &opcion);

        switch(opcion){ //Cada que el usuario seleccione una opcion, tendrá un msj de bienvenida para que sepa donde esta
          case 1:
          printf("\t\t==================================\n");
            printf("\t\tAmigos.\n");
          printf("\t\t==================================\n");
            break;
          case 2:
            printf("\t\t==================================\n");
            printf("\t\tVe tus nuevas notificaciones.\n");
            printf("\t\t==================================\n");
            break;
          case 3:
            printf("\t\t==================================\n");
            printf("\t\tEnvia solicitudes.\n");
            printf("\t\t==================================\n");
            break;
          case 4:
            printf("\t\t==================================\n");
            printf("\t\tIngresaste a tu perfil.\n");
            printf("\t\t==================================\n");
            break;
          case 5:
            printf("\t\t==================================\n");
            printf("\t\tNueva publicacion.\n");
            printf("\t\t==================================\n");
            break;
          case 6:
            //Cerrar sesion y regresar al menu principal
            loggedIn = 0;
            printf("\t\t==================================\n");
            printf("\t\tCerrando sesion...\n");
            printf("\t\t==================================\n");
            break;
          default:
            printf("\t\t\tOpcion invalida, intente de nuevo\n");
            printf("\t\t==================================\n");
        }
        //Si el usuario selecciona la opcion 6 (cerrar sesion), sale del bucle y regresa al menu principal
        if(!loggedIn){
          break;
        }
      }
    }
  }
}
*/
//ELIMINE TODAS LAS FUNCIONES YA QUE SOLO DEBIAN SER ILUSTRATIVAS YA QUE LA MANERA DE REGISTRAR Y VER LOS USUARIOS REGISTRADOS SE HACE MEDIANTE LAS ESTRUCTURAS DE DATOS ELEGIDAS