#ifndef BOOK_H
#define BOOK_H

#include "Resource.h"
#include <string>

class Book:public Resource
{
    private:
    std::string title;
    std::string author;
    std::string isbn;
    int edition;

    public:

    Book(int resourceId,
         int ownerId,
         ImportanceLevel importance,
         const std::string&title,
         const std::string&author,
         const std::string&isbn,
         int edition
        );

    //getters
    std::string getTitle() const;
    std::string getAuthor() const;
    int getEdition() const;
    std::string getIsbn() const;
    std::string getResourceType() const override;


};
#endif