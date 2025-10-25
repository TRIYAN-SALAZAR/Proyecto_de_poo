#include "Book.h"
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
