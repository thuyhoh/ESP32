import cv2
import urllib.request
import numpy as np

url = "http://"
while True:
    img = urllib.request.urlopen(url)
    img_array = np.array(bytearray(img.read()), dtype=np.uint8)
    frame = cv2.imdecode(img_array, -1)  # Decode the image array to a frame
    cv2.imshow("Live Stream", frame)  # Display the frame in a window
    if cv2.waitKey(1) & 0xFF == ord('q'):
        frame.release()
        cv2.destroyAllWindows()
        break  # Exit the loop if 'q' is pressed
