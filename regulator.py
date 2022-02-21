"""Implements Proportional-Integral-Derivative controller algorithm. (Utdyp)
"""

import time


class PID:
    """PID controller algorithm implementation. (Utdyp)"""

    def __init__(self, kp, ki, kd, setpoint):

        self.Kp = kp
        self.Ki = ki
        self.Kd = kd

        self.setpoint = setpoint

        self.proportional_term = 0
        self.integral_term = 0
        self.derivative_term = 0

        self.previous_error = 0
        self.previous_time = None

    def calculate_output(self, current_pos):
        """Calculates thrust output with current error value. (Utdyp)"""

        current_time = time.time()
        dt = current_time - self.previous_time if self.previous_time is not None else 0
        self.previous_time = current_time

        current_error = self.setpoint - current_pos
        de = (current_error - self.previous_error) / 2

        self.proportional_term = self.Kp * current_error
        self.integral_term += self.Ki * (self.previous_error + current_error) / 2 * dt
        self.derivative_term = self.Kd * de / dt if dt is not 0 else 0

        return self.proportional_term + self.integral_term + self.derivative_term
