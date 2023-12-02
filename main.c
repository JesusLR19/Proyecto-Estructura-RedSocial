#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
/*Estructura para los comentarios de post que como tal van debajo de los post,
 * alpha de comentarios solamente. EDT: Ahora es una lista doblemente enlazada,
 * sugerencia realizada por Jesus Eduardo*/
struct Comentarios {
  char comentario[MED];
  char autor[10];
  struct Comentarios *siguiente;
  struct Comentarios *anterior;
};

struct Publicaciones {
  char post[MAX];
  char autor[10];
  int idPost;
  struct Comentarios *listaComentarios;
  int numComentarios;
  struct Publicaciones *siguiente;
};
typedef struct Notif {
  char msg[MIN];
}Notif;
// Nodo de cola simple para las notificaciones
typedef struct NodoCola {
  Notif notif;
  struct NodoCola *sgt;
} NodoCola;
//Estructura de la cola para las notificaciones
typedef struct Cola{
  NodoCola *ini;
  NodoCola *fin;
}Cola;

typedef struct ListaFeed { // El nodo de nuestra lista doblemente enlazada para
                           // el timeline de publicaciones
  struct Publicaciones *p; // Declaramos un puntero de tipo struct publicaciones
  struct ListaFeed *sgt;
  struct ListaFeed *ant;
} ListaFeed;

// Nodo de arbol para mantener una conexion entre los usuarios
typedef struct NodoUsuario {
  Usuario user;
  struct NodoUsuario *izquierda;
  struct NodoUsuario *derecha;
  struct NodoUsuario **amigos;//Arreglo de punteros tipo NodoUsuario para apuntar a otros nodos amigos
  Cola *notificaciones;
  int numNotif;
  int num_amigos; // Contador para saber cuantos amigos cuenta cada usuario
  int max_amigos; // El maximo de amigos que puede tener cada usuario por
                  // defecto se asignan 2, si requiere mas entonces el espacio
                  // se duplica
} NodoUsuario;

NodoUsuario *sesionActual; // Creamos un puntero global de tipo NodoUsuario para
                           // ayudarnos a mantener una sesion iniciada
//Funcion para crear cola [Jesus E. Lopez]
Cola *crearCola(){
  Cola *c = (Cola*)malloc(sizeof(Cola));
  if (c == NULL){
    puts("Error al asignar memoria para la cola");
    return 0;
  }
  c->ini = NULL;
  c->fin = NULL;
  return c;
}
NodoCola *crearNodo(char *msg){
  NodoCola *c = (NodoCola*)malloc(sizeof(NodoCola));
  strcpy(c->notif.msg,msg);
  c->sgt = NULL;
  return c;
}


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
  NodoUsuario *nuevoUsuario = (NodoUsuario *)malloc(sizeof(NodoUsuario)); // Creamos un nuevo nodo
  if (!nuevoUsuario) {
    printf("Error al asignar memoria para el nuevo usuario");
    return NULL;
  }

  // Inicializamos los punteros del nuevo nodo
  nuevoUsuario->izquierda = NULL;
  nuevoUsuario->derecha = NULL;
  //Ingresar los valores para cada campo del nodo usuario
  fflush(stdin);
  printf("Ingresa el nombre de usuario: ");
  fgets(nuevoUsuario->user.nombre_usuario,sizeof(nuevoUsuario->user.nombre_usuario), stdin);
  strtok(nuevoUsuario->user.nombre_usuario, "\n");
  printf("Ingresa el nombre completo: ");
  fgets(nuevoUsuario->user.nombre_completo,sizeof(nuevoUsuario->user.nombre_completo), stdin);
  strtok(nuevoUsuario->user.nombre_completo, "\n");
  printf("Ingresa tu correo electronico: ");
  fgets(nuevoUsuario->user.correo_electronico,sizeof(nuevoUsuario->user.correo_electronico), stdin);
  strtok(nuevoUsuario->user.correo_electronico, "\n");
  printf("Ingresa tu password: ");
  fgets(nuevoUsuario->user.password, sizeof(nuevoUsuario->user.password),stdin);
  strtok(nuevoUsuario->user.password, "\n");

  //====================================================================================
  NodoCola *cola = (NodoCola*)malloc(sizeof(NodoCola));
  if(!cola){
    puts("Error al asignar memoria para la cola");
    return NULL;
  }
  //Inicializamos la cola
  nuevoUsuario->notificaciones = crearCola();
  nuevoUsuario->notificaciones->ini = NULL;
  nuevoUsuario->notificaciones->fin = NULL;
  char msgBienvenida[MIN] = "Bienvenido a la red social!";
  strcpy(cola->notif.msg,msgBienvenida);
  cola->sgt = NULL;
  nuevoUsuario->numNotif = 1;
  if(nuevoUsuario->notificaciones->ini == NULL){
    nuevoUsuario->notificaciones->ini = cola;
  }else{
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
// Funcion recurisva para busqueda de un nombre de usuario en el arbol binario
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
//Funcion de para iniciar sesion [Jesus E. Lopez]
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
//Funcion para cerrar sesion [Jesus E. Lopez]
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
    sesionActual->amigos = (NodoUsuario **)realloc(sesionActual->amigos, sizeof(NodoUsuario *) * sesionActual->max_amigos);
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
  printf("Ingresa el nombre de usuario del amigo a enviar solicitud de amistad: ");
  scanf("%s", nombre_amigo);
  // Buscamos el amigo en el arbol
  amigo = buscarUsuarioArbol(raiz, nombre_amigo);
  if (amigo == NULL) {
    printf("Usuario no encontrado\n");
    return;
  }
  char noti[MIN]; //Cadena de caracteres para almacenar el nombre de la sesion actual
  char add[MIN] = " te envio una solicitud de amistad"; //Cadena para acompletar el mensaje
  strcpy(noti,sesionActual->user.nombre_usuario); //Obtenemos el nombre del usuario actual
  strcat(noti,add); //Concatenamos las cadenas de caracteres

  //Insertamos nuevo nodo con nueva notificacion al amigo
  if(!amigo->notificaciones->ini){
    amigo->notificaciones->ini = crearNodo(noti);
  }else{
    amigo->notificaciones->fin->sgt = crearNodo(noti);
  }
  amigo->notificaciones->fin = crearNodo(noti);
  //TERMINA INSERCION
 
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
//Funcion para aceptar solicitudes [Jesus E. Lopez]
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


//Funcion que recorre los amigos que tienes agregados y los imprime por pantalla[Jesus E. Lopez]
void mostrarAmigos(){
  printf("Amigos de %s\n", sesionActual->user.nombre_usuario);
  for (int i = 0; i < sesionActual->num_amigos; i++) {
    printf("- %s\n", sesionActual->amigos[i]->user.nombre_usuario);
  }
}
//Funcion para mostrar las notif. de la sesion actual [Jesus E. Lopez]
void mostrarNotif(){
  NodoCola *aux = sesionActual->notificaciones->ini;
  if(!aux){
    printf("No tienes notificaciones\n");
    return;
  }
  printf("Tienes %d notificaciones \n",sesionActual->numNotif);
  while(aux != NULL){
    printf("%s\n",aux->notif.msg);
    aux = aux->sgt;
  }
}

/*
// MI GENTE TAMO EN ALPHA 2.0
// Funciones 2.0, ahora sin timestamp xd
void agregarComentario(struct Publicaciones *publicaciones, char autor[]) {
  if (publicaciones->numComentarios < MAX_COMENTARIOS) {
    // Crear comentario y darle espacio dinamico acorde al comentario (maximo de
    // 120 char) el formateo de replit es raro a veces, por eso la forma de
    // sintaxis de las funciones
    struct Comentarios *nuevoComentario =
        (struct Comentario *)malloc(sizeof(struct Comentarios));
    // Se lee el comentario a través del teclado y se almacena en comentario
    printf("Escribe lo que quieres comentar: \n");
    // fgets deja un \n independientemente de lo que lea asi que lo eliminamos.
    fgets(nuevoComentario->comentario, sizeof(nuevoComentario->comentario),
          stdin);
    nuevoComentario->comentario[strcspn(nuevoComentario->comentario, "\n")] =
        '\0';
    // Se almacena el autor en autor achi es jajasj que inteligente es el
    // completador de replit
    strcpy(nuevoComentario->autor, autor);

    // Se agrega el comentario a la lista
    nuevoComentario->anterior = NULL;
    nuevoComentario->siguiente = publicaciones->listaComentarios;
    // Si ya hay datos en la lista xdd
    if (publicaciones->listaComentarios != NULL) {
      publicaciones->listaComentarios->anterior = nuevoComentario;
    }
    // Actualizamos el puntero de la lista de comentarios para que apunte al
    // nuevo comentario
    publicaciones->listaComentarios = nuevoComentario;

    // Se incrementa acá la cantidad de comentarios
    publicaciones->numComentarios++;
  } else {
    printf("Ya se ha alcanzado el limite de comentarios en este post\n");
  }
}

// Funcion para agregar una publicacion, codigo reciclado de lo de arriba jiji
void agregarPost(struct Publicaciones **timeline, char autor[]) {
  // Estructura para crear y alojar memoria para un post
  struct Publicaciones *nuevoPost =
      (struct Publicaciones *)malloc(sizeof(struct Publicaciones));

  // Se lee el post por teclado (200 caracteres como twitter)
  printf("Escribe tu publicación: \n");
  fgets(nuevoPost->post, sizeof(nuevoPost->post), stdin);
  // Se elimina el caracter extra de fgets
  nuevoPost->post[strcspn(nuevoPost->post, "\n")] = '\0';
  // Se almacena el autor en el post, que util es el autocompletado dio mio
  strcpy(nuevoPost->autor, autor);
  nuevoPost->numComentarios = 0;
  // Se iniciaiza la lista de comentarios del post como tal
  nuevoPost->listaComentarios = NULL;
  nuevoPost->siguiente = NULL;

  nuevoPost->siguiente = *timeline;
  *timeline = nuevoPost;
}

void Timeline(struct Publicaciones *timeline, char autor[]) {
  int idPost = 1;
  printf("Linea de Tiempo\n");
  // esto lleva el conteo de los post de modo que se sabe cual fue el primero y
  // el siguiente xdd, lo mismo para comentarios
  int i, j = 1;
  while (timeline != NULL) {
    printf("[%d] [%s] %s\n", i, timeline->autor, timeline->post);

    // Acá se cicla para poner los comentarios despues de los post (osea abajo
    // como yo habia dicho arriba)
    struct Comentarios *comentario = timeline->listaComentarios;
    while (comentario != NULL) {
      printf("[%d] [%s] %s\n", comentario->j, comentario->autor,
             comentario->comentario);
      comentario = comentario->siguiente;
      i++;
    }
    timeline = timeline->siguiente;
    idPost++;
  }
  printf("\n\n");
  // Acá empieza el menú de la linea de tiempo o Feed, escoge el usuario en
  // teoria que post quiere comentar a través de un switch case
  int opcion;

  printf("Elige el post en el que quieres comentar o presiona la tecla 0 para "
         "salir: ");
  scanf("%d", &opcion);
  // Se busca el post que se quiere comentar
  if (opcion != 0) {
    struct Publicaciones *post = timeline;
    while (post != NULL && post->idPost != opcion) {
      post = post->siguiente;
    }
    if (post != NULL) {
      agregarComentario(post, autor);
    } else {
      printf("El post no existe \n");
    }
  }
}
*/
int main() {
  Usuario *raiz = NULL;

  raiz = registrarNuevoUsuario(raiz);
  raiz = registrarNuevoUsuario(raiz);
  // raiz = registrarNuevoUsuario(raiz);
  menuInicioSesion(raiz);
  mostrarNotif();
  enviarSolicitud(raiz);
  cerrarSesion();
  menuInicioSesion(raiz);
  mostrarNotif();
  aceptarSolicitud(raiz);
  mostrarAmigos();
  // cerrarSesion();
  free(raiz);
  return 0;
}