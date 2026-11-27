# Análisis de tablas de dispersión

* *Matías Camacho Hoyo*
* *Jose Rivas Ceacero*

1: Exploración Cuadrática (Hash 1)
2: Dispersión Doble 1 - Fibonacci (Hash 2)
3: Dispersión Doble 2 - MurmurHash (Hash 3)

## Tamaño de tabla: 206879
| función | máximo de colisiones | supera 10 colisiones | factor de carga | promedio de colisiones |
|---------|:--------------------:|:--------------------:|:---------------:|:----------------------:|
| Hash 1  |          206         |         33259        |       0.65      |         21.6131        |
| Hash 2  |          254         |          168         |       0.65      |         0.597978       |
| Hash 3  |          21          |          176         |       0.65      |         0.636360       |

## Tamaño de tabla: 197711
| función | máximo de colisiones | supera 10 colisiones | factor de carga | promedio de colisiones |
|---------|:--------------------:|:--------------------:|:---------------:|:----------------------:|
| Hash 1  |          240         |         32125        |       0.68      |         21.0285        |
| Hash 2  |          40          |          209         |       0.68      |         0.607098       |
| Hash 3  |          24          |          234         |       0.68      |         0.698954       |

## Justificación de la configuración elegida
Hemos optado por un **factor de carga $\lambda = 0.65$ (tamaño primo 206.879)** junto con la **Dispersión Doble 1 (Hashing de Fibonacci - Hash 2)**.
Al analizar los datos obtenidos con los 134.438 árboles reales, se observa que la exploración cuadrática sufre un fuerte agrupamiento primario (*clustering*), disparando el promedio a más de 21 colisiones por inserción y provocando que más de 32.000 elementos superen las 10 colisiones. Por el contrario, las funciones de dispersión doble rompen los patrones secuenciales de los identificadores (`assetnum`), reduciendo el promedio por debajo de 1 colisión. Entre ambas, la Dispersión Doble 1 con factor de carga 0.65 ofrece el **menor promedio de colisiones global (0.598)** y la **menor cantidad de inserciones críticas por encima de 10 colisiones (168)**, garantizando un acceso prácticamente en tiempo constante $O(1)$.

## Comparación de tiempos dados en milisegundos

Tiempo en realizar las búsquedas en la Tabla Hash: 0.0932 ms.

Tiempo en realizar las búsquedas en la Lista: 3351.28 ms.