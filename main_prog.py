import RPi.GPIO as GPIO
import time
import datetime as dt
import picamera
import logging
from relay_controller import RelayController
import threading


def setup_logging(log_file='FAZAAA3.log'):
    """Set up logging to output to both console and a log file."""
    logger = logging.getLogger()
    logger.setLevel(logging.DEBUG)  # Set the logging level to DEBUG to capture all messages

    # Create file handler for logging to a file
    file_handler = logging.FileHandler(log_file)
    file_handler.setLevel(logging.INFO)  # Log level for file

    # Create console handler for logging to console
    console_handler = logging.StreamHandler()
    console_handler.setLevel(logging.DEBUG)  # Log level for console

    # Create a formatter and set it for both handlers
    formatter = logging.Formatter('%(asctime)s - %(levelname)s - %(message)s')
    file_handler.setFormatter(formatter)
    console_handler.setFormatter(formatter)

    # Add the handlers to the logger
    logger.addHandler(file_handler)
    logger.addHandler(console_handler)

# GPIO pin setup
SENSOR_PIN = 24  # GPIO pin connected to PIR sensor

def setup_gpio():
    """Set up GPIO mode and pins."""
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(SENSOR_PIN, GPIO.IN)
    logging.info("GPIO setup complete.")

def capture_image(image_path='sensor_cam_image'):
    """Capture an image and return the file path."""
    current_time = dt.datetime.now()
    time_string = current_time.strftime("%Y%m%d_%H%M%S")
    new_img_path = f'{image_path}_{time_string}.jpg'

    # Initialize camera and capture image
    with picamera.PiCamera() as picam:
        time.sleep(2)  # Warm-up time for camera
        picam.capture(new_img_path)
        picam.close()
        logging.info(f'Image captured and saved at {new_img_path}')

    return new_img_path

# def get_system_temp(self):
#     # Read the CPU temperature
#     temp = os.popen("vcgencmd measure_temp").readline()
#     temp_value = temp.replace("temp=", "").replace("'C\n", "")
#     logging.info(f"System Temperature: {temp_value}°C")
#     return float(temp_value)

def get_cpu_temp():
    """Retrieve the CPU temperature."""
    try:
        with open("/sys/class/thermal/thermal_zone0/temp", "r") as file:
            temp = float(file.read().strip()) / 1000.0
            return temp
    except FileNotFoundError:
        logging.error("Could not read CPU temperature.")
        return None

def log_temperature(interval=60):
    """Log the temperature at a regular interval."""
    while True:
        temp = get_cpu_temp()
        if temp:
            logging.info(f"CPU Temperature: {temp:.2f}°C")
        time.sleep(interval)

def main_loop():
    """Main loop for motion detection and camera control."""
    try:

        logging.info("FAZAAA3 is ready!")

        while True:
            if GPIO.input(SENSOR_PIN):
                current_time = dt.datetime.now()
                logging.info(f"{current_time} : Motion detected")
                relay.turn_on()
                capture_image()
                # Keep camera on as long as motion is detected
                while GPIO.input(SENSOR_PIN):
                    time.sleep(0.2)
                    
                relay.turn_off()

            else:
                time.sleep(0.5)  # Check for motion every 0.5 seconds

    except KeyboardInterrupt:
        logging.info("Exiting FAZAAA3")

    finally:
        GPIO.cleanup()
        relay.cleanup()
        logging.info("GPIO cleanup completed.")

if __name__ == "__main__":
    setup_logging()  # Initialize logging
    logging.info("Initializing FAZAAA3...")
    # Initialize relay on the specified GPIO pin (e.g., 17)
    relay = RelayController(relay_pin=17)

    setup_gpio()    # Initialize GPIO
    time.sleep(3)   # Allow for initialization

    # Start the temperature logging thread
    temp_thread = threading.Thread(target=log_temperature, args=(60,), daemon=True)
    temp_thread.start()

    main_loop()     # Start main loop
