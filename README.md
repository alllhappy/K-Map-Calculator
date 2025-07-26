
# K Map Calculator with ESP32

[Watch the demo video](https://youtube.com/shorts/qmkZrWnDo98?feature=share)

[Project Report pdf](https://github.com/alllhappy/K-Map-Calculator/blob/master/Kmap%20report.pdf)
## Credits

* **Software and circuit design** by me
* **Hardware, Wirings and ZeroPCB** by my friend Shivam
* **Structure and Body Design** by my friend Aadarsh Sahni


Key features
- LED groupings + string solution as output
- Ability to see multiple groupings if possible
- Ability to give don't care as input to a cell
- Can solve for upto 4 variables

This is our 4th semester project of the Electronic Design Workshop
(EDW) course.The project is about the
making of K-Map Calculator that will solve the case of 4 variables. The main code
to solve the K-Map was developed ourselves in C++. Firstly circuits were simu-
lated on Wokwi (esp32 simulator) to test the outcomes. Then it was implemented
on breadboard. Finally the PCB was made using a zeroboard and enclosure was
designed. The results of the calculator are accurate. Calculator takes input by 16
switches. Each switch represents a K-Map cell (minterm). Each cell have 3 inputs
possible: logic-1, 0 & Don’t Care. The calculator displays the K-Map groups via
LED output and the final string expression on the LCD screen.

For full report refer to [Project Report pdf](https://github.com/alllhappy/K-Map-Calculator/blob/master/Kmap%20report.pdf)

![calculator Image 1](https://drive.google.com/uc?export=view&id=1AIB66eV3pdjYRqu7uFOelXpbPVFUAfPu)


## Setup Steps
1. Download the Repo
2. Open the project in Arudino IDE
3. Make sure to install the necessary libraries **FastLed** and **LiquidCrystal**.
4. Make wire connections as per the circuit diagrams given in the project report file. In case a different microcontroller is used make necessary wiring changes and configure the GPIO pins in the file calcFinal.ino .
5. Load the program in microcontroller.


