#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Estructuras
#define MAX 200
#define MED 100
#define MIN 50
#define MAX_COMENTARIOS 10
// Estructura del Usuario, dijo la profa que no debe ser dinamica entonces
// tamaño fijo
typedef struct {
  char nombre_usuario[10];
  char nombre_completo[50];
  char correo_electronico[50];
  char password[20];
} Usuario;
/*Estructura para los comentarios de post que como tal van debajo de los post
 * EDT: Ahora es una lista doblemente enlazada, sugerencia realizada por Jesus
 * Eduardo*/
struct Fecha {
  int dia, mes, anio, hora, minuto, segundo;
};
struct Comentarios {
  int idComentario;
  char comentario[MED];
  char autor[10];
  struct Comentarios *siguiente;
  struct Comentarios *anterior;
};

struct Publicaciones {
  char post[MAX]; // Se declara tamaño maximo del post
  char autor[10]; // Se declara tamaño maximo del nombre del autor
  int idPost;     // Para saber que post es para comentar
  struct Comentarios
      *listaComentarios;           // Para ubicar que post se quiere comentar
  int numComentarios;              // Cuantos comentarios hay en un post
  struct Publicaciones *siguiente; // Puntero al siguiente post
  struct Fecha
      fechapublicacion; // Para adjuntar la fecha de cuando se hizo el post
};
//[Jesus E. Lopez]
typedef struct Notif {
  char msg[MIN];
} Notif;
// Nodo de cola simple para las notificaciones [Jesus E. Lopez]
typedef struct NodoCola {
  Notif notif;
  struct NodoCola *sgt;
} NodoCola;
// Estructura de la cola para las notificaciones [Jesus E. Lopez]
typedef struct Cola {
  NodoCola *ini;
  NodoCola *fin;
} Cola;

typedef struct ListaFeed { // El nodo de nuestra lista doblemente enlazada para
                           // el timeline de publicaciones
  struct Publicaciones *p; // Declaramos un puntero de tipo struct publicaciones
  struct ListaFeed *sgt;
  struct ListaFeed *ant;
} ListaFeed;

// Nodo de arbol para mantener una conexion entre los usuarios[Jesus E. Lopez]
typedef struct NodoUsuario {
  Usuario user;
  struct NodoUsuario *izquierda;
  struct NodoUsuario *derecha;
  struct NodoUsuario **amigos; // Arreglo de punteros tipo NodoUsuario para
                               // apuntar a otros nodos amigos
  Cola *notificaciones;
  int numNotif;
  int num_amigos; // Contador para saber cuantos amigos cuenta cada usuario
  int max_amigos; // El maximo de amigos que puede tener cada usuario por
                  // defecto se asignan 2, si requiere mas entonces el espacio
                  // se duplica
} NodoUsuario;
//[Jesus E. Lopez]
NodoUsuario *sesionActual; // Creamos un puntero global de tipo NodoUsuario para
                           // ayudarnos a mantener una sesion iniciada

// Funcion para reemplazar el fflush por que aqui en replit no funciona[Jesus E.
// Lopez]
void clean_stdin() {
  int c;
  do {
    c = getchar();
  } while (c != '\n' && c != EOF);
}
// Funcion para crear cola [Jesus E. Lopez]
Cola *crearCola() {
  Cola *c = (Cola *)malloc(sizeof(Cola));
  if (c == NULL) {
    puts("Error al asignar memoria para la cola");
    return 0;
  }
  c->ini = NULL;
  c->fin = NULL;
  return c;
}
// Funcion para crear un nodo para agregar a la cola [Jesus E. Lopez]
NodoCola *crearNodo(char *msg) {
  NodoCola *c = (NodoCola *)malloc(sizeof(NodoCola));
  strcpy(c->notif.msg, msg);
  c->sgt = NULL;
  return c;
}
// Funcion para insertar nodo en el arbol [Jesus E. Lopez]
NodoUsuario *insertarEnArbol(NodoUsuario *raiz, NodoUsuario *nuevoUsuario) {
  if (raiz == NULL) {    // Verificamos si la raiz es null,
    return nuevoUsuario; // si es NULL significa que esta vacia y entonces
                         // insertamos el nuevo nodo en la raiz
  }
  // Si no esta vacio entonces procedemos a insertarlo en el arbol dependiendo
  // si es menor o mayor que el valor de la raiz
  if (strcmp(nuevoUsuario->user.nombre_usuario, raiz->user.nombre_usuario) <
      0) { // Si strncmp da un valor negativo indica que debe ir del lado
           // izquierdo de la raiz
    raiz->izquierda = insertarEnArbol(raiz->izquierda, nuevoUsuario);
  } else { // Si el valor no es negativo entonces va del lado derecho de la
           // raiz
    raiz->derecha = insertarEnArbol(raiz->derecha, nuevoUsuario);
  }
  return raiz; // Retornamos el nodo raiz actualizado
}

