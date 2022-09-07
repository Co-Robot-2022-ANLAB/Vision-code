#ifndef __RPI_H
#define __RPI_H

#include <iostream>
#include <unistd.h>
#include <wiringPi.h>

#define LOG_

namespace rpi {
// BCM 기준 핀 번호.
// int human_pins[3] = {  3,  5,  7 };
// int robot_pins[3] = {  8, 10, 11 };

// wPi 기준 핀 번호.
int human_pins[3] = {  9, 21, 11 };
int robot_pins[3] = { 10, 12, 14 };
size_t num_pins = sizeof(human_pins) / sizeof(human_pins[0]);

// 환경 초기화 함수.
void init()
{
#ifdef LOG
    std::cout << "[Init] Init.\n";
#endif

    // 라이브러리 불러오기.
    if (wiringPiSetup() == -1)
        exit(1);

    // 사용할 핀 입력으로 설정.
    for (size_t i = 0; i < num_pins; i++)
    {
        pinMode(human_pins[i], INPUT);
        pinMode(robot_pins[i], INPUT);
    }

#ifdef LOG
    std::cout << "[Init] Done.\n";
#endif
}

// 해당 핀에 물체가 감지되었는가 반환.
// false: 감지 안 됨.
// true: 감지됨
bool get_from_pins(int* pins, size_t count)
{
#ifdef LOG
    std::cout << "[Input] Get from pins...\n";
#endif

    bool res = true;
    for (int i = 0; i < count; i++)
        res &= digitalRead(pins[i]);
    return !res;

#ifdef LOG
    std::cout << "[Input] Done\n";
#endif
}

// 사람 쪽과 로봇 쪽을 모두 확인하여 같은 공간에 있다고 판단되면 true.
bool is_not_safe()
{
    return get_from_pins(human_pins, num_pins) && get_from_pins(robot_pins, num_pins);
}

bool get_from_human()
{
    return get_from_pins(human_pins, num_pins);
}

bool get_from_robot()
{
    return get_from_pins(robot_pins, num_pins);
}

};

#endif