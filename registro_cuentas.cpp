#include "registro_cuentas.hpp"

// Constructor para inicializar la tabla hash
registro_cuentas::registro_cuentas() : factor_de_carga(0.0), ranuras(15), ocupadas(0) {
    tabla = new cuenta[ranuras];
    for (int i = 0; i < ranuras; ++i) {
        tabla[i] = {"", "", ""};
    }
}

// Destructor para liberar memoria
registro_cuentas::~registro_cuentas() {
    delete[] tabla;
}

// Función hash utilizando el método de plegamiento
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

// Función de gestión de colisiones utilizando sondeo lineal
int registro_cuentas::p(const string& rol, int i) {
    return (hash(rol) + i) % ranuras;
}

// Función para obtener una cuenta dado su rol
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

// Función para agregar una nueva cuenta a la tabla hash
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

// Función para eliminar una cuenta dado su rol
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

// Función para modificar la descripción de una cuenta dada su rol
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

// Función para mostrar las estadísticas de la tabla hash
void registro_cuentas::estadisticas() {
    cout << "RANURAS OCUPADAS: " << ocupadas << endl;
    cout << "RANURAS TOTALES: " << ranuras << endl;
    cout << "FACTOR DE CARGA: " << factor_de_carga << endl;
}

// Función para redimensionar la tabla hash
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
