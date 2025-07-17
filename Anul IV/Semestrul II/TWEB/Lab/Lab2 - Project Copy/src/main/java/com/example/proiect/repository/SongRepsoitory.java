package com.example.proiect.repository;

import com.example.proiect.entity.Song;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.stereotype.Repository;

import java.util.List;
import java.util.Optional;

@Repository
public interface SongRepsoitory extends JpaRepository<Song, Integer> {

    Optional<List<Song>> getSongsBySinger(String singer);

}