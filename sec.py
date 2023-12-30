import cv2
import pyttsx3

# Initialize the text-to-speech engine
engine = pyttsx3.init()

# Set properties (optional)
engine.setProperty('rate', 150)  # Speed of speech

# Load the pre-trained Haar Cascade model for person detection
cascade_path = cv2.data.haarcascades + 'haarcascade_frontalface_default.xml'
cascade = cv2.CascadeClassifier(cascade_path)

# Initialize the video capture
cap = cv2.VideoCapture(0)  # 0 for default camera, you can change it to the video file path if needed

while True:
    ret, frame = cap.read()

    if not ret:
        break

    # Convert the frame to grayscale for Haar Cascade
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Detect persons in the frame
    persons = cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))

    for (x, y, w, h) in persons:
        # Draw bounding box
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)

        # Get the name of the person (you can replace this with your own logic)
        person_name = "Jane Doe"

        # Speak the name using pyttsx3
        engine.say(f"Person detected. Name: {person_name}")
        engine.runAndWait()

    cv2.imshow('Person Detection', frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
