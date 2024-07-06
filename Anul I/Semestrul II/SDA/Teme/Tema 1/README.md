# Tema1-SDA
Tema 1 - SDA --> Anul 1/ Semestrul 2

open_train_station -> am inceput prin initializarea unei noi gari prin alocarea dinamica de memorie, apoi am alocat pentru fiecare peron memorie si am setat locomotive_power cu -1.

delvag -> functia delvag sterge si dezaloca recursiv toate vagoanele de pe un peron dat.

close_train_station -> incazul in care datele nu sunt corecte, functia se va incheia. In caz contrar, se va parcurge fiecare peron, se vor sterge cu ajutorul functiei delvag toate vagoanele de pe un peron si se vor dezaloca apoi peroanele si gara.

show_existing_trains -> in cazul in care datele sunt corecte, se va parcurge fiecare peron in parte, se va afisa numarul peronului, iar daca exista tren pe acel peron se va afisa puterea locomotivei. Daca exista vagone se vor afisa pe rand vagoanele existente, cu greutatea lor caracteristica.

arrive_train -> in cazul in care datele sunt corecte, daca nu exista o locomotiva pe un peron se initializa un nou peronn cu greutatea primita si primul vagon va pointa la NULL.

leave_train -> in cazul in care datele sunt corecte, se vor sterge toate vagoanele de pe acel peron, folosind functia delvag si apoi se va schimba greutatea locomotivei cu -1.

add_train_car -> in cazul in care datele sunt corecte, daca trenul este gol, se va adauga vagonul pe prima pozitie, atunci se va aloca dinamic un nou vagon, se va initializa cu greutatea primita si va pointa la NULL. In caz contrar, daca vagonul se adauga la finalul trenului, se va parcurge trenul pana la ultimul vagon, se va aloca dinamic noul vagon care va pointa la NULL si care are greutatea primita.

remove_train_cars -> in cazul in care datele sunt corecte, daca trenul nu are decat un singur vagon, se va sterge si functia se termina. Altfel, cat timp mai sunt vagoane in tren, se va parcurgge si se vor sterge, tinand cont de anumite conditii (daca se sterge primul vagon, daca se sterge ultimul vagon, daca se sterg vagoane din interiroul trenului).

move_train_cars -> in cazul in care datele sunt corecte, se va gasi pozitia din a de la care trebuie loate vagoanele si se va setermina secventa de cars_no vagoane necesare. Apoi, se va parcurge trenul de la peronul b si se vor introduce vagoanele din a, refacandu-se legaturile necesare.

sumvag -> determina suma vagoanelor de pe un peron.

find_express_train -> in cazul in care datele sunt corecte, se vor parcurge peroanele si cu ajutorul functiei sumvag se va determina suma vagoanelor de pe peronul curect. In final, se va returna peronul in care diferenta dintre locomotiva si suma vagoanelor este maxima.

find_overload_train -> in cazul in care datele sunt corecte, se vor parcurge peroanele si cu ajutorul functiei sumvag se va determina suma vagoanelor de pe peronul curect. Se va returna peronul pe care suma vagoanelor este mai mare decat puterea locomotivei, in caz ca exista un astfel de tren sau se va returna -1 in cazul in care nu exista.

find_optimal_train -> in cazul in care datele sunt corecte, se vor parcurge peroanele si cu ajutorul functiei sumvag se va determina suma vagoanelor de pe peronul curect. In final, se va returna peronul pe care diferenta dintre puterea locomotivei si suma vagoanelor este minima, dar si pozitiva.

secv -> functia determina suma maxima dintr-o secventa de cars_no de pe un peron dat si actualizeaza totodata un pointer la primul vagon din secventa cu suma maxima.

find_heaviest_sequence_train -> in cazul in care datele sunt corecte, se vor parcurge peroanele din gara si cu ajutorul functiei secv se va determmina secevnta cu suma maxima de pe un peron si daca aceasta este mai mare decat suma maxima de pe toate peroanele, aceasta se va actualiza, si se va schimba pointerul de start_car. In final, se va returna peronul pe care se afla secventa maxima.

order_train -> in cazul in care datele sunt corecte, se va forma un nou sir de vagoane, primul fiind start si se vor adauga pe rand, prin fata, vagoanele cu cea mai mica greutate, care este totodata mai mare decat cele din start. Astfel se va forma un nou sir cu vagoanele de pe peronul platform, ordonate in mod descrescator. In final se va dezaloca vechiul tren si se va reuni cu nou tren, primul vagon fiind cel de start.

fix_overload_train -> se va folosi functia find_overload_train pentru a gasi peronul pe care se afla trenul supraincarcat. In cazul in  care  acesta nu exista, functia se incheie. In caz contrar, se va calcula suma vagoanelor de pe acel peron cu functia sumvag si se va parcurge trenul. se vor "sterge" vagonul curent din tren, scazandu-se greutatea lui din suma greutatii totale si se va verifica daca diferenta dintre puterea lcomotivei si suma noua a vagoanleor este pozitiva si minima. In final, dupa ce s-a gasit vagonul care trebuie scos, se va parcurge trenul si se va sterge vagonul cu proprietatea data.
