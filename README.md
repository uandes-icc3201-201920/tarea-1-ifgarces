# Informe - Tarea 1-2
Ignacio F. Garcés Santander


### 0. COMANTERIOS
* Se me olvidó que era en parejas, me acordé el último día a as 10 de la noche. Acabo de notar que me agregaron una pareja aleatoria, no lo contacté porque no sabía, pero la persona tampoco no me contactó. Hice la tarea yo solo.
** No alcancé a probarlo en Linux por temas de tiempo y porque el computador se queda pegado la mayoría de las veces al abrir la máquina virtual con Linux, y si es que funciona, toma como 10 minutos abrirlo, y tengo que cerrar todos los otros procesos del computador para que medio aguante, lo que dificulta enormemente el trabajo. Por favor considerar en caso de que no funcione bien en Linux. Considerar también ojalá que lo hice solo.


### 1. DIFICULTADES ENCONTRADAS
- Evitar inanición y condición de carrera.
- Lidiar con C/C++ y sus charasteriscos, sus char[] y sus segmentation faults que no dicen en qué linea está el problema.


### 2. FUNCIONES NO IMPLEMENTADAS
- Temporizador para que el servidor indique error si un cliente no se ha conectado en cierto tiempo.
- Hay que hacer `make clean` de nuevo para volver a ejecutar los programas, ya que no hice la función que permite conectarse al socket aunque este esté creado, por lo que debe borrarse a mano para poder establecer otro igual.
- No pude hacerlo con `pthreads`. Tuve que hacer que el servidor atienda clientes con `fork()`. Esto porque por algún motivo, la función `read()` del socket provocaba el infame segmentation fault (quinta línea de la función `process_client_request`). Edit: al final lo traté de hacer con threads y tamién pasó lo mismo.


### 3. PROBLEMAS CONOCIDOS
- Por algún motivo, si se usan las flechas direccionales en la consola al escribir el input del cliente, el servidor enloquece un poco. Es decir, si trato de escribir los paréntesis () primero, luego devolverme con la flecha para rellenar los parámetros del comando que esté escribiendo, ocurren problemas en el `strtok()` y por lo tanto da un resultado inesperado. Aunque puede que no sea problema de mi código.
- Solo se reconocen los comandos en miníscula, y sin espacios. Por ejemplo, no se reconoce `INSERT(0,hola)` ni `insert (0,hola)`. O quizás sí pero cosas raras pasarían.


### 4. FUNCIONAMIENTO (EXTRA)
Esto es para explicar un poco cómo lo hice.
- El cliente le envía las instrucciones al buffer (char) en el formato siguiente:
  
  `<nombre_instrucción>:<argumento1>:<argumento2>`
  
  donde los argumentos son los parámetros que requiera esa función. Si no requiere alguno, se vuelve `"null"`. Entonces, el servidor reconoce lo que el cliente quiere haciendo split (`strtok`) con `":"`. Luego, la respuesta del servidor se imprime en el cliente en color amarillo.

- El tamaño máximo (número de caracteres) del buffer es de `BUFF_SIZE`. La cantidad máxima de clientes que pueden conectarse es `MAX_CONNECTIONS`. La cantidad máxima de pares clave valor que se pueden colocar es `DATABASE_SIZE`.

- En `client.cpp`, hay un diccionario de la forma:
  
  `map<int socketID, string status> ClientsMap`
  
  donde socketID es el socket por el cual está conectado al servidor, y status puede ser `connected` si está conectado y no ha mandado una petición al servidor aún, `disconnected` si está desconectado, y `processing` si se está procesando la su solicitud en el servidor (cuando está dentro del ciclo).