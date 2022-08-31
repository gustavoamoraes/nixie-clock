#include <Arduino.h>

#include "ring_module_main.h"
#include "constants.h"

RingModuleMain::RingModuleMain () : m_IndexChild(NULL)
{
    for (size_t i = 0; i < LED_RING_COUNT; i++)
    {
        profile.ledRing[i].setColor(m_OtherColor, RgbLed::TimingFunction::Constant);
    }
}

void RingModuleMain::update () 
{   
    if((m_IndexChild != NULL) & m_IndexChild->isActive)
        return m_IndexChild->update();
}

void RingModuleMain::change (int q) 
{
    if((m_IndexChild != NULL) & m_IndexChild->isActive)
        return m_IndexChild->change(q);

    profile.ledRing[m_ChildIndex].setColor(m_ChildMenuColors[m_ChildIndex], RgbLed::TimingFunction::Constant);
    m_ChildIndex = (m_ChildCount + ((m_ChildIndex + q) % m_ChildCount)) % m_ChildCount;
    profile.ledRing[m_ChildIndex].setColor(m_ChildMenuColors[m_ChildIndex], RgbLed::TimingFunction::Ocilating);
}

void RingModuleMain::back () 
{
    if((m_IndexChild != NULL) & m_IndexChild->isActive)
        return m_IndexChild->back();
}

void RingModuleMain::select () 
{
    if((m_IndexChild != NULL) & m_IndexChild->isActive)
        return m_IndexChild->select();

    m_IndexChild = m_Childs[m_ChildIndex];
    m_IndexChild->isActive = true;
}

Profile* RingModuleMain::getProfile ()
{
    if((m_IndexChild != NULL) & m_IndexChild->isActive)
        return m_IndexChild->getProfile();

    return &profile;
}