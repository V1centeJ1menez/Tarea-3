#include "tda_registro_cuentas.hpp"

/*****
* registro_cuentas registro_cuentas
******
* Constructor para inicializar la tabla hash
*
* Inicializa:
* factor_de_carga (float) : Inicializado a 0.0, representa el factor de carga de la tabla hash.
* ranuras (int) : Inicializado a 15, representa el número total de ranuras en la tabla hash.
* ocupadas (int) : Inicializado a 0, representa el número de ranuras ocupadas en la tabla hash.
* tabla (cuenta*) : Inicializado con un array dinámico de tamaño 'ranuras', donde cada entrada es una cuenta vacía.
******
* Input:
* Ninguno
******
* Returns:
* Ninguno
*****/
registro_cuentas::registro_cuentas() : factor_de_carga(0.0), ranuras(15), ocupadas(0) {
    tabla = new cuenta[ranuras];
    for (int i = 0; i < ranuras; ++i) {
        tabla[i] = {"", "", ""};
    }
}

/*****
* registro_cuentas ~registro_cuentas
******
* Destructor para liberar la memoria de la tabla hash
*
* Libera la memoria asignada dinámicamente para la tabla hash.
******
* Input:
* Ninguno
******
******
* Returns:
* Ninguno
*****/
registro_cuentas::~registro_cuentas() {
    delete[] tabla;
}


/*****
* int registro_cuentas::hash
******
* Función hash utilizando el método de plegamiento
*
* Divide la cadena de entrada en partes de longitud fija (part_length) y calcula un valor numérico para cada parte.
* Suma estos valores y toma el módulo con el número total de ranuras para obtener el valor hash.
******
* Input:
* const string& rol : Referencia constante a la cadena que representa el rol, utilizado como clave.
******
* Returns:
* int : Valor hash calculado a partir de la cadena rol.
*****/
int registro_cuentas::hash(const string& rol) {
    size_t part_length = 3; // Longitud de cada parte para el plegamiento
    int hash_value = 0;
    for (size_t i = 0; i < rol.length(); i += part_length) {
        int part_value = 0;
        for (size_t j = 0; j < part_length && i + j < rol.length(); ++j) {
            part_value = part_value * 256 + rol[i + j];
        }
        hash_value = (hash_value + part_value) % ranuras;
    }
    return hash_value;
}

/*****
* int registro_cuentas::p
******
* Función de gestión de colisiones utilizando sondeo lineal
*
** Calcula una nueva ranura a revisar en caso de colisión, utilizando sondeo lineal.
* La fórmula utilizada es (hash(rol) + i) % ranuras, donde i es el número de intentos de sondeo.
******
* Input:
* const string& rol : Referencia constante a la cadena que representa el rol, utilizado como clave.
* int i : Número de intentos de sondeo realizados hasta ahora.
******
* Returns:
* int : Índice de la ranura a revisar en la tabla hash.
*****/
int registro_cuentas::p(const string& rol, int i) {
    return (hash(rol) + i) % ranuras;
}

/*****
* cuenta registro_cuentas::obtener
******
* Función para obtener una cuenta dado su rol
*
* Utiliza la función de gestión de colisiones para buscar la cuenta en la tabla hash.
* Recorre las ranuras de la tabla utilizando sondeo lineal.
* Si encuentra una cuenta con el rol dado, la devuelve.
* Si encuentra una ranura vacía, termina la búsqueda y devuelve una cuenta vacía.
******
* Input:
* const string& rol : Referencia constante a la cadena que representa el rol, utilizado como clave.
******
* Returns:
* cuenta : La cuenta que coincide con el rol dado, o una cuenta vacía si no se encuentra.
*****/
cuenta registro_cuentas::obtener(const string& rol) {
    for (int i = 0; i < ranuras; ++i) {
        int pos = p(rol, i);
        if (tabla[pos].rol == rol) {
            return tabla[pos];
        }
        if (tabla[pos].rol == "") {
            break;
        }
    }
    return {"", "", ""};
}

/*****
* void registro_cuentas::agregar
******
* Función para agregar una nueva cuenta a la tabla hash
*
* Utiliza la función de gestión de colisiones para encontrar una ranura vacía.
* Si encuentra una ranura vacía, agrega la cuenta en esa posición.
* Si encuentra una cuenta con el mismo rol, muestra un mensaje indicando que el rol ya existe.
* Actualiza el factor de carga después de agregar la cuenta.
* Si el factor de carga supera 0.75, redimensiona la tabla al doble de su tamaño actual.
******
* Input:
* const cuenta& c : Referencia constante a la cuenta que se va a agregar a la tabla.
******
* Returns:
* void : No retorna ningún valor.
*****/
void registro_cuentas::agregar(const cuenta& c) {
    for (int i = 0; i < ranuras; ++i) {
        int pos = p(c.rol, i);
        if (tabla[pos].rol == "") {
            tabla[pos] = c;
            ocupadas++;
            factor_de_carga = static_cast<float>(ocupadas) / ranuras;
            if (factor_de_carga > 0.75) {
                redimensionar(ranuras * 2);
            }
            return;
        }
        if (tabla[pos].rol == c.rol) {
            cout << "Rol ya existente" << endl;
            return;
        }
    }
    // Si llegamos aquí, significa que la tabla está llena y no pudimos agregar la cuenta
    cout << "Tabla llena, redimensionar no implementado correctamente" << endl;
}

