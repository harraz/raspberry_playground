from imageai.Detection import ObjectDetection
import os

execution_path = os.getcwd()
detector = ObjectDetection()
detector.setModelTypeAsRetinaNet()

#dataset to train model
detector.setModelPath(os.path.join(execution_path,"/resnet50_coco_best_v2.0.1.h5"))
detector.loadModel()

#load and save the output the file
detections=detector.detectObjectsFromImage(input_image=os.path.join(execution_path , "images.jpg"), output_image_path=os.path.join(execution_path , "output_file_name"))

##give prediction like
for eachObject in detections:
    print(eachObject["name"] , " : " , eachObject["percentage_probability"] )