// Funcion para registrar un nuevo usuario [Jesus E. Lopez]
NodoUsuario *registrarNuevoUsuario(NodoUsuario *raiz) {
  NodoUsuario *nuevoUsuario =
      (NodoUsuario *)malloc(sizeof(NodoUsuario)); // Creamos un nuevo nodo
  if (!nuevoUsuario) {
    printf("Error al asignar memoria para el nuevo usuario");
    return NULL;
  }

  // Inicializamos los punteros del nuevo nodo
  nuevoUsuario->izquierda = NULL;
  nuevoUsuario->derecha = NULL;
  // Ingresar los valores para cada campo del nodo usuario
  clean_stdin();
  printf("Ingresa el nombre de usuario: ");
  fgets(nuevoUsuario->user.nombre_usuario,
        sizeof(nuevoUsuario->user.nombre_usuario), stdin);
  strtok(nuevoUsuario->user.nombre_usuario, "\n");
  printf("Ingresa el nombre completo: ");
  fgets(nuevoUsuario->user.nombre_completo,
        sizeof(nuevoUsuario->user.nombre_completo), stdin);
  strtok(nuevoUsuario->user.nombre_completo, "\n");
  printf("Ingresa tu correo electronico: ");
  fgets(nuevoUsuario->user.correo_electronico,
        sizeof(nuevoUsuario->user.correo_electronico), stdin);
  strtok(nuevoUsuario->user.correo_electronico, "\n");
  printf("Ingresa tu password: ");
  fgets(nuevoUsuario->user.password, sizeof(nuevoUsuario->user.password),
        stdin);
  strtok(nuevoUsuario->user.password, "\n");

  //====================================================================================
  NodoCola *cola = (NodoCola *)malloc(sizeof(NodoCola));
  if (!cola) {
    puts("Error al asignar memoria para la cola");
    return NULL;
  }
  // Inicializamos la cola
  nuevoUsuario->notificaciones = crearCola();
  nuevoUsuario->notificaciones->ini = NULL;
  nuevoUsuario->notificaciones->fin = NULL;
  char msgBienvenida[MIN] = "Bienvenido a la red social!";
  strcpy(cola->notif.msg, msgBienvenida);
  cola->sgt = NULL;
  nuevoUsuario->numNotif = 1;
  if (nuevoUsuario->notificaciones->ini == NULL) {
    nuevoUsuario->notificaciones->ini = cola;
  } else {
    nuevoUsuario->notificaciones->fin->sgt = cola;
  }
  nuevoUsuario->notificaciones->fin = cola;

  //====================================================================================
  printf("Usuario registrado correctamente\n");
  nuevoUsuario->num_amigos = 0; // Inicializamos el numero de amigos en cero
  nuevoUsuario->max_amigos = 2; // De primera instancia asignamos memoria para 2
                                // amigos para no utilizar mucha memoria
  nuevoUsuario->amigos = (NodoUsuario **)malloc(
      sizeof(NodoUsuario *) *
      nuevoUsuario->max_amigos); // Asignamos la memoria para los amigos
  if (!nuevoUsuario->amigos) {
    printf("Error al asignar memoria para los amigos");
    return NULL;
  }
  return insertarEnArbol(raiz, nuevoUsuario);
}
// Funcion recurisva para busqueda de un nombre de usuario en el arbol
// binario[Jesus E. L.]
NodoUsuario *buscarUsuarioArbol(NodoUsuario *actual, char *nombre_usuario) {
  if (actual == NULL) {
    return NULL;
  } // Verificamos el nodo actual si es el que estamos buscando lo retornamos
  if (strcmp(actual->user.nombre_usuario, nombre_usuario) == 0) {
    return actual;
  } // Decidimos si vamos a recorrer por izquierda o derecha
  if (strcmp(actual->user.nombre_usuario, nombre_usuario) <
      0) { // Si es menor lo buscamos en la izquierda
    return buscarUsuarioArbol(actual->derecha, nombre_usuario);
  } else { // Si es mayor lo buscamos en la derecha
    return buscarUsuarioArbol(actual->izquierda, nombre_usuario);
  }
}
void imprimirUsuarios(NodoUsuario *raiz) {
  if (raiz == NULL) { // Condicion de salida de la recursion
    return;
  }
  // llamada recursiva a la funcion para recorrer por izquierda
  imprimirUsuarios(raiz->izquierda);
  printf("Usuario:%s\n", raiz->user.nombre_usuario);

  // llamada recursiva a la funcion para recorrer por derecha
  imprimirUsuarios(raiz->derecha);
}
// Funcion de para iniciar sesion [Jesus E. Lopez]
void menuInicioSesion(NodoUsuario *raiz) {
  // Definimos variables que ingresara el usuario para buscar y comparar en el
  // arbol
  char nombre_usuario[10];
  char password[30];
  // Creamos un
  NodoUsuario *usuario;

  printf("Ingresa tu nombre de usuario: ");
  scanf("%s", nombre_usuario);
  printf("Ingresa tu password: ");
  scanf("%s", password);

  usuario = buscarUsuarioArbol(raiz, nombre_usuario);

  if (usuario != NULL && strcmp(usuario->user.password, password) == 0) {
    sesionActual = usuario;
    printf("Sesion iniciada\n");
  } else {
    printf("Error al iniciar sesion, contraseña o usuario incorrectos\n");
  }
}
// Funcion para cerrar sesion [Jesus E. Lopez]
void cerrarSesion() {
  sesionActual = NULL;
  printf("Sesion cerrada\n");
}

