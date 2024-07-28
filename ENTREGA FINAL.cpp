#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct fecha{
	int dia;
	int mes;
	int anio;
};

struct MatXOpc {
    int idOpcion;
    int idMaterial;
    int cantidad;
    struct MatXOpc *sgte;
};

struct Material {
    int CodMat;
    char denominacion[50];
    int CostoMat;
    int StockMat;
    struct Material* izquierda;
    struct Material* derecha;
};

struct opciones {
    int cod_opcion;
    char nombre[45];
    float costo_MO;
    int cant_ventas;
    struct opciones *sgte;
};

struct tiempo {
    int hora, minuto;
};

struct tareas {
    int cod_opcion, cod_tarea, orden;
    char nom_tarea[45];
    struct tiempo estimado;
    struct tareas *sgte, *ant;
};

struct Trabajos_Pendientes {
    int idTrabajo;
    int Altura; // 1 si es en altura, 0 si no es en altura
    int idOpcion;
    int idCliente;
    int idTecnico;
    int pendiente; // 1 si el trabajo está pendiente, 0 si está terminado
    char Ubicacion[50];
    struct fecha fech;
    float MonTotal; 
    struct Trabajos_Pendientes *sgte;
};


struct clientes {
    int codCliente;
    char nombre[50];
    int dni;
    struct clientes *sgte;
};
struct tecnicos {
    int codTecnico;
    char nombre[50];
    int dni;
    struct tecnicos *sgte;
};


bool fechaValida(int dia, int mes, int anio);
int convertirFechaADias(int dia, int mes, int anio);
const char* obtener_nombre_cliente(int id_cliente, struct clientes *ini_clientes);
const char* obtener_nombre_opcion(int id_opcion, struct opciones *ini_opciones);
//Modulos de clientes
void insertarPila(struct clientes **pila, struct clientes *nuevoNodo);
void insertarClientes(struct clientes **pila);
void imprimirPila(struct clientes *pila);
void agregar_cliente(struct clientes *inicio);
void verificar_cliente(int id, struct clientes *ini);

//Modulos de tecnicos
void insertarLista(struct tecnicos **lista, struct tecnicos *nuevoNodo);
void insertarTecnicos(struct tecnicos **lista);
void imprimirlista(struct tecnicos *lista);
void agregar_tecnico(struct tecnicos *inicio);
void verificar_tecnico(int id, struct tecnicos *ini);

//Modulos de Trabajos pendientes
void Carga_Trabajo(struct Trabajos_Pendientes **Entrada, struct Trabajos_Pendientes **Salida, struct MatXOpc *ini_matxopc, struct Material *ini_mat, struct opciones *ini_opciones, struct clientes *ini_clientes, struct tecnicos *ini_tecnicos);
void Guardar_Cola_En_Archivo(struct Trabajos_Pendientes *Entrada, const char *nombreArchivo);
void Cargar_Desde_Archivo(struct Trabajos_Pendientes **Entrada, struct Trabajos_Pendientes **Salida, const char *nombreArchivo);
void Mostrar_Trabajos(struct Trabajos_Pendientes *Entrada);
float buscartotal(int idopcion, struct MatXOpc *, struct Material *, struct opciones *);

//Modulos de Trabajos finalizados
void Cargar_Trabajosfina(struct Trabajos_Pendientes **entrada,struct Trabajos_Pendientes **salida, const char *nombreArchivo);
void Alta_Trabajos(struct Trabajos_Pendientes **cola1, struct Trabajos_Pendientes **cola2);
void guardar_cola_finalizado(struct Trabajos_Pendientes *E, const char *archivo);
void listarentrefechas_fina(struct Trabajos_Pendientes *E2, struct opciones *opc);
void buscarnombre(char *nombre, int id, struct opciones *prin);
void contar_ventas_entre_fechas(struct Trabajos_Pendientes *E2, struct opciones *opciones, float fecha_inicio, float fecha_fin);
void listar_mas_vendi(struct opciones *opciones);
void listaropcvendi(struct Trabajos_Pendientes *E2, struct opciones *ini);
void contador(struct opciones *raiz_opcion);
void listar_trabajos_tecnico(struct Trabajos_Pendientes *E, struct Trabajos_Pendientes *E2, struct opciones *ini_opciones, struct clientes *ini_clientes);

//Modulos de opciones:
void calcular_tiempo(int, struct tareas *);
float buscarprecio(int, struct MatXOpc *, struct Material *);
void insertar_opcion(struct opciones **, struct opciones **);
struct opciones *buscar_anterior(int, struct opciones *);
void recorrer_opciones(struct opciones *, struct MatXOpc *, struct Material *, struct tareas *);
void guardar_opciones_en_archivo(struct opciones *, const char *);   //FACU FIJATE ESTE
void cargar_opciones_desde_archivo(struct opciones **, const char *); //Y ESTE TAMBIEN
void agregar_opcion(struct opciones **);


//Modulos de tareas:

void insertar_tareas(struct tareas **, struct tareas **);
struct tareas *buscar_tareas_anterior(int, int,int, int, struct tareas *);
void recorrer_tareas(struct tareas *);
void guardar_tareas_en_archivo(struct tareas *, const char *);
void cargar_tareas_desde_archivo(struct tareas **, const char *);
void agregar_tarea(struct tareas **);
struct tareas *buscar_tarea(int, int, struct tareas *);


//Modulos de Materiales

void nuevoNodoM(struct Material** raizM);
struct Material* insertarMaterial(struct Material* raizM, struct Material* nodoM);
float busquedaMaterial(struct Material* raiz, int mat);
void guardarMaterialesEnArchivo(struct Material* raiz, const char* nombreArchivo);
void guardarMaterialEnArchivo_auxiliar(FILE* archivo, struct Material* raiz);
void cargarMaterialesDesdeArchivo(struct Material** raiz, const char* nombreArchivo);
void recorrer(struct Material* raiz);

//MODULOS MATERIALES X OPC

struct MatXOpc *CargaMatXOpc(struct MatXOpc *inicio, struct MatXOpc *nv);
void agregarMaterialXOpcion(struct MatXOpc **ini);
void guardar_MaterialXOpcion_en_archivo(struct MatXOpc *, const char *);
void cargar_MaterialXOpcion_desde_archivo(struct MatXOpc **, const char *);


