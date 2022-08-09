#include <iostream>
#include <fstream>
#include <iomanip>
#include "listascomp.hpp"
#include "listasordc.hpp"
#include "rwstring.hpp"

// TP3 de Luca Camio K1123

using namespace std;

struct Operacion
{
    string accion;
    int plazo;
    string bolsa;
    float precio = 0;
    int cantidad = 0;
};

struct Accion
{
    string accion;
    int cantidad;
};

struct Bolsa
{
    string bolsa;
    float precio = 0;
    Nodo<Accion>* list = nullptr;
};

struct Plazo
{
    int plazo;
    int cant = 0;
    Nodo<Bolsa>* lista = nullptr;
};

const int larAccion = 12;
const int larBolsa = 14;

fstream& operator >> (fstream& fs,Operacion& op)
{
    op.accion = readstring(fs,larAccion);
    fs.read((char*) &op.plazo, sizeof(op.plazo));
    op.bolsa = readstring(fs,larBolsa);
    fs.read((char*) &op.precio, sizeof(op.precio));
    fs.read((char*) &op.cantidad, sizeof(op.cantidad));
    return fs;
}

ostream& operator << (ostream& os, const Operacion& op)
{
    os << op.accion << "\t\t" << op.plazo << "\t" << op.bolsa << "\t" << op.precio << "\t" << op.cantidad << endl;
	return os;
}

int criterioPlazo(Plazo a,Plazo b)
{
    return a.plazo - b.plazo;
}

int criterioBolsa(Bolsa a,Bolsa b)
{
    return a.bolsa.compare(b.bolsa);
}





int main()
{
    fstream arch;
    Operacion op;
    Accion acc;
    Bolsa bol;
    Plazo pl;

    Nodo<Plazo>* listaPlazo = nullptr;
    Nodo<Plazo>* extPlazo = nullptr;
    Nodo<Bolsa>* extBolsa = nullptr;

    arch.open("Datos.bin",ios:: in | ios::binary);
    if(!arch)
    {
        cout << "No se pudo abrir el archivo de lectura" << endl;
        return EXIT_FAILURE;
    }
    cout << "Leyendo archivo " << endl << endl;

    while(arch>>op)
    {
        cout << op;
        pl.plazo = op.plazo;
        bol.bolsa = op.bolsa;
        acc.accion = op.accion;
        acc.cantidad = op.cantidad;

        extPlazo = insertar_unico(pl,listaPlazo,criterioPlazo);
        extBolsa = insertar_unico(bol,extPlazo->dato.lista,criterioBolsa);
        agregar(extBolsa->dato.list,acc);

        extPlazo->dato.cant++;
        extBolsa->dato.precio += op.precio*op.cantidad;
    }
    arch.close();

    while(listaPlazo)
    {
        if(listaPlazo->dato.plazo == 0)
        {
            cout << "Plazo: CI, cantidad de operaciones: " << listaPlazo->dato.cant << endl;
        }
        else
        {
            if(listaPlazo->dato.plazo == 1)
            {
                cout << "Plazo: 24Hs, cantidad de operaciones: " << listaPlazo->dato.cant << endl;
            }
            else
            {
                if(listaPlazo->dato.plazo == 2)
                {
                    cout << "Plazo: 48Hs, cantidad de operaciones: " << listaPlazo->dato.cant << endl;
                }
                else
                {
                   cout << "Plazo: 72Hs, cantidad de operaciones: " << listaPlazo->dato.cant << endl; 
                }                
            }
        }       
            while(listaPlazo->dato.lista)
            {
                cout << " Bolsa  Monto" << endl;
                cout << listaPlazo->dato.lista->dato.bolsa << " " << listaPlazo->dato.lista->dato.precio << endl;
                cout << endl << " Accion Cant" << endl << " -------------------------------" << endl;
                while(listaPlazo->dato.lista->dato.list)
                {
                    cout << listaPlazo->dato.lista->dato.list->dato.accion << " " << listaPlazo->dato.lista->dato.list->dato.cantidad << endl;
                    listaPlazo->dato.lista->dato.list = listaPlazo->dato.lista->dato.list->sig;
                }
                listaPlazo->dato.lista = listaPlazo->dato.lista->sig;
            }
            listaPlazo = listaPlazo->sig;
    }

}