/*****
* void registro_cuentas::eliminar
******
* Función para eliminar una cuenta dado su rol
*
* Utiliza la función de gestión de colisiones para buscar la cuenta en la tabla hash.
* Recorre las ranuras de la tabla utilizando sondeo lineal.
* Si encuentra una cuenta con el rol dado, la elimina y actualiza el factor de carga.
* Si encuentra una ranura vacía sin encontrar el rol, muestra un mensaje indicando que el rol no existe.
******
* Input:
* const string& rol : Referencia constante a la cadena que representa el rol, utilizado como clave.
******
* Returns:
* void : No retorna ningún valor.
*****/
void registro_cuentas::eliminar(const string& rol) {
    for (int i = 0; i < ranuras; ++i) {
        int pos = p(rol, i);
        if (tabla[pos].rol == rol) {
            tabla[pos] = {"", "", ""};
            ocupadas--;
            factor_de_carga = static_cast<float>(ocupadas) / ranuras;
            return;
        }
        if (tabla[pos].rol == "") {
            break;
        }
    }
    cout << "Rol no existente" << endl;
}

/*****
* void registro_cuentas::modificar
******
* Función para modificar la descripción de una cuenta dada su rol
*
* Utiliza la función de gestión de colisiones para buscar la cuenta en la tabla hash.
* Recorre las ranuras de la tabla utilizando sondeo lineal.
* Si encuentra una cuenta con el rol dado, modifica su descripción y termina.
* Si encuentra una ranura vacía sin encontrar el rol, muestra un mensaje indicando que el rol no existe.
******
* Input:
* const string& rol : Referencia constante a la cadena que representa el rol de la cuenta.
* const string& descripcion : Referencia constante a la nueva descripción que se asignará a la cuenta.
******
* Returns:
* void : No retorna ningún valor.
*****/
void registro_cuentas::modificar(const string& rol, const string& descripcion) {
    for (int i = 0; i < ranuras; ++i) {
        int pos = p(rol, i);
        if (tabla[pos].rol == rol) {
            tabla[pos].descripcion = descripcion;
            return;
        }
        if (tabla[pos].rol == "") {
            break;
        }
    }
    cout << "Rol no existente" << endl;
}

/*****
* void registro_cuentas::estadisticas
******
* Función para mostrar las estadísticas de la tabla hash
*
* Muestra en la salida estándar el número de ranuras ocupadas, el número total de ranuras,
* y el factor de carga de la tabla hash.
******
* Input:
* Ninguno
******
* Returns:
* void : No retorna ningún valor.
*****/
void registro_cuentas::estadisticas() {
    cout << "RANURAS OCUPADAS: " << ocupadas << endl;
    cout << "RANURAS TOTALES: " << ranuras << endl;
    cout << "FACTOR DE CARGA: " << factor_de_carga << endl;
}

/*****
* void registro_cuentas::redimensionar
******
* Función para redimensionar la tabla hash
*
* 1. Crea una nueva tabla con el nuevo tamaño.
* 2. Inicializa todas las ranuras de la nueva tabla a vacías.
* 3. Guarda el tamaño y la tabla actual.
* 4. Asigna la nueva tabla y el nuevo tamaño a los atributos de la clase.
* 5. Reasigna las cuentas de la tabla vieja a la nueva tabla utilizando la función agregar.
* 6. Elimina la memoria ocupada por la tabla vieja.
******
* Input:
* int nuevo_tamano : Nuevo tamaño de la tabla hash.
******
* Returns:
* void : No retorna ningún valor.
*****/
void registro_cuentas::redimensionar(int nuevo_tamano) {
    cuenta* nueva_tabla = new cuenta[nuevo_tamano];
    for (int i = 0; i < nuevo_tamano; ++i) {
        nueva_tabla[i] = {"", "", ""};
    }

    int viejo_tamano = ranuras;
    cuenta* vieja_tabla = tabla;
    tabla = nueva_tabla;
    ranuras = nuevo_tamano;
    ocupadas = 0;

    for (int i = 0; i < viejo_tamano; ++i) {
        if (vieja_tabla[i].rol != "") {
            agregar(vieja_tabla[i]);
        }
    }

    delete[] vieja_tabla;
}
