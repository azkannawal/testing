import cv2
import pytesseract
import pyttsx3

# Initialize the text-to-speech engine
engine = pyttsx3.init()

# Load the pre-trained EAST text detection model
net = cv2.dnn.readNet('C:/Users/ferif/Downloads/frozen_east_text_detection.pb')

# Open a connection to the camera (0 is the default camera, change if using a different camera)
cap = cv2.VideoCapture(0)

# Check if the camera opened successfully
if not cap.isOpened():
    print("Error: Could not open camera.")
    exit()

while True:
    # Read a frame from the camera
    ret, frame = cap.read()

    # Get the frame dimensions
    height, width = frame.shape[:2]

    # Prepare the frame for text detection (resizing and normalization)
    blob = cv2.dnn.blobFromImage(frame, 1.0, (width, height), (123.68, 116.78, 103.94), swapRB=True, crop=False)

    # Set the blob as input to the network and perform a forward pass
    net.setInput(blob)
    scores, geometry = net.forward(["feature_fusion/Conv_7/Sigmoid", "feature_fusion/concat_3"])

    # Apply non-maximum suppression to get the final bounding boxes
    rectangles, confidences = cv2.dnn.NMSBoxesRotated(
        boxes=geometry,  # Corrected argument: pass the 'geometry' directly
        scores=scores[0], score_threshold=0.5, nms_threshold=0.4)

    # Iterate over the detected text regions
    for i in range(len(rectangles)):
        # Extract the coordinates of the bounding box
        box = cv2.boxPoints(rectangles[i]).astype(int)
        box = box.reshape((-1, 1, 2))

        # Draw the bounding box on the frame
        cv2.polylines(frame, [box], isClosed=True, color=(0, 255, 0), thickness=2)

        # Extract the text within the bounding box
        text = frame[int(box[1][0][1]):int(box[3][0][1]), int(box[1][0][0]):int(box[3][0][0])]
        text = pytesseract.image_to_string(text, config='--psm 6')

        # Print the detected text
        print("Detected Text:", text)

        # Use pyttsx3 to convert the text to speech
        engine.say(text)
        engine.runAndWait()

    # Display the frame with detected text
    cv2.imshow('Text Detection', frame)

    # Break the loop if 'q' key is pressed
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release the camera and close all windows
cap.release()
cv2.destroyAllWindows()
