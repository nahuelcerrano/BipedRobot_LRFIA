# BipedRobot LRFIA

Al usarse un el controlador **PCA9685** con **16 canales**, el conceso de la definiciones de los pines será:
**Los servos de la derecha tendrá los canales pares
Los servos de la izquierda tendrá los canales impares**

Respetando que los pines desde el numero mas chico hasta el mas grande
Asi representando que va desde el pie hasta la cadera (de abajo hacia arriba)

## *Mapeo de canales*

| ***Servos*** | ***Canales*** | ***Columnas*** |
| --- | --- | --- |
| servoPieDer | Canal 0 | Columna x |
| servoPieIzq | Canal 1 | Columna x |
| servoRodillaDer | Canal 2 | Columna x |
| servoRodillaIzq | Canal 3 | Columna x |
| servoCaderaDer | Canal 4 | Columna x |
| servoCaderaIzq  | Canal 5 | Columna x |

Como medida de seguridad para la conversion de los grados a pulsos se establecio

<aside>
💡

**0 grados = 150 ms
180 grados = 450 ms**

</aside>
