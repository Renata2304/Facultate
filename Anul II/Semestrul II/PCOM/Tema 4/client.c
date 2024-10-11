#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <stdbool.h>
#include <ctype.h>
#include "helpers.h"
#include "requests.h"
#include "parson.h"
#include "parson.c"

// functie care verifica daca un id introdus este numar
bool valid_number(char *str) {
    for (int poz = 0; poz < strlen(str); poz++) {
        if(str[poz] < '0' || str[poz] > '9') {
            return false; // cazul in care contine un alt caracter
        }
    }
    return true;
}
// functie care verifica daca un sir de caractere contine si alte
// caractere pe langa litere
bool valid_str(char *str) {
    for (int poz = 0 ; poz < strlen(str); poz++) {
        if (isalpha(str[poz]) == 0) {
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[]) {
    int logged_in = 0, library = 0, socket;
	char host[16] = "34.254.242.81";
	int port = 8080;
	
	char *user[1], *cookies[1], token[BUFLEN];
	char command[100];

	while(1) {
		fgets(command, 100, stdin);
        socket = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
        
		if (!strncmp(command, "register", 8)) { // cazul de register
            char *username = calloc(100, sizeof(char));
            char *password = calloc(100, sizeof(char));

            printf("username="); fgets(username, 100, stdin);
            username[strlen(username) - 1] = 0; // stergerea '\n' de la finalul sirului citit
            printf("password="); fgets(password, 100, stdin);
            password[strlen(password) - 1] = 0;
            
            // verificarea formatului datelor de intrare
            if (strstr(username, " ") || strstr(password, " ")) {
                printf("Invalid data format! Please try again!\n");
                free(username); free(password); 
                continue;
            }

            // crearea unui nou json
            JSON_Value *value = json_value_init_object();
            JSON_Object *object = json_value_get_object(value);
            json_object_set_string(object, "username", username);
            json_object_set_string(object, "password", password);
            user[0] = json_serialize_to_string(value);

            char *message = compute_post_request(host, "/api/v1/tema/auth/register", 
                                            "application/json", user, 1, NULL, 0, NULL);
            
            socket = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
            
            send_to_server(socket, message);

            char* rsp =  receive_from_server(socket);

            // verifcarea daca username-ul a fost deja folosit
            if (strstr(rsp, "is taken") == NULL) {
                printf("User is successfully registered!\n");
            } else { // afisarere eroare daca e cazul
                printf("The username is already taken! Please register with another username!\n");
            }

            free(username); free(password); // eliberarea memoriei
		} 
        else if (!strncmp(command, "login", 5)) { // cazul de login
            if (logged_in == 1) { // daca user-ul s-a conectat deja
                printf("The user is already connected!\n");
                continue;
            }
            // citirea inputului
            char *username = calloc(100, sizeof(char));
            char *password = calloc(100, sizeof(char));

            printf("username="); fgets(username, 100, stdin);
            username[strlen(username) - 1] = 0; // stergerea caracterului '\n'
            printf("password="); fgets(password, 100, stdin);
            password[strlen(password) - 1] = 0;

            // verifcare format date
            if (strstr(username, " ") || strstr(password, " ")) {
                printf("Invalid data format!\n");
                printf("Please try again!\n");
                continue;
            }

            // crearea unui nou json
            JSON_Value *value = json_value_init_object();
            JSON_Object *object = json_value_get_object(value);
            json_object_set_string(object, "username", username);
            json_object_set_string(object, "password", password);
            user[0] = json_serialize_to_string(value);
            
            // trimitere request
            char *message = compute_post_request(host, "/api/v1/tema/auth/login", 
                                        "application/json", user, 1, NULL, 0, NULL);
            socket = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
            send_to_server(socket, message);
            char* rsp = receive_from_server(socket);

            char *error_message = strstr(rsp, "error");
            if (error_message) {
                if(strstr(error_message, "Credentials")) {
                    printf("Incorrect credentials! Please try again!\n");
                    continue;
                } else {
                    printf("An error occured while trying to log in! Please try again!\n");
                    continue;
                }
            } else {
                // salvarea cookie-urilor
                char* new_str = strstr(rsp, "connect.sid=");
                cookies[0] = malloc(100 * sizeof(char));
                memcpy(cookies[0], strtok(new_str, ";"), 100);

                logged_in = 1;
                printf("Succesfully logged in!\n");
            }

        } 
        else if (!strncmp(command, "logout", 6)) { // cazul de logout
            if (logged_in == 1) { // verificare daca user-ul este logat
                // trimitere request
                char *message = compute_get_request(host, "/api/v1/tema/auth/logout", NULL, 
                                                    cookies, 1, NULL);                                    
                socket = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
                send_to_server(socket, message);
                
                char* rsp = receive_from_server(socket);

                free(cookies[0]);

                logged_in = 0;
                library = 0;
                printf("Success!\n"); // in cazul in care actiunea s-a efectuat cu succes
            } else {
                printf("No use is logged in!\n"); // caz de eroare
            }
        } 
        else if (!strncmp(command, "enter_library", 13)) { // cazul enter_library
            if (!logged_in) { // daca user-ul nu este logat
                printf("The user is not logged in!\n");
            } else {
                // trimitere request
                char *message = compute_get_request(host, "/api/v1/tema/library/access",
                                                    NULL, cookies, 1, NULL);
                socket = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
                send_to_server(socket, message);
                
                char* rsp = receive_from_server(socket);
                // salvarea token-ului
                char* copy_tok = (strstr(rsp, "{") + 10);
                memcpy(token, strtok(copy_tok, "\""), BUFLEN);
                
                library = 1;
                // afisare caz fara erori
                printf("User has successfully entered the library!\n");
            }
        }
        else if (!strncmp(command, "get_books", 9)) { // cazul get_books
            if (library != 1) { // verificare daca user-ul are acces la biblioteca
                printf("User does not have access to the library!\n");
            } else {
                // salvarea headerului Authorization
                char auth[BUFLEN] = "Authorization: Bearer ";
                strcat(auth, token);
                // trimitere request
                char *message = compute_get_request(host, "/api/v1/tema/library/books", 
                                                    NULL, cookies, 1, auth);
                socket = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
                send_to_server(socket, message);
                char* rsp = receive_from_server(socket);
                // afisarea tuturor cartilor
                printf("%s\n", strstr(rsp, "[{"));
            }
        }
        else if (!strncmp(command, "get_book", 8)) { // cazul get_book
            if (library != 1) { // verificare daca user-ul are acces la biblioteca
                printf("User does not have access to the library!\n");
            } else {
                // salvarea headerului Authorization
                char auth[BUFLEN] = "Authorization: Bearer ";
                strcat(auth, token);
                // citirea id-ului cartii dorite
                char str[100]; int nr;
                fgets (str, 100, stdin);
                str[strlen(str) - 1] = 0;

                // testarea numarului primit
                if (!valid_number(str)) {
                    printf("Invalid id format! Please try again!\n");
                    continue;
                } else {
                    nr = atoi(str); // transformarea lui in integer
                }

                char route[BUFLEN];
                sprintf(route, "%s%d", "/api/v1/tema/library/books/", nr);

                // trimitere request
                char *message = compute_get_request(host, route, NULL, cookies, 1, auth);
                socket = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
                send_to_server (socket, message);
                char* rsp = receive_from_server(socket);

                if (strstr(rsp, "error")) { // afisare in caz de eroare
                    printf ("%s\n", "No book was found!\n");
                } else { // afisarea cartii dorite
                    printf ("%s\n", strstr(rsp, "{"));
                }
            }
        }
        else if (!strncmp(command, "add_book", 8)) { // cazul add_book
            if (library != 1) { // verificare daca user-ul are acces la biblioteca
                printf("User does not have access to the library!\n");
            } else {
                char title[200], author[200], genre[200], publisher[200], png[100];
                int page_count;
                char auth[BUFLEN] = "Authorization: Bearer ";
                strcat(auth, token);

                printf("title="); fgets(title, 200, stdin);
                title[strlen(title) - 1] = 0;
                printf("author="); fgets(author, 200, stdin);
                author[strlen(author) - 1] = 0;
                printf("genre="); fgets(genre, 200, stdin);
                genre[strlen(genre) - 1] = 0;
                printf("publisher="); fgets(publisher, 200, stdin);
                publisher[strlen(publisher) - 1] = 0;
                printf("page_count="); fgets(png, 200, stdin);
                png[strlen(png) - 1] = 0;
                
                // tesatrea formatului datelor de intrare 
                if (!strlen(title) || !strlen(author) || !strlen(genre) || 
                    !strlen(publisher) || !strlen(png)) {
                    printf("Invalid or incomplete information for the book!\n");
                }

                // testarea numarului primit
                if (!valid_number(png)) {
                    printf("Invalid id format! Please try again!\n");
                    continue;
                } else {
                    page_count = atoi(png); // transformarea lui in integer
                }
                
                // construirea unui nou json cu datele primite in input
                JSON_Value *value = json_value_init_object();
                JSON_Object *object = json_value_get_object(value);
                json_object_set_string(object, "title", title);
                json_object_set_string(object, "author", author);
                json_object_set_string(object, "genre", genre);
                json_object_set_string(object, "publisher", publisher);
                json_object_set_number(object, "page_count", page_count);
                
                user[0] = json_serialize_to_string(value);

                // trimitere cerere
                char *message = compute_post_request(host, "/api/v1/tema/library/books", 
                                "application/json", user, 1, NULL, 0, auth);

                socket = open_connection(host, port, AF_INET, SOCK_STREAM, 0);                
                send_to_server(socket, message);
                char* resp = receive_from_server(socket);
                // afisare in cazul in care nu au fost erori
                printf("Book has been added with success!\n");
            }
        }
        else if (!strncmp(command, "delete_book", 11)) { // cazul de delete_book
            if (library != 1) { // verificare daca user-ul are acces la biblioteca
                printf("User does not have access to the library!\n");
            } else {
                // salvarea headerului Authorization
                char auth[BUFLEN] = "Authorization: Bearer ";
                strcat(auth, token);

                // citirea id-ului cartii dorite
                char str[100]; int nr;
                fgets (str, 100, stdin);
                str[strlen(str) - 1] = 0;

                // testarea numarului primit
                if (!valid_number(str)) {
                    printf("Invalid id format! Please try again!\n");
                    continue;
                } else {
                    nr = atoi(str); // transformarea lui in integer
                }

                char route[BUFLEN];
                sprintf(route, "%s%d", "/api/v1/tema/library/books/", nr);

                char *message = compute_delete_request(host, route, NULL, cookies, 1, auth);
                socket = open_connection(host, port, AF_INET, SOCK_STREAM, 0);

                send_to_server (socket, message);
                char* rsp = receive_from_server(socket);

                if (strstr(rsp, "error")) { // afisare erori, daca exista
                    printf ("No book was found!\n");
                } else {
                    printf("Book deleted successfully!\n");
                }
            }
        }
        else if (!strncmp(command, "exit", 4)){ // cazul de exit
			break;
        }
    }
    return 0;
}
