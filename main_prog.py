import RPi.GPIO as GPIO 
import time
import datetime as dt

sensor = 24

GPIO.setmode(GPIO.BCM)
GPIO.setup(sensor, GPIO.IN)

time.sleep(5)

image_path = 'sensor_cam_image'

def capture_image(image_path='sensor_cam_image'):
    import picamera
    import datetime as dt

    # Capture system time
    current_time = dt.datetime.now()

    # Convert to string
    time_string = current_time.strftime("%Y%m%d_%H%M%S")

    picam = picamera.PiCamera()
    # sleep(2)
    # picam.image_effect(time_string)
    print(f'{time_string}: Image taken')
	
    new_img_path = f'{image_path}_{time_string}.jpg' 
	
    picam.capture(new_img_path)
	
    picam.close()
	
    return new_img_path

def cp_subproc(input_file):
	
    import subprocess

    # Command to capture a single frame
    cmd = [
        'cp',
        f'{input_file}',
        'smb://ls-xl876/share/raspberry_pi/'
		]
    # Run the command
    result = subprocess.run(cmd, capture_output=True, text=True)

    print(result.stdout)

while True:
	if GPIO.input(sensor):
		current_time = dt.datetime.now()
		print(f"{current_time} : motion detected")
		captured_img = capture_image()
		while GPIO.input(sensor):
			time.sleep(3)
			# cp_subproc(captured_img)
	else:
		time.sleep(0.5)
		print("no motion detected")
        # time.sleep(1)