int main() {
	//Cargar opciones a partir del txt
	 struct opciones *raiz_opcion = NULL;
    cargar_opciones_desde_archivo(&raiz_opcion, "opciones.txt"); // ACTUALIZA LA LISTA A PARTIR DEL TXT
	//Cargar clientes desde archivo
	struct clientes *pila = NULL;
    insertarClientes(&pila);
    
    //Cargar tecnicos desde archivo
    struct tecnicos *lista = NULL;
    insertarTecnicos(&lista);
    
	//Cargar trabajos pendientes desde archivo
	struct Trabajos_Pendientes *E = NULL;
    struct Trabajos_Pendientes *S = NULL;
     Cargar_Desde_Archivo(&E, &S, "trabajos_pendientes.txt");
    
    //Cargar trabajos finalizados desde archivo 
    struct Trabajos_Pendientes *E2 = NULL;
	struct Trabajos_Pendientes *S2 = NULL;
    Cargar_Desde_Archivo(&E2, &S2, "trabajos_finalizados.txt");

    // Cargar materiales por opcion desde archivo
    struct MatXOpc *inicio_MaterialesPorOpcion = NULL;  
    cargar_MaterialXOpcion_desde_archivo(&inicio_MaterialesPorOpcion, "MaterialesPorOpcion.txt");

    struct Material* raizM = NULL;
    // Cargar materiales desde el archivo
    cargarMaterialesDesdeArchivo(&raizM, "material.txt");
    
     // ACTUALIZA LAS ESTRUCTURAS TAREAS CON EL TXT
    struct tareas *raiz_tareas = NULL;
    cargar_tareas_desde_archivo(&raiz_tareas, "tareas.txt"); 
    

    int opcion_o_tarea, eleccion;

    do {
       	system("cls");
	   	printf("----------BIENVENIDO------------\n");
	    printf("INGRESE 1 PARA PANEL DE OPCIONES\n");
        printf("INGRESE 2 PARA PANEL DE TAREAS\n");
        printf("INGRESE 3 PARA PANEL DE MATERIALES\n");
        printf("INGRESE 4 PARA PANEL DE MATERIALES POR OPCION\n");
        printf("INGRESE 5 PARA PANEL DE TRABAJOS\n");
        printf("INGRESE 6 PARA LISTAR LOS CLIENTES\n");
        printf("INGRESE 7 PARA LISTAR LOS TECNICOS\n");
        printf("INGRESE 8 PARA LISTAR TRABAJOS DE UN TECNICO\n");
        printf("INGRESE 0 PARA FINALIZAR\n");
        printf("ELECCION: ");
        scanf("%d", &opcion_o_tarea);
        system("cls");
        
        switch (opcion_o_tarea) {
            case 0:
                printf("FIN DE EJECUCION,HASTA LUEGO\n");
                break;
            case 1: {
                do {
                    printf("Menu:\n");
                    printf("1. Agregar nueva opcion\n");
                    printf("2. Modificar opcion existente\n");
                    printf("3. Mostrar todas las opciones\n");
                    printf("4. Salir\n");
                    printf("Ingrese su eleccion: ");
                    scanf("%d", &eleccion);
                    
                    getchar(); // Limpiar buffer de entrada
                    switch (eleccion) {
                        case 1:
                            agregar_opcion(&raiz_opcion);
                            break;
                        case 2: {
                            int codigo;
                            printf("Ingrese el codigo de la opcion a modificar: ");
                            scanf("%d", &codigo);
                            getchar(); // Limpiar buffer de entrada
                            struct opciones *opcion = buscar_anterior(codigo, raiz_opcion);
                            if (opcion->cod_opcion== codigo) {
                                printf("Ingrese el nuevo nombre de la opcion: ");
                                fgets(opcion->nombre, sizeof(opcion->nombre), stdin);
                                opcion->nombre[strcspn(opcion->nombre, "\n")] = 0; // Eliminar salto de línea
                                printf("Ingrese el nuevo costo de la mano de obra de la opcion: ");
                                scanf("%f", &opcion->costo_MO);
                            } else {
                                printf("Opcion no encontrada.\n");
                            }
                            break;
                        }
                        case 3:
                            recorrer_opciones(raiz_opcion, inicio_MaterialesPorOpcion, raizM, raiz_tareas);
                            break;
                        case 4:
                            guardar_opciones_en_archivo(raiz_opcion, "opciones.txt"); // Guarda las opciones en el archivo antes de salir
                            break;
                        default:
                            printf("Opcion invalida.\n");
                            break;
                    }
                } while (eleccion != 4);
                break;
            }
            case 2: {
                do {
                    printf("Menu:\n");
                    printf("1. Agregar nueva tarea\n");
                    printf("2. Modificar tarea existente\n");
                    printf("3. Mostrar todas las tareas\n");
                    printf("4. Salir\n");
                    printf("Ingrese su eleccion: ");
                    scanf("%d", &eleccion);
                    
                    getchar(); // Limpiar buffer de entrada
                    switch (eleccion) {
                        case 1:
                            agregar_tarea(&raiz_tareas);
                            break;
                        case 2: {
                            int cod_opcion, cod_tarea;
                            printf("Ingrese el codigo de opcion de la tarea a modificar: ");
                            scanf("%d", &cod_opcion);
                            printf("Ingrese el codigo de la tarea a modificar: ");
                            scanf("%d", &cod_tarea);
                            getchar(); // Limpiar buffer de entrada
                            struct tareas *tarea = buscar_tarea(cod_opcion, cod_tarea, raiz_tareas);
                            if (tarea) {
                                printf("Ingrese el nuevo nombre de la tarea: ");
                                fgets(tarea->nom_tarea, sizeof(tarea->nom_tarea), stdin);
                                tarea->nom_tarea[strcspn(tarea->nom_tarea, "\n")] = 0; // Eliminar salto de línea
                                printf("Ingrese el nuevo orden de la tarea: ");
                                scanf("%d", &tarea->orden);
                                printf("Ingrese el nuevo tiempo estimado (hora minuto): ");
                                scanf("%d %d", &tarea->estimado.hora, &tarea->estimado.minuto);
                            } else {
                                printf("Tarea no encontrada.\n");
                            }
                            break;
                        }
                        case 3:
                            recorrer_tareas(raiz_tareas);
                            break;
                        case 4:
                            // Guardar las tareas en el archivo antes de salir
                            guardar_tareas_en_archivo(raiz_tareas, "tareas.txt");
                            break;
                        default:
                            printf("Opcion invalida.\n");
                            break;
                    }
                } while (eleccion != 4);
                break;
            }
            case 3: {
                do {
                    printf("1. Agregar nuevo material\n");
                    printf("2. Mostrar todos los materiales\n");
                    printf("3. Salir\n");
                    printf("Ingrese su eleccion: ");
                    scanf("%d", &eleccion);
                    
                    getchar(); // Limpiar buffer de entrada
                    switch (eleccion) {
                        case 1:
                            nuevoNodoM(&raizM);
                            break;
                        case 2:
                            recorrer(raizM);
                            break;
                        case 3:
                            // Guardar los materiales en el archivo antes de salir
                            guardarMaterialesEnArchivo(raizM, "material.txt");
                            break;
                        default:
                            printf("Opcion invalida.\n");
                            break;
                    }
                } while (eleccion != 3);
                break;
            }
            case 4: {
                do {
                    printf("1. Agregar nuevo Material a una Opcion\n");
                    printf("2. Volver\n");
                    printf("Ingrese su eleccion: ");
                    scanf("%d", &eleccion);
                    getchar(); // Limpiar el buffer
                    
                    switch (eleccion) {
                        case 1:
                            agregarMaterialXOpcion(&inicio_MaterialesPorOpcion);
                            break;
                        case 2:
                            guardar_MaterialXOpcion_en_archivo(inicio_MaterialesPorOpcion, "MaterialesPorOpcion.txt");
                            break;
                        default:
                            printf("Opcion invalida.\n");
                            break;
                    }
                } while (eleccion != 2);
                break;
            }
            
            case 5:{
			
			
				int opcion;
					            	
						do {
					        printf("\n--- Menu ---\n");
					        printf("1. Cargar trabajo\n");
					        printf("2. Mostrar trabajos\n");
					        printf("3. Dar de alta un trabajo\n");
					        printf("4. Listar entre dos fechas trabajos terminados\n");
					        printf("5. Listar las 4 opciones mas vendidas entre fechas\n");
					        printf("0. Salir\n");
					        printf("Seleccione una opcion: ");
					        scanf("%d", &opcion);
					
					        switch (opcion) {
					            case 1:
					                Carga_Trabajo(&E, &S, inicio_MaterialesPorOpcion, raizM, raiz_opcion, pila, lista);
					                guardar_cola_finalizado(E, "trabajos_pendientes.txt");
					                break;
					            case 2:
					                Mostrar_Trabajos(E);
					                break;
					            case 3: 
					                Alta_Trabajos(&E, &E2);
					                guardar_cola_finalizado(E2, "trabajos_finalizados.txt");
					                guardar_cola_finalizado(E, "trabajos_pendientes.txt");
					                break;
					            case 4:
					                listarentrefechas_fina(E2, raiz_opcion);
					                break;
					            case 5:
									contador(raiz_opcion);
					                listaropcvendi(E2, raiz_opcion);
									break;
								
					            case 0:
					                printf("Saliendo...\n");
					                break;
					            default:
					                printf("Opcion no valida. Intente nuevamente.\n");
					                break;
					        }
					    } while (opcion != 0);
					
					    break;
			}
			
			case 6:{
				imprimirPila(pila);
				break;
			}	
			case 7:{
				imprimirlista(lista);
				break;
			}
			case 8:{
				listar_trabajos_tecnico(E , E2, raiz_opcion,pila);
				system("pause");
				break;
			}
            default:
                printf("Opcion invalida.\n");
                break;
 	}
    
	} while (opcion_o_tarea != 0);
    
   
    
    return 0;
}




//MODULOS DE OPCIONES

void insertar_opcion(struct opciones **nodo, struct opciones **raiz){
    struct opciones *ant = NULL;
    ant = buscar_anterior((*nodo)->cod_opcion, *raiz);
    if (ant != NULL) {
        (*nodo)->sgte = ant->sgte;
        ant->sgte = *nodo;
    } else {
        (*nodo)->sgte = *raiz;
        *raiz = *nodo;
    }
    *nodo = NULL;
}

struct opciones *buscar_anterior(int codigo, struct opciones *raiz) {
    struct opciones *ant = NULL;
    while (raiz != NULL) {
        if (raiz->cod_opcion > codigo) {
            raiz = NULL;
        } else {
            ant = raiz;
            raiz = raiz->sgte;
        }
    }
    return(ant);
}

void recorrer_opciones(struct opciones *prin, struct MatXOpc *tope_matxopc, struct Material *raiz_mat, struct tareas *nod_tarea) {
	float total_mat=0, total=0;
    while (prin != NULL) {
        printf("Nombre de la opcion: %s\n", prin->nombre);
        printf("Codigo: %d\n", prin->cod_opcion);
        printf("Costo de mano de obra: %.2f\n", prin->costo_MO);
        total_mat = buscarprecio(prin->cod_opcion, tope_matxopc, raiz_mat);
        printf("Costo de materiales: %.2f\n", total_mat);
        total = total_mat + prin->costo_MO;
        printf("Total de costos: %.2f\n", total);
        calcular_tiempo(prin->cod_opcion, nod_tarea);
        prin = prin->sgte;
    }
}

float buscarprecio(int id_opcion, struct MatXOpc *matxopcion, struct Material *materiales) {
    float total_mat = 0;

    // Recorre la lista de MatXOpc
    while (matxopcion != NULL) {
        if (id_opcion == matxopcion->idOpcion) {
            struct Material *mat_actual = materiales;

            while (mat_actual != NULL) {
                if (matxopcion->idMaterial == mat_actual->CodMat) {
                    total_mat = total_mat + matxopcion->cantidad * mat_actual->CostoMat;
                    break;
                } else if (matxopcion->idMaterial > mat_actual->CodMat) {
                    mat_actual = mat_actual->derecha;
                } else {
                    mat_actual = mat_actual->izquierda;
                }
            }
        }
        matxopcion = matxopcion->sgte;
    }

    return total_mat;
}

void guardar_opciones_en_archivo(struct opciones *raiz, const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo");
        return;
    }
    
    struct opciones *actual = raiz;
    while (actual != NULL) {
        fprintf(archivo, "%d\n%s\n%.2f\n", actual->cod_opcion, actual->nombre, actual->costo_MO);
        actual = actual->sgte;
    }
    
    fclose(archivo);
}

void cargar_opciones_desde_archivo(struct opciones **raiz, const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        // No hay archivo previo, no hacer nada
        return;
    }
    
    while (!feof(archivo)) {
        struct opciones *nueva_opcion = (struct opciones *) malloc(sizeof(struct opciones));
        if (nueva_opcion == NULL) {
            printf("No hay espacio de memoria suficiente");
            fclose(archivo);
            return;
        }
        
        if (fscanf(archivo, "%d\n", &nueva_opcion->cod_opcion) != 1) break;
        fgets(nueva_opcion->nombre, sizeof(nueva_opcion->nombre), archivo);
        nueva_opcion->nombre[strcspn(nueva_opcion->nombre, "\n")] = 0; // Eliminar salto de línea
        fscanf(archivo, "%f\n", &nueva_opcion->costo_MO);
        nueva_opcion->sgte = NULL;
        insertar_opcion(&nueva_opcion, raiz);
    }
    
    fclose(archivo);
}

void agregar_opcion(struct opciones **raiz) {
    struct opciones *nueva_opcion = (struct opciones *) malloc(sizeof(struct opciones));
    if (nueva_opcion != NULL) {
        printf("Ingrese el nombre de la opcion: ");
        fgets(nueva_opcion->nombre, sizeof(nueva_opcion->nombre), stdin);
        nueva_opcion->nombre[strcspn(nueva_opcion->nombre, "\n")] = 0; // Eliminar salto de línea
        printf("Ingrese el codigo de la opcion: ");
        scanf("%d", &nueva_opcion->cod_opcion);
        printf("Ingrese el costo de la mano de obra de la opcion: ");
        scanf("%f", &nueva_opcion->costo_MO);
        printf("Ingrese las nuevas tareas, con el codigo ingresado!");
        
        
        nueva_opcion->sgte = NULL;
        insertar_opcion(&nueva_opcion, raiz);
    } else {
        printf("No hay espacio de memoria suficiente\n");
    }
}

void calcular_tiempo(int cod, struct tareas *tope_tareas){
	int hora=0, minutos=0, orden=(-1);
	while(tope_tareas != NULL){
		if((tope_tareas->cod_opcion == cod) && (tope_tareas->orden != orden)){
			hora = hora + tope_tareas->estimado.hora;
			minutos = minutos + tope_tareas->estimado.minuto;
			orden = tope_tareas->orden;
			tope_tareas = tope_tareas->sgte;
		}else{
			tope_tareas = tope_tareas->sgte;
		}
	}
	if(minutos >= 60){
		hora = hora + (minutos / 60);
		minutos = minutos % 60;
		printf("Tiempo estimado: %d:%dhs\n\n", hora, minutos);
	}else{
		printf("Tiempo estimado: %d:%dhs\n\n", hora, minutos);
	}
}

//MODULOS DE TAREAS

void insertar_tareas(struct tareas **nodo, struct tareas **raiz) {
    struct tareas *ant, *aux;
    ant = buscar_tareas_anterior((*nodo)->cod_opcion, (*nodo)->orden, (*nodo)->estimado.hora, (*nodo)->estimado.minuto, *raiz);

    if (*raiz != NULL) {
        if (ant != NULL) {
            aux = ant->sgte;
            (*nodo)->sgte = aux;
            if (aux != NULL) {
                aux->ant = *nodo;
            }
            ant->sgte = *nodo;
            (*nodo)->ant = ant;
        } else {
            (*nodo)->sgte = *raiz;
            if (*raiz != NULL) {
                (*raiz)->ant = *nodo;
            }
            *raiz = *nodo;
        }
    } else {
        *raiz = *nodo;
        (*nodo)->sgte = NULL;
        (*nodo)->ant = NULL;
    }

    *nodo = NULL;
}


