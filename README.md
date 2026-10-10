# BipedRobot LRFIA

Al usarse un el controlador **PCA9685** con **16 canales**, el conceso de la definiciones de los pines será:
**Los servos de la derecha tendrá los canales pares
Los servos de la izquierda tendrá los canales impares**

Respetando que los pines desde el numero mas chico hasta el mas grande
Asi representando que va desde el pie hasta la cadera (de abajo hacia arriba)

## *Mapeo de canales*

| ***Servos*** | ***Canales*** | ***Columnas*** |
| --- | --- | --- |
| servoPieDer | Canal 0 | Columna 4 |
| servoPieIzq | Canal 2 | Columna 2 |
| servoRodillaDer | Canal 4 | Columna 5 |
| servoRodillaIzq | Canal 1 | Columna 3 |
| servoCaderaDer | Canal 3 | Columna 1 |
| servoCaderaIzq  | Canal 5 | Columna 6 |

Como medida de seguridad para la conversion de los grados a pulsos se establecio

<aside>
💡

**0 grados = 150 ms,
180 grados = 450 ms**

</aside>
