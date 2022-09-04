#include <Arduino.h>

#include "ring_module_main.h"
#include "modifiers.h"
#include "constants.h"

RingModuleMain::RingModuleMain () : m_IndexChild(NULL)
{
    for (size_t i = 0; i < m_ChildCount; i++)
    {   
        m_Profile.ledRing[i].setColor(m_ChildMenuColors[i]);
    }

    for (size_t i = 0; i < NIXIE_COUNT; i++)
    {
        m_Profile.nixies[i].setModifier(DefaultOcilation);
        m_Profile.nixies[i].setDigit(i);
    }

    change(0);
}

void RingModuleMain::update () 
{   
    if((m_IndexChild != NULL) && m_IndexChild->isActive)
        return m_IndexChild->update();
}

void RingModuleMain::change (int q) 
{
    if((m_IndexChild != NULL) && m_IndexChild->isActive)
        return m_IndexChild->change(q);

    m_Profile.ledRing[m_ChildIndex].setModifier(NULL);
    m_ChildIndex = (m_ChildCount + ((m_ChildIndex + q) % m_ChildCount)) % m_ChildCount;
    m_Profile.ledRing[m_ChildIndex].setModifier(DefaultColorOcilation);

    m_IndexChild = m_Childs[m_ChildIndex];

    Profile* childProfile = m_IndexChild->getProfile();

    for (size_t i = 0; i < NIXIE_COUNT; i++)
    {
        m_Profile.nixies[i].setDigit(childProfile->nixies[i].getDigit());
    }
}

void RingModuleMain::back () 
{
    if((m_IndexChild != NULL) && m_IndexChild->isActive)
        return m_IndexChild->back();
}

void RingModuleMain::select () 
{   
    if((m_IndexChild != NULL) && m_IndexChild->isActive)
        return m_IndexChild->select();

    m_IndexChild->isActive = true;
}

Profile* RingModuleMain::getProfile ()
{   
    if((m_IndexChild != NULL) && m_IndexChild->isActive)
        return m_IndexChild->getProfile();

    return &m_Profile;
}