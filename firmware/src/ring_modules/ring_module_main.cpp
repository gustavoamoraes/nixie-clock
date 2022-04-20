#include <Arduino.h>

#include "ring_module_main.h"

void RingModuleMain::update () 
{   
    if(indexChild->isActive)
        return indexChild->update();
}

void RingModuleMain::change (int q) 
{
    if(indexChild->isActive)
        return indexChild->change(q);

    childIndex = childIndex + q % childCount;
}

void RingModuleMain::back () 
{
    if(indexChild->isActive)
        return indexChild->back();
}

void RingModuleMain::select () 
{
    if(indexChild->isActive)
        return indexChild->select();

    indexChild = &childs[childIndex];
}

Profile* RingModuleMain::getProfile ()
{
    if(indexChild->isActive)
        return indexChild->getProfile();

    return &profile;
}