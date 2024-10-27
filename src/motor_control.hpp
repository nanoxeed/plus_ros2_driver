#include <stdio.h>
#include <float.h>


class MotorControl
{
public:
    bool m_bConnected = false;
    int m_r = 0;
    int m_l = 0;
    char m_ip[256];
    
    MotorControl();
    void start();

    void drive(int rval, int lval);
};
