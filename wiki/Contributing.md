# Contribuir al Proyecto

## Bienvenido

¡Gracias por tu interés en contribuir al Mini-Kernel ARMv7-A! Este proyecto educativo se beneficia de las contribuciones de estudiantes, desarrolladores y entusiastas de sistemas embebidos.

## Código de Conducta

### Nuestros Valores

- **Respeto**: Tratamos a todos con respeto, independientemente de su nivel de experiencia
- **Aprendizaje**: Fomentamos un ambiente de aprendizaje colaborativo
- **Calidad**: Nos esforzamos por mantener código limpio y bien documentado
- **Inclusividad**: Todos son bienvenidos a contribuir y aprender

### Comportamiento Esperado

- Usar lenguaje respetuoso e inclusivo
- Ser constructivo en las críticas y sugerencias
- Ayudar a otros miembros de la comunidad
- Enfocarse en lo que es mejor para el proyecto educativo

## Tipos de Contribuciones

### 📝 Documentación

- Mejorar la documentación existente
- Agregar ejemplos de código
- Crear tutoriales paso a paso
- Traducir documentación a otros idiomas
- Corregir errores de ortografía/gramática

### 🐛 Reportes de Errores

- Reportar bugs en el código
- Documentar problemas de compilación
- Identificar problemas de compatibilidad
- Reportar errores en la documentación

### 💡 Nuevas Características

- Implementar nuevos drivers
- Agregar funcionalidades al kernel
- Mejorar el scheduler
- Implementar nuevas syscalls
- Agregar herramientas de debugging

### 🧪 Testing

- Escribir unit tests
- Crear test cases de integración
- Validar en diferentes plataformas
- Performance testing
- Documentar procedimientos de testing

## Configuración del Entorno

### Prerequisitos

```bash
# Ubuntu/Debian
sudo apt update
sudo apt install gcc-arm-none-eabi qemu-system-arm build-essential git

# macOS
brew install arm-none-eabi-gcc qemu git

# Verificar instalación
arm-none-eabi-gcc --version
qemu-system-arm --version
```

### Fork y Clone

```bash
# 1. Hacer fork del repositorio en GitHub

# 2. Clonar tu fork
git clone https://github.com/TU-USUARIO/armv7a-minikernel.git
cd armv7a-minikernel

# 3. Agregar upstream remote
git remote add upstream https://github.com/ebelmonteutn/armv7a-minikernel.git

# 4. Verificar remotes
git remote -v
```

### Build y Test

```bash
# Compilar el proyecto
make clean && make

# Ejecutar tests básicos
make run UART=1

# Verificar que funciona
# Deberías ver output de las tareas
```

## Proceso de Contribución

### 1. Planificación

#### Issues

Antes de comenzar a trabajar:

1. **Revisar issues existentes**: Verifica si ya hay un issue relacionado
2. **Crear nuevo issue**: Si no existe, crea uno describiendo:
   - Problema a resolver
   - Solución propuesta
   - Impacto en el proyecto

#### Discusión

- Comenta en el issue para indicar que trabajarás en él
- Discute el enfoque con los maintainers
- Espera feedback antes de comenzar trabajo extenso

### 2. Desarrollo

#### Crear Branch

```bash
# Actualizar main
git checkout main
git pull upstream main

# Crear branch para tu feature/fix
git checkout -b feature/nueva-funcionalidad
# o
git checkout -b fix/corregir-bug
```

#### Convenciones de Naming

**Branches:**
- `feature/descripcion-corta`: Nuevas características
- `fix/descripcion-bug`: Corrección de bugs
- `docs/tema`: Mejoras de documentación
- `refactor/area`: Refactoring de código

**Commits:**
- `feat: agregar soporte para DMA`
- `fix: corregir stack overflow en tarea2`
- `docs: actualizar guía de instalación`
- `test: agregar tests para driver UART`
- `refactor: simplificar inicialización GIC`

### 3. Estándares de Código

#### Estilo C

```c
// Usar estilo K&R con modificaciones
void funcion_ejemplo(uint32_t parametro) {
    if (parametro > 0) {
        // Indentar con 4 espacios
        hacer_algo();
    }
}

// Nombrar variables en snake_case
uint32_t variable_ejemplo = 0;
static uint32_t contador_global = 0;

// Nombrar constantes en UPPER_CASE
#define MAX_TASKS 4
#define BUFFER_SIZE 256

// Comentarios Doxygen para APIs públicas
/*!
 * @brief   Descripción breve de la función
 * @param   parametro Descripción del parámetro
 * @return  Descripción del valor de retorno
 */
uint32_t funcion_publica(uint32_t parametro);
```

#### Estilo Assembly

