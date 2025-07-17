package com.example.proiect.service;

import com.example.proiect.entity.Song;
import com.example.proiect.repository.SongRepsoitory;
import com.example.proiect.service.dto.SongDto;
import jakarta.transaction.Transactional;
import org.springframework.stereotype.Service;

import java.util.Collections;
import java.util.List;
import java.util.Optional;
import java.util.stream.Collectors;

@Service
@Transactional
public class SongServie {

    private final SongRepsoitory songRepository;

    public SongServie(SongRepsoitory songRepository) {
        this.songRepository = songRepository;
    }

    public List<SongDto> getSongsBySinger(String singer) {
        Optional<List<Song>> optionalSongList = songRepository.getSongsBySinger(singer);
        return optionalSongList.map(songs ->
                songs.stream()
                        .map(song -> new SongDto()
                                .setSinger(song.getSinger())
                                .setSTitle(song.getSTitle()))
                        .collect(Collectors.toList())
        ).orElse(Collections.emptyList());
    }

    public Song addSong(SongDto songDto) {
        Song song = new Song();
        song.setSinger(songDto.getSinger());
        song.setSTitle(songDto.getSTitle());

        return songRepository.save(song);
    }
}
