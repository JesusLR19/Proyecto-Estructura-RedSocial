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

//Estructura para representar un usuario
typedef struct{
  char nombreUser[MAX_NAME_LENGTH];
  char clave[MAX_CLAVE_LENGTH];
}Usuario;

//NO SON NECESARIAS LAS FUNCIONES DE INTERFAZ, SOLO PARA EL CODIGO DE PRUEBA
//LAS FUNCIONES SE ESTAN CREANDO EN EL ARCHIVO MAIN
void mainRegistrar(Usuario* usuarios, int* numUsuarios);
void mainIngresar(Usuario* usuarios, int numUsuarios);
void mainVerUsuarios(Usuario* usuarios, int numUsuarios);
void menu();
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
//Funcion para registrar a un nuevo usuario
void mainRegistrar(Usuario* usuarios, int* numUsuarios){
  //Verifica si se alcanzo el limite de usuarios maximos
  if(*numUsuarios >= MAX_USERS){
    printf("\t\t\tNo se pueden registrar mas usuarios");
    printf("\t\t==================================\n");
    return;
  }

  //Solicita al usuario ingresar los datos de nuevo usuario
  printf("\n\t\t\tREGISTRAR USUARIO\n");
  printf("\t\t==================================\n");
  printf("\t\tNombre de usuario: ");
  scanf("%s", usuarios[*numUsuarios].nombreUser);
  printf("\t\tClave: ");
  scanf("%s", usuarios[*numUsuarios].clave);
  printf("\t\t==================================\n");

  //Incrementa el contador de usuarios y muestra un mensaje de exito
  (*numUsuarios)++;
  printf("\t\tUsuario registrado exitosamente\n");
  printf("\t\t==================================\n");
}

//Funcion para iniciar sesion
void mainIngresar(Usuario* usuarios, int numUsuarios){
  char nombreUser[MAX_NAME_LENGTH];
  char clave[MAX_CLAVE_LENGTH];

  //Solicita al usuario ingresar nombre de usuario y clave para el inicio de sesion
  printf("\n\t\t\tINGRESAR AL SISTEMA\n");
  printf("\t\t==================================\n");

  printf("\n\t\tUSUARIO: ");
  scanf("%s", nombreUser);
  printf("\n\t\tCLAVE: ");
  scanf("%s", clave);
  printf("\t\t==================================\n");

  //Itera sobre los usuarios registrados para verificar las credenciales
  for(int i = 0; i < numUsuarios; i++){
    if(strcmp(usuarios[i].nombreUser, nombreUser) == 0 && strcmp(usuarios[i].clave,clave)==0){
      printf("\n\t\tInicio de sesion exitoso\n");
      printf("\n\t\tBienvenido, %s\n",nombreUser);
      return; //Exito
    }
  }

  //Muestra un mensaje de error si las credenciales son incorrectas
  printf("\t\tUsuario o clave incorrecta.\n");
  printf("\t\tIntente de nuevo.\n");
  return;
}

//Funcion para mostrar los usuarios registrados
void mainVerUsuarios(Usuario* usuarios, int numUsuarios){
  printf("\n\t\t\tUSUARIOS REGISTRADOS\n");
  printf("\t\t==================================\n");
  for(int i = 0; i < numUsuarios; i++){
    printf("\n\t\t %s\n", usuarios[i].nombreUser);
  }
  printf("\t\t==================================\n");
}