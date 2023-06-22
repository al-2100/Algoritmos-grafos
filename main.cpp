#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

//Variables globales
struct Grafo {
    int numVertices;
    std::vector<std::vector<int>> matrizAdyacencia;
};

struct Vertice {
    float x;
    float y;
    bool isDragging;
};

struct Arista {
    Arista(int i, int i1, int i2) {
        this->origen = i;
        this->destino = i1;
        this->peso = i2;
    }
    Arista(){
    };

    int origen;
    int destino;
    int peso;
};
enum class Algoritmo {
    KRUSKAL,
    DIJKSTRA
};

bool mostrarOriginal = false;
//---------------------------------------------------------------//
//Utilitarios
int leerEntero() {
    int num;
    std::cin >> num;
    return num;
}

int generarNumeroAleatorio(int min, int max) {
    return min + rand() % (max - min + 1);
}
//---------------------------------------------------------------//
//Kruskal
bool compararAristas(const Arista& arista1, const Arista& arista2) {
    return arista1.peso < arista2.peso;
}

std::vector<Arista> kruskal(Grafo& grafo) {
    std::vector<Arista> arbolMST;

    // Crear una lista de aristas ordenadas por peso
    std::vector<Arista> aristas;
    for (int i = 0; i < grafo.numVertices; ++i) {
        for (int j = i + 1; j < grafo.numVertices; ++j) {
            if (grafo.matrizAdyacencia[i][j] != 0) {
                Arista arista;
                arista.origen = i;
                arista.destino = j;
                arista.peso = grafo.matrizAdyacencia[i][j];
                aristas.push_back(arista);
            }
        }
    }
    std::sort(aristas.begin(), aristas.end(), compararAristas);

    // Crear una lista para almacenar los conjuntos disjuntos
    std::vector<int> conjuntos(grafo.numVertices);
    for (int i = 0; i < grafo.numVertices; ++i) {
        conjuntos[i] = i;
    }

    // Aplicar el algoritmo de Kruskal
    for (const Arista& arista : aristas) {
        int conjuntoOrigen = conjuntos[arista.origen];
        int conjuntoDestino = conjuntos[arista.destino];

        if (conjuntoOrigen != conjuntoDestino) {
            // Agregar la arista al árbol MST
            arbolMST.push_back(arista);

            // Unir los conjuntos
            for (int i = 0; i < grafo.numVertices; ++i) {
                if (conjuntos[i] == conjuntoDestino) {
                    conjuntos[i] = conjuntoOrigen;
                }
            }
        }
    }

    return arbolMST;
}
//---------------------------------------------------------------//
//Dijkstra
std::vector<Arista> dijkstra(const Grafo& grafo, int origen, int destino) {
    int numVertices = grafo.numVertices;
    std::vector<int> distancia(numVertices, INT_MAX);
    std::vector<int> padre(numVertices, -1);
    std::vector<bool> visitado(numVertices, false);
    std::vector<Arista> aristas;

    distancia[origen] = 0;

    while (!visitado[destino]) {
        int minDistancia = INT_MAX;
        int minVertice = -1;

        // Encontrar el vértice con la distancia mínima no visitado
        for (int i = 0; i < numVertices; ++i) {
            if (!visitado[i] && distancia[i] < minDistancia) {
                minDistancia = distancia[i];
                minVertice = i;
            }
        }

        if (minVertice == -1) {
            // No hay un camino al destino
            break;
        }

        visitado[minVertice] = true;

        // Actualizar las distancias de los vértices adyacentes no visitados
        for (int i = 0; i < numVertices; ++i) {
            if (!visitado[i] && grafo.matrizAdyacencia[minVertice][i] != 0 &&
                distancia[minVertice] != INT_MAX &&
                distancia[minVertice] + grafo.matrizAdyacencia[minVertice][i] < distancia[i]) {
                distancia[i] = distancia[minVertice] + grafo.matrizAdyacencia[minVertice][i];
                padre[i] = minVertice;
            }
        }
    }

    // Construir el vector de aristas desde el nodo de origen hasta el nodo de destino
    if (visitado[destino]) {
        int verticeActual = destino;
        while (padre[verticeActual] != -1) {
            int verticePadre = padre[verticeActual];
            int pesoArista = grafo.matrizAdyacencia[verticePadre][verticeActual];
            Arista arista(verticePadre, verticeActual, pesoArista);
            aristas.push_back(arista);
            verticeActual = verticePadre;
        }

        std::reverse(aristas.begin(), aristas.end());
    }

    return aristas;
}


//---------------------------------------------------------------//
//Funciones para visualización
void generarColoresAleatorios(int numVertices, std::vector<float>& vertexColorsR, std::vector<float>& vertexColorsG, std::vector<float>& vertexColorsB) {
    srand(static_cast<unsigned int>(time(nullptr)));

    for (int i = 0; i < numVertices; ++i) {
        float r, g, b;

        // Generar colores aleatorios en diferentes tonalidades
        if (i % 3 == 0) { // Rojo
            r = static_cast<float>(generarNumeroAleatorio(200, 255)) / 255.0f;
            g = static_cast<float>(generarNumeroAleatorio(0, 100)) / 255.0f;
            b = static_cast<float>(generarNumeroAleatorio(0, 100)) / 255.0f;
        } else if (i % 3 == 1) { // Azul
            r = static_cast<float>(generarNumeroAleatorio(0, 100)) / 255.0f;
            g = static_cast<float>(generarNumeroAleatorio(0, 100)) / 255.0f;
            b = static_cast<float>(generarNumeroAleatorio(200, 255)) / 255.0f;
        } else { // Verde
            r = static_cast<float>(generarNumeroAleatorio(0, 100)) / 255.0f;
            g = static_cast<float>(generarNumeroAleatorio(200, 255)) / 255.0f;
            b = static_cast<float>(generarNumeroAleatorio(0, 100)) / 255.0f;
        }

        vertexColorsR.push_back(r);
        vertexColorsG.push_back(g);
        vertexColorsB.push_back(b);
    }
}

void dibujarGrafo(GLFWwindow* window, Grafo& grafo, std::vector<Vertice>& vertices, std::vector<Arista>& aristas, int pasoActual, const std::vector<float>& vertexColorsR, const std::vector<float>& vertexColorsG, const std::vector<float>& vertexColorsB)  {
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, 0, 1);
    glMatrixMode(GL_MODELVIEW);

    // Dibujar el grafo original si mostrarOriginal es verdadero
    if (mostrarOriginal) {
        glColor3f(0.7f, 0.7f, 0.7f);  // Color gris claro

        glBegin(GL_LINES);
        for (int i = 0; i < grafo.numVertices; ++i) {
            for (int j = i + 1; j < grafo.numVertices; ++j) {
                if (grafo.matrizAdyacencia[i][j] != 0) {
                    float x1 = vertices[i].x;
                    float y1 = vertices[i].y;
                    float x2 = vertices[j].x;
                    float y2 = vertices[j].y;
                    glVertex2f(x1, y1);
                    glVertex2f(x2, y2);
                }
            }
        }
        glEnd();
    }

    // Dibujar vértices
    for (int i = 0; i < grafo.numVertices; ++i) {
        float x = vertices[i].x;
        float y = vertices[i].y;

        // Obtener el color correspondiente al vértice
        float r = vertexColorsR[i];
        float g = vertexColorsG[i];
        float b = vertexColorsB[i];
        glColor3f(r, g, b);

        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);

        float vertexRadius = 10.0f;
        int numCirclePoints = 100;
        for (int j = 0; j <= numCirclePoints; ++j) {
            float theta = static_cast<float>(j) / static_cast<float>(numCirclePoints) * 2 * 3.14159;
            float vx = x + vertexRadius * std::cos(theta);
            float vy = y + vertexRadius * std::sin(theta);
            glVertex2f(vx, vy);
        }

        glEnd();
    }

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);

    // Dibujar aristas
    for (int i = 0; i < pasoActual; ++i) {
        int origen = aristas[i].origen;
        int destino = aristas[i].destino;
        int peso = aristas[i].peso;

        // Calcular el grosor de la arista en función del peso
        float grosor = static_cast<float>(peso) / 100.0f * 2.0f;

        // Colores de las aristas
        glColor3f(0.0f, 0.0f, 0.0f);  // Color negro

        float x1 = vertices[origen].x;
        float y1 = vertices[origen].y;
        float x2 = vertices[destino].x;
        float y2 = vertices[destino].y;

        glLineWidth(grosor);
        glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();
    }

    glEnd();
}
//---------------------------------------------------------------//
//Callbacks GLFW
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_O && action == GLFW_PRESS) {
        mostrarOriginal = !mostrarOriginal;
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        auto* vertices = reinterpret_cast<std::vector<Vertice>*>(glfwGetWindowUserPointer(window));
        //Condicional para detectar el arrastre del mouse
        if (action == GLFW_PRESS) {
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);

            int windowWidth, windowHeight;
            glfwGetWindowSize(window, &windowWidth, &windowHeight);

            for (int i = 0; i < vertices->size(); ++i) {
                float vertexX = (*vertices)[i].x;
                float vertexY = (*vertices)[i].y;

                float distance = std::sqrt(std::pow(vertexX - mouseX, 2) + std::pow(vertexY - (windowHeight - mouseY), 2));

                if (distance <= 10.0f) {
                    (*vertices)[i].isDragging = true;
                    break;
                }
            }
        } else if (action == GLFW_RELEASE) {
            for (int i = 0; i < vertices->size(); ++i) {
                (*vertices)[i].isDragging = false;
            }
        }
    }
}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    auto* vertices = reinterpret_cast<std::vector<Vertice>*>(glfwGetWindowUserPointer(window));

    for (int i = 0; i < vertices->size(); ++i) {
        if ((*vertices)[i].isDragging) {
            int windowHeight;
            glfwGetWindowSize(window, nullptr, &windowHeight);
            (*vertices)[i].x = static_cast<float>(xpos);
            (*vertices)[i].y = static_cast<float>(windowHeight - ypos);
            break;
        }
    }
}
//---------------------------------------------------------------//
Algoritmo obtenerAlgoritmo() {
    std::cout << "Seleccione el algoritmo:\n";
    std::cout << "1. Kruskal\n";
    std::cout << "2. Dijkstra\n";

    int opcion = leerEntero();

    switch (opcion) {
        case 1:
            return Algoritmo::KRUSKAL;
        case 2:
            return Algoritmo::DIJKSTRA;
        default:
            std::cout << "Opción inválida. Seleccionando Kruskal por defecto.\n";
            return Algoritmo::KRUSKAL;
    }
}

std::vector<std::vector<int>> leerMatrizAdyacencia(int numVertices) {
    std::vector<std::vector<int>> matriz(numVertices, std::vector<int>(numVertices));

    std::cout << "Ingresa la matriz de adyacencia:\n";
    for (int i = 0; i < numVertices; ++i) {
        for (int j = 0; j < numVertices; ++j) {
            std::cin >> matriz[i][j];
        }
    }

    return matriz;
}
//---------------------------------------------------------------//
int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    //Inicia glfw
    if (!glfwInit()) {
        return -1;
    }
    //Se crea ventana "window"
    GLFWwindow* window = glfwCreateWindow(1000, 800, "Algoritmos", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //Entrada de datos
    std::cout << "Ingrese el numero de vertices del grafo: ";
    int numVertices = leerEntero();
    std::vector<std::vector<int>> matrizAdyacencia = leerMatrizAdyacencia(numVertices);

    Grafo grafo;
    grafo.numVertices = numVertices;
    grafo.matrizAdyacencia = matrizAdyacencia;

    std::vector<Vertice> vertices(numVertices);
    std::vector<Arista> aristas;
    //Selección del algoritmo a usar
    Algoritmo algoritmo = obtenerAlgoritmo();
    int origen, destino;
    if(algoritmo == Algoritmo::KRUSKAL) {
        aristas = kruskal(grafo);
    } else {
        std::cout << "Ingrese el origen: ";
        std::cin >> origen;
        std::cout << "Ingrese el destino: ";
        std::cin >> destino;
        aristas = dijkstra(grafo, origen - 1, destino - 1);
    }
    for (const Arista& arista : aristas) {
        std::cout << "Aristas:\nOrigen: " << arista.origen + 1 << ", Destino: " << arista.destino + 1 << ", Peso: " << arista.peso << std::endl;
    }
    // Inicializar las coordenadas iniciales de los vértices
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    float centerX = width / 2.0f;
    float centerY = height / 2.0f;
    float radius = (width < height) ? width / 3.0f : height / 3.0f;
    float angle = 2 * 3.14159 / numVertices;

    for (int i = 0; i < numVertices; ++i) {
        float x = centerX + radius * cos(i * angle);
        float y = centerY + radius * sin(i * angle);
        vertices[i].x = x;
        vertices[i].y = y;
        vertices[i].isDragging = false;
    }
    //Callbacks
    glfwSetWindowUserPointer(window, &vertices);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetKeyCallback(window, keyCallback);

    int pasoActual = 0;
    bool ejecutando = true;
    bool espacioPresionadoAnteriormente = false;
    bool rPresionadoAnteriormente = false;
    //Vectores para darle color a los vértices
    std::vector<float> vertexColorsR;
    std::vector<float> vertexColorsG;
    std::vector<float> vertexColorsB;
    generarColoresAleatorios(numVertices, vertexColorsR, vertexColorsG, vertexColorsB);
    //Bucle principal del programa
    while (!glfwWindowShouldClose(window) && ejecutando) {
        dibujarGrafo(window, grafo, vertices, aristas, pasoActual, vertexColorsR, vertexColorsG, vertexColorsB);

        glfwSwapBuffers(window);
        glfwPollEvents();

        //"espacio" para los pasos
        bool espacioPresionado = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
        if (espacioPresionado && !espacioPresionadoAnteriormente) {
            // Detectar transición de "no presionado" a "presionado"
            if (pasoActual < aristas.size()) {
                ++pasoActual;
            }
        }
        espacioPresionadoAnteriormente = espacioPresionado;
        if (pasoActual >= aristas.size()) {
            pasoActual = aristas.size();
        }

        //"r" para reiniciar el algoritmo
        bool rPresionado = glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS;
        if (rPresionado && !rPresionadoAnteriormente) {
            // Detectar transición de "no presionado" a "presionado"
            pasoActual = 0;
            aristas.clear();
            std::shuffle(vertices.begin(), vertices.end(), std::mt19937(std::random_device()()));

            if(algoritmo == Algoritmo::KRUSKAL) {
                aristas = kruskal(grafo);
            } else {
                aristas = dijkstra(grafo, origen, destino);
            }
        }
        rPresionadoAnteriormente = rPresionado;

        //"esc" para salir del programa
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            ejecutando = false;
        }
    }

    glfwTerminate();
    return 0;
}