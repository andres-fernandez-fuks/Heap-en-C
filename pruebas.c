#include "heap.h"
#include "testing.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

// FUNCIONES DE COMPARACION //


int cmp_cadenas_normal(const void* punt_1,const void* punt_2) {
	const char* cadena_1 = punt_1;
	const char* cadena_2 = punt_2;
	return strcmp(cadena_1,cadena_2);
}

int cmp_numeros_normal(const void* punt_1,const void* punt_2) {
	int num_1 = *(int*) punt_1;
	int num_2 = *(int*) punt_2;
	if (num_1 > num_2) return 1;
	if (num_1 == num_2) return 0;
	return -1;
}

int cmp_numeros_inversa(const void* punt_1,const void* punt_2) {
	int num_1 = *(int*) punt_1;
	int num_2 = *(int*) punt_2;
	if (num_1 < num_2) return 1;
	if (num_1 == num_2) return 0;
	return -1;
}


// FUNCIONES AUXILIARES //

size_t posicion_hijo_iizq(size_t pos_padre) {
	return pos_padre*2+1;
}

size_t posicion_hijo_dder(size_t pos_padre) {
	return pos_padre*2+2;
}

bool es_heap(void** arreglo,size_t pos_padre,size_t cant,cmp_func_t cmp) {
	void* padre = arreglo[pos_padre];
	void* hijo_izq;
	void* hijo_der;
	size_t pos_hijo_izq = posicion_hijo_iizq(pos_padre);
	size_t pos_hijo_der = posicion_hijo_dder(pos_padre);
	if (pos_hijo_izq >= cant) return true;
	hijo_izq = arreglo[pos_hijo_izq];
	if (pos_hijo_der >= cant) hijo_der = NULL;
	else hijo_der = arreglo[pos_hijo_der];
	if (hijo_der == NULL) {
		if (cmp(padre,hijo_izq) > 0) return true;
	}
	if (cmp(padre,hijo_izq) > 0 || cmp(padre,hijo_der) > 0) return true;
	return false;
}

bool determinar_es_heap(heap_t* heap,size_t cant,cmp_func_t cmp) {
	int i = 0;
    void** arreglo = malloc(sizeof(void*)*cant);
    bool ok;
    while (!heap_esta_vacio(heap)) {
    	void* desencolado = heap_desencolar(heap);
    	arreglo[i] = desencolado;
    	i++;
    }

    for (size_t h = 0; h < cant; h++) {
		ok = es_heap(arreglo,h,cant,cmp);
		if (!ok) break;
    }
    free(arreglo);
    return ok;
}


// FUNCIONES DE PRUEBA //


static void prueba_crear_heap_vacio() {
	printf("\n");
	printf("PRUEBAS CREAR HEAP VACIO\n");
	printf("\n");

	
    heap_t* heap = heap_crear(cmp_cadenas_normal);

    print_test("Prueba heap crear heap vacío", heap);
    print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);
    print_test("Prueba heap está vacío es true", heap_esta_vacio(heap));
    print_test("Prueba heap ver máximo devuelve NULL", !heap_ver_max(heap));
    print_test("Prueba heap desencolar devuelve NULL", !heap_desencolar(heap));
    heap_destruir(heap,NULL);

}

static void prueba_crear_heap_con_arreglo() {
	printf("\n");
	printf("PRUEBAS CREAR HEAP CON ARREGLO\n");
	printf("\n");

	int arreglo_1[10];
	for (int i = 0; i<10; i++) {
		arreglo_1[i] = i;
	}

	void** arreglo_2 = malloc(sizeof(int*)*10);
	for (int i = 0; i<10; i++) {
		arreglo_2[i] = &arreglo_1[i];
	}

	heap_t* heap = heap_crear_arr(arreglo_2,10,cmp_numeros_normal);
	
    print_test("Prueba heap crear heap con un arreglo inicial", heap);
    print_test("Prueba heap la cantidad de elementos es la correcta", heap_cantidad(heap) == 10);
    print_test("Prueba heap está vacío es false", !heap_esta_vacio(heap));
    print_test("Prueba heap ver máximo es el correcto", *(int*) heap_ver_max(heap) == 9);
    free(arreglo_2);
    print_test("Prueba heap libero el arreglo original y el heap sigue funcionando", *(int*) heap_ver_max(heap) == 9);

    bool ok = determinar_es_heap(heap,heap_cantidad(heap),cmp_numeros_normal);

    print_test("Prueba heap desencolar devuelve los elementos correctos", ok);
    
    heap_destruir(heap,NULL);

}


static void prueba_heap_encolar_maximos() {
	printf("\n");
	printf("PRUEBAS HEAP ENCOLAR (HEAP DE MÁXIMOS)\n");
	printf("\n");
	heap_t* heap = heap_crear(cmp_numeros_normal);
    print_test("Prueba heap crear heap vacío", heap);
    int arreglo[15];
    bool ok;

	for (int i = 0; i<15; i++) {
		arreglo[i] = i;
		ok = heap_encolar(heap,&arreglo[i]);
		if(!ok) break;
	}

	print_test("Prueba heap encolo varios elementos", ok);
    print_test("Prueba heap está vacío es false", !heap_esta_vacio(heap));
    print_test("Prueba heap ver máximo es el correcto", *(int*) heap_ver_max(heap) == 14);

    ok = determinar_es_heap(heap,heap_cantidad(heap),cmp_numeros_normal);

    print_test("Prueba heap los elementos se habían encolado correctamente", ok);
    heap_destruir(heap,NULL);
}

static void prueba_heap_encolar_minimos() {
	printf("\n");
	printf("PRUEBAS HEAP ENCOLAR (HEAP DE MÍNIMOS)\n");
	printf("\n");
    heap_t* heap = heap_crear(cmp_numeros_inversa);
    print_test("Prueba heap crear heap vacío", heap);
    int arreglo[15];
    bool ok;

	for (int i = 0; i<15; i++) {
		arreglo[i] = 14-i;
		ok = heap_encolar(heap,&arreglo[i]);
		if(!ok) break;
	}

	print_test("Prueba heap encolo varios elementos", ok);
    print_test("Prueba heap está vacío es false", !heap_esta_vacio(heap));
    print_test("Prueba heap ver máximo es el correcto", *(int*) heap_ver_max(heap) == 0);

    ok = determinar_es_heap(heap,heap_cantidad(heap),cmp_numeros_inversa);

    print_test("Prueba heap los elementos se habían encolado correctamente", ok);
    heap_destruir(heap,NULL);

}


static void prueba_heap_valor_null() {
	printf("\n");
	printf("PRUEBAS HEAP VALOR NULL\n");
	printf("\n");
    heap_t* heap = heap_crear(cmp_numeros_normal);

    int* valor = NULL;

    /* Inserta 1 valor y luego lo borra */
    print_test("Prueba heap crear heap vacío", heap);
    print_test("Prueba heap el heap está vacío", heap_esta_vacio(heap));
    print_test("Prueba heap encolar elemento NULL devuelve false", heap_encolar(heap,valor) == false);
    print_test("Prueba heap el heap sigue estando vacío", heap_esta_vacio(heap));
    print_test("Prueba heap desencolar devuelve NULL", !heap_desencolar(heap));

    heap_destruir(heap,NULL);

}

static void prueba_heap_volumen(size_t largo) {
	printf("\n");
	printf("PRUEBAS HEAP VOLUMEN\n");
	printf("\n");

	heap_t* heap = heap_crear(cmp_numeros_normal);

    print_test("Prueba heap crear heap vacío", heap);
    

    int arreglo[largo];
    bool ok;
	for (int i = 0; i<largo; i++) {
		arreglo[i] = i;
		ok = heap_encolar(heap,&arreglo[i]);
		if(!ok) break;
	}

	print_test("Prueba heap encolo varios elementos", ok);
    print_test("Prueba heap está vacío es false", !heap_esta_vacio(heap));
    print_test("Prueba heap ver máximo es el correcto", *(int*) heap_ver_max(heap) == largo-1);

    ok = determinar_es_heap(heap,heap_cantidad(heap),cmp_numeros_normal);

    print_test("Prueba heap los elementos se habían encolado correctamente", ok);
    print_test("Prueba heap está vacío es true", heap_esta_vacio(heap));
    
    heap_destruir(heap,NULL);
}

static void prueba_heap_sort() {
	printf("\n");
	printf("PRUEBAS HEAP SORT\n");
	printf("\n");

	printf("CREO UN ARREGLO ORDENADO DE MAYOR A MENOR\n");

	int arreglo_1[15];
	for (int i = 0; i<15; i++) {
		arreglo_1[i] = 14-i;
	}

	void** arreglo_2 = malloc(sizeof(int*)*15);
	for (int i = 0; i<15; i++) {
		arreglo_2[i] = &arreglo_1[i];
	}

	heap_sort(arreglo_2, 15,cmp_numeros_normal);
	printf("APLICO HEAP SORT\n");

	bool ok = true;
	for (int j = 0; j<15; j++) {
		ok = es_heap(arreglo_2,j,15,cmp_numeros_inversa);
		if(!ok) break;
	}
	print_test("El arreglo se encuentra ahora ordenado de menor a mayor", ok);
	free(arreglo_2);

}


void pruebas_heap_alumno() {
	prueba_crear_heap_vacio();
	prueba_crear_heap_con_arreglo();
	prueba_heap_encolar_maximos();
	prueba_heap_encolar_minimos();
	prueba_heap_valor_null();
	prueba_heap_volumen(5000);
	prueba_heap_sort();
}