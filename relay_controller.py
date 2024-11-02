# relay_controller.py
import RPi.GPIO as GPIO

class RelayController:
    def __init__(self, relay_pin):
        # Disable GPIO warnings
        GPIO.setwarnings(False)
        
        # Initialize the GPIO pin
        self.relay_pin = relay_pin
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(self.relay_pin, GPIO.OUT)
        GPIO.output(self.relay_pin, GPIO.LOW)
        
        # Set initial state of the relay to OFF (HIGH)
        GPIO.output(self.relay_pin, GPIO.HIGH)
        print("Relay initialized and set to OFF")

    def turn_on(self):
        # Activate relay (assuming LOW triggers the relay)
        GPIO.output(self.relay_pin, GPIO.HIGH)
        # print("Relay turned ON")

    def turn_off(self):
        # Deactivate relay
        GPIO.output(self.relay_pin, GPIO.LOW)
        # print("Relay turned OFF")

    def cleanup(self):
        # Clean up GPIO settings
        GPIO.cleanup()
        # print("GPIO cleanup done")
