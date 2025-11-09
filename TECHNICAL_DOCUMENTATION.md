# Documentación técnica — Proyecto de POO

> Archivo: `TECHNICAL_DOCUMENTATION.md`
> Propósito: Documentación técnica completa del sistema, diseño, contratos y diagrama UML (texto) para el proyecto C++.

## Resumen
Este repositorio implementa una aplicación de consola en C++17 para la gestión de productos, usuarios y ventas. Contiene:
- Módulos de dominio: `Product` (y subclases `Electronic`, `Book`), `Sale`, `User`.
- Gestión: `Gestor` (repositorio de productos), `UserManager`, `SalesManager`.
- Interfaz de usuario: `Menu` con selector interactivo (flechas + Enter) Windows-first y fallback a entrada numérica.
- Utilidades de consola: `UTILITIES.h` (lectura de teclas, resaltado con ANSI/WinAPI, limpiezas) y `TableFormat.h` (anchos de columnas y helpers de impresión).

El sistema prioriza compatibilidad con Windows (PowerShell/cmd.exe) y mantiene un modo de entrada numérica cuando la terminal no soporta lecturas de tecla única.

## Quick start (compilar y ejecutar)
Desde la raíz del proyecto (donde está `main.cpp`) en PowerShell o cmd.exe:

```powershell
cd "c:\Users\triya\Documents\CUCEI\3 SEMESTRE\POO\Proyecto de POO"
# Compilar con g++ (instalar MinGW o GCC apropiado en Windows)
g++ -std=c++17 -Iinclude main.cpp src/*.cpp -o app.exe -Wall -Wextra
# Ejecutar
.\app.exe
```

Notas:
- El proyecto asume cabeceras en el directorio `include/` y fuentes en `src/`.
- Si usa MSVC, adapte la línea de compilación a `cl.exe` o use un proyecto VS.

## Estructura del proyecto
```
main.cpp
include/
  Book.h
  Electronic.h
  Gestor.h
  Menu.h
  Product.h
  UTILITIES.h
  TableFormat.h
src/
  Book.cpp
  Electronic.cpp
  Gestor.cpp
  Menu.cpp
  Product.cpp
  (otros: UserManager, SalesManager, Sale, etc.)
```

## Módulos principales y responsabilidades
- Menu
  - Controla el flujo principal de la aplicación y los submenús.
  - Implementa `selectFromList()` y `interactiveSelectMainMenu()` para entrada interactiva (flechas + Enter) con fallback.
  - Contrato clave de `selectFromList()`:
    - retorna >= 0 : índice seleccionado (0-based cuando se usan labels; en `interactiveSelectMainMenu()` se mapea a la opción numérica real)
    - retorna -1 : Escape / cancelar
    - retorna -2 : pedir al llamador que use modo numérico (fallback)
- Gestor
  - Repositorio/CRUD de productos; método `showAll()` y `showByType()` imprimen tablas usando `TableFormat`.
- Product / Electronic / Book
  - Modelo de dominio de productos. Cada clase implementa `printRow()` (ahora produce la fila completa con `\n`) y operadores necesarios (==, !=, +, <, > si aplica).
- User / UserManager
  - Gestión de usuarios, autenticación y roles. `UserManager::allUsers()` devuelve colección (se usa `std::list<User>` internamente para punteros estables).
- SalesManager / Sale
  - Registro de ventas, búsqueda por ID y por código de usuario.
- UTILITIES
  - Funciones de consola: `readKey()`, `supportsAnsi()`, `printHighlightedLine()`, `clearScreen()`, `waitForEnter()`, `readPassword()`.
  - Soporta fallback WinAPI para consolas sin VT (cmd.exe).
- TableFormat
  - Contiene constantes de ancho de columna (ej. `ProductWidths::Name`) y helpers `formatPrice(float)` y `printSeparator(ostream, widths)`.

## Contratos y decisiones de diseño
- Entrada interactiva (selector): Windows-first para una buena UX en PowerShell/cmd.exe. En terminales sin soporte single-key reads, `selectFromList()` devuelve -2 tras varios intentos y el llamador debe mostrar la versión numérica y leer por `std::cin`.
- `printRow()` de los modelos imprime la línea completa con `\n`. No añada `<< "\n"` extra en los lugares que llamen a `operator<<`.
- Formateo tabular centralizado en `TableFormat.h`. Añadir una nueva columna requiere actualizar `TableFormat` y sus anchuras para mantener alineación.
- Punteros a usuarios: `UserManager` mantiene `std::list<User>` para evitar invalidación de punteros/referencias al modificar la colección.

## Clases principales (resumen de atributos y métodos)
(Solo campos relevantes y métodos públicos, nombres aproximados según el código)

- Product
  - int id
  - int stock
  - float price
  - std::string name
  - std::string description
  - virtual void printRow(std::ostream&) const
  - virtual void showInfo() const
  - getters/setters
- Electronic : Product
  - std::string brand, model, specifications
  - int warrantyMonths
  - overrides printRow
- Book : Product
  - std::string author, publisher, isbn, genre
  - int pages
  - overrides printRow
- Gestor
  - std::vector<Product*> products (o contenedor manejado)
  - addProduct(Product*), removeProduct(int id), findProduct(int id), showAll(), showByType(std::string)
