# Informe - Tarea 1-1
## Ignacio F. Garcés Santander

### 1. DIFICULTADES ENCONTRADAS
- C es complicado.
- Como el buffer trabaja en forma de bits, es complicado enviar más de un número a la vez. Hay que "serializar", traté de hacerlo.
- Que el servidor sepa qué operación quiere hacer el cliente con los datos que le pasa (Relacionado con lo anterior). Por eso, traté de usar la variable clientCMDid. Otra alternativa era tener dos buffers, pero mejor no.
- El tema del temporizador. De alguna forma tenía que interrumpirse el estado de waiting del cliente cuando pasaban 10 segundos sin conectarse al servidor. Traté de hacerlo con un nuevo programa timer.cpp, pero no se me ocurrió cómo forzar a que cliente muera luego de dicho tiempo.

### 2. FUNCIONES NO IMPLEMENTADAS
- El temporizador.
- El tema de crear el socket en un directorio dado por el cliente o servidor (flag -s).
- 

### 3. PROBLEMAS CONOCIDOS
- Que había que eliminar a cada rato el socket porque sino, no podían crear el socket para establecer la comuncación, pues ya estaba creado.