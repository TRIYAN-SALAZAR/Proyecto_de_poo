#include "Book.h"
#include <iomanip>
#include <iostream>

Book::Book()
    : Product(), author(""), publisher(""), isbn(""), pages(0), genre("")
{
}

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
    const int wId = 6;
    const int wAutor = 18;
    const int wTitulo = 20;
    const int wEditorial = 16;
    const int wIsbn = 16;
    const int wPag = 8;
    const int wPrecio = 10;
    const int wStock = 8;

    auto truncate = [](const std::string &s, std::size_t w) -> std::string {
        if (s.size() <= w) return s;
        if (w <= 3) return s.substr(0, w);
        return s.substr(0, w - 3) + "...";
    };

    std::ostringstream oss; oss << std::fixed << std::setprecision(2) << getPrice();
    out << std::left << std::setw(wId) << getId()
        << std::setw(wAutor) << truncate(author, wAutor - 1)
        << std::setw(wTitulo) << truncate(getName(), wTitulo - 1)
        << std::setw(wEditorial) << truncate(publisher, wEditorial - 1)
        << std::setw(wIsbn) << truncate(isbn, wIsbn - 1)
        << std::setw(wPag) << pages
        << std::setw(wPrecio) << oss.str()
        << std::setw(wStock) << getStock();
}
