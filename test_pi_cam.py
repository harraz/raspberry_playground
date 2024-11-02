# import picamera
# from time import sleep

# picam = picamera.PiCamera()
# sleep(2)
# config = picam.capture('inland_camera_test_img.jpg')

# main_script.py
from relay_controller import RelayController
import time

# Initialize relay on the specified GPIO pin (e.g., 17)
relay = RelayController(relay_pin=17)

try:
    # Toggle relay on and off in a loop
    while True:
        relay.turn_on()
        time.sleep(2)  # Relay on for 2 seconds

        relay.turn_off()
        time.sleep(5)  # Relay off for 5 seconds

except KeyboardInterrupt:
    print("Program interrupted")

finally:
    relay.cleanup()