// Funcion para agregar amigos [Jesus E. Lopez]
void agregarAmigo(NodoUsuario *raiz) {
  // Verificamos que haya una sesion iniciada para poder agregar amigos, si no
  // hay entonces salimos de la funcion
  if (sesionActual == NULL)
    return;
  if (sesionActual->num_amigos == sesionActual->max_amigos) {
    // Duplicamos los amigos maximos que puede tener y reasignamos la memoria
    sesionActual->max_amigos = sesionActual->max_amigos * 2;
    sesionActual->amigos = (NodoUsuario **)realloc(
        sesionActual->amigos, sizeof(NodoUsuario *) * sesionActual->max_amigos);
    if (!sesionActual->amigos) {
      printf("Error al asignar memoria para los amigos, No hay mas espacio en "
             "memoria.");
      return;
    }
  }
  // Declaramos una variable para hacer la busqueda del amigo
  char nombre_amigo[10];
  NodoUsuario *amigo;
  // Pedimos el nombre del amigo
  printf(
      "Ingresa el nombre de usuario del amigo a enviar solicitud de amistad: ");
  scanf("%s", nombre_amigo);
  // Buscamos el amigo en el arbol
  amigo = buscarUsuarioArbol(raiz, nombre_amigo);
  if (amigo == NULL) {
    printf("Usuario no encontrado\n");
    return;
  }
  char noti[MIN]; // Cadena de caracteres para almacenar el nombre de la sesion
                  // actual
  char add[MIN] =
      " te envio una solicitud de amistad"; // Cadena para acompletar el mensaje
  strcpy(noti, sesionActual->user
                   .nombre_usuario); // Obtenemos el nombre del usuario actual
  strcat(noti, add);                 // Concatenamos las cadenas de caracteres

  // Insertamos nuevo nodo con nueva notificacion al amigo
  if (!amigo->notificaciones->ini) {
    amigo->notificaciones->ini = crearNodo(noti);
  } else {
    amigo->notificaciones->fin->sgt = crearNodo(noti);
  }
  amigo->notificaciones->fin = crearNodo(noti);
  // TERMINA INSERCION

  amigo->numNotif++;
  sesionActual->amigos[sesionActual->num_amigos++] = amigo;

  printf("Solicitud de amistad enviada a %s\n", amigo->user.nombre_usuario);
}
// Funcion para enviar solicitud de amistad [Jesus E. Lopez]
void enviarSolicitud(NodoUsuario *raiz) {
  // Verificamos que haya una sesion iniciada para poder enviar solicitudes
  if (sesionActual == NULL)
    return;
  printf("[Enviar solicitud de amistad]\n");
  // Llamamos a la funcion para agregar amigos que se encarga de buscar el amigo
  // y enviar la solicitud
  agregarAmigo(raiz);
}
// Funcion para aceptar solicitudes [Jesus E. Lopez]
void aceptarSolicitud(NodoUsuario *raiz) {
  if (sesionActual == NULL) {
    printf("Inicia sesion para poder aceptar solicitudes\n");
    return;
  }
  // Declaramos variables para buscar el usuario y la solicitud
  char nombre_usuario[10];
  NodoUsuario *usuario;
  printf("Ingresa el nombre de usuario para aceptar la solicitud\n");
  scanf("%s", nombre_usuario);
  usuario = buscarUsuarioArbol(raiz, nombre_usuario);
  // Condicion por si al hacer la busqueda no retorna ningun valor es decir que
  // no se encuentra en el arbol
  if (usuario == NULL) {
    printf("El usuario no se encuentra en la red social\n");
    return;
  }

  // Añadimos el usuario al arreglo de amigos del usuario
  sesionActual->amigos[sesionActual->num_amigos] = usuario;
  sesionActual->num_amigos++;
  printf("Solicitud de amistad aceptada\n");
  printf("Ahora %s y %s son amigos\n", sesionActual->user.nombre_usuario,
         usuario->user.nombre_usuario);
}

