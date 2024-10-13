import cv2

# Initialize the camera
cap = cv2.VideoCapture(0)  # 0 corresponds to the first camera detected

# Check if the camera is opened successfully
if not cap.isOpened():
    print("Error: Could not open camera.")
    exit()

while True:
    ret, frame = cap.read()
    if not ret:
        print("Error: Could not read frame.")
        break

    # Process and display the frame here
    cv2.imshow("USB Camera", frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release the camera and close the window
cap.release()
cv2.destroyAllWindows()
