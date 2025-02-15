# Proyecto Crematoria – Jugador IA en C++

Este proyecto representa el desarrollo de un jugador inteligente para el juego "Crematoria", realizado como parte de mi formación en la UNI. Durante este proyecto, apliqué intensamente los conceptos aprendidos en la teoría de grafos para diseñar algoritmos de movimiento y toma de decisiones complejas en un entorno de juego multifacético.

Toda la documentación del proyecto está en: [Juego EDA](P66622_en.pdf)
Todo el código está en: [Código](./game)
El código de mi jugador es: [Mi Jugador](./game/AISAM.cc)

## Instrucciones para Ejecutar el Programa

1. **Preparar el entorno:**
   - Asegúrate de contar con una versión reciente de **g++**, **make** y un navegador moderno (Firefox o Chrome).

   - Abre una terminal y navega al directorio donde se encuentra el código fuente del proyecto, en el directorio "game".
   
   - Si por ejemplo estás usando Linux, ejecuta:

      ```bash
      cp AIDummy.o.Linux AIDummy.o

      cp Board.o.Linux Board.o
      ```

2. **Compilar el juego:**
   - Ejecuta el comando:
     ```bash
     make all
     ```
     Esto compilará el proyecto y generará el ejecutable llamado `Game`.

3. **Ejecutar una partida:**
   - Inicia una partida utilizando el siguiente comando:
     ```bash
     ./Game SAM Demo Demo Demo -s 30 -i default.cnf -o default.res
     ```
     Donde:
     - `Sambhav` representa mi jugador, 'Demo' son jugadores de demostración.
     - `-s 30` establece la semilla para la generación aleatoria.
     - `-i default.cnf` indica el archivo de configuración del tablero.
     - `-o default.res` define el archivo de salida con el resultado de la partida.

4. **Visualizar la partida:**
   - Abre el archivo `viewer.html` en tu navegador.
   - Carga el archivo `default.res` para ver la animación de la partida.

5. **Opciones adicionales:**
   - Para conocer todas las opciones disponibles, ejecuta:
     ```bash
     ./Game --help
     ```
   - Si necesitas limpiar la compilación y recompilar desde cero, utiliza:
     ```bash
     make clean
     ```