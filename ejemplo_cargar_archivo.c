#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Libro {
    char isbn[20];
    char titulo[100];
    int anio;
    struct Libro *sgte;
} Libro;

// Prototipos
void cargarBiblioteca(Libro **inicio);
void mostrarBiblioteca(Libro *inicio);
void liberarMemoria(Libro **inicio);

int main() {
    // 1. Inicializar la lista como vacía
    Libro *miBiblioteca = NULL;

    printf("Iniciando programa de biblioteca...\n");

    // 2. Cargar datos desde el archivo
    cargarBiblioteca(&miBiblioteca);

    // 3. Mostrar los datos cargados para verificar
    mostrarBiblioteca(miBiblioteca);

    // 4. Liberar la memoria al salir
    liberarMemoria(&miBiblioteca);

    system("pause");
    return 0;
}

void cargarBiblioteca(Libro **inicio) {
    FILE *archivo;

    archivo = fopen("biblioteca.txt", "r");

    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo 'biblioteca.txt'.\n");
        return;
    }

    int contador = 0;
    Libro *nuevo;
    
    char isbn_temp[20];
    char titulo_temp[100];
    int anio_temp;

    // --- Lógica de Lectura ---
    // fscanf retorna el número de elementos que leyó con éxito.
    // Le pedimos que lea 3 campos: string (%s), string (%s) y entero (%d).
    // El bucle se repite mientras fscanf logre leer 3 campos.
   
    while (fscanf(archivo, "%20[^,],%100[^,],%d", isbn_temp, titulo_temp, &anio_temp) == 3) {
        
        // 1. Reservar memoria (solo si la lectura fue exitosa)
        nuevo = (Libro*) malloc(sizeof(Libro));
        if (nuevo == NULL) {
            printf("Error: No se pudo asignar memoria.\n");
            fclose(archivo); // Cerrar archivo antes de salir
            return;
        }

        // 2. Copiar datos de las variables temporales al nodo
        strcpy(nuevo->isbn, isbn_temp);
        strcpy(nuevo->titulo, titulo_temp);
        nuevo->anio = anio_temp;
        nuevo->sgte = NULL;

        // 3. Lógica para agregar al FINAL
        if (*inicio == NULL) {
            // La lista está vacía, este es el primer nodo
            *inicio = nuevo;
        } else {
            // La lista NO está vacía, recorrer hasta el último nodo
            Libro *aux = *inicio;
            while (aux->sgte != NULL) {
                aux = aux->sgte;
            }
            // Conectar el nuevo nodo al final
            aux->sgte = nuevo;
        }
        contador++; // Incrementar el contador de libros cargados
    }

    fclose(archivo); // Cerrar el archivo al terminar
    printf("%d libros cargados desde el archivo.\n", contador);
}

void mostrarBiblioteca(Libro *inicio) {
    if (inicio == NULL) {
        printf("\nLa biblioteca esta vacia. No hay libros que mostrar.\n");
        return;
    }

    printf("\n--- MI BIBLIOTECA ---\n");
    Libro *aux = inicio;
    while (aux != NULL) {
        // Imprimir los datos de cada nodo
        printf("ISBN: %s | Titulo: %s | Anio: %d\n", aux->isbn, aux->titulo, aux->anio);
        aux = aux->sgte; // Avanzar al siguiente nodo
    }
    printf("-------------------------------------------\n");
}

void liberarMemoria(Libro **inicio) {
    Libro *aux;
    int contador = 0;
    
    printf("\nLiberando memoria...\n");

    while (*inicio != NULL) {
        aux = *inicio;             // 1. Apuntar al nodo actual
        *inicio = (*inicio)->sgte; // 2. Mover el inicio al siguiente
        free(aux);                 // 3. Liberar el nodo apuntado por aux
        contador++;
    }
    printf("Memoria liberada (%d nodos).\n", contador);
}