// Funcion que recorre los amigos que tienes agregados y los imprime por pantalla[Jesus E. Lopez
//MODIFICACION DE LA FUNCION YA QUE IMPRIMIA POR PANTALLA SIN VERIFICAR QUE HUBIERA UNA CONEXION BIDIRECCIONAL 
void mostrarAmigos() {
  if(sesionActual == NULL){
    puts("Debes iniciar sesion para usar esta funcion");
    return;
  }
  printf("Amigos de %s\n",sesionActual->user.nombre_usuario);
  //No funcionaba por que me faltaba la condicion para seguir iterando en el ciclo for
  for(int i = 0; i < sesionActual->num_amigos; i++){
    NodoUsuario *amigo = sesionActual->amigos[i];

    int esAmigo = 0;
    for(int j = 0; j < amigo->num_amigos; j++){
      if(amigo->amigos[j] == sesionActual){
        esAmigo = 1;
        break;
      }
    }
    if(esAmigo){
      printf("- %s\n",amigo->user.nombre_usuario);
    }
  }
}
// Funcion para mostrar las notif. de la sesion actual [Jesus E. Lopez]
void mostrarNotif() {
  NodoCola *aux = sesionActual->notificaciones->ini;
  if (!aux) {
    printf("No tienes notificaciones\n");
    return;
  }
  printf("Tienes %d notificaciones \n", sesionActual->numNotif);
  while (aux != NULL) {
    printf("%s\n", aux->notif.msg);
    aux = aux->sgt;
  }
}
// Funcion para comentar en los posts, llamada por la funcion VerMuro [Daniel
// G.]
int contadorComentarios = 1;

void comentarPost(struct Publicaciones *publicacion, char *nombreUsuario) {
  // Crea un nuevo nodo de comentario
  struct Comentarios *nuevoComentario =
      (struct Comentarios *)malloc(sizeof(struct Comentarios));

  if (!nuevoComentario) {
    printf("Error al asignar memoria para el comentario\n");
    return;
  }

  // Inicializa los datos del nuevo comentario
  printf("Escribe tu comentario:\n");
  getchar(); // Limpiar el buffer de entrada
  fgets(nuevoComentario->comentario, sizeof(nuevoComentario->comentario),
        stdin);
  strtok(nuevoComentario->comentario, "\n");

  strncpy(nuevoComentario->autor, nombreUsuario,
          sizeof(nuevoComentario->autor));

  nuevoComentario->idComentario = contadorComentarios++;

  // Agrega el nuevo comentario al inicio de la lista de comentarios de la
  // publicación
  nuevoComentario->siguiente = publicacion->listaComentarios;
  nuevoComentario->anterior = NULL;

  if (publicacion->listaComentarios != NULL) {
    publicacion->listaComentarios->anterior = nuevoComentario;
  }

  publicacion->listaComentarios = nuevoComentario;

  publicacion->numComentarios++;

  printf("Comentario realizado con éxito\n");
}
struct Fecha obtenerFecha() {
  time_t t; // Variable para el tiempo con time.h, almacena la hora actual desde
            // el 1 de enero de 1970, despues se le suman años para que sea la
            // actualidad que crazy.
  struct tm *fechaActual; // Se crea un puntero para controlar la fecha,
                          // que contiene la informacion del dia mes y año, tm
                          // es propia de la libreria time
  time(&t); // Se obtiene la hora actual en segundos, se almacena en t
  fechaActual =
      localtime(&t); // Se convierte de segundos a tiempo local (medio raro)

  struct Fecha fecha;               // Aquí se guarda la información de fecha
  fecha.dia = fechaActual->tm_mday; // Se almacena el dia
  fecha.mes = fechaActual->tm_mon + 1; // Se almacenan los meses
  fecha.anio =
      fechaActual->tm_year +
      1900; // Se almacenan el año actual, pero se suma 1900 porque tm_year como
            // tal siempre devuelve el año desde 1900, dependiendo del equipo
            // tm_year podria devolver correctamente el año actual, pero
            // conociendo replit se hará de todas formas la suma.
  fecha.hora = fechaActual->tm_hour;   // Se almacena la hora
  fecha.minuto = fechaActual->tm_min;  // Se almacena los minutos
  fecha.segundo = fechaActual->tm_sec; // Se almacenan los segundos
  return fecha;                        // Se devuelve la fecha actual completa
}
// Funcion para Ver el Feed o Muro, solo se ven posts de amigos y desde aca se
// pueden comentar los posts [Daniel G.]
void verMuro(struct Publicaciones *miLineaDeTiempo) {
  if (sesionActual == NULL) {
    printf("Inicia sesion para ver el muro\n");
    return;
  }

  printf("Muro de %s:\n", sesionActual->user.nombre_usuario);

  struct Publicaciones *publicacionActual = miLineaDeTiempo;
  while (publicacionActual != NULL) {
    struct Fecha fechapost = obtenerFecha();
    printf("[%02d][%02d][%d]   [%02d]:[%02d]:[%02d]", fechapost.dia,
           fechapost.mes, fechapost.anio, fechapost.hora, fechapost.minuto,
           fechapost.segundo); // Se muestra finalmente ajsdjas, dia mes año,
                               // hora, minutos, segundos en cada post c:
    printf(" ID[%d] [%s]\n", publicacionActual->idPost,
           publicacionActual
               ->autor); // Se muestra el id del post y el autor del post
    printf("%s\n", publicacionActual->post); // Se muestra el post como tal
    struct Comentarios *comentarioActual = publicacionActual->listaComentarios;
    printf("Comentarios:\n"); // Se muestran los comentarios de dicho pot

    while (comentarioActual != NULL) {
      printf("\t[%s]: %s\n",
             comentarioActual->autor, // Se muestran los siguientes comentarios
             comentarioActual->comentario);
      comentarioActual = comentarioActual->siguiente;
    }

    printf("==================================\n");

    publicacionActual = publicacionActual->siguiente; // Apunta al siguiente
                                                      // post
  }

  // Permitir al usuario comentar una publicación
  printf("Selecciona el ID de la publicacion para comentar (o introduce 0 para "
         "salir): ");
  int idSeleccionado;
  scanf("%d", &idSeleccionado);

  if (idSeleccionado != 0) {
    // Buscar la publicación seleccionada, funciona a través del idPost
    struct Publicaciones *publicacionSeleccionada = miLineaDeTiempo;
    while (publicacionSeleccionada != NULL &&
           publicacionSeleccionada->idPost != idSeleccionado) {
      publicacionSeleccionada = publicacionSeleccionada->siguiente;
    }

    if (publicacionSeleccionada != NULL) {
      // Comentar en la publicación seleccionada
      comentarPost(publicacionSeleccionada, sesionActual->user.nombre_usuario);
    } else {
      printf("Publicacion no encontrada\n");
    }
  }
}