```assembly
; Comentarios con punto y coma
; Usar mayúsculas para instrucciones
; Indentar operandos consistentemente

funcion_asm:
    STMFD SP!, {R0-R3, LR}    ; Comentario descriptivo
    
    MOV R0, #0                ; Inicializar registro
    BL otra_funcion           ; Llamar función
    
    LDMFD SP!, {R0-R3, PC}    ; Retornar
```

#### Headers

```c
/*!
 * @file    nombre_archivo.h
 * @brief   Descripción breve del archivo
 * @author  Tu Nombre <email@ejemplo.com>
 * @date    2025-01-XX
 */
#ifndef NOMBRE_ARCHIVO_H_
#define NOMBRE_ARCHIVO_H_

#include <stdint.h>

// Contenido del header

#endif // NOMBRE_ARCHIVO_H_
```

### 4. Testing

#### Tests Requeridos

```bash
# 1. Compilación sin warnings
make clean && make

# 2. Test básico de ejecución
timeout 30s make run UART=1 > test_output.txt 2>&1 &
sleep 10
pkill qemu

# Verificar que las tareas funcionan
grep -q "Fibonacci" test_output.txt || echo "FAIL: Tarea 1 no funciona"
grep -q "Collatz" test_output.txt || echo "FAIL: Tarea 2 no funciona"
grep -q "Factorización" test_output.txt || echo "FAIL: Tarea 3 no funciona"
```

#### Tests Específicos

Para cambios en drivers:
```bash
# Test UART
make test-uart

# Test Timer  
make test-timer

# Test GIC
make test-interrupts
```

### 5. Documentación

#### Documentar Cambios

- **README.md**: Si cambias build process o requirements
- **Wiki**: Para nuevas características o cambios significativos
- **Comentarios de código**: Para lógica compleja
- **Changelog**: Resumir cambios importantes

#### Ejemplo de Documentación

```c
/*!
 * @brief   Inicializa el controlador DMA
 * 
 * Esta función configura el controlador DMA con los parámetros
 * por defecto y habilita los canales especificados.
 * 
 * @param   channels Máscara de bits de canales a habilitar (1-8)
 * @return  0 en éxito, -1 si error de configuración
 * 
 * @note    Debe llamarse después de gic_init()
 * @warning No thread-safe, llamar solo durante inicialización
 * 
 * @example
 * ```c
 * // Habilitar canales 1 y 3
 * if (dma_init(0x05) != 0) {
 *     my_printf("Error inicializando DMA\n");
 * }
 * ```
 */
int dma_init(uint32_t channels);
```

### 6. Pull Request

#### Antes de Crear PR

```bash
# Actualizar tu branch con upstream
git fetch upstream
git checkout main
git merge upstream/main
git checkout tu-branch
git rebase main

# Verificar que todo compila y funciona
make clean && make
make run  # Test manual

# Revisar cambios
git log --oneline upstream/main..HEAD
git diff upstream/main..HEAD
```

#### Template de PR

```markdown
## Descripción

Breve descripción de los cambios realizados.

## Tipo de Cambio

- [ ] Bug fix (cambio que corrige un issue)
- [ ] Nueva característica (cambio que agrega funcionalidad)
- [ ] Breaking change (fix/feature que causa incompatibilidad)
- [ ] Documentación (cambios solo en documentación)

## ¿Cómo Ha Sido Probado?

Describe las pruebas que realizaste:

- [ ] Tests unitarios pasan
- [ ] Test de integración pasa
- [ ] Probado en QEMU
- [ ] Probado en hardware real (si aplica)

## Checklist:

- [ ] Mi código sigue las convenciones del proyecto
- [ ] He realizado self-review de mi código  
- [ ] He comentado áreas complejas
- [ ] He actualizado la documentación correspondiente
- [ ] Mis cambios no generan nuevos warnings
- [ ] He agregado tests que prueban mi funcionalidad
- [ ] Tests nuevos y existentes pasan

## Screenshots/Logs (si aplica)

```

## Revisión de Código

### Para Reviewers

#### Qué Revisar

1. **Funcionalidad**
   - ¿El código hace lo que dice que hace?
   - ¿Maneja correctamente casos edge?
   - ¿Es thread-safe si es necesario?

2. **Calidad**
   - ¿El código es legible y mantenible?
   - ¿Hay duplicación innecesaria?
   - ¿Se siguen las convenciones del proyecto?

3. **Performance**
   - ¿Hay optimizaciones obvias?
   - ¿El uso de memoria es razonable?
   - ¿Impacta negativamente el tiempo de boot?

4. **Seguridad**
   - ¿Hay buffer overflows potenciales?
   - ¿Se validan correctamente las entradas?
   - ¿Hay race conditions?

#### Dar Feedback Constructivo

```markdown
# ❌ No hacer
"Este código está mal"

# ✅ Hacer  
"Considerar usar `uart_puts()` en lugar de loop manual para mejor performance y legibilidad"

# ✅ Sugerir alternativas
"Alternativa: podríamos usar un lookup table aquí para evitar el cálculo runtime"

# ✅ Hacer preguntas
"¿Qué pasa si `buffer` es NULL aquí? ¿Deberíamos agregar una validación?"
```

