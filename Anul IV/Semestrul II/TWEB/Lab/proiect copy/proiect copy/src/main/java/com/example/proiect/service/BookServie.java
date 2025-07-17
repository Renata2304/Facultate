package com.example.proiect.service;

import com.example.proiect.entity.Book;
import com.example.proiect.repository.BookRepsoitory;
import com.example.proiect.service.dto.BookDto;
import jakarta.transaction.Transactional;
import org.springframework.stereotype.Service;

import java.util.Collections;
import java.util.List;
import java.util.Optional;
import java.util.stream.Collectors;

@Service
@Transactional
public class BookServie {

    private final BookRepsoitory bookRepsoitory;

    public BookServie(BookRepsoitory bookRepsoitory) {
        this.bookRepsoitory = bookRepsoitory;
    }

    public List<BookDto> getBooksByAuthor(String author) {
        Optional<List<Book>> optionalBookList = bookRepsoitory.getBooksByAuthor(author);
        return optionalBookList.map(books ->
                books.stream()
                        .map(book -> new BookDto()
                                .setAuthor(book.getAuthor())
                                .setTitle(book.getTitle()))
                        .collect(Collectors.toList())
        ).orElse(Collections.emptyList());
    }

    public Book addBook(BookDto bookDto) {
        Book book = new Book();
        book.setAuthor(bookDto.getAuthor());
        book.setTitle(bookDto.getTitle());

        return bookRepsoitory.save(book);
    }
}
