CREATE SCHEMA proiect;
SET search_path = proiect, pg_catalog;

CREATE TABLE book (
                      id uuid,
                      title TEXT NOT NULL,
                      author TEXT NOT NULL,
                      PRIMARY KEY (id)
);

CREATE TABLE song (
                      id uuid,
                      title TEXT NOT NULL,
                      singer TEXT NOT NULL,
                      PRIMARY KEY (id)
);

