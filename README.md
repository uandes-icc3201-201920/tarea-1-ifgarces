# Informe - Tarea 1-2
Ignacio F. Garcés Santander
(Se me olvidó que era en parejas, me acordé el último día. Favor no bajarme puntaje por eso.)


### 1. DIFICULTADES ENCONTRADAS
- Evitar inanición y condición de carrera.
- Lidiar con C/C++ y sus charasteriscos, sus char[] y sus segmentation faults que no dicen en qué linea está el problema.


### 2. FUNCIONES NO IMPLEMENTADAS
- Temporizador para que el servidor indique error si un cliente no se ha conectado en cierto tiempo.
- Hay que hacer make [clean] de nuevo para volver a ejecutar los programas, ya que no hice la función que permite conectarse al socket aunque este esté creado, por lo que debe borrarse a mano para poder establecer otro igual.


### 3. PROBLEMAS CONOCIDOS
- Por algún motivo, si se usan las flechas direccionales en la consola al escribir el input del cliente, el servidor enloquece un poco. Es decir, si trato de escribir los paréntesis () primero, luego devolverme con la flecha para rellenar los parámetros del comando que esté escribiendo, ocurren problemas en el strtok() y por lo tanto da un resultado inesperado. Aunque puede que no sea problema de mi código.
- Solo se reconocen los comandos en miníscula, y sin espacios. Por ejemplo, no se reconoce "INSERT(0,hola)" ni "insert (0,hola)". O quizás sí pero cosas raras pasarían.


### 4. FUNCIONAMIENTO (EXTRA)
El cliente le envía las instrucciones al buffer (char) en el formato siguiente:

"<nombre_instrucción>:<argumento1>:<argumento2>:<threadID_cliente_que_mandó_petición>"

donde los argumentos son los parámetros que requiera esa función. Si no requiere alguno, se vuelve "null". Entonces, el servidor reconoce lo que el cliente quiere haciendo split (strtok) con ":". Luego, la respuesta del servidor se imprime en el cliente en color amarillo.

El tamaño máximo (número de caracteres) del buffer es de BUFF_SIZE. La cantidad máxima de clientes que pueden conectarse es MAX_CONNECTIONS. La cantidad máxima de pares clave valor que se pueden colocar es DATABASE_SIZE.