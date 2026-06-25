import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from gravitacion import sim

print("====================================================")
print("    SIMULACION DE CUERPOS CELESTES   ")
print("====================================================\n")

def pedir_numero(mensaje, tipo=float):
    while True:
        try:
            return tipo(input(mensaje))
        except ValueError:
            print("❌ Entrada inválida. Por favor, use números o notación científica (ej: 7.5e11 o 5000).\n")


masa_estrella = pedir_numero("Entregame la masa de la estrella\n> ")         
masa_planeta = pedir_numero("\nEntregame la masa del planeta\n> ")
radio_orbita_planeta = pedir_numero("\nQue tan grande es el radio de orbita del planeta?\n(Que tan alejado del sol se encunetra) > ")
numero_asteroides = pedir_numero("\nEntregame el numero de asteroides dentro del disco\n> ", tipo=int)
radio_disco_asteroides = pedir_numero("\nQue tan grande es el radio del disco de asteroides?\n(Que tan alejados del sol se encuentran) > ") 

sistema = sim(masa_estrella, masa_planeta, radio_orbita_planeta, numero_asteroides, radio_disco_asteroides)

fig, ax = plt.subplots(figsize=(12, 12))

estrella_gf = ax.scatter([], [], color='darkgoldenrod', s=200, edgecolors='black', label='Estrella')
planeta_gf = ax.scatter([], [], color='red', s=100, label='Planeta')
asteroides_gf = ax.scatter([], [], color='blue', s=4, alpha=0.6, label='Asteroides')

if radio_disco_asteroides >= radio_orbita_planeta:
    limite_grafico = radio_disco_asteroides * 2.0
else: 
    limite_grafico = radio_orbita_planeta * 2.0

ax.set_xlim(-limite_grafico, limite_grafico)
ax.set_ylim(-limite_grafico, limite_grafico)
ax.axhline(0, color='black', linewidth=0.5, linestyle='--')
ax.axvline(0, color='black', linewidth=0.5, linestyle='--')
ax.set_xlabel("X (metros)")
ax.set_ylabel("Y (metros)")
ax.grid(True, linestyle=':', alpha=0.5)
ax.legend(loc="upper right")

informacion = ax.text(
    0.02, 0.95, '', transform=ax.transAxes, fontsize=12, bbox=dict(facecolor='white', alpha=0.7, edgecolor='gray')
)

pasos_de_tiempo = pedir_numero("\nIngrese la cantidad de dias totales a simular\n> ", tipo=int)
pasos_por_frame = 5
frames_totales = pasos_de_tiempo // pasos_por_frame

def actualizar(frame):
    sistema_actualizado = sistema.run(pasos_por_frame)

    coor_estrella = [(sistema_actualizado[0].x, sistema_actualizado[0].y)]
    coor_planeta = [(sistema_actualizado[1].x, sistema_actualizado[1].y)]
    coor_asteroides = []
    for i in range(2, len(sistema_actualizado)):
        coor_asteroides.append((sistema_actualizado[i].x, sistema_actualizado[i].y))

    if not coor_asteroides:
        coor_asteroides = [(float('nan'), float('nan'))]
    
    estrella_gf.set_offsets(coor_estrella)
    planeta_gf.set_offsets(coor_planeta)
    asteroides_gf.set_offsets(coor_asteroides)

    informacion.set_text(f"Días simulados: {frame * pasos_por_frame + 5} / {pasos_de_tiempo}\nAsteroides vivos: {len(sistema_actualizado) - 2}")
    
    return estrella_gf, planeta_gf, asteroides_gf, informacion

animation = FuncAnimation(fig, actualizar, frames=frames_totales, interval=20, blit=True, repeat=False, cache_frame_data=False)

plt.title("Simulacion de Cuerpos Celestes")
plt.show()