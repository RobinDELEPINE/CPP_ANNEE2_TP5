#include <iostream>           
#include <thread>             
#include <mutex>              
#include <condition_variable> 
#include "Controleur.hpp" 

// Mutex pour synchroniser l'accès à la condition variable
mutex mtx;

// Condition variable pour synchroniser les threads
condition_variable cv;

// Instance de la classe contrôleur pour gérer les trains sur la voie unique
Controleur ctrl(0);  

// TrainAB 
// Thread pour gérer les trains circulant de A vers B
void circuleAB(int numTrain){
  cout << "Train n° " << numTrain << " arrive en A vers B " << endl;

   // Simulation de temps pour l'arrivée du train
  this_thread::sleep_for(chrono::milliseconds(rand() % 1000) );

  // Lock sur le mutex pour accéder à la condition variable
  unique_lock< mutex> lck(mtx); 

  // Attente d'autorisation de circuler sur la voie unique de la part du contrôleur
  cv.wait(lck,[numTrain]{return ctrl.controlinEnA(numTrain);}); // ATTENTE D'AUTORISATION DE CIRCULER
  lck.unlock();

  // DEBUT DU PARCOURS A->B
  cout << "Train n° "<< numTrain << " circule de A vers B  >>>>>> " << endl;

  // Simulation de temps pour le parcours sur la voie unique
  this_thread::sleep_for(chrono::milliseconds(rand() % 100) );

  // FIN DU PARCOURS A->B
  cout << "Train n° " << numTrain << " quitte le tronçon de voie unique " << endl;
  
  // Lock sur le mutex pour signaler la sortie du train au contrôleur
  lck.lock();
  ctrl.controloutEnB(numTrain);  // SIGNAL DE SORTIE AU CONTROLEUR

  // Unlock du mutex et notification de tous les threads en attente
  lck.unlock();
  cv.notify_all();
}

// TrainBA 
// Thread pour gérer les trains circulant de B vers A
void circuleBA(int numTrain){
  cout << "Train n° " << numTrain << " arrive en B vers A " << endl;

  // Simulation de temps pour l'arrivée du train
  this_thread::sleep_for(chrono::milliseconds(rand() % 1000) );

  // Lock sur le mutex pour accéder à la condition variable
  unique_lock< mutex> lck(mtx);

  // Attente d'autorisation de circuler sur la voie unique de la part du contrôleur
  cv.wait(lck,[numTrain]{return ctrl.controlinEnB(numTrain);}); // ATTENTE D'AUTORISATION DE CIRCULER
  lck.unlock();
  // DEBUT DU PARCOURSs B->A
  cout << "Train n° "<< numTrain << " circule de B vers A  <<<<<<<<" << endl;
  this_thread::sleep_for(chrono::milliseconds(rand() % 100) );
  
  // FIN DU PARCOURS B->A
  cout << "Train n° " << numTrain << " quitte le tronçon de voie unique " << endl;
  lck.lock();
  ctrl.controloutEnA(numTrain);   // SIGNAL DE SORTIE AU CONTROLEUR

  // Unlock du mutex et notification de tous les threads en attente
  lck.unlock();
  cv.notify_all();
}

int main(){
  const int nbr = 9;
  srand( time(nullptr));
  thread trainsAB[nbr];
  thread trainsBA[nbr];

  // INITALISE TRAINS ET CONTROLEUR
  for (int i = 1; i < nbr; i++){
    trainsAB[i] =  thread(circuleAB, i);
    trainsBA[i] =  thread(circuleBA, -i);
  }
// JOIN DES THREADS 
  for (int i = 1; i < nbr; i++) {
    if (trainsAB[i].joinable()) trainsAB[i].join(); 
    if (trainsBA[i].joinable()) trainsBA[i].join();   
  }
  return 0;
}

