package com.example.proiect.controller;

import com.example.proiect.entity.Book;
import com.example.proiect.service.BookServie;
import com.example.proiect.service.dto.BookDto;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

@RestController
@RequestMapping("/book")
public class BookController {

    private final BookServie bookServie;

    public BookController(BookServie bookServie){
        this.bookServie = bookServie;
    }
    @GetMapping("/getByAuthor")
    public ResponseEntity<List<BookDto>> getBooksByAuthor(String author){
        List<BookDto> bookDtoList = bookServie.getBooksByAuthor(author);
        return ResponseEntity.status(200).body(bookDtoList);
    }

    @PostMapping("/addBook")
    public ResponseEntity<Book> addBook(@RequestBody BookDto bookDto){
        Book book = bookServie.addBook(bookDto);
        return ResponseEntity.status(201).body(book);
    }
}