struct tareas *buscar_tareas_anterior(int opcion, int orden, int hora, int minutos, struct tareas *ini) {
    struct tareas *ant = NULL;

    // Convertir hora y minutos a minutos totales
    int tiempo_nuevo = hora * 60 + minutos;

    while (ini != NULL) {
        // Si la opción actual es mayor, hemos pasado el punto de inserción
        if (ini->cod_opcion > opcion) {
            break;
        }

        // Si encontramos la opción correcta
        if (ini->cod_opcion == opcion) {
            // Si encontramos el mismo orden
            if (ini->orden == orden) {
                // Convertir el tiempo actual a minutos totales
                int tiempo_actual = ini->estimado.hora * 60 + ini->estimado.minuto;

                // Si el tiempo actual es mayor o igual al nuevo, seguimos buscando
                if (tiempo_actual < tiempo_nuevo) {
                    break;
                }
            } else if (ini->orden > orden) {
                // Si encontramos una orden mayor, hemos pasado el punto de inserción
                break;
            }
        }

        // Avanzar al siguiente nodo
        ant = ini;
        ini = ini->sgte;
    }

    return ant;
}


void recorrer_tareas(struct tareas *prin) {
    while (prin != NULL) {
        printf("Codigo de la opcion: %d\n", prin->cod_opcion);
        printf("Codigo de la tarea: %d\n", prin->cod_tarea);
        printf("Nombre de la tarea: %s\n", prin->nom_tarea);
        printf("Orden de la tarea: %d\n", prin->orden);
        printf("Tiempo estimado: %d:%d\n\n", prin->estimado.hora, prin->estimado.minuto);
        prin = prin->sgte;
    }
}

void guardar_tareas_en_archivo(struct tareas *raiz, const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo");
        return;
    }

    struct tareas *actual = raiz;
    while (actual != NULL) {
        fprintf(archivo, "%d\n%d\n%s\n%d\n%d\n%d\n", actual->cod_opcion, actual->cod_tarea, actual->nom_tarea, actual->orden, actual->estimado.hora, actual->estimado.minuto);
        actual = actual->sgte;
    }

    fclose(archivo);
}

void cargar_tareas_desde_archivo(struct tareas **raiz, const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        // No hay archivo previo, no hacer nada
        return;
    }

    while (!feof(archivo)) {
        struct tareas *nueva_tarea = (struct tareas *) malloc(sizeof(struct tareas));
        if (nueva_tarea == NULL) {
            printf("No hay espacio de memoria suficiente");
            fclose(archivo);
            return;
        }

        if (fscanf(archivo, "%d\n%d\n%[^\n]\n%d\n%d\n%d\n", &nueva_tarea->cod_opcion, &nueva_tarea->cod_tarea, nueva_tarea->nom_tarea, &nueva_tarea->orden, &nueva_tarea->estimado.hora, &nueva_tarea->estimado.minuto) != 6) break;

        nueva_tarea->ant = NULL;
        nueva_tarea->sgte = NULL;
        insertar_tareas(&nueva_tarea, raiz);
    }

    fclose(archivo);
}

void agregar_tarea(struct tareas **raiz) {
    struct tareas *nueva_tarea = (struct tareas *) malloc(sizeof(struct tareas));
    if (nueva_tarea != NULL) {
        printf("Ingrese el codigo de opcion vinculado: ");
        scanf("%d", &nueva_tarea->cod_opcion);
        printf("Ingrese el codigo de la tarea: ");
        scanf("%d", &nueva_tarea->cod_tarea);
        getchar(); // Limpiar buffer de entrada
        printf("Ingrese el nombre de la tarea: ");
        fgets(nueva_tarea->nom_tarea, sizeof(nueva_tarea->nom_tarea), stdin);
        nueva_tarea->nom_tarea[strcspn(nueva_tarea->nom_tarea, "\n")] = 0; // Eliminar salto de línea
        printf("Ingrese el orden de la tarea: ");
        scanf("%d", &nueva_tarea->orden);
        printf("Ingrese el tiempo estimado\n");
        printf("Hora: ");
        scanf("%d", &nueva_tarea->estimado.hora);
        printf("Minuto: ");
        scanf("%d", &nueva_tarea->estimado.minuto);
        nueva_tarea->sgte = NULL;
        nueva_tarea->ant = NULL;
        insertar_tareas(&nueva_tarea, raiz);
    } else {
        printf("No hay espacio de memoria suficiente\n");
    }
}

struct tareas *buscar_tarea(int cod_opcion, int cod_tarea, struct tareas *raiz) {
    while (raiz != NULL) {
        if (raiz->cod_opcion == cod_opcion && raiz->cod_tarea == cod_tarea) {
            return raiz;
        }
        raiz = raiz->sgte;
    }
    return NULL;
}


//Modulo Material 

// Función para crear y agregar un nuevo nodo de material
void nuevoNodoM(struct Material** raizM) {
    struct Material* nodoM = (struct Material*) malloc(sizeof(struct Material));
    if (nodoM != NULL) {
        printf("Ingrese ID: ");
        scanf("%d", &nodoM->CodMat);
        getchar(); // Limpiar buffer de entrada
        printf("Nombre del material: ");
        fgets(nodoM->denominacion, sizeof(nodoM->denominacion), stdin);
        nodoM->denominacion[strcspn(nodoM->denominacion, "\n")] = 0; // Eliminar salto de línea
        printf("Stock disponible: ");
        scanf("%d", &nodoM->StockMat);
        printf("Precio unitario: ");
        scanf("%d", &nodoM->CostoMat);
        nodoM->derecha = NULL;
        nodoM->izquierda = NULL;
        *raizM = insertarMaterial(*raizM, nodoM);
    } else {
        printf("No se completo el registro, ingrese nuevamente.\n");
    }
}

// Función para insertar un material en el árbol
struct Material* insertarMaterial(struct Material* raizM, struct Material* nodoM) {
    if (raizM != NULL) {
        if (raizM->CodMat == nodoM->CodMat) {
            printf("Ya existe material con ese ID.\n");
        } else if (raizM->CodMat > nodoM->CodMat) {
            raizM->izquierda = insertarMaterial(raizM->izquierda, nodoM);
        } else {
            raizM->derecha = insertarMaterial(raizM->derecha, nodoM);
        }
    } else {
        raizM = nodoM;
    }
    return raizM;
}

// Función para buscar el costo de un material por su ID
float busquedaMaterial(struct Material* raiz, int mat) {
    if (raiz != NULL) {
        if (raiz->CodMat == mat) {
            return raiz->CostoMat;
        } else if (raiz->CodMat > mat) {
            return busquedaMaterial(raiz->izquierda, mat);
        } else {
            return busquedaMaterial(raiz->derecha, mat);
        }
    } else {
        printf("No se encontró material con ID %d.\n", mat);
        return -1;
    }
}

