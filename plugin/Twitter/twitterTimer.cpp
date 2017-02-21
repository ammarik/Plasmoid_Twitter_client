#include "twitterTimer.h"

//------------------------------------------------------------------------------//

TwitterTimer::TwitterTimer()
{
    m_timer = new QTimer(this);
    m_timer->setSingleShot(true);   
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
}  

//------------------------------------------------------------------------------//

TwitterTimer::~TwitterTimer()
{

}

//------------------------------------------------------------------------------//

///Starts timer for a given length
/**
 * @param duration - duration of timer
 */
void TwitterTimer::startTimer(int duration)
{
    m_timer->start(duration);
}

//------------------------------------------------------------------------------//

/// Stops the timer.
void TwitterTimer::stopTimer()
{
    m_timer->stop();
}

//------------------------------------------------------------------------------//

///Emits signal that informs aboud end of timer.
void TwitterTimer::timeout()
{
    emit sig_timeout();
}