// Declaración del contador global
int contadorPublicaciones = 1;
int contadorImagenes = 1;
// Funcion para publicar en el Feed o Muro [Daniel G.]
void publicarEnMuro(struct Publicaciones **miLineaDeTiempo,
                    char *nombreUsuario) {
  char opcionm;
  clean_stdin();
  // Con esto ahora se apunta al final de la lista y no al inicio de modo que es
  // Hola-Mundo, y no Mundo-Hola :D
  struct Publicaciones *ultimoPost = *miLineaDeTiempo;
  while (ultimoPost != NULL && ultimoPost->siguiente != NULL) {
    ultimoPost = ultimoPost->siguiente;
  }
  do {
    printf("Selecciona 1 para publicar texto en el muro, y 2 para publicar una "
           "Imagen en el muro: ");
    scanf("%c", &opcionm);
    if (opcionm == '1') {
      // Crea un nuevo nodo de publicación
      struct Publicaciones *nuevaPublicacion =
          (struct Publicaciones *)malloc(sizeof(struct Publicaciones));

      if (!nuevaPublicacion) {
        printf("Error al asignar memoria para la publicacion\n");
        return;
      }

      // Inicializa los datos de la nueva publicación
      printf("Escribe tu publicacion:\n");
      getchar(); // Limpiar el buffer de entrada
      fgets(nuevaPublicacion->post, sizeof(nuevaPublicacion->post), stdin);
      strtok(nuevaPublicacion->post, "\n");

      strncpy(nuevaPublicacion->autor, nombreUsuario,
              sizeof(nuevaPublicacion->autor));
      nuevaPublicacion->idPost = contadorPublicaciones++;
      nuevaPublicacion->listaComentarios =
          NULL; // Inicializa la lista de comentarios
      nuevaPublicacion->numComentarios = 0;
      // Si está vacio entonces es el primero en a lista
      if (ultimoPost == NULL) {
        *miLineaDeTiempo = nuevaPublicacion; // Y asigna el nuevo nodo como el
                                             // primero en la lista
      } else {
        ultimoPost->siguiente =
            nuevaPublicacion; // Si no esta vacia entonces actualiza al puntero
                              // siguiente para apuntar al ultimo nodo, de fin a
                              // inicio
      }
      nuevaPublicacion->siguiente =
          NULL; // Y al final el siguiente apunta a null indicando que es el
                // ultimo de la lista
      printf("Publicacion realizada con exito\n");
      break;
    } else if (opcionm == '2') {
      char imagen[20]; // 20 porque ImagenN.jpg tiene 11 caracteres y asi se
                       // pueden tener numeros de imagen de 2 cifras, no se
                       // pasen con las imagenes jaja
      sprintf(
          imagen, "Imagen%d.jpg",
          contadorImagenes); // Con esto se forma una cadena con los comonentes
                             // de la imagen y el contador de imagenes de esta
                             // manera es posible verlo en el muro.
      // Crea un nuevo nodo de publicación
      struct Publicaciones *nuevaPublicacion =
          (struct Publicaciones *)malloc(sizeof(struct Publicaciones));

      if (!nuevaPublicacion) {
        printf("Error al asignar memoria para la publicacion\n");
        return;
      }
      strncpy(nuevaPublicacion->post, imagen, sizeof(nuevaPublicacion->post));
      strncpy(nuevaPublicacion->autor, nombreUsuario,
              sizeof(nuevaPublicacion->autor));
      nuevaPublicacion->idPost = contadorPublicaciones++;
      nuevaPublicacion->listaComentarios =
          NULL; // Inicializa la lista de comentarios
      nuevaPublicacion->numComentarios = 0;
      contadorImagenes++; // Se aumenta para que el nombre de la imagen tambien
                          // sea incrementada
      // Si está vacio entonces es el primero en a lista
      if (ultimoPost == NULL) {
        *miLineaDeTiempo = nuevaPublicacion; // Y asigna el nuevo nodo como el
                                             // primero en la lista
      } else {
        ultimoPost->siguiente =
            nuevaPublicacion; // Si no esta vacia entonces actualiza al puntero
                              // siguiente para apuntar al ultimo nodo, de fin a
                              // inicio
      }

      printf("Publicacion realizada con exito\n");
      break;
    } else {
      printf("Opcion no valida, vuelva a intentarlo.\n");
    }
  } while (1);
}
// Funcion para continuar con la tecla enter [Jesus E. Lopez]
void continuar() {
  printf("Presiona enter para continuar");
  while (getchar() != '\n')
    ;
}
//Funcion para eliminar la primera notificacion en la cola [Jesus E. Lopez]
void eliminarNotif(){
  if(sesionActual->notificaciones->ini == NULL){
    printf("La cola esta vacia\n");
    return;
  }
  NodoCola *temp = sesionActual->notificaciones->ini;
  sesionActual->notificaciones->ini = sesionActual->notificaciones->ini->sgt;
  free(temp);
  printf("Notificacion eliminada satisfactoriamente\n");
}
int main() {
  Usuario *raiz = NULL;
  struct Publicaciones *miLineaDeTiempo = NULL;

  char opcion;
  do {
    // Menu principal
    system("clear");
    printf("RED SOCIAL UV 2023\n");
    printf("==================================\n");
    printf("[1]. Registrar usuario nuevo\n");
    printf("[2]. Iniciar sesion\n");
    printf("[3]. Ver usuarios\n");
    printf("[4]. Salir\n");
    printf("==================================\n");
    printf("Selecciona una opcion: ");
    // Leer la opción del usuario
    if (scanf(" %c", &opcion) != 1) {
      // Limpiar el búfer de entrada en caso de entrada no válida
      while (getchar() != '\n')
        opcion = '0'; // Establecer una opción no válida para repetir el bucle
    }
    // Realiza acciones segun la opcion seleccionada
    switch (opcion) {
    case '1':
      raiz = registrarNuevoUsuario(raiz);
      continuar();
      break;
    case '2':
      menuInicioSesion(raiz);
      continuar();
      clean_stdin();
      if (sesionActual == NULL) {
        break;
      }

      char opcion2;
      do {
        system("clear");
        printf("======================================\n");
        printf("Bienvenido a la red social de Luzio UV\n");
        printf("======================================\n");
        printf("Elige una opcion, polluelo.\n");
        printf("[1]. Ver amigos\n");
        printf("[2]. Notificaciones\n");
        printf("[3]. Mandar solicitud\n");
        printf("[4]. Aceptar solicitud\n");
        printf("[5]. Ver muro\n");
        printf("[6]. Publicar en el muro\n");
        printf("[7]. Cerrar Sesion\n");
        printf("==================================\n");
        printf("Elige la opción que desees ejecutar: ");
        // Leer la opción del usuario
        if (scanf(" %c", &opcion2) != 1) {
          // Limpiar el búfer de entrada en caso de entrada no válida
          while (getchar() != '\n')
            ;
          opcion2 =
              '0'; // Establecer una opción no válida para repetir el bucle
        }
        switch (opcion2) {
        case '1':
          mostrarAmigos();
          clean_stdin();
          continuar();
          break;
        case '2':
          clean_stdin();
          char opcion3;
          do {
            system("clear");
            printf("======================================\n");
            printf("           Notificaciones             \n");
            printf("======================================\n");
            printf("Elige una opcion.\n");
            printf("[1]. Ver notificaciones\n");
            printf("[2]. Eliminar primera notificacion\n");
            printf("[3]. Salir del menu notificaciones.\n");
            if (scanf(" %c", &opcion3) != 1) {
              // Limpiar el búfer de entrada en caso de entrada no válida
              while (getchar() != '\n')
                ;
              opcion3 =
                  '0'; // Establecer una opción no válida para repetir el bucle
            }
              switch(opcion3){
                case '1':
                  mostrarNotif();
                  clean_stdin();
                  continuar();
                  break;
                case '2':
                  eliminarNotif();
                  clean_stdin();
                  continuar();
                  break;
                default:
                  puts("Opcion no valida");
                  break;
              }
          }while(opcion3 != '3');
          break;
        case '3':
          enviarSolicitud(raiz);
          clean_stdin();
          continuar();
          break;
        case '4':
          aceptarSolicitud(raiz);
          clean_stdin();
          continuar();
          break;
        case '5':
          verMuro(miLineaDeTiempo);
          clean_stdin();
          continuar();
          break;
        case '6':
          publicarEnMuro(&miLineaDeTiempo, sesionActual->user.nombre_usuario);
          continuar();
          break;
        case '7':
          cerrarSesion();
          clean_stdin();
          continuar();
          break;
        default:
          printf("Ingresa una opcion valida\n");
          continuar();
          break;
        }

      } while (opcion2 != '7');
      break;
    case '3':
      // Aqui va funcion para ver todos los usuarios registrados
      printf("Usuarios registrados en la red social:\n");
      imprimirUsuarios(raiz);
      clean_stdin();
      continuar();
      break;

    case '4':
      printf("Saliendo del programa...\n");
      break;
    default:
      printf("Opcion invalida, intente de nuevo\n");
    }
  } while (opcion != '4');

  free(raiz);
  return 0;
}