// Función para recorrer y mostrar todos los materiales en el árbol
void recorrer(struct Material* raiz) {
    if (raiz != NULL) {
        recorrer(raiz->izquierda);
        printf("ID: %d\n", raiz->CodMat);
        printf("Nombre: %s\n", raiz->denominacion);
        printf("Stock: %d\n", raiz->StockMat);
        printf("Costo: %d\n\n", raiz->CostoMat);
        recorrer(raiz->derecha);
    }
}

// Función para guardar los materiales en un archivo
void guardarMaterialesEnArchivo(struct Material* raiz, const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo");
        return;
    }
    guardarMaterialEnArchivo_auxiliar(archivo, raiz);
    fclose(archivo);
}

// Función auxiliar para guardar los materiales en un archivo
void guardarMaterialEnArchivo_auxiliar(FILE* archivo, struct Material* raiz) {
    if (raiz != NULL) {
        guardarMaterialEnArchivo_auxiliar(archivo, raiz->izquierda);
        fprintf(archivo, "%d\n%s\n%d\n%d\n", raiz->CodMat, raiz->denominacion, raiz->CostoMat, raiz->StockMat);
        guardarMaterialEnArchivo_auxiliar(archivo, raiz->derecha);
    }
}

// Función para cargar los materiales desde un archivo
void cargarMaterialesDesdeArchivo(struct Material** raiz, const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
    	printf("\nERROR AL ABRIR MATERIALES\n");
        // No hay archivo previo, no hacer nada
        return;
    }

    while (!feof(archivo)) {
        struct Material* nueva_tarea = (struct Material*) malloc(sizeof(struct Material));
        if (nueva_tarea == NULL) {
            printf("No hay espacio de memoria suficiente");
            fclose(archivo);
            return;
        }

        if (fscanf(archivo, "%d\n%[^\n]\n%d\n%d\n", &nueva_tarea->CodMat, nueva_tarea->denominacion, &nueva_tarea->StockMat, &nueva_tarea->CostoMat) != 4) {
        	printf("\nERROR AL ACTUALIZAR MATERIALES\n");
            free(nueva_tarea);
            break;
        }

        nueva_tarea->izquierda = NULL;
        nueva_tarea->derecha = NULL;
        *raiz = insertarMaterial(*raiz, nueva_tarea);
    }

    fclose(archivo);
}

//Modulo Materiales por Opcion
void agregarMaterialXOpcion(struct MatXOpc **ini) {
    struct MatXOpc *nodo = (struct MatXOpc *) malloc(sizeof(struct MatXOpc));
    if (nodo == NULL) {
        printf("Error al asignar memoria");
        return;
    }
    printf("\nidOpcion:");
    scanf("%d", &nodo->idOpcion);
    printf("\nidMaterial:");
    scanf("%d", &nodo->idMaterial);
    printf("\ncantidad:");
    scanf("%d", &nodo->cantidad);
    nodo->sgte = NULL;

    *ini = CargaMatXOpc(*ini, nodo);
    printf("\nEl proceso finalizo correctamente\n");
}


struct MatXOpc *CargaMatXOpc(struct MatXOpc *inicio, struct MatXOpc *nv) {
    if (inicio == NULL) {
        return nv;
    }
    
    struct MatXOpc *actual = inicio;
    while (actual->sgte != NULL) {
        if (actual->idMaterial == nv->idMaterial && actual->idOpcion == nv->idOpcion) {
            printf("\nDicho material ya se encuentra creado para esa opcion.\n");
            free(nv); 
            return inicio;
        }
        actual = actual->sgte;
    }

    if (actual->idMaterial == nv->idMaterial && actual->idOpcion == nv->idOpcion) {
        printf("\nDicho material ya se encuentra creado para esa opcion.\n");
        free(nv);
    } else {
        actual->sgte = nv;
    }

    return inicio;
}


void guardar_MaterialXOpcion_en_archivo(struct MatXOpc *inicio, const char *archivo) {
    FILE *pa = fopen(archivo, "w");
    if (pa == NULL) {
        printf("Error al abrir el archivo");
        return;
    }
    struct MatXOpc *actual = inicio;
    while (actual != NULL) {
        fprintf(pa, "%d\n%d\n%d\n", actual->idOpcion, actual->idMaterial, actual->cantidad);
        actual = actual->sgte;
    }
    fclose(pa);
}


void cargar_MaterialXOpcion_desde_archivo(struct MatXOpc **raiz, const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        return; 
    }

    while (!feof(archivo)) {
        struct MatXOpc *nueva_opcion = (struct MatXOpc *) malloc(sizeof(struct MatXOpc));
        if (nueva_opcion == NULL) {
            printf("No hay espacio de memoria suficiente");
            fclose(archivo);
            return;
        }

        if (fscanf(archivo, "%d\n%d\n%d\n", &nueva_opcion->idOpcion, &nueva_opcion->idMaterial, &nueva_opcion->cantidad) != 3) {
            free(nueva_opcion);
            break;
        }

        nueva_opcion->sgte = NULL;
        
        *raiz = CargaMatXOpc(*raiz, nueva_opcion);
    }

    fclose(archivo);
}


//Modulo de Trabajos
void Carga_Trabajo(struct Trabajos_Pendientes **Entrada, struct Trabajos_Pendientes **Salida, struct MatXOpc *ini_matxopc, struct Material *ini_mat, struct opciones *ini_opciones, struct clientes *ini_clientes, struct tecnicos *ini_tecnicos) {
    int dia, mes, anio;
    int idcliente, idtecnico;
	
	struct Trabajos_Pendientes *nuevoTrabajo = (struct Trabajos_Pendientes *)malloc(sizeof(struct Trabajos_Pendientes));
    if (nuevoTrabajo == NULL) {
        printf("Error al asignar memoria.\n");
        return;
    }
    printf("Ingrese el ID del Trabajo: ");
    scanf("%d", &nuevoTrabajo->idTrabajo);
    printf("El trabajo es en altura? (1 si, 0 no): ");
    scanf("%d", &nuevoTrabajo->Altura);
    printf("Ingrese el ID de la Opcion: ");
    scanf("%d", &nuevoTrabajo->idOpcion);
    printf("Ingrese el ID del Cliente: ");
    scanf("%d", &idcliente);
    verificar_cliente(idcliente,ini_clientes);
    nuevoTrabajo->idCliente = idcliente;
    printf("Ingrese el ID del Tecnico: ");
    scanf("%d", &idtecnico);
    verificar_tecnico(idtecnico,ini_tecnicos);
    nuevoTrabajo->idTecnico = idtecnico;
    nuevoTrabajo->pendiente = 1;

    printf("Ingrese la Ubicacion: ");
    getchar();
    fgets(nuevoTrabajo->Ubicacion, sizeof(nuevoTrabajo->Ubicacion), stdin);
    nuevoTrabajo->Ubicacion[strcspn(nuevoTrabajo->Ubicacion, "\n")] = '\0';
    
    printf("Ingrese el dia de la fecha: ");
	scanf("%d", &nuevoTrabajo->fech.dia);
    printf("Ingrese el mes: ");
	scanf("%d", &nuevoTrabajo->fech.mes);
    printf("Ingrese el año: ");
	scanf("%d", &nuevoTrabajo->fech.anio);
	
    nuevoTrabajo->MonTotal = buscartotal(nuevoTrabajo->idOpcion, ini_matxopc, ini_mat, ini_opciones);
    nuevoTrabajo->sgte = NULL;

    if (*Entrada == NULL) {
        *Entrada = nuevoTrabajo;
        *Salida = nuevoTrabajo;
    } else {
        (*Salida)->sgte = nuevoTrabajo;
        *Salida = nuevoTrabajo;
    }
    printf("Trabajo cargado exitosamente.\n");
}

float buscartotal(int id_opcion, struct MatXOpc *matxopcion, struct Material *materiales, struct opciones *ini_opciones){

	float total = 0;
	float total_mat = 0;
	float totalMO=0;
	//Recorre las opciones para buscar la mano de obra
	while(ini_opciones != NULL){
		if(id_opcion == ini_opciones->cod_opcion){
			totalMO = ini_opciones->costo_MO;
		}
		ini_opciones = ini_opciones->sgte;
	}
    // Recorre la lista de MatXOpc
    while (matxopcion != NULL) {
        if (id_opcion == matxopcion->idOpcion) {
            struct Material *mat_actual = materiales;

            while (mat_actual != NULL) {
                if (matxopcion->idMaterial == mat_actual->CodMat) {
                    total_mat = total_mat + matxopcion->cantidad * mat_actual->CostoMat;
                    break;
                } else if (matxopcion->idMaterial > mat_actual->CodMat) {
                    mat_actual = mat_actual->derecha;
                } else {
                    mat_actual = mat_actual->izquierda;
                }
            }
        }
        matxopcion = matxopcion->sgte;
    }
 	
 	total = total_mat+totalMO;
    return total;
}

//resuelto
void guardar_cola_finalizado(struct Trabajos_Pendientes *E, const char *archivo) {
    FILE *arch = fopen(archivo, "w");
    if (arch == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }

    struct Trabajos_Pendientes *actual = E;
    while (actual != NULL) {
        fprintf(arch, "%d\n%d\n%d\n%d\n%d\n%d\n%s\n%d\n%d\n%d\n%.2f\n", 
                actual->idTrabajo, 
                actual->Altura, 
                actual->idOpcion, 
                actual->idCliente, 
                actual->idTecnico, 
                actual->pendiente, 
                actual->Ubicacion, 
                actual->fech.dia, 
                actual->fech.mes, 
                actual->fech.anio, 
                actual->MonTotal);
        actual = actual->sgte;
    }

    fclose(arch);
}

void Alta_Trabajos(struct Trabajos_Pendientes **cola1, struct Trabajos_Pendientes **cola2) {
    // Verificar cola 1 vacía
    if (*cola1 != NULL) {
        // Desencolar 
        struct Trabajos_Pendientes *trabajo = *cola1;
        *cola1 = (*cola1)->sgte;
        
        trabajo->pendiente = 0;
        
        // Encolar
        if (*cola2 == NULL) {
            *cola2 = trabajo;
            trabajo->sgte = NULL;
        } else {
            struct Trabajos_Pendientes *temp = *cola2;
            while (temp->sgte != NULL) {
                temp = temp->sgte;
            }
            temp->sgte = trabajo;
            trabajo->sgte = NULL;
        }
    	printf("\nTrabajo Completado con Exito");
    	printf("\nID TRABAJO: %d\n\n",trabajo->idTrabajo);
	} else {
        printf("\n\nNo hay mas Trabajos pendientes en la Cola.\n\n");
    }
    
}


bool fechaValida(int dia, int mes, int anio) {
   
    if (anio < 0){
		printf("ingrese un anio mayor a 0");
		return false;
	}
	
	if (mes < 1 || mes > 12) {
		printf("ingrese un mes valido (1 - 12)");
		return false;
	}
	
	if (dia < 1){
		printf("ingrese un dia mayor a 0");
		return false;
	} 

    int diasPorMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (anio % 4 == 0 && (anio % 100 != 0 || anio % 400 == 0)) {
        diasPorMes[1] = 29; // Año bisiesto
    }
    if (dia > diasPorMes[mes - 1]) {
	printf("ingrese un dia valido para el mes.");
	return false;
	}

    return true;
}

int convertirFechaADias(int dia, int mes, int anio) {
    // Simplificación para convertir la fecha a un número total de días
    return anio * 365 + mes * 30 + dia;
}

 void listarentrefechas_fina(struct Trabajos_Pendientes *E2, struct opciones *opc) {
    FILE *arch=fopen("trabajos_pendientes.txt","r");
    int dia1, dia2, mes1, mes2, anio1, anio2,cont;
    int total1, total2, fech3;
    char nombre_opc[50];
    struct Trabajos_Pendientes *aux;
    cont=0;
    aux=E2;

    printf("--------Busqueda entre fechas--------\n");
    printf("Ingrese la primer fecha\n");
    do {
        printf("Ingrese el dia: ");
        scanf("%d", &dia1);
        printf("Ingrese el mes: ");
        scanf("%d", &mes1);
        printf("Ingrese el año: ");
        scanf("%d", &anio1);
    } while (!fechaValida(dia1, mes1, anio1));

    printf("Ingrese la segunda fecha: \n");
    do {
        printf("Ingrese el dia: ");
        scanf("%d", &dia2);
        printf("Ingrese el mes: ");
        scanf("%d", &mes2);
        printf("Ingrese el año: ");
        scanf("%d", &anio2);
    } while (!fechaValida(dia2, mes2, anio2));

    total1 = convertirFechaADias(dia1, mes1, anio1);
    total2 = convertirFechaADias(dia2, mes2, anio2);

    while (aux != NULL) {
        fech3 = convertirFechaADias(aux->fech.dia, aux->fech.mes, aux->fech.anio);
        if ((total1 <= fech3) && (fech3 <= total2)) {
            cont=cont +1;
			printf("\n\nID Trabajo: %d\n", aux->idTrabajo);
            buscarnombre(nombre_opc, aux->idOpcion, opc); // Se asume que existe una función llamada buscarnombre para obtener el nombre de la opción
            printf("Opcion: %s\n", nombre_opc);
            printf("Ubicacion: %s\n", aux->Ubicacion);
            printf("Monto Total: %.2f\n", aux->MonTotal);
            printf("--------------------------\n\n");
       
	    }
       aux=aux->sgte; 
    }
    
    if(cont==0){
    	printf("\n\n no hay trabajos entre esas fechas");
	}
}


void buscarnombre(char *nombre, int id, struct opciones *prin) {
    while (prin != NULL) {
        if (id == prin->cod_opcion) {
            strcpy(nombre, prin->nombre);
            return;  // Salir de la función una vez encontrado el nombre
        }
        prin = prin->sgte;  // Avanzar al siguiente elemento en la lista
    }
    // Si no se encuentra el nombre, se puede establecer un valor predeterminado o un mensaje de error
    strcpy(nombre, "Nombre no encontrado");
}



void contar_ventas_entre_fechas(struct Trabajos_Pendientes *E2, struct opciones *opciones, int fecha_inicio, int fecha_fin) {
    struct Trabajos_Pendientes *trabajo = E2;
    while (trabajo != NULL) {
        // Convertir la fecha del trabajo a días
        int trabajo_fecha = convertirFechaADias(trabajo->fech.dia, trabajo->fech.mes, trabajo->fech.anio);
        if (trabajo_fecha >= fecha_inicio && trabajo_fecha <= fecha_fin) {
            // Aumentar la cantidad de ventas para la opción actual
            struct opciones *opcion_actual = opciones;
            while (opcion_actual != NULL) {
                if (opcion_actual->cod_opcion == trabajo->idOpcion) {
                    opcion_actual->cant_ventas++;
                    break;
                }
                opcion_actual = opcion_actual->sgte;
            }
        }
        trabajo = trabajo->sgte;
    }
}

