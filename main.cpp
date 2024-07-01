#include <iostream>
#include <fstream>
#include "tda_registro_cuentas.hpp"

using namespace std;

int main() {

    registro_cuentas registro;
    ifstream infile("./pruebas/prueba_6.txt");
    string line, command;

    while (getline(infile, line)) {
        // Limpiamos cualquier espacio en blanco extra al inicio de la línea
        size_t pos = line.find_first_not_of(" \t");
        if (pos != string::npos) {
            line = line.substr(pos);
        }

        // Encontramos el comando y los argumentos
        pos = line.find(' ');
        if (pos != string::npos) {
            command = line.substr(0, pos);
            line = line.substr(pos + 1);
        } else {
            command = line;
            line = "";
        }

        // Procesamos el comando y los argumentos
        if (command == "AGREGAR") {
            cuenta c;
            pos = line.find(' ');
            if (pos != string::npos) {
                c.rol = line.substr(0, pos);
                line = line.substr(pos + 1);
            } else {
                c.rol = line;
                line = "";
            }

            pos = line.find(' ');
            if (pos != string::npos) {
                c.nombre = line.substr(0, pos);
                line = line.substr(pos + 1);
            } else {
                c.nombre = line;
                line = "";
            }

            c.descripcion = line;
            registro.agregar(c);

        } else if (command == "QUITAR") {
            size_t pos = line.find(' ');
            string rol = (pos == string::npos) ? line : line.substr(0, pos);
            registro.eliminar(rol);

        } else if (command == "MODIFICAR") {
            size_t pos = line.find(' ');
            string rol = (pos == string::npos) ? line : line.substr(0, pos);
            string descripcion = (pos == string::npos) ? "" : line.substr(pos + 1);
            registro.modificar(rol, descripcion);

        } else if (command == "OBTENER") {
            string rol = line;
            cuenta c = registro.obtener(rol);
            if (c.rol == "") {
                cout << "Rol no existente" << endl;
            } else {
                cout << c.nombre << " " << c.descripcion << endl;
            }

        } else if (command == "ESTADISTICAS") {
            registro.estadisticas();
        }
    }

    return 0;
}
