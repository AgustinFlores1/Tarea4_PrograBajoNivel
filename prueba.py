import matplotlib.pyplot as plt
from gravitacion import sim

print("====================================================")
print("     ESCENARIO DE CAOS INMEDIATO: PLANETA CANÍBAL   ")
print("====================================================\n")

# --- 1. CONFIGURACIÓN EXTREMA PARA FORZAR COLISIONES ---
masa_estrella = 1.988e30          # Sol
masa_planeta_gigante = 1.898e27 * 50  # ¡50 veces Júpiter!
radio_cinturon = 1.0e12           # Radio del cinturón de asteroides
# Ponemos la órbita del planeta justo en el centro del cinturón
radio_orbita_planeta = radio_cinturon  

print(f"Planeta modificado: Masa x50 y colocado a {radio_orbita_planeta:.1e}m (dentro del cinturón).")

# Inicializar
sistema = sim(masa_estrella, masa_planeta_gigante, radio_orbita_planeta, 1000, radio_cinturon)

# --- MOMENTO 1: ESTADO INICIAL (Pocos días) ---
pasos_iniciales = 10
print(f"\nEjecutando estado inicial ({pasos_iniciales} días)...")
resultado_inicial = sistema.run(pasos_iniciales)
asteroides_iniciales = len(resultado_inicial) - 2
print(f"Asteroides iniciales vivos: {asteroides_iniciales}\n")

# --- MOMENTO 2: LA GRAN LIMPIEZA (10,000 días son suficientes ahora) ---
# Al estar tan cerca y ser tan masivo, 10,000 días (27 años) serán devastadores
pasos_largos = 10000 
print(f"Ejecutando evolución caótica por {pasos_largos} días más...")
resultado_final = sistema.run(pasos_largos)
print("¡Simulación completada con éxito!\n")

# 2. Análisis numérico de colisiones
estrella = resultado_final[0]
planeta = resultado_final[1]
asteroides_finales = len(resultado_final) - 2
asteroides_destruidos = asteroides_iniciales - asteroides_finales

print("--- INFORME DE DAÑOS Y COLISIONES ---")
print(f"Asteroides sobrevivientes: {asteroides_finales}")
print(f"Asteroides eliminados (chocaron o escaparon): {asteroides_destruidos}")
print(f"Porcentaje de destrucción: {(asteroides_destruidos / asteroides_iniciales) * 100:.1f}%\n")

# 3. GENERAR GRÁFICO COMPARATIVO (Caos visual)
print("Generando gráfico del desastre orbital...")
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 7))

# --- Subplot 1: Estado Inicial (Orden) ---
ax1.scatter(resultado_inicial[0].x, resultado_inicial[0].y, color='gold', s=150, edgecolors='black', label='Sol')
# Graficar el planeta gigante (más grande visualmente)
ax1.scatter(resultado_inicial[1].x, resultado_inicial[1].y, color='firebrick', s=200, label='Planeta Gigante')
ast_init_x = [resultado_inicial[i].x for i in range(2, len(resultado_inicial))]
ast_init_y = [resultado_inicial[i].y for i in range(2, len(resultado_inicial))]
ax1.scatter(ast_init_x, ast_init_y, color='gray', s=3, alpha=0.5, label='Asteroides')
ax1.set_title(f"Día {pasos_iniciales}: Orden antes del caos ({asteroides_iniciales} vivos)")
ax1.grid(True, linestyle=':', alpha=0.5)
ax1.legend()

# --- Subplot 2: Estado Final (Caos y Colisiones) ---
ax2.scatter(estrella.x, estrella.y, color='gold', s=150, edgecolors='black', label='Sol')
ax2.scatter(planeta.x, planeta.y, color='firebrick', s=200, label='Planeta Gigante')
ast_fin_x = [resultado_final[i].x for i in range(2, len(resultado_final))]
ast_fin_y = [resultado_final[i].y for i in range(2, len(resultado_final))]
# Pintar los sobrevivientes en rojo oscuro para mostrar drama
ax2.scatter(ast_fin_x, ast_fin_y, color='darkred', s=4, alpha=0.7, label='Sobrevivientes Dispersos')
ax2.set_title(f"Día {pasos_iniciales + pasos_largos}: Tras la Limpieza ({asteroides_finales} vivos)")
ax2.grid(True, linestyle=':', alpha=0.5)
ax2.legend()

# Ajustes estéticos globales
limite_vista = radio_cinturon * 1.5
for ax in (ax1, ax2):
    ax.axhline(0, color='black', linewidth=0.5, linestyle='--')
    ax.axvline(0, color='black', linewidth=0.5, linestyle='--')
    ax.set_xlabel("Posición X (metros)")
    ax.set_ylabel("Posición Y (metros)")
    ax.set_xlim(-limite_vista, limite_vista)
    ax.set_ylim(-limite_vista, limite_vista)

plt.tight_layout()
plt.savefig("caos_planetario.png")
print("¡Gráfico del caos guardado como 'caos_planetario.png'!")