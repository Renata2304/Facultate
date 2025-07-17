package com.example.proiect.service.dto;

public class SongDto {

    private String title;
    private String singer;

    public String getSTitle() {
        return title;
    }

    public SongDto setSTitle(String title) {
        this.title = title;
        return this;
    }

    public String getSinger() {
        return singer;
    }

    public SongDto setSinger(String singer) {
        this.singer = singer;
        return this;
    }
}
