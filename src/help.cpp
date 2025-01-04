#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#include "commands.h"

int help(std::vector<std::string> args) {
    std::cout
<< "COMANDO: " << args[0] << " 1.1; CREADO POR: DarkShark; (C) 2024 DarkShark" << std::endl
<< "Descripción: Este comando fue desarrollado con el  propósito de poder trabajar con direcciones MAC en redes locales" << std::endl
<< std::endl
<< "/help                                -> muestra este mensaje" << std::endl
<< "/change <nueva_mac> <interfaz>       -> cambia la dirección MAC de <interfaz> por <nueva_mac>" << std::endl
<< "         -random                     -> genera una dirección MAC aleatoria" << std::endl
<< "         -copy_of=<interfaz_fuente>  -> copia la direcccion mac de la interfaz <interfaz_fuente> en la interfaz <interfaz>" << std::endl
<< "/resolve <ip_objetivo> <interfaz>    -> busca en la red local <interfaz> la dirección MAC de <ip_objetivo>" << std::endl;
    return 0;
}