- User
  - int code
  - std::string name
  - roles flags (isAdmin, isSuperAdmin, isSeller, isWarehouse)
  - addNewUser(const User& creator) // flujo interactivo
- UserManager
  - std::list<User> users
  - User* authenticate(name, pwd)
  - User* findByName(name)
  - addUser(User), removeByName(name), allUsers() -> iterable
- Sale
  - int saleId
  - int userCode
  - std::string userName
  - std::string productType
  - std::string productName
  - int units
  - float totalPrice
- SalesManager
  - addSale(Sale) -> saleId
  - findById(int)
  - findByUserCode(int)
  - allSales()
- Menu
  - Gestor* gestor
  - UserManager* userManager
  - SalesManager* salesManager
  - User* currentUser
  - int selectFromList(const vector<string>& items, const string& title, int startIndex, bool allowEsc)
  - int interactiveSelectMainMenu()
  - bool processOption(int opt)

## Diagrama UML (PlantUML - texto)
Si quieres renderizarlo, copia el bloque PlantUML a un archivo `.puml` y usa PlantUML para generar la imagen.

```plantuml
@startuml
class Product {
  - int id
  - int stock
  - float price
  - string name
  - string description
  + virtual void printRow(ostream&)
}
class Electronic {
  - string brand
  - string model
  - int warrantyMonths
  + void printRow(ostream&)
}
class Book {
  - string author
  - string publisher
  - string isbn
  - int pages
  + void printRow(ostream&)
}
class Gestor {
  - vector<Product*> products
  + addProduct(Product*)
  + removeProduct(int)
  + findProduct(int) : Product*
  + showAll()
}
class User {
  - int code
  - string name
  - bool isAdmin
  - bool isSuperAdmin
  - bool isSeller
  - bool isWarehouse
  + authenticate(...)
}
class UserManager {
  - list<User> users
  + User* authenticate(string, string)
  + addUser(User)
  + removeByName(string)
}
class Sale { + /* fields shown in doc */ }
class SalesManager { + addSale(Sale) }
class Menu {
  - Gestor* gestor
  - UserManager* userManager
  - SalesManager* salesManager
  - User* currentUser
  + selectFromList(...)
  + interactiveSelectMainMenu()
}

Product <|-- Electronic
Product <|-- Book
Gestor --> Product : manages
Menu --> Gestor
Menu --> UserManager
Menu --> SalesManager
UserManager --> User : stores
SalesManager --> Sale : stores
@enduml
```

También incluyo una versión ASCII simplificada:

Product <|-- Electronic
Product <|-- Book
Gestor --> Product
Menu --> Gestor, UserManager, SalesManager
UserManager --> User

## Casos de uso y flujos relevantes
- Login: `Menu::run()` pide usuario y password; `UserManager::authenticate()` retorna puntero a `User` o nullptr.
- Selección de menú: `interactiveSelectMainMenu()` intenta entrada por teclas; si devuelve -2 se muestra la versión numérica y se lee por `std::cin`.
- Venta: `salesMenu()` crea `Sale`, decrementa stock en `Product`, registra la venta en `SalesManager` y muestra la fila en formato tabular usando `TableFormat::formatPrice`.

## Pruebas y calidad
- Recomendado: añadir tests unitarios (Catch2 o GoogleTest) para:
  - Gestor CRUD
  - UserManager autenticar/roles
  - SalesManager registrar/buscar
  - Product operators y `printRow()` formateo
- Gate de calidad local: compilar con `-Wall -Wextra` y ejecutar `mvn -B test` sólo si hay pruebas Java (no aplica aquí). Para C++ usar un task de compilación o CI.

## Riesgos y puntos de conflicto futuros
- Cambios en la impresión tabular: cualquier cambio en columnas debe actualizar `TableFormat.h` y todos los `printRow()`.
- Cambio de `printRow()` para no incluir `\n` invalidará muchos llamadores. El contrato actual es que `printRow()` sí incluye la nueva línea.
- Cambio del contenedor en `UserManager` de `std::list<User>` a `std::vector<User>` invalidará punteros a `User` existentes; mantener la elección o migrar a `std::list`/`std::deque` y documentar.
- Lectura single-key: si se desea soporte POSIX completo, hay que implementar `readKey()` usando `termios` (Linux/macOS) — el diseño actual devuelve -2 para fallback en terminales no compatibles.

## Recomendaciones para mantenibilidad
- Mantener `TableFormat.h` como la única fuente de verdad para anchuras y formateo de precios.
- Centralizar la lógica de resaltado en `UTILITIES.h::printHighlightedLine()` para soportar VT/WinAPI sin cambios en Menus.
- Añadir pruebas unitarias y un pequeño script `run_smoke.sh` o una task de VS Code para compilar y probar los flujos básicos.

## ¿Cómo obtener el UML como imagen?
- Instalar PlantUML y Graphviz, guardar el bloque PlantUML en `diagram.puml` y ejecutar:

```powershell
plantuml diagram.puml
```

o usar un renderizador online (PlantUML server, www.plantuml.com/plantuml).

---

Si quieres, puedo:
- Generar una imagen PNG del diagrama UML y añadirla al repo.
- Extraer automáticamente las firmas de las clases desde los `.h` para un diagrama más exacto.
- Crear un README corto con pasos rápidos para contribución.

Dime qué prefieres y lo hago.