import time
import numpy as np
import tensorflow as tf
from tensorflow.lite.python.interpreter import Interpreter
from tensorflow.keras.preprocessing import image
import subprocess
import json

# Load the TensorFlow Lite model
interpreter = Interpreter(model_path=
                          '/home/pi/playground1/mobilenet_v2_1.0_224.tflite')
interpreter.allocate_tensors()

# Get input and output tensors
input_details = interpreter.get_input_details()
output_details = interpreter.get_output_details()

# Load labels
with open('imagenet-simple-labels.json') as f:
    labels = json.load(f)

def capture_image(image_path):
    """Capture an image using ffmpeg."""
    command = [
        'ffmpeg', '-f', 'v4l2', '-i', '/dev/video0',
        '-frames:v', '1' , '-y', image_path
    ]
    subprocess.run(command)

def classify_image(image_path):
    """Classify an image using TensorFlow Lite."""
    # Load and preprocess the image
    img = image.load_img(image_path, target_size=(224, 224))
    img_array = image.img_to_array(img)
    img_array = np.expand_dims(img_array, axis=0)
    img_array = img_array / 255.0

    # Run inference
    interpreter.set_tensor(input_details[0]['index'], img_array)
    interpreter.invoke()
    predictions = interpreter.get_tensor(output_details[0]['index'])

    # Print top predictions with labels
    top_k = predictions[0].argsort()[-5:][::-1]
    for i in top_k:
        print(f"Label: {labels[i]}, Score: {predictions[0][i]:.2f}")

  

# while True:
# Capture image
image_path = '/home/pi/playground1/ffmpeg_image.jpg'

capture_image(image_path)

# image_path ='/home/pi/playground1/1a045aba379f6f829997b785d01ce81b.jpg'
# image_path = '/home/pi/playground1/35672988480_7c43a394ca_b.jpg'
# image_path = 'deer_images/deer_1.jpeg'
# Classify image
classify_image(image_path)

# Wait for 3 seconds
time.sleep(3)
