#include <iostream>
#include <fstream>
#include <iomanip>
#include "listas.hpp"
#include "listasordc.hpp"
#include "rwstring.hpp"

using namespace std;

struct Pedido
{
    int id;
    string cliente;
    string producto;
    float valor;
};

struct Despacho
{
    string cliente;
    int bultos= 0;
    float valor= 0;
};
const int lclie = 12;
const int lprod = 10;

fstream& operator >> (fstream &fs, Pedido& ped)
{
	fs.read((char*) &ped.id, sizeof(ped.id));
	ped.cliente = readstring(fs,lclie);
	ped.producto = readstring(fs,lprod);
	fs.read((char*) &ped.valor, sizeof(ped.valor));
	return fs;
}

ostream& operator << (ostream &os, const Pedido& ped)
{
	os <<left<< setw(8)<< ped.id << setw(lclie) << ped.cliente << setw(lprod) << ped.producto << "\t" << ped.valor << "\t" << endl;
	return os;
}

ostream& operator << (ostream &os, const Despacho& des)
{
	os <<setw(lclie)<< des.cliente << setw(12) << des.bultos << setw(6) << des.valor;
	return os;
}

int critPed (Pedido a, Pedido b)
{
    if (a.cliente == b.cliente)
            return a.producto.compare(b.producto);
    else
        return a.cliente > b.cliente ? 1 : -1;
}

int critCliProd(Pedido a, Pedido b)
{
    if(a.cliente == b.cliente)
	{
		return a.producto.compare(b.producto);
	}
	return a.cliente > b.cliente ? 1 : -1 ;
}
void despachar(Nodo<Pedido>*& listaOrd, Nodo<Despacho>*& listaDesp )  
{                      
    
    Nodo<Pedido>* ext;
    Pedido ped;
    Despacho des;
    bool enable = true; 
  
    while(listaOrd)
    {   
        cout << endl << "Ingrese nombre del cliente y producto a despachar (Si quiere salir escriba SALIR):" << endl;
        cin >> ped.cliente;
        if(ped.cliente == "SALIR"  )
        {
            break;
        }
        cin >> ped.producto;
        if(ped.producto == "SALIR")
        {
            break;
        }
        ext = extraer(ped,listaOrd,critCliProd);
            while(ext != nullptr)
            {   
                enable = false;
                des.cliente = ext->dato.cliente;
                des.bultos++;    
                des.valor += ext->dato.valor;
                cout << ext->dato << endl; 
                ext = extraer(ped,listaOrd,critCliProd);
            }
            if (enable)
            {
                cout << endl << "No se encontro pedido de la combinacion " << ped.cliente << "/" << ped.producto << endl;
            }
            else
            {
            cout << endl << " ---------- Pedidos encontrados de la combinacion " << ped.cliente << "/" << ped.producto << " ----------" << endl;
            push(listaDesp,des);
            enable = true;
            }
    }
            if(!listaOrd)
            {
                cout<<endl << "La lista se encuentra vacia, por favor vuelva a leer los datos " << endl;
            }
}


int main()
{
    fstream arch;
    Nodo<Pedido>* listaOrd = nullptr;
    Nodo<Despacho>* listaDesp = nullptr;
    Pedido ped;
    Despacho des;
    int opcion;

    do{
		cout<<" MENU -INGRESE UNA OPCION- \n" << endl <<"1. Leer datos"<< endl <<"2. Mostrar "<<endl <<
		"3. Despachar"<< endl << "4.Salir: "<< endl << "Opcion: " << endl;
		cin>>opcion;
		switch(opcion)
        {

		    case 1:
            // system("cls");
            while(listaOrd)
            {
                pop(listaOrd);
            }
            arch.open("Datos.bin", ios:: in | ios::binary);
            if(!arch)
            {
                cout<< "No se pudo abrir el archivo de lectura" << endl;
                return EXIT_FAILURE;    
            }
				cout<< "ID      Cliente     Producto    Valor" << endl;
                cout<< "-------------------------------------" << endl;
                while(arch >> ped)
                {
				    cout << ped;
                    insertar(ped, listaOrd, critPed);
                }
                arch.close();
                // system("pause");
                // system("cls");
			    break;

			case 2: cout<<"Pedidos: " << endl;

                cout<< "ID      Cliente     Producto    Valor" << endl;
                cout<< "-------------------------------------" << endl;
                mostrar( listaOrd);
                cout << endl << "Pedidos despachados:" << endl;
                cout<< "Cliente     Bultos   Valor" << endl;
                cout<< "-------------------------------------" << endl;
                mostrar(listaDesp);
                // system("pause");
                // system("cls");			
			break;
			case 3: 
                despachar(*& listaOrd, *&listaDesp);


            break;
            default:
                    if(opcion < 1 || opcion > 4){
                        // system("cls");
                        cout<< endl;
                        cout<<endl << " El numero que ha ingresado no se encuentra dentro de las opciones del MENU " << endl;
                        // system("pause");
                        // system("cls");
                    }
            break;
        }
        
                    
    }while(opcion!=4);
    
    while(listaDesp)
    {
        pop(listaDesp);
    }
    while (listaOrd)
    {
        pop(listaOrd);
    }
    
    return 0;
}