### Para Contributors

#### Responder a Reviews

- **Ser receptivo**: Todas las sugerencias son oportunidades de aprender
- **Hacer preguntas**: Si no entiendes una sugerencia, pregunta
- **Explicar decisiones**: Si no implementas una sugerencia, explica por qué
- **Agradecer**: Reconoce el tiempo que invirtieron en revisar tu código

## Mantenimiento del Proyecto

### Roles

#### Maintainers

- **Enzo Belmonte** (Owner): Decisiones arquitecturales, releases
- **Contributors regulares**: Review de PRs, triage de issues

#### Como Convertirse en Maintainer

1. Contribuir consistentemente por 3+ meses
2. Demostrar buen juicio técnico  
3. Ayudar con reviews y triage de issues
4. Ser propuesto por maintainer existente

### Release Process

#### Versionado

Usamos [Semantic Versioning](https://semver.org/):

- **MAJOR**: Cambios incompatibles en API
- **MINOR**: Nueva funcionalidad backward-compatible  
- **PATCH**: Bug fixes backward-compatible

Ejemplos:
- `v1.0.0`: Release inicial
- `v1.1.0`: Agregar soporte DMA
- `v1.1.1`: Fix bug en driver UART

#### Proceso de Release

1. **Preparación**
   ```bash
   git checkout main
   git pull upstream main
   
   # Actualizar CHANGELOG.md
   # Actualizar version en defines.h
   git commit -m "chore: prepare v1.1.0 release"
   ```

2. **Tag y Release**
   ```bash
   git tag -a v1.1.0 -m "Release v1.1.0"
   git push upstream v1.1.0
   ```

3. **GitHub Release**
   - Crear release en GitHub
   - Incluir changelog
   - Subir archivos binarios si aplica

## Recursos para Contributors

### Documentación Técnica

- [Architecture](Architecture.md): Entender la arquitectura del sistema
- [API Reference](API-Reference.md): APIs disponibles
- [Build Guide](Build-and-Development.md): Configuración de desarrollo
- [Debugging Guide](Debugging-and-Troubleshooting.md): Técnicas de debug

### Herramientas Útiles

```bash
# Static analysis
cppcheck --enable=all src/

# Code formatting (si implementamos)
clang-format -i src/**/*.c

# Git hooks útiles
cp scripts/pre-commit.sh .git/hooks/pre-commit
chmod +x .git/hooks/pre-commit
```

### Recursos de Aprendizaje

#### ARM Architecture
- [ARM Developer Documentation](https://developer.arm.com/documentation/)
- [Cortex-A8 Technical Reference Manual](https://developer.arm.com/documentation/ddi0344/)

#### Embedded Systems
- [Making Embedded Systems](https://www.oreilly.com/library/view/making-embedded-systems/9781449308889/)
- [The Definitive Guide to ARM Cortex-A](https://www.arm.com/resources/ebook/definitive-guide-arm-cortex-a)

#### Git y GitHub
- [Git Handbook](https://guides.github.com/introduction/git-handbook/)
- [Contributing to Open Source](https://opensource.guide/how-to-contribute/)

## FAQ para Contributors

### ¿Qué necesito saber antes de contribuir?

**Conocimientos básicos:**
- C programming
- Assembly ARM (básico)
- Git fundamentals
- Linux/Unix command line

**Conocimientos avanzados (para features complejas):**
- ARM architecture
- Embedded systems
- Operating system concepts

### ¿Cómo elijo qué trabajar?

1. **Para principiantes**: Look for issues labeled `good first issue`
2. **Documentación**: Always needed, great starting point
3. **Bug fixes**: Usually bien delimitados
4. **Features**: Discutir primero en issues

### ¿Cuánto tiempo toma un review?

- **Bug fixes simples**: 1-3 días
- **Nuevas features**: 1-2 semanas  
- **Cambios grandes**: 2-4 semanas

### ¿Puedo trabajar en múltiples PRs simultáneamente?

Sí, pero recomendamos:
- Máximo 2-3 PRs activos por persona
- Terminar PRs existentes antes de nuevos grandes
- Comunicar si necesitas help

## Contacto

### Canales de Comunicación

- **GitHub Issues**: Para bugs y feature requests
- **GitHub Discussions**: Para preguntas y discusiones generales
- **Email**: ebelmonte@frba.utn.edu.ar (para temas sensibles)

### Tiempo de Respuesta

- **Issues críticos**: 24-48 horas
- **PRs y issues normales**: 3-7 días
- **Emails**: 1-2 semanas

---

¡Gracias por contribuir al Mini-Kernel ARMv7-A! Cada contribución, sin importar su tamaño, ayuda a hacer este proyecto mejor para toda la comunidad educativa.