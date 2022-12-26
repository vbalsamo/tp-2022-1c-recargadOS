# tp-2022-1c-recargadOS

Proyecto final de la materia Sistemas Operativos de la UTN.

Consiste de 4 módulos: Consola (N instancias), Kernel, CPU y Memoria (1 instancia cada uno).<br>
El proceso de ejecución del mismo consiste en crear procesos a través de instancias del módulo Consola, las cuales enviarán la información necesaria al módulo Kernel para que el mismo pueda crear las estructuras necesarias a fin de administrar y planificar su ejecución mediante diversos algoritmos. Estos procesos serán ejecutados en el módulo CPU, quien interpretará sus instrucciones y hará las peticiones a Memoria que fuera necesarias. Este último módulo implementará un esquema de memoria virtual (paginación a demanda), por lo que incorporará también un área de SWAP.<br>
Una vez que un proceso finalice tras haber sido ejecutadas todas sus instrucciones, el Kernel devolverá un mensaje de finalización a su Consola correspondiente y cerrará la conexión.
