#ifndef REGISTRO_CUENTAS_H
#define REGISTRO_CUENTAS_H

#include <iostream>
#include <string>

using namespace std;

struct cuenta {
    string rol, nombre, descripcion;
};

class registro_cuentas {
private:
    float factor_de_carga = 0.0;
    cuenta* tabla;
    int ranuras = 15;
    int ocupadas = 0;

    int hash(const string& rol); // Se obtiene el hash dado el rol utilizando el método de plegamiento
    int p(const string& rol, int i); // Se obtiene la ranura a revisar en caso de colisión dado el rol y el intento i

public:
    registro_cuentas(); // Constructor para inicializar la tabla hash
    ~registro_cuentas(); // Destructor para liberar la memoria de la tabla hash

    cuenta obtener(const string& rol); // Dado el rol, devuelve la cuenta con ese rol
    void agregar(const cuenta& c); // Se agrega una cuenta a la tabla
    void modificar(const string& rol, const string& descripcion); // Se modifica la descripcion del rol
    void eliminar(const string& rol); // Se elimina la cuenta
    void redimensionar(int n); // Se redimensiona la tabla a n espacios
    void estadisticas(); // Muestra las estadísticas de la tabla hash
    
};

#endif // REGISTRO_CUENTAS_H
