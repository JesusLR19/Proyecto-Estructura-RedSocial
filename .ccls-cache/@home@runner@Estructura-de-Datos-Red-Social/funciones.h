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

// Funcion para registrar un nuevo usuario
NodoUsuario *registrarNuevoUsuario(NodoUsuario *raiz, Cola *c) {
  NodoUsuario *nuevoUsuario =
      (NodoUsuario *)malloc(sizeof(NodoUsuario)); // Creamos un nuevo nodo
  if (!nuevoUsuario) {
    printf("Error al asignar memoria para el nuevo usuario");
    return NULL;
  }
  /*NodoCola *notificaciones = (NodoCola*)malloc(sizeof(NodoCola));
  if(!nuevoUsuario->notificaciones){
    printf("Error al asignar memoria para las notificaciones");
    return NULL;
  }*/
  // Inicializamos los punteros del nuevo nodo
  nuevoUsuario->izquierda = NULL;
  nuevoUsuario->derecha = NULL;
  printf("Ingresa el nombre de usuario: ");
  fgets(nuevoUsuario->user.nombre_usuario,
        sizeof(nuevoUsuario->user.nombre_usuario), stdin);
  strtok(nuevoUsuario->user.nombre_usuario, "\n");
  printf("Ingresa el nombre completo: ");
  fgets(nuevoUsuario->user.nombre_completo,
        sizeof(nuevoUsuario->user.nombre_completo), stdin);
  strtok(nuevoUsuario->user.nombre_completo, "\n");
  printf("Ingresa tu correo electronico");
  // Media hora buscando el error y es un parentesis al terminar correo
  // electronico XD
  fgets(nuevoUsuario->user.correo_electronico,
        sizeof(nuevoUsuario->user.correo_electronico), stdin);
  strtok(nuevoUsuario->user.correo_electronico, "\n");
  printf("Ingresa tu password: ");
  fgets(nuevoUsuario->user.password, sizeof(nuevoUsuario->user.password),
        stdin);
  strtok(nuevoUsuario->user.password, "\n");

  //====================================================================================
  //Creamos un nodo para las notificaciones y lo añadimos a una cola inicialmente vacia
  //====================================================================================
  //Asigngamos espacio en memoria para el nodo que añadiremos a la cola
  nuevoUsuario->notificaciones = (NodoCola*)malloc(sizeof(NodoCola));
  //Verificamos que el nodo se haya creado correctamente
  if(nuevoUsuario->notificaciones == NULL){
    puts("Error al asignar memoria para las notificaciones");
    return NULL;
  }
  //Creamos un arreglo de caracteres para mandar la primer notificacion al usuario
  char notiP[MIN] = "Bienvenido a la red social";
  strcpy(nuevoUsuario->notificaciones->noti[0].notif,notiP);
  //Declaramos en NULL el puntero de sgt de nuestro NodoCola
  nuevoUsuario->notificaciones->sgt = NULL;
  //Si la cola esta vacia entonces agregamos el nuevo NodoCola al inicio 
  if(!c->ini)
    c->ini = nuevoUsuario->notificaciones;
  //Si no esta vacia lo agregamos al final de la cola
  else
    c->fin->sgt = nuevoUsuario->notificaciones;
  //====================================================================================
  //Verificar que todo se haya asignado correctamente
  if (nuevoUsuario->user.nombre_usuario != NULL &&
      nuevoUsuario->user.nombre_completo != NULL &&
      nuevoUsuario->user.correo_electronico != NULL &&
      nuevoUsuario->user.password != NULL) {
    printf("Usuario registrado correctamente\n");
  } else {
    printf("Error al registrar el usuario\n");
    return NULL;
  }
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
// IDEA DE INICIO DE SESION
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
void cerrarSesion() {
  sesionActual = NULL;
  printf("Sesion cerrada\n");
}
// Funcion de prueba para agregar amigos
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
  sesionActual->amigos[sesionActual->num_amigos++] = amigo;
  printf("Solicitud de amistad enviada a %s\n", amigo->user.nombre_usuario);
}
// Funcion para enviar solicitud de amistad
void enviarSolicitud(NodoUsuario *raiz) {
  // Verificamos que haya una sesion iniciada para poder enviar solicitudes
  if (sesionActual == NULL)
    return;
  printf("[Enviar solicitud de amistad]\n");
  // Llamamos a la funcion para agregar amigos que se encarga de buscar el amigo
  // y enviar la solicitud
  agregarAmigo(raiz);
}
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
  sesionActual->amigos[sesionActual->num_amigos++] = usuario;
  printf("Solicitud de amistad aceptada\n");
  printf("Ahora %s y %s son amigos\n", sesionActual->user.nombre_usuario,
         usuario->user.nombre_usuario);
}


//Funcion que recorre los amigos que tienes agregados y los imprime por pantalla
void mostrarAmigos(){
  printf("Amigos de %s\n", sesionActual->user.nombre_usuario);
  for (int i = 0; i < sesionActual->num_amigos; i++) {
    printf("%s\n", sesionActual->amigos[i]->user.nombre_usuario);
  }
}
