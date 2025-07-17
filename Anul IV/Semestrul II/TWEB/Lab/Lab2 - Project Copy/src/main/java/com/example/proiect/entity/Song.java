package com.example.proiect.entity;

import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.Id;
import jakarta.persistence.Table;

import java.util.UUID;

@Entity
@Table(name = "song")
public class Song {
    @Id
    @GeneratedValue()
    private UUID id;
    private String sTitle;
    private String singer;

    public String getSTitle() {
        return sTitle;
    }

    public Song setSTitle(String sTitle) {
        this.sTitle = sTitle;
        return this;
    }

    public String getSinger() {
        return singer;
    }

    public Song setSinger(String singer) {
        this.singer = singer;
        return this;
    }
}
