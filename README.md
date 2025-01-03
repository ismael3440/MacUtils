
# MacUtils

Este comando fue diseñado para distribuciones de Linux y para trabajar con direcciones MAC en redes locales.

## Instalación 
Clonar el repositorio:
```bash
git clone https://github.com/ismael3440/macutils.git
cd macutils/
```

Compilarlo
```bash
mkdir build/
cmake -G Ninja -B build -S .
ninja -j$(nproc) -C build
```

Compilarlo en Termux
```bash
pkg install ninja
mkdir build/
cmake -G Ninja -B build -S . -DCMAKE_INSTALL_PREFIX=$PREFIX
ninja -j$(nproc) -C build
```

Compilacion en un directorio personalizado
```bash
mkdir build/
cmake -G Ninja -B build -S . -DCMAKE_INSTALL_PREFIX=$HOME/.local
ninja -j$(nproc) -C build
```

## Uso
Salida de ayuda:
```
COMANDO: macutils 1.1; CREADO POR: DarkShark; (C) 2024 DarkShark
Descripción: Este comando fue desarrollado con el  propósito de poder trabajar con direcciones MAC en redes locales

/help                               -> muestra este mensaje
/restore <nueva_mac> <interfaz>     -> cambia la dirección MAC de <interfaz> por <nueva_mac>
         -random                    -> genera una dirección MAC aleatoria
         -copy_of=<interfaz_fuente> -> copia la direcccion mac de la interfaz <interfaz_fuente> en la interfaz <interfaz>
/search <ip_objetivo> <interfaz>    -> busca en la red local <interfaz> la dirección MAC de <ip_objetivo>
```
Ejemplos:
```bash
sudo macutils /restore 1F:2E:3D:4C:5B:6A eth0
```
```bash
sudo macutils /search 192.168.1.1 eth0
```
## 🌟 ¡Apoya el Proyecto!  
Si te ha gustado este proyecto, no olvides dejar tu estrella ⭐ para ayudarnos a seguir mejorándolo.  

## 🗨️ Sugerencias y Dudas  
Si tienes alguna sugerencia o duda, no dudes en abrir un [issue](https://github.com/ismael3440/macutils/issues). Estaremos encantados de leerte y responderte.
