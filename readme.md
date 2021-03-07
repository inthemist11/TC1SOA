## **Tarea 1: Sincronización de "threads"** 
Del curso Sistema Operativos Avanzados, de la Maestría de Ciencias de la Computación del Tecnológico de Costa Rica.

##### Equipo de trabajo:
- Carvajal Barboza Nicole
- Gonzáles Villanueva Ruben
- López Díaz Edisson
- Mena Kikut Otto
- Soto Rojas Cristina


# Uso del programa

#### Compilación
Se utiliza el compilador de GNU, el GCC, usando la siguiente línea:

`gcc main.c -o main -lpthread -lm -w`

- **main.c**: Nombre del archivo principal.
- **-o**: Indicar el nombre del archivo de salida.
- **main**: Nombre del archivo de salida.
- **-lpthread**: Indicar que se usarán *pthreads*.
- **-lm**: ????
- **-lpthread**: Indica que no imprima *warnings*.

#### Ejecución
Se usa el archivo de salida resultante de la compilación "**main**". Para ejecutarlo es suficiente con la siguiente línea:
`./main`

Sin embargo ésto ejecutará el programa con variables estándares que hemos definido, **para indicar cuántos carros vienen del éste y del oeste se usan los parámetros**, con la siguiente línea:

`./main 4 5` 

Donde el primero, el 4, indica la cantidad de carros que quieren ingresar desde el este/derecha, y el segundo, el 5, la cantidad de carros que quieren ingresar desde el oeste/izquierda.

# Opciones extras incluídas
Se incluyeron **dos parámetros extra que son opcionales** para indicar **el *lambda* que se usará para la función exponencial** para cada dirección, este/derecha y oeste/izquierda. Se hace uso de la siguiente manera:

`./main 4 5 6 7` 

Donde el tercero, el 6, indica el *lambda* el este/derecha, y el cuarto, el 7, el *lambda* para el oeste/izquierda.

# No funcional en el programa
Actualmente **todo funciona en el programa y se programó tal como se solicitó en el enunciado**.

<br /><br /><br />



-------------
# Otros
## Ejecuciones del programa
A continuación dos ejecuciones del programa, mostrando el comando de ejecución y capturas de pantalla del resultado.

### Caso 1

