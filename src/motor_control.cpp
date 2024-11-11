#include <stdio.h>
#include <memory.h>
#include <iostream>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "motor_control.hpp"

#define SERVER_PORT 10200      // 通信ポート
#define DEF_IP "192.168.0.162" // サーバーIP
#define UDP_PORT 1234          // UDPポート


void *udp_recvfunc(void *pdata)
{
    printf("UDP Receive Thread Start!\n");
    MotorControl *pcontroller = (MotorControl *)pdata;
    unsigned short port = UDP_PORT;
    int sockClient = -1;
    struct sockaddr_in sockAddrInServer;

    while (1)
    {
        if (sockClient == -1)
        {
            memset(&sockAddrInServer, 0, sizeof(sockAddrInServer));
            sockAddrInServer.sin_port = htons(port);
            sockAddrInServer.sin_family = AF_INET;
            sockAddrInServer.sin_addr.s_addr = INADDR_ANY;

            sockClient = socket(AF_INET, SOCK_DGRAM, 0);
            if (sockClient < 0)
            {
                printf("ERROR:socket\n");
                usleep(100000);
                continue;
            }
            if (bind(sockClient, (struct sockaddr *)&sockAddrInServer, sizeof(sockAddrInServer)) != 0)
            {
                printf("ERROR:bind\n");
                close(sockClient);
                sockClient = -1;
                usleep(100000);
                continue;
            }

            pcontroller->m_bUdpConnected = true;
            printf("UDP Connected\n");
        }
        else
        {
            char buff[256];
            int len = recv(sockClient, buff, sizeof(buff), 0);
            if (len > 0)
            {
                pcontroller->m_sensor_mutex.lock();
                pcontroller->m_dist = atof(buff);
                printf("UDP:dist=%f\n", pcontroller->m_dist);
                pcontroller->m_sensor_mutex.unlock();
            }
        }
    }
}


void *threadfunc(void *pdata)
{
    printf("Motor Control Thread Start!\n");
    MotorControl *pcontroller = (MotorControl *)pdata;
    unsigned short port = SERVER_PORT;
    int sockClient = -1;
    struct sockaddr_in sockAddrInServer;

    while (1)
    {
        if (!pcontroller->m_bConnected)
        {
            //
            memset(&sockAddrInServer, 0, sizeof(sockAddrInServer));
            sockAddrInServer.sin_port = htons(port);
            sockAddrInServer.sin_family = AF_INET;
            sockAddrInServer.sin_addr.s_addr = inet_addr(pcontroller->m_ip);

            sockClient = socket(AF_INET, SOCK_STREAM, 0);
            if (sockClient < 0)
            {
                printf("ERROR:socket\n");
                usleep(100000);
                continue;
            }
            if (connect(sockClient, (struct sockaddr *)&sockAddrInServer, sizeof(sockAddrInServer)) != 0)
            {
                printf("ERROR:connect\n");
                close(sockClient);
                pcontroller->m_bConnected = false;
                sockClient = -1;
                usleep(100000);
                continue;
            }

            pcontroller->m_bConnected = true;
            printf("Connected\n");
        }
        else
        {
            char buff[256];
            pcontroller->m_command_mutex.lock();
            sprintf(buff, "###,%3d,%3d,%03d,%03d\r", pcontroller->m_r, pcontroller->m_l, 0, 0);
            pcontroller->m_command_mutex.unlock();
            if (send(sockClient, buff, ((int)strlen(buff) + 0), 0) < 0)
            {
                // 送信エラー
                printf("ERROR:send\n");
                break;
            }
            printf("SC:send[%s]\n", buff);

            memset(buff, 0, sizeof(buff));
            int len = recv(sockClient, buff, 7, 0);
            if (len <= 0)
            {
                close(sockClient);
                pcontroller->m_bConnected = false;
                sockClient = -1;
                break;
            }
        }
        usleep(10000);
    }

    return NULL;
}

MotorControl::MotorControl() {}

void MotorControl::start()
{
    pthread_t th;

    pthread_create(&th, NULL, threadfunc, this);
    pthread_create(&th, NULL, udp_recvfunc, this);
}

void MotorControl::drive(int rval, int lval)
{
    printf("JOY STICK MESSAGE %d %d\n", rval, lval);
    set_commands(rval, lval);
}

float MotorControl::get_dist()
{
    std::lock_guard<std::mutex> lock(m_sensor_mutex);
    return m_dist;
}

void MotorControl::set_commands(int rval, int lval)
{
    std::lock_guard<std::mutex> lock(m_command_mutex);
    m_r = rval;
    m_l = lval;
}
