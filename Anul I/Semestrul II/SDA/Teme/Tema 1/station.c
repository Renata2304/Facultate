#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "station.h"


/* Creeaza o gara cu un numar fix de peroane.
 * 
 * platforms_no: numarul de peroane ale garii
 *
 * return: gara creata
 */
TrainStation* open_train_station(int platforms_no) {
    // initailizare gara 
    TrainStation* station = (TrainStation*) malloc(sizeof(TrainStation));
    station->platforms_no = platforms_no;
    station->platforms = (Train**) malloc(platforms_no * sizeof(Train*));
    // initializarea peroanelor si primului vagon de pe acel peron
    for(int i = 0; i < station->platforms_no; i++) {
        station->platforms[i] = (Train*) malloc(sizeof(Train));

        station->platforms[i]->train_cars = NULL;
        station->platforms[i]->locomotive_power = -1;
    }
    return station;
}
// functie recursiva care sterge fiecare vagon dintr-un sir de vagoane
void delvag(TrainCar* tc) {
    if(tc == NULL) return;

    delvag(tc->next);
    free(tc);
}
/* Elibereaza memoria alocata pentru gara.
 *
 * station: gara existenta
 */
void close_train_station(TrainStation *station) {
    if(station == NULL || station->platforms == NULL) 
        return;
    // stergerea persoanelor si a vagoanelor de pe acestea
    for(int i = 0; i < station->platforms_no; i++) {
        delvag(station->platforms[i]->train_cars);
        station->platforms[i]->train_cars = NULL;
        station->platforms[i]->locomotive_power = -1;
        free(station->platforms[i]);
    }
    // stergerea garii
    free(station->platforms);
    free(station);
}


/* Afiseaza trenurile stationate in gara.
 *
 * station: gara existenta
 * f: fisierul in care se face afisarea
 */
void show_existing_trains(TrainStation *station, FILE *f) {
    if(station == NULL || station->platforms == NULL) 
        return;
    
    for(int i = 0; i < station->platforms_no; i++) {
        fprintf(f, "%d: ", i);  // afisarea peronului
        if(station->platforms[i]->locomotive_power != -1) {
            fprintf(f, "(%d)", station->platforms[i]->locomotive_power);  // afisarea locomotivei
            TrainCar* tc = station->platforms[i]->train_cars;
            while(tc) {  // afisarea vagoanelor de pe peronul curent
                fprintf(f, "-|%d|", tc->weight);
                tc = tc->next;
            }
        }
        fprintf(f, "\n");
    }
}


/* Adauga o locomotiva pe un peron.
 * 
 * station: gara existenta
 * platform: peronul pe care se adauga locomotiva
 * locomotive_power: puterea de tractiune a locomotivei
 */
void arrive_train(TrainStation *station, int platform, int locomotive_power) {
    if(station == NULL || station->platforms==NULL)
        return;
    // daca deja exista o locomotiva pe peron
    if(platform < 0 || station->platforms_no <= platform)
        return;
    // initializarea noii locomotive si a primului vagon
    if(station->platforms[platform]->locomotive_power == -1) {
        station->platforms[platform]->locomotive_power = locomotive_power;
        station->platforms[platform]->train_cars = NULL;
    }
}


/* Elibereaza un peron.
 * 
 * station: gara existenta
 * platform: peronul de pe care pleaca trenul
 */
void leave_train(TrainStation *station, int platform) {
    if(station == NULL || station->platforms==NULL)
        return;
    
    if(platform < 0 || station->platforms_no <= platform)
        return;
    
    // stergerea tuturor vagoanelor si a locomotivei de pe peron
    delvag(station->platforms[platform]->train_cars);
    station->platforms[platform]->train_cars = NULL;
    station->platforms[platform]->locomotive_power = -1;
    
}

/* Adauga un vagon la capatul unui tren.
 * 
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 * weight: greutatea vagonului adaugat
 */
void add_train_car(TrainStation *station, int platform, int weight) {
    if(station == NULL || station->platforms==NULL)
        return;
    
    if(platform < 0 || station->platforms_no <= platform || station->platforms[platform]->locomotive_power == -1)
        return;


    // daca se adauga vagonul pe prima pozitie
    if(station->platforms[platform]->train_cars == NULL) {
        station->platforms[platform]->train_cars = (TrainCar*) malloc(sizeof(TrainCar));
        station->platforms[platform]->train_cars->weight =  weight;
        station->platforms[platform]->train_cars->next = NULL;
        return;
    }

    TrainCar* tc = station->platforms[platform]->train_cars;
    // se parcurge trenul pana se ajunge la ultimul vagon
    while(tc->next) {
        tc = tc->next;
    }
    // se adauga la finalul trenului un nou vagon
    TrainCar* new_tc = (TrainCar*) malloc(sizeof(TrainCar));
    new_tc->next = NULL;
    new_tc->weight = weight;

    tc->next = new_tc;
}

/* Scoate vagoanele de o anumita greutate dintr-un tren.
 * 
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 * weight: greutatea vagonului scos
 */
void remove_train_cars(TrainStation *station, int platform, int weight) {
    if(station == NULL || station->platforms==NULL)
        return;

    if(platform < 0 || station->platforms_no <= platform)
        return;

    if(station->platforms[platform]->train_cars == NULL)
        return;

    TrainCar* tc = station->platforms[platform]->train_cars;
    TrainCar* prev = NULL;

    // daca se sterge singurul vagon
    if(tc->next==NULL && tc->weight==weight) {  
        free(tc);
        station->platforms[platform]->train_cars = NULL;
        return;
    }  

    while(tc != NULL) {
        if(tc->weight == weight) { // daca vagonul trebuie sters
            // daca se sterge primul vagon
            if(prev == NULL) {
                TrainCar* aux = tc->next;
                station->platforms[platform]->train_cars = aux;
                free(tc);
                tc = aux;
            } else { if(tc->next == NULL) { // daca se sterge ultimul vagon
                        prev->next = NULL;
                        free(tc);
                        tc = NULL;
                    } else { // daca se sterge un vagon din interiorul trenului
                        prev->next = tc->next;
                        free(tc);
                        tc = prev->next;
                    }
                }
        } else { // trecerea la un nou vagon
           prev = tc;
           tc = tc->next;
          }
    }
}


/* Muta o secventa de vagoane dintr-un tren in altul.
 * 
 * station: gara existenta
 * platform_a: peronul pe care se afla trenul de unde se scot vagoanele
 * pos_a: pozitia primului vagon din secventa
 * cars_no: lungimea secventei
 * platform_b: peronul pe care se afla trenul unde se adauga vagoanele
 * pos_b: pozitia unde se adauga secventa de vagoane
 */
void move_train_cars(TrainStation *station, int platform_a, int pos_a, 
                                int cars_no, int platform_b, int pos_b) {
    if(station == NULL || station->platforms==NULL)
        return;

    if(platform_a < 0 || station->platforms_no <= platform_a)
        return;
    if(platform_b < 0 || station->platforms_no <= platform_b)
        return;

    if(station->platforms[platform_a]->train_cars == NULL)
        return;

    if(pos_a <= 0 || pos_b <= 0)
        return;
    
    if(pos_b > 1 && station->platforms[platform_b]->train_cars == NULL)
        return;
    
    if(cars_no < 1)
        return;
    
    TrainCar* a = station->platforms[platform_a]->train_cars, *prev_a = NULL;

    pos_a--;

    while(pos_a > 0 && a) { // gasirea vagonului a
        prev_a = a;
        a = a->next;
        pos_a--;    
    }
    if(a == NULL) return;
    
    // refacerea legaturii dintre vagoanele de pe peronul a
    int c_nr = cars_no;
    TrainCar *last_a = a, *a_next = last_a->next;
    while(c_nr > 1 && last_a && a_next) {
        a_next = a_next->next;
        last_a = last_a->next;
        c_nr --;
    }
    if(c_nr > 1 || last_a == NULL) return;

    TrainCar* b = station->platforms[platform_b]->train_cars, *prev_b = NULL;
    
    pos_b--;

    if(pos_b == 0) { // daca vagoanele din a se introduc la inceputul lui b
        station->platforms[platform_b]->train_cars = a;
        last_a->next = b;

        // mutare vagoane din a
        if(prev_a == NULL) // daca s-a luat de la primul vagon
            station->platforms[platform_a]->train_cars = a_next;
        else
            prev_a->next = a_next; // daca nu s-a luat de la primul vagon
        return;
    }

    // gasirea vagonului  b
    while(pos_b > 0 && b) { 
        prev_b = b;
        b = b->next;
        pos_b--;
    }
    if (pos_b != 0 || prev_b == NULL) 
        return;

    // mutarea vagoanelor din a in b
    if(pos_b == 0) {
        prev_b->next = a;
        last_a->next = b;
        
        // mutare vagoane din a
        if(prev_a == NULL) // daca s-a luat de la primul vagon
            station->platforms[platform_a]->train_cars = a_next;
        else
            prev_a->next = a_next; // daca nu s-a luat de la primul vagon
        return;
    }
    
}

// functie care calculeaza suma greutatilor vagoanelor
int sumvag(TrainCar* tc){
    int s = 0;
    while(tc) {
        s+= tc->weight;
        tc = tc->next;
    }
    return s;
}

/* Gaseste trenul cel mai rapid.
 * 
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_express_train(TrainStation *station) {
    int difmax = -1, imax = -1;

    if(station == NULL || station->platforms==NULL)
        return -1 ;

    for(int i = 0; i < station->platforms_no; i++) {

        int lp = station->platforms[i]->locomotive_power;
        int sv = sumvag(station->platforms[i]->train_cars);

        if(lp - sv > difmax) {
            difmax = lp - sv;
            imax = i;
        }
    }
    return imax;
}


/* Gaseste trenul supraincarcat.
 * 
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_overload_train(TrainStation *station) {
    if(station == NULL || station->platforms==NULL)
        return -1 ;

    for(int i = 0; i < station->platforms_no; i++) {

        int lp = station->platforms[i]->locomotive_power;
        int sv = sumvag(station->platforms[i]->train_cars);

        if(lp < sv) {
            return i;
        }
    }
    return -1; // daca nu s-a gasit niciun tren supraincarcat
}


/* Gaseste trenul cu incarcatura optima.
 * 
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_optimal_train(TrainStation *station) {
    int difmin = 1000000, imin = -1;

    if(station == NULL || station->platforms==NULL)
        return -1 ;

    for(int i = 0; i < station->platforms_no; i++) {

        int lp = station->platforms[i]->locomotive_power;
        int sv = sumvag(station->platforms[i]->train_cars);

        if(lp - sv < difmin && lp - sv >= 0) {
            difmin = lp - sv;
            imin = i;
        }
    }
    return imin;
}

// functie care gaseste cea mai grea secventa de cars_no dintr-un tren de pe un peron
// functia ajuta la find_heaviest_sequence_train
int secv(TrainCar *tc, int cars_no, TrainCar ** start) {
    if(tc == NULL) 
        return -1;

    TrainCar* prev_tc = tc;
    int sum = tc->weight;

    // determinare secventei de vagoane
    while(tc && cars_no > 1) { 
        tc = tc->next;
        if(tc == NULL) 
            return -1;
        cars_no--;
        sum += tc->weight;
    }

    int smax = sum;
    TrainCar* lmax_prev = prev_tc;
    // cautare secventei care indeplineste cerinta
    while(tc) { 
        
        sum -= prev_tc->weight;
        
        prev_tc = prev_tc->next;
        tc = tc->next;
        if(tc == NULL) 
            break;
        
        sum += tc->weight;

        if(sum > smax) {
            lmax_prev = prev_tc;
            smax = sum;
        }
    }

    *start = lmax_prev;
    return smax;
}

/* Gaseste trenul cu incarcatura nedistribuita bine.
 * 
 * station: gara existenta
 * cars_no: lungimea secventei de vagoane
 *
 * return: peronul pe care se afla trenul
 */
int find_heaviest_sequence_train(TrainStation *station, int cars_no, TrainCar **start_car) {
    
    if(station == NULL || station->platforms==NULL)
        return -1 ;
    if(cars_no <= 0)
        return -1;
    
    int sumax = -1, imax = -1;
    
    for(int i = 0; i < station->platforms_no; i++) {

        TrainCar *tc = station->platforms[i]->train_cars, *start = NULL;
        int sum = secv(tc, cars_no, &start);
        // daca s-a gasit o nou secventa maxima
        if(sum > sumax) {
            sumax = sum;
            *start_car = start;
            imax = i;
        }
    }   
    if(sumax != -1) 
        return imax;

    *start_car = NULL;
    return -1; // daca nu s-a gasit nicio secventa maxima
}

/* Ordoneaza vagoanele dintr-un tren in ordinea descrescatoare a greutatilor.
 * 
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 */
void order_train(TrainStation *station, int platform) {
    if(station == NULL || station->platforms==NULL)
        return;

    if(platform < 0 || station->platforms_no <= platform)
        return;

    if(station->platforms[platform]->train_cars == NULL)
        return;

    int wgmin = 10000, wg = -1, ok = 1;
    TrainCar* start;
    start = NULL;

    // parcurgerea si initializarea primului element din lista noua
    TrainCar* tc = station->platforms[platform]->train_cars;

    while(tc) {
        if(tc->weight < wgmin && tc->weight>0) {
            wgmin = tc->weight;
        }
        tc = tc->next;
    }

    if(wgmin == 10000)
        return;

    // parcurgerea si copierea tuturor vagoanelor cu lungimea minima din vechiul tren in noul tren
    tc = station->platforms[platform]->train_cars;
    while(tc) {
        if(tc->weight == wgmin && tc->weight>0) {
            {   TrainCar* aux = (TrainCar*) calloc(1, sizeof(TrainCar));
                aux->weight = tc->weight;
                aux->next = start;
                start = aux;
            }   
        }
        tc = tc->next;
    }

    wg = wgmin;

    while(ok) {
        
        tc = station->platforms[platform]->train_cars;
        
        ok = 0; wgmin = 100000;
        // cautarea celei mai mici greutati, mai mari decat a celei deja ezistente in start
        while(tc) {
            if(tc->weight < wgmin && tc->weight > wg) {
                wgmin = tc->weight;
                ok = 1;
            } 
            tc = tc->next;
        }

        if(!ok)
            break;

        tc =  station->platforms[platform]->train_cars;
        // copierea in noul sir a tuturor vagoanelor cu greutatea minima
        while(tc) {
            if(tc->weight == wgmin && tc->weight>0) {
            {   TrainCar* aux = (TrainCar*) calloc(1, sizeof(TrainCar));
                aux->weight = tc->weight;
                aux->next = start;
                start = aux;
                aux = NULL;
            }   
            }
            tc = tc->next;
        }

        wg = wgmin;
    }
    // stergerea vechiului tren si copierea celui nou peste cel vechi
    delvag(station->platforms[platform]->train_cars);
    station->platforms[platform]->train_cars = start;
}


/* Scoate un vagon din trenul supraincarcat.
 * 
 * station: gara existenta
 */
void fix_overload_train(TrainStation *station) {
    if(station == NULL || station->platforms == NULL)
        return;

    int i = find_overload_train(station);
    if(i == -1)  // daca nu exista niciun tren supraincarcat
        return;

    int difmin = 100000;
    TrainCar* tc = station->platforms[i]->train_cars;
    int lp = station->platforms[i]->locomotive_power;
    int sv = sumvag(station->platforms[i]->train_cars);
    int pos = -1, curr = 0;

    // determinarea vagonului care trebuie scos pentru a avea un tren in care diferenta dintre puterea
    // locomotivei si suma vagoanelor sa fie minima 
    while(tc) {
        int wg = tc->weight;
        sv = sv - wg; // "stergerea" vagonului curent din suma vagoanelor
        if(lp - sv < difmin && lp - sv >= 0) {
            difmin = lp-sv;
            pos = curr;
        }
        sv = sv + wg; // reactualizarea sumei
        curr++;
        tc = tc->next;
    }
    // daca nu s-a gasit un vagon cu proprietatea dorita
    if(pos == -1)
        return;

    //  stergerea vagonului cu proprietatea ceruta
    TrainCar* prev = NULL; tc = station->platforms[i]->train_cars;
    while(tc) {
        if(pos == 0) { // daca se sterge primul vagon
            if(prev == NULL) 
                 station->platforms[i]->train_cars = tc->next;
            else 
                prev->next = tc->next;
            free(tc);
            return;
        }
        pos--;
        prev = tc;
        tc = tc->next;
    }

}
