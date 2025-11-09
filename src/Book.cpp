#include <iomanip>
#include <iostream>
#include "Book.h"
#include "UTILITIES.h"
#include "TableFormat.h"

Book::Book()
    : Product(), author(""), publisher(""), isbn(""), pages(0), genre("")
{
}

Book::Book(int id, const std::string& name, float price, const std::string& description, int stock) 
    : Product(id, stock, price, name, description), author(""), publisher(""), isbn(""), pages(0), genre(""){}

Book::Book(int id, const std::string &name, float price, const std::string &description, int stock,
           const std::string &author, const std::string &publisher, const std::string &isbn, int pages, const std::string &genre)
    : Product(id, stock, price, name, description), author(author), publisher(publisher), isbn(isbn), pages(pages), genre(genre)
{
}

std::string Book::getAuthor() const { return author; }
void Book::setAuthor(const std::string &author) { this->author = author; }

std::string Book::getPublisher() const { return publisher; }
void Book::setPublisher(const std::string &publisher) { this->publisher = publisher; }

std::string Book::getIsbn() const { return isbn; }
void Book::setIsbn(const std::string &isbn) { this->isbn = isbn; }

int Book::getPages() const { return pages; }
void Book::setPages(int pages) { this->pages = pages; }

std::string Book::getGenre() const { return genre; }
void Book::setGenre(const std::string &genre) { this->genre = genre; }

void Book::showInfo() const
{
    std::cout << "Libro ID: " << getId() << "\n"
              << "Nombre: " << getName() << "\n"
              << "Autor: " << author << "\n"
              << "Editorial: " << publisher << "\n"
              << "ISBN: " << isbn << "\n"
              << "Paginas: " << pages << "\n"
              << "Genero: " << genre << "\n"
              << "Descripcion: " << getDescription() << "\n"
              << "Precio: " << getPrice() << "\n"
              << "Stock: " << getStock() << std::endl;
}

float Book::calculateFinalPrice() const
{
    const float CULTURAL_TAX = 0.05f;
    return getPrice() * (1.0f + CULTURAL_TAX);
}

void Book::printRow(std::ostream &out) const
{
    using namespace TableFormat;
    const int wId = BookWidths::Id;
    const int wAutor = BookWidths::Autor;
    const int wTitulo = BookWidths::Titulo;
    const int wEditorial = BookWidths::Editorial;
    const int wIsbn = BookWidths::Isbn;
    const int wPag = BookWidths::Pag;
    const int wPrecio = BookWidths::Precio;
    const int wStock = BookWidths::Stock;

    auto truncate = [](const std::string &s, std::size_t w) -> std::string {
        if (s.size() <= w) return s;
        if (w <= 3) return s.substr(0, w);
        return s.substr(0, w - 3) + "...";
    };

    out << std::left << std::setw(wId) << getId()
        << std::setw(wAutor) << truncate(author, wAutor - 1)
        << std::setw(wTitulo) << truncate(getName(), wTitulo - 1)
        << std::setw(wEditorial) << truncate(publisher, wEditorial - 1)
        << std::setw(wIsbn) << truncate(isbn, wIsbn - 1)
        << std::setw(wPag) << pages
        << std::setw(wPrecio) << TableFormat::formatPrice(getPrice())
        << std::setw(wStock) << getStock()
        << "\n";
}

std::istream& operator>>(std::istream &input, Book& book) {
    std::string author, publisher, isbn, genre;
    int pages = 0;

    std::cout << "Autor: ";
    std::getline(input, author);
    
    clearInput();
    std::cout << "Editorial: ";
    std::getline(input, publisher);
    
    std::cout << "ISBN: ";
    std::getline(input, isbn);

    std::cout << "Paginas: ";
    input >> pages;
    
    clearInput();
    std::cout << "Genero: ";
    std::getline(input, genre);

    book.setAuthor(author);
    book.setPublisher(publisher);
    book.setIsbn(isbn);
    book.setGenre(genre);
    book.setPages(pages);

    return input;
}