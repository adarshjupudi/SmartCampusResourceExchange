#include "Book.h"

Book::Book(int resourceId,
           int ownerId,
           ImportanceLevel importance,
           const std::string &title,
           const std::string &author,
           const std::string &isbn,
           int edition)
    : Resource(resourceId, ownerId, importance, title),
      title(title),
      author(author),
      isbn(isbn),
      edition(edition)
{
}

std::string Book::getTitle() const
{
    return title;
}

std::string Book::getAuthor() const
{
    return author;
}

int Book::getEdition() const
{
    return edition;
}

std::string Book::getIsbn() const
{
    return isbn;
}

std::string Book::getResourceType() const
{
    return "Book";
}