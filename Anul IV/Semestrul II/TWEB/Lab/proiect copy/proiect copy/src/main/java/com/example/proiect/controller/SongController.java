package com.example.proiect.controller;

import com.example.proiect.entity.Song;
import com.example.proiect.service.SongServie;
import com.example.proiect.service.dto.SongDto;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

@RestController
@RequestMapping("/song")
public class SongController {

    private final SongServie songServie;

    public SongController(SongServie songServie){
        this.songServie = songServie;
    }
    @GetMapping("/getBySinger")
    public ResponseEntity<List<SongDto>> getSongsBySinger(String singer){
        List<SongDto> songDtoList = songServie.getSongsBySinger(singer);
        return ResponseEntity.status(200).body(songDtoList);
    }

    @PostMapping("/addSong")
    public ResponseEntity<Song> addSong(@RequestBody SongDto songDto){
        Song song = songServie.addSong(songDto);
        return ResponseEntity.status(201).body(song);
    }
}
