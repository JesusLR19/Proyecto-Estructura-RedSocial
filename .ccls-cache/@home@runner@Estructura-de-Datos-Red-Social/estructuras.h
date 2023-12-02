#define MAX 200
#define MED 100
#define MIN 50
#define MAX_COMENTARIOS 10
// Estructura del Usuario, dijo la profa que no debe ser dinamica entonces
// tamaño fijo
typedef struct {
  /*Al ser los datos char permite el uso de caracteres especiales por lo que no
   * deberia haber     un try catch de ningun tipo para evitar que tipeen cosas
   * que no van, asi que BAJO ESTE          ARGUMENTO, puede contener numeros o
   * letras ya que en char vendrían a ser sus equivalencias      en ASCII*/
  char nombre_usuario[10];
  char nombre_completo[50];
  char correo_electronico[50];
  /*recordatorio, a replit no le gusta la "ñ", y lo de contraseña es temporal,
   * puede ser           implementado de otra manera*/
  char password[20];
  /*esto en teoria despues es accedido por un ciclo en el main como tal que
   * puede dar tanto       los nombres completos como datos en especifico a
   * través de la creación de un objeto; p.e:       struct Usuario Usuarios */
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

/*Lo de arriba necesita un enlace a la interfaz por lo que es necesaria la
 * funcion que solicite los datos y los guarde*/
/*Estructura estatica llamada Publicaciones sirve para almacenar los post con un
 * tamaño maximo de 200 caracteres y ahora los almacena en una lista doblemente
 * enlazada */
struct Publicaciones {
  char post[MAX];
  char autor[10];
  int idPost;
  /*Llamada al comentario del post, en alpha solo pueden existir 10 comentarios,
   * toca ver si aguanta mas y si no hay perdida de datos o algo similar
   * xd*/
  struct Comentarios *listaComentarios;
  int numComentarios;
  struct Publicaciones *siguiente;
};
struct Notificaciones {
  char notif[MIN];
};
// Nodo de lista doblemente enlazada para las notificaciones
typedef struct NodoCola {
  struct Notificaciones noti[10];
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
  struct NodoUsuario **amigos;
  NodoCola *notificaciones;
  int num_amigos; // Contador para saber cuantos amigos cuenta cada usuario
  int max_amigos; // El maximo de amigos que puede tener cada usuario por
                  // defecto se asignan 2, si requiere mas entonces el espacio
                  // se duplica
} NodoUsuario;