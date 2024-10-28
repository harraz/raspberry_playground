import picamera
from time import sleep

picam = picamera.PiCamera()
sleep(2)
config = picam.capture('inland_camera_test_img.jpg')

