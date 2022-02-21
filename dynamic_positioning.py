"""
"""

import time

import regulator


def dynamic_positioning():
    """
    """

    Kp = 1.0
    Ki = 10.0
    Kd = 0.1

    setpoint = 0.0

    pid = regulator.PID(Kp,Ki,Kd,setpoint)

    while 1:




dynamic_positioning()