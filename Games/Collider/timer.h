#ifndef TIMER_H
#define TIMER_H

class timer{
private:
    bool started;
    bool paused;
    int startingPos;
    int pausePos;
public:
    timer();
    void start();
    void pause();
    void unpause();
    void stop();
    bool isPaused();
    int getTime();

};

#endif
