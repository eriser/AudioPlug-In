//
//  EffectProcessor.h
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 29/09/2014.
//
// Description :
// Classe d'interface entre les effet et le processeurs principal.
// La fonction process redistribue à tout les autres classes le processing audio en fonction des effets activé.

#ifndef __HeyMate__EffectProcessor__
#define __HeyMate__EffectProcessor__
#include <vector>

#include "pluginterfaces/vst/ivstaudioprocessor.h"

// include des classes d'effets.
#include "EffectBase.h"


class HEProcessor;

class HEEffectProcessor
{
public:
    HEEffectProcessor(HEProcessor * settingsMainProcessor); // création des effets.
    // certains effets on besoin d'un lien vers le mainProcessor.
    ~HEEffectProcessor(); // destructions des effets.
    
    // process est chargé de redistribuer la fonction process aux effets suivant les effets activés.
    Steinberg::tresult process (Steinberg::Vst::ProcessData& data);
    
    // Passage de l'activation aux effets.
    Steinberg::tresult setActive (Steinberg::TBool state);
    
    // ajout d'un effet à la liste des effets
    Steinberg::tresult addEffect (HEEffect * newEffect);
    // suppression d'un effet
    Steinberg::tresult deleteEffect (int effectTag);
    // suppression de tout les effets
    Steinberg::tresult deleteAllEffect (int effectTag);
    
    // passage des valeurs des paramètres
    Steinberg::tresult setState (Steinberg::IBStream* state);
    Steinberg::tresult getState (Steinberg::IBStream* state);
    
private:
    // Liste des classes d'effets.
    std::vector <HEEffect*> effects; // liste des effets
    
    // Liste de variable qui permettent l'entré dans le processeur.
    // C'est variables seront activé ou désactivé selon l'utilisateur.
    
    // lien vers le processeur.
    HEProcessor * mainProcessor;
    
};

#endif /* defined(__HeyMate__EffectProcessor__) */
