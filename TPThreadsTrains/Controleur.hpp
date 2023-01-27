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

#endif //CONTROLEUR_HPP