// Función para listar las opciones más vendidas entre fechas especificadas
void listaropcvendi(struct Trabajos_Pendientes *E2, struct opciones *ini) {
    struct Trabajos_Pendientes *aux1 = E2;
    int dia1, dia2, mes1, mes2, anio1, anio2;
    int total1, total2;

    printf("--------Busqueda entre fechas--------\n");
    printf("Ingrese la primer fecha\n");
    printf("Ingrese el dia: ");
    scanf("%d", &dia1);
    printf("Ingrese el mes: ");
    scanf("%d", &mes1);
    printf("Ingrese el año: ");
    scanf("%d", &anio1);
    printf("Ingrese la segunda fecha: \n");
    printf("Ingrese el dia: ");
    scanf("%d", &dia2);
    printf("Ingrese el mes: ");
    scanf("%d", &mes2);
    printf("Ingrese el año: ");
    scanf("%d", &anio2);
    total1 = convertirFechaADias(dia1, mes1, anio1);
    total2 = convertirFechaADias(dia2, mes2, anio2);

    contar_ventas_entre_fechas(E2, ini, total1, total2);

    listar_mas_vendi(ini);
}

// Función para listar las 4 opciones más vendidas
void listar_mas_vendi(struct opciones *opciones) {
    struct opciones *actual;
    struct opciones *siguiente;
    int temp_ventas;
    char temp_nombre[45];

    // Ordenar las opciones por cantidad de ventas de manera descendente
    for (actual = opciones; actual != NULL; actual = actual->sgte) {
        for (siguiente = actual->sgte; siguiente != NULL; siguiente = siguiente->sgte) {
            if (siguiente->cant_ventas > actual->cant_ventas) {
                // Intercambiar la cantidad de ventas
                temp_ventas = actual->cant_ventas;
                actual->cant_ventas = siguiente->cant_ventas;
                siguiente->cant_ventas = temp_ventas;

                // Intercambiar los nombres (esto es opcional, solo si se quiere mostrar los nombres correctamente)
                strcpy(temp_nombre, actual->nombre);
                strcpy(actual->nombre, siguiente->nombre);
                strcpy(siguiente->nombre, temp_nombre);
            }
        }
    }

    // Mostrar las cuatro opciones más vendidas
    printf("Las cuatro opciones más vendidas son:\n");
    for (int i = 0; i < 4 && opciones != NULL; i++) {
        printf("%s - Cantidad de ventas: %d\n", opciones->nombre, opciones->cant_ventas);
        opciones = opciones->sgte;
    }
}

void contador(struct opciones *raiz_opcion){
	
	// Inicializar el contador de ventas a 0
        struct opciones *cont = raiz_opcion;
        while (cont != NULL) {
        cont->cant_ventas = 0;
        cont = cont->sgte;
		}

}


//funciona
void Cargar_Desde_Archivo(struct Trabajos_Pendientes **Entrada, struct Trabajos_Pendientes **Salida, const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo %s. Se comenzará con una cola vacía.\n", nombreArchivo);
        return;
    }

    while (1) {
        struct Trabajos_Pendientes *nuevoTrabajo = (struct Trabajos_Pendientes *)malloc(sizeof(struct Trabajos_Pendientes));
        if (nuevoTrabajo == NULL) {
            printf("Error al asignar memoria.\n");
            fclose(archivo);
            return;
        }
        if (fscanf(archivo, "%d\n", &nuevoTrabajo->idTrabajo) != 1) break;
        if (fscanf(archivo, "%d\n", &nuevoTrabajo->Altura) != 1) break;
        if (fscanf(archivo, "%d\n", &nuevoTrabajo->idOpcion) != 1) break;
        if (fscanf(archivo, "%d\n", &nuevoTrabajo->idCliente) != 1) break;
        if (fscanf(archivo, "%d\n", &nuevoTrabajo->idTecnico) != 1) break;
        if (fscanf(archivo, "%d\n", &nuevoTrabajo->pendiente) != 1) break;
        if (fgets(nuevoTrabajo->Ubicacion, sizeof(nuevoTrabajo->Ubicacion), archivo) == NULL) break;
        if (fscanf(archivo, "%d\n", &nuevoTrabajo->fech.dia) != 1) break;
        if (fscanf(archivo, "%d\n", &nuevoTrabajo->fech.mes) != 1) break;
        if (fscanf(archivo, "%d\n", &nuevoTrabajo->fech.anio) != 1) break;
        if (fscanf(archivo, "%f\n", &nuevoTrabajo->MonTotal) != 1) break;

        nuevoTrabajo->Ubicacion[strcspn(nuevoTrabajo->Ubicacion, "\n")] = '\0';
        nuevoTrabajo->sgte = NULL;

        if (*Entrada == NULL) {
            *Entrada = nuevoTrabajo;
            *Salida = nuevoTrabajo;
        } else {
            (*Salida)->sgte = nuevoTrabajo;
            *Salida = nuevoTrabajo;
        }
    }

    fclose(archivo);
}
//funciona
void Mostrar_Trabajos(struct Trabajos_Pendientes *Entrada) {
   	system("cls");
    struct Trabajos_Pendientes *actual = Entrada;
   	if(actual!=NULL){
   		
	
    while (actual != NULL) {
        printf("ID Trabajo: %d\n", actual->idTrabajo);
        printf("Altura: %d\n", actual->Altura);
        printf("ID Opcion: %d\n", actual->idOpcion);
        printf("ID Cliente: %d\n", actual->idCliente);
        printf("ID Tecnico: %d\n", actual->idTecnico);
        printf("Pendiente: %s\n", (actual->pendiente == 1) ? "SI" : "NO");
        printf("Ubicacion: %s\n", actual->Ubicacion);
        printf("Dia: %d\n", actual->fech.dia);
        printf("Mes: %d\n", actual->fech.mes);
        printf("Anio: %d\n", actual->fech.anio);
        printf("Monto Total: %.2f\n", actual->MonTotal);
        printf("--------------------------\n");
        actual = actual->sgte;
    }
	}else{
		printf("\nCola de Trabajos Pendientes Vacia\n\n");
	}
	system("pause");
    system("cls");
}

//Modulo de Clientes
void insertarPila(struct clientes **pila, struct clientes *nuevoNodo) {
    nuevoNodo->sgte = *pila;
    *pila = nuevoNodo;
}

void agregar_cliente(struct clientes *inicio) {
    struct clientes *nuevo_cliente = (struct clientes*) malloc(sizeof(clientes));
    if (nuevo_cliente == NULL) {
        printf("Error: No se pudo asignar memoria para el nuevo cliente.\n");
        return;
    }

    printf("Ingrese el código del cliente: ");
    scanf("%d", &nuevo_cliente->codCliente);

    printf("Ingrese el nombre del cliente: ");
    scanf(" %[^\n]", nuevo_cliente->nombre); 

    printf("Ingrese el DNI del cliente: ");
    scanf("%d", &nuevo_cliente->dni);

    nuevo_cliente->sgte = NULL;

    if (inicio == NULL) {
        inicio = nuevo_cliente;
    } else {
        struct clientes *actual = inicio;
        while (actual->sgte != NULL) {
            actual = actual->sgte;
        }
        actual->sgte = nuevo_cliente;
    }
}


void verificar_cliente(int id, struct clientes *ini){
	struct clientes *aux = ini;
	int log=0;
	while(aux!=NULL){
		if(aux->codCliente==id){
			printf("Cliente Encontrado!\n");
			return;
		}
			
	aux=aux->sgte;
	}
	printf("Cliente no encontrado. Agregrando nuevo cliente...\n");
	agregar_cliente(ini);
}

void insertarClientes(struct clientes **pila) {
    FILE *archivo;
    archivo = fopen("clientes.txt", "r");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo.\n");
        return;
    }

    struct clientes *nuevoCliente;

    while (1) {
        nuevoCliente = (struct clientes *)malloc(sizeof(struct clientes));
        if (nuevoCliente == NULL) {
            printf("Error: No se pudo asignar memoria para el nuevo cliente.\n");
            return;
        }
        if (fscanf(archivo, "%d %49[^0-9] %d", &nuevoCliente->codCliente, nuevoCliente->nombre, &nuevoCliente->dni) == EOF)
            break;
        insertarPila(pila, nuevoCliente); // Insertar nuevo cliente en la pila
    }
    fclose(archivo);
}

void imprimirPila(struct clientes *pila) {
    printf("Clientes en la pila:\n");
    struct clientes *temp = pila; // Utilizamos una variable temporal para no perder la referencia original
    while (temp != NULL) {
        printf("Codigo de Cliente: %d\n", temp->codCliente);
        printf("Nombre: %s\n", temp->nombre);
        printf("DNI: %d\n\n", temp->dni);
        temp = temp->sgte;
    }
    system("pause");
}

//Modulo de Tecnicos
void insertarLista(struct tecnicos **lista, struct tecnicos *nv) {
    nv->sgte = *lista;
    *lista = nv;
}

void insertarTecnicos(struct tecnicos **lista) {
    FILE *archivo;
    archivo = fopen("tecnicos.txt", "r");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo.\n");
        return;
    }

    struct tecnicos *nuevoTecnico;

    while (1) {
        nuevoTecnico = (struct tecnicos *)malloc(sizeof(struct tecnicos));
        if (nuevoTecnico == NULL) {
            printf("Error: No se pudo asignar memoria para el nuevo cliente.\n");
            return;
        }
        if (fscanf(archivo, "%d %49[^0-9] %d", &nuevoTecnico->codTecnico, nuevoTecnico->nombre, &nuevoTecnico->dni) == EOF)
            break;
        insertarLista(lista, nuevoTecnico); // Insertar nuevo cliente en la pila
    }
    fclose(archivo);
}

void imprimirlista(struct tecnicos *lista) {
    printf("Tecnicos en la lista:\n");
    struct tecnicos *temp = lista; // Utilizamos una variable temporal para no perder la referencia original
    while (temp != NULL) {
        printf("Codigo de Tecnico: %d\n", temp->codTecnico);
        printf("Nombre: %s\n", temp->nombre);
        printf("DNI: %d\n\n", temp->dni);
        temp = temp->sgte;
    }
		system("pause");

}

void agregar_tecnico(struct tecnicos *inicio) {
    struct tecnicos *nuevo_tecnico = (struct tecnicos*) malloc(sizeof(tecnicos));
    if (nuevo_tecnico == NULL) {
        printf("Error: No se pudo asignar memoria para el nuevo técnico.\n");
        return;
    }

    printf("Ingrese el código del técnico: ");
    scanf("%d", &nuevo_tecnico->codTecnico);

    printf("Ingrese el nombre del técnico: ");
    scanf(" %[^\n]", nuevo_tecnico->nombre); // Para leer una línea completa, se utiliza "%[^\n]"

    printf("Ingrese el DNI del técnico: ");
    scanf("%d", &nuevo_tecnico->dni);

    nuevo_tecnico->sgte = NULL;

    if (inicio == NULL) {
        inicio = nuevo_tecnico;
    } else {
        struct tecnicos *actual = inicio;
        while (actual->sgte != NULL) {
            actual = actual->sgte;
        }
        actual->sgte = nuevo_tecnico;
    }
}

void verificar_tecnico(int id, struct tecnicos *ini){
	struct tecnicos *aux = ini;
	while(aux!=NULL){
		if(aux->codTecnico==id){
			printf("Tecnico Encontrado!\n");
			return;
		}
			
	aux=aux->sgte;
	}
	printf("Cliente no encontrado. Agregrando nuevo cliente...\n");
	agregar_tecnico(ini);
}

void listar_trabajos_tecnico(struct Trabajos_Pendientes *E, struct Trabajos_Pendientes *E2, struct opciones *ini_opciones, struct clientes *ini_clientes) {
     struct Trabajos_Pendientes *aux = E;
    struct Trabajos_Pendientes *aux2 = E2;
    int id_tecnico=0, encontro=0;
    
    printf("Ingrese el id del tecnico a consultar: ");
    if (scanf("%d", &id_tecnico) == 0) {
        printf("Error al leer el ID del técnico.\n");
        return;
    }

    if (aux == NULL) {
        printf("No se encontraron trabajos pendientes.\n");
    }

    if (aux2 == NULL) {
        printf("No se encontraron trabajos finalizados.\n");
    }

    while (aux != NULL) {
        if (aux->idTecnico == id_tecnico) {
        	printf("------------------------------------\n");
        	printf("Trabajos pendientes del técnico:\n");
            printf("ID Trabajo: %d\n", aux->idTrabajo);
            printf("Nombre de la opción: %s\n", obtener_nombre_opcion(aux->idOpcion, ini_opciones));
            printf("Nombre del cliente: %s\n", obtener_nombre_cliente(aux->idCliente, ini_clientes));
            printf("Ubicación: %s\n", aux->Ubicacion);
            if (aux->Altura == 1) {
                printf("Requiere trabajo en altura\n");
            } else {
                printf("No requiere trabajo en altura\n");
            }
            printf("--------------------------\n");
            encontro=1;
        }
        aux = aux->sgte;
    }

 
    while (aux2 != NULL) {
        if (aux2->idTecnico == id_tecnico) {
        	printf("Trabajos realizados del técnico:\n");
            printf("ID Trabajo: %d\n", aux2->idTrabajo);
            printf("Nombre de la opción: %s\n", obtener_nombre_opcion(aux2->idOpcion, ini_opciones));
            printf("Nombre del cliente: %s\n", obtener_nombre_cliente(aux2->idCliente, ini_clientes));
            printf("Ubicación: %s\n", aux2->Ubicacion);
            if (aux2->Altura == 1) {
                printf("Requiere trabajo en altura\n");
            } else {
                printf("No requiere trabajo en altura\n");
            }
            printf("--------------------------\n");
            encontro=1;
        }
        aux2 = aux2->sgte;
    }
    
    if(encontro==0){
    	printf("No se encontro un tecnico con ese id!\n");
	}
}


const char* obtener_nombre_opcion(int id_opcion, struct opciones *ini_opciones) {
    struct opciones *aux = ini_opciones;
    while (aux != NULL) {
        if (aux->cod_opcion == id_opcion) {
            return aux->nombre;
        }
        aux = aux->sgte;
    }
    return "Desconocido";
}

// Función para obtener el nombre del cliente dado su ID
const char* obtener_nombre_cliente(int id_cliente, struct clientes *ini_clientes) {
    struct clientes *aux = ini_clientes;
    while (aux != NULL) {
        if (aux->codCliente == id_cliente) {
            return aux->nombre;
        }
        aux = aux->sgte;
    }
    return "Desconocido";
}

