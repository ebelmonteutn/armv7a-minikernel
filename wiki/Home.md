# Mini-Kernel ARMv7-A - Wiki

Bienvenido al wiki del Mini-Kernel ARMv7-A, un sistema operativo bare-metal educativo diseñado para la arquitectura ARM Cortex-A8.

## 🎯 Propósito del Proyecto

Este proyecto fue desarrollado como parte de la materia **Sistemas Digitales II** de la UTN.BA para demostrar los conceptos fundamentales de:

- Programación de sistemas a bajo nivel
- Arquitectura de procesadores ARM
- Desarrollo de kernels bare-metal
- Manejo de interrupciones y excepciones
- Scheduler de tareas cooperativo
- API de llamadas al sistema

## 🚀 Inicio Rápido

1. **[Configuración del Entorno](Build-and-Development.md)** - Configura las herramientas necesarias
2. **[Compilación](Build-and-Development.md#compilación)** - Compila y ejecuta el kernel
3. **[Debugging](Debugging-and-Troubleshooting.md)** - Aprende a depurar el kernel

## 📚 Documentación Principal

### Arquitectura y Diseño
- **[Arquitectura del Sistema](Architecture.md)** - Visión general de la arquitectura ARMv7-A
- **[Gestión de Memoria](Memory-Management.md)** - Mapa de memoria y gestión
- **[Capa de Abstracción de Hardware](Hardware-Abstraction-Layer.md)** - HAL del proyecto

### Servicios del Kernel
- **[Servicios del Kernel](Kernel-Services.md)** - Syscalls, scheduler e interrupciones
- **[Gestión de Tareas](Task-Management.md)** - Sistema de tareas y planificación
- **[API Reference](API-Reference.md)** - Referencia completa de la API

### Desarrollo
- **[Build y Desarrollo](Build-and-Development.md)** - Guía completa de desarrollo
- **[Debugging y Troubleshooting](Debugging-and-Troubleshooting.md)** - Resolución de problemas
- **[Contribuir](Contributing.md)** - Cómo contribuir al proyecto

### Referencia
- **[Glosario](Glossary.md)** - Términos técnicos y definiciones

## 🔧 Características Principales

- **Arquitectura ARMv7-A**: Optimizado para ARM Cortex-A8
- **Bare-Metal**: Sin dependencias de SO
- **Scheduler Cooperativo**: Planificación simple de tareas
- **Manejo de Excepciones**: IRQ, SVC, y otros handlers
- **API de Sistema**: Llamadas al sistema via SVC
- **Emulación QEMU**: Ejecuta en QEMU realview-pb-a8

## 📊 Estado del Proyecto

- ✅ **Bootloader**: Completo y funcional
- ✅ **Memory Management**: Layout básico implementado
- ✅ **Exception Handlers**: IRQ y SVC handlers
- ✅ **Task Scheduler**: Scheduler cooperativo básico
- ✅ **System Calls**: API básica de syscalls
- ✅ **UART Communication**: Comunicación serie funcional
- 🔄 **Timer Support**: Funcionalidad básica
- 📋 **Documentación**: En desarrollo

## 👨‍💻 Autor

**Enzo Belmonte**  
Estudiante de Ingeniería Electrónica, UTN.BA  
📧 ebelmonte@frba.utn.edu.ar

## 📄 Licencia

Este proyecto está licenciado bajo la Licencia MIT. Ver el archivo [LICENSE](../LICENSE) para más detalles.