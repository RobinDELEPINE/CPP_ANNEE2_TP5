#include <iostream>           
#include <thread>             
#include "Semaphore.hpp"

int ressourcePartagee;
Semaphore lecture(8);
Semaphore ecriture(1);

void lecteur(int numLecteur){
  for (int i = 0; i < 4; i++){
    std::cout << "Lecteur n°" << numLecteur << " en cours " << endl;
    this_thread::sleep_for(chrono::milliseconds(rand() % 100) );
    lecture.P(numLecteur);
    std::cout << "    Valeur lue = " << ressourcePartagee << "  " << endl;
    lecture.V(numLecteur);
  }
}

void ecrivain(int numEcrivain){
  int x;
  for (int i = 0; i < 4; i++){
    std::cout << "Ecrivain n°" << numEcrivain << " en cours " << endl;
    ecriture.P(numEcrivain);
    for (int i = 0; i < 8; i++)
    {
      lecture.P(numEcrivain);
    }
    x = ressourcePartagee;
    this_thread::sleep_for(chrono::milliseconds(rand() % 100) );
    std::cout << "    valeur à incrémenter de la ressourcePartagee = " << x << "  " << endl;
    ressourcePartagee = x+1 ;
    for (int i = 0; i < 8; i++)
    {
      lecture.V(numEcrivain);
    }
    ecriture.V(numEcrivain);
  }
}

int main() {
  const int nbr = 8;
  std::thread r[nbr];
  std::thread w[nbr];
  ressourcePartagee = 0;

  for (int i = 1; i < nbr; i++){
    r[i] = std::thread(lecteur, -i);
    w[i] = std::thread(ecrivain, i);
  }
  // Join des threads 
  for (int i = 1; i < nbr; i++) {
  r[i].join(); 
  w[i].join();   
  }
  return 0;
}
