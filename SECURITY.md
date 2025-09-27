# Política de Seguridad

## Versiones Soportadas

Este proyecto es un mini-kernel educativo desarrollado para la materia Sistemas Digitales II de la UTN.BA. Las siguientes versiones están actualmente soportadas con actualizaciones de seguridad:

| Versión | Soportada          |
| ------- | ------------------ |
| main    | :white_check_mark: |

## Reportar una Vulnerabilidad de Seguridad

Si descubres una vulnerabilidad de seguridad en este proyecto, por favor repórtala de manera responsable:

### Proceso de Reporte

1. **NO** abras un issue público para vulnerabilidades de seguridad
2. Envía un email directamente al mantenedor: `ebelmonte@frba.utn.edu.ar`
3. Incluye en tu reporte:
   - Descripción detallada de la vulnerabilidad
   - Pasos para reproducir el problema
   - Impacto potencial
   - Posibles soluciones o mitigaciones

### Tiempo de Respuesta

- Reconocimiento inicial: 48 horas
- Evaluación completa: 1 semana
- Resolución: Dependiendo de la severidad, entre 1-4 semanas

## Consideraciones de Seguridad para Sistemas Bare-Metal

### Limitaciones Inherentes

Este mini-kernel está diseñado con fines educativos y presenta las siguientes limitaciones de seguridad:

- **Sin aislamiento de memoria**: Las tareas no tienen protección de memoria entre sí
- **Sin autenticación**: No existe control de acceso o autenticación de usuarios
- **Sin cifrado**: Los datos se transmiten y almacenan sin cifrado
- **Scheduler cooperativo**: Las tareas maliciosas pueden afectar todo el sistema

### Vectores de Ataque Conocidos

1. **Desbordamiento de buffer**: Los arrays de tamaño fijo pueden ser vulnerables
2. **Acceso directo a hardware**: Las tareas pueden acceder directamente a registros del hardware
3. **Corrupción de pila**: Sin protección MPU, las pilas pueden ser corrompidas
4. **Inyección de código**: Sin ASLR ni DEP en el entorno emulado

### Entorno de Ejecución

- **Emulación QEMU**: El kernel se ejecuta en un entorno emulado (realview-pb-a8)
- **Sin MMU**: No hay unidad de manejo de memoria para protección
- **Modo privilegiado**: Todo el código se ejecuta en modo privilegiado

## Mejores Prácticas de Seguridad

### Para Desarrolladores

1. **Validación de entrada**: Siempre valida los parámetros de entrada
2. **Límites de buffer**: Verifica los límites de arrays y strings
3. **Inicialización de variables**: Inicializa todas las variables antes del uso
4. **Manejo de errores**: Implementa manejo adecuado de condiciones de error

### Para Contribuidores

1. **Revisión de código**: Todo código debe ser revisado antes de ser integrado
2. **Pruebas de seguridad**: Considera casos de uso maliciosos al escribir pruebas
3. **Documentación**: Documenta las implicaciones de seguridad de nuevas características
4. **Seguimiento de dependencias**: Mantén actualizadas las herramientas de desarrollo

## Contexto Educativo

Este proyecto está destinado para:
- Aprendizaje de conceptos de sistemas operativos
- Comprensión de arquitectura ARMv7-A
- Experimentación con programación bare-metal

**ADVERTENCIA**: Este kernel NO debe ser usado en entornos de producción o sistemas críticos de seguridad.

## Recursos Adicionales

- [ARM Security Documentation](https://developer.arm.com/documentation/security)
- [OWASP Embedded Application Security](https://owasp.org/www-project-embedded-application-security/)
- [NIST Cybersecurity Framework](https://www.nist.gov/cyberframework)

## Contacto

Para preguntas relacionadas con seguridad que no sean vulnerabilidades críticas, puedes:
- Abrir un issue en el repositorio
- Contactar al autor: Enzo Belmonte (`ebelmonte@frba.utn.edu.ar`)

---

*Última actualización: Enero 2025*