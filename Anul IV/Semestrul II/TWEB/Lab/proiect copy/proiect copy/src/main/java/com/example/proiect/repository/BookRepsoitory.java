package com.example.proiect.repository;

import com.example.proiect.entity.Book;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.stereotype.Repository;

import java.util.List;
import java.util.Optional;

@Repository
public interface BookRepsoitory extends JpaRepository<Book, Integer> {

    Optional<List<Book>> getBooksByAuthor(String author);

}