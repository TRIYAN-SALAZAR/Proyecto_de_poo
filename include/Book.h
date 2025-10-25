#ifndef BOOK_H
#define BOOK_H

#include <Product.h>
#include <string>

class Book : public Product
{
    public:
        Book();
        Book(int id, const std::string& name, float price, const std::string& description, int stock,
             const std::string& author, const std::string& publisher, const std::string& isbn, int pages, const std::string& genre);

        std::string getAuthor() const;
        void setAuthor(const std::string& author);

        std::string getPublisher() const;
        void setPublisher(const std::string& publisher);

        std::string getIsbn() const;
        void setIsbn(const std::string& isbn);

        int getPages() const;
        void setPages(int pages);

        std::string getGenre() const;
        void setGenre(const std::string& genre);

        void showInfo() const override;
        float calculateFinalPrice() const override;

    private:
        std::string author;
        std::string publisher;
        std::string isbn;
        int pages;
        std::string genre;
};

#endif // BOOK_H
