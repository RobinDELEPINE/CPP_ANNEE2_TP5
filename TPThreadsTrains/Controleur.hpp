#ifndef CONTROLEUR_HPP
#define CONTROLEUR_HPP

#include <thread>             
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <string.h>

using namespace std; 

class Controleur {
  public:
    Controleur (int valeur_initiale) : 
    val(valeur_initiale)
    {
    }
    
    bool controlinEnB(int numero);

    bool controlinEnA(int numero);

    bool controloutEnB(int numero);

    bool controloutEnA(int numero);

  private:
    int val;
    int nbr_train_circulant;
    string sens;
    int train_en_attente;
    mutex mtx;
};

bool Controleur::controlinEnB(int numero) { 

  unique_lock<mutex> lck(this->mtx);

  //vérifie qu'aucun train ne circule sur la voie
  if(this->nbr_train_circulant==0){ 
    //MAJ de sens
     this->sens = "B";
     //MAJ du nombre de train sur la voie
     this->nbr_train_circulant++;
     this->train_en_attente = numero;
     //on deverouille le mutex
    lck.unlock();
    return true;
  }
  else if(this->sens == "A" &&  this->train_en_attente == -numero){
     this->nbr_train_circulant--;
     this->train_en_attente = 0;
    lck.unlock();
    return true;
  }
  else{
    lck.unlock();
    return false;
  }
}  

bool Controleur::controlinEnA(int numero) { 

  unique_lock<mutex> lck(this->mtx);

  if(this->nbr_train_circulant==0){
     this->sens = "A";
     this->nbr_train_circulant++;
     this->train_en_attente = numero;
    lck.unlock();
    return true;
  }
  else if(this->sens == "B" &&  this->train_en_attente == -numero){
     this->nbr_train_circulant--;
     this->train_en_attente = 0;
    lck.unlock();
    return true;
  }
  else{
    lck.unlock();
    return false;
  }
}  

bool Controleur::controloutEnB(int numero){

  unique_lock<mutex> lck(this->mtx);

  if( this->nbr_train_circulant==1 &&  this->sens == "B" &&  this->train_en_attente == numero){
     this->nbr_train_circulant--;
     this->train_en_attente = 0;
    lck.unlock();
    return true ;
  }
  else{
    lck.unlock();
    return false;
  }
}

bool Controleur::controloutEnA(int numero){

  unique_lock<mutex> lck(this->mtx);

  if( this->nbr_train_circulant==1 &&  this->sens == "A" &&  this->train_en_attente == numero){
     this->nbr_train_circulant--;
     this->train_en_attente = 0;
    lck.unlock();
    return true;
  }
  else{
    lck.unlock();
    return false;
  }
}
/*
// Méthode qui vérifie si un train peut entrer sur le tronçon de voie unique allant de B vers A
bool Controleur::controlinEnB(int numero) { 
  // On vérifie si un train est déjà présent sur le tronçon de voie unique
  if (this->val != 0) {
    cout << "Train n° " << numero << " attend en B car un train est déjà sur le tronçon de voie unique" << endl;
    return false;
  }
  // Si aucun train n'est présent, on autorise le train à entrer sur le tronçon de voie unique
  else {
    cout << "Train n° " << numero << " entre en B vers A" << endl;
     this->val = numero;
    return true;
  }
}  

// Méthode qui vérifie si un train peut entrer sur le tronçon de voie unique allant de A vers B
bool Controleur::controlinEnA(int numero) { 
  // On vérifie si un train est déjà présent sur le tronçon de voie unique
  if ( this->val != 0) {
    cout << "Train n° " << numero << " attend en A car un train est déjà sur le tronçon de voie unique" << endl;
    return false;
  }
  // Si aucun train n'est présent, on autorise le train à entrer sur le tronçon de voie unique
  else {
      cout << "Train n° " << numero << " entre en A vers B" << endl;
       this->val = numero;
      return true;
  }
}  

// Méthode qui informe le contrôleur qu'un train est sorti du tronçon de voie unique allant de B vers A
bool Controleur::controloutEnB(int numero){ 
  // On vérifie si c'est bien le train qui a été autorisé à entrer sur le tronçon de voie unique qui en sort
  if ( this->val == numero) {
    cout << "Train n° " << numero << " sort en B vers A" << endl;
     this->val = 0;
    return true;
  }
  else {
    cout << "Erreur : Train n° " << numero << " tente de sortir en B vers A alors qu'il n'a pas été autorisé à entrer" << endl;
    return false;
  }
}
// Méthode qui informe le contrôleur qu'un train est sorti du tronçon de voie unique allant de B vers A
bool Controleur::controloutEnA(int numero){ 
  // On vérifie si c'est bien le train qui a été autorisé à entrer sur le tronçon de voie unique qui en sort
  if ( this->val == numero) {
    cout << "Train n° " << numero << " sort en A vers B" << endl;
     this->val = 0;
    return true;
  }
  else {
    cout << "Erreur : Train n° " << numero << " tente de sortir en B vers A alors qu'il n'a pas été autorisé à entrer" << endl;
    return false;
  }
}
*/
#endif //CONTROLEUR_HPP
