#include <stdio.h>
#include <float.h>
#include <mutex>


class MotorControl
{
public:
    bool m_bConnected = false;
    bool m_bUdpConnected = false;
    int m_r = 0;
    int m_l = 0;
    float m_dist = 0;
    char m_ip[256];
    std::mutex m_command_mutex;
    std::mutex m_sensor_mutex;

    MotorControl();
    void start();

    void drive(int rval, int lval);
    float get_dist();
private:
    void set_commands(int rval, int lval);
};
