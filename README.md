# Moonlight Engine
**Moonlight Engine** es un motor gráfico y de juego desarrollado en **C++**, diseñado para ofrecer una arquitectura modular y un flujo de trabajo similar al de motores profesionales como Unity o Unreal Engine.  
El proyecto forma parte de la asignatura **Motores de Videojuegos** y tiene como objetivo crear una base funcional para el desarrollo de escenas interactivas y herramientas de edición.

**Repositorio del proyecto:** [https://github.com/Deivid13SB/MotorsVideojocs](https://github.com/Deivid13SB/MotorsVideojocs)

---

## Integrantes del grupo

| Nombre | GitHub |
|:--------|:--------|
| David Subirats Bonet | [@Deivid13SB](https://github.com/Deivid13SB) |
| Mario Torrents Rodríguez | [@MarioTorrentsRodriguez](https://github.com/MarioTorrentsRodriguez) |

---

## Cómo usar el motor

### Ejecución
1. Clona el repositorio y abre el proyecto en **Visual Studio 2022**.  
2. Compila en modo **Debug** o **Release** según se desee.  
3. Ejecuta el motor desde el editor o desde el ejecutable generado.

### Controles básicos
- **W / A / S / D** → Mover la cámara  
- **Shift Izquierdo** → Aumentar velocidad de movimiento  
- **Botón derecho del ratón** → Rotar cámara  
- **Rueda del ratón** → Zoom in / out  
- **Espacio** → Reproducir / Pausar la escena  
- **Ctrl + S** → Guardar la escena actual  
- **Ctrl + O** → Cargar una escena existente  
- **Alt + Enter** → Cambiar a pantalla completa  

### Interfaz del Editor
- **Panel de Jerarquía:** muestra todos los GameObjects de la escena  
- **Inspector:** permite modificar componentes (Transform, Mesh, Material, etc.)  
- **Viewport:** vista 3D principal para editar y visualizar la escena  
- **Console:** muestra logs, errores y advertencias en tiempo real  

---

## Funcionalidades extra

Además de los requisitos base, el motor incluye varias características adicionales:

**Sistema de materiales y shaders personalizados** (edición y recarga en tiempo real)  
**Cámara libre con control de velocidad ajustable**  
**Guardado y carga de escenas en formato JSON**  
**Iluminación dinámica y sombras básicas**  
**Soporte para modelos FBX y texturas PNG/JPG**  
**Panel de configuración del motor** (vsync, resolución, framerate, etc.)  
**Gestor de tiempo** con deltaTime y contador de FPS en pantalla  

---

## Comentarios para el profesor

- Los módulos del motor siguen una arquitectura **modular** (Window, Renderer, Input, Textures, Models, etc.)  
- El sistema de carga de assets es **asíncrono** y fácilmente ampliable  
- El **editor** utiliza **ImGui** para la creación de herramientas y menús de depuración  
- Los módulos ModuleRenderer y ModuleCamera incluyen funcionalidades extendidas para renderizado avanzado  
- La jerarquía de objetos y el sistema de componentes están parcialmente implementados para futuras expansiones  

---

## Licencia

Este proyecto se distribuye bajo la licencia **MIT**, permitiendo su uso y modificación con fines educativos.
