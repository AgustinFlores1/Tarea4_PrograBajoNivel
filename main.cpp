#include <iostream>
#include <vector>
#include <iomanip> // Para ordenar la salida en consola
#include "sim.h"   // Asumiendo que tu clase 'sim' está en este encabezado

int main() {
    // === 1. CONFIGURACIÓN DE PARÁMETROS INICIALES ===
    double masa_estrella = 1.988e30;  // Masa del Sol (kg)
    double masa_planeta  = 1.898e27;  // Masa de la Tierra (kg)
    double radio_orbita_planeta = 7.78e11; // 1 Unidad Astronómica (distancia Tierra-Sol en metros)
    int cantidad_asteroides = 1000;
    double radio_cinturon_asteroides = 1.0e12; // Un poco más lejos que el planeta

    std::cout << "====================================================\n";
    std::cout << " INICIALIZANDO SIMULACIÓN DEL SISTEMA PLANETARIO\n";
    std::cout << "====================================================\n\n";

    // === 2. CREACIÓN DEL OBJETO DE SIMULACIÓN ===
    sim mi_sistema(masa_estrella, masa_planeta, radio_orbita_planeta, cantidad_asteroides, radio_cinturon_asteroides);

    // Configuración de formato para mostrar números grandes de forma legible
    std::cout << std::fixed << std::setprecision(2);

    // === 3. PRIMERA CORRIDA: SIMULAR 180 DÍAS (Medio año) ===
    int pasos_1 = 180;
    std::cout << "Ejecutando primera corrida por " << pasos_1 << " días...\n";
    std::vector<cuerpo> estado_medio_ano = mi_sistema.run(pasos_1);

    std::cout << "\n--- ESTADO A LOS 180 DÍAS ---\n";
    std::cout << "Estrella - Posicion: (" << estado_medio_ano[0].x << ", " << estado_medio_ano[0].y << ") | Vel: (" << estado_medio_ano[0].vx << ", " << estado_medio_ano[0].vy << ")\n";
    std::cout << "Planeta  - Posicion: (" << estado_medio_ano[1].x << ", " << estado_medio_ano[1].y << ") | Vel: (" << estado_medio_ano[1].vx << ", " << estado_medio_ano[1].vy << ")\n";
    std::cout << "Asteroides vivos: " << estado_medio_ano.size() - 2 << "\n";

    std::cout << "\n----------------------------------------------------\n\n";

    // === 4. SEGUNDA CORRIDA CONSECUTIVA: SIMULAR 185 DÍAS MÁS ===
    // Esto probará que tu simulación realmente continúa desde donde quedó
    int pasos_2 = 10000;
    std::cout << "Ejecutando segunda corrida por " << pasos_2 << " días más (Total: 365 días)...\n";
    std::vector<cuerpo> estado_ano_completo = mi_sistema.run(pasos_2);

    std::cout << "\n--- ESTADO AL AÑO COMPLETO (365 DÍAS) ---\n";
    std::cout << "Estrella - Posicion: (" << estado_ano_completo[0].x << ", " << estado_ano_completo[0].y << ")\n";
    std::cout << "Planeta  - Posicion: (" << estado_ano_completo[1].x << ", " << estado_ano_completo[1].y << ")\n";
    std::cout << "Asteroides vivos: " << estado_ano_completo.size() - 2 << "\n";
    
    // Mostramos qué pasó con cada asteroide
    std::cout << "\nEstado de los asteroides restantes:\n";
    if (estado_ano_completo.size() <= 2) {
        std::cout << "Todos los asteroides chocaron o escaparon del sistema.\n";
    } else {
        for (size_t i = 2; i < estado_ano_completo.size(); i++) {
            std::cout << " Asteroide [" << i - 1 << "] - Posicion: (" 
                      << estado_ano_completo[i].x << ", " << estado_ano_completo[i].y 
                      << ") | Vel: (" << estado_ano_completo[i].vx << ", " << estado_ano_completo[i].vy << ")\n";
        }
    }

    std::cout << "\n====================================================\n";
    std::cout << "            PRUEBA DE SIMULACIÓN FINALIZADA         \n";
    std::cout << "====================================================\n";

    return 0;
}