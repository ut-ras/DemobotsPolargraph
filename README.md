# DemobotsPolargraph
R.A.S. Demobots is making a polargraph for Arduino


# Equations
If 0,0 is center </br>
set lMotorLength = sqrt((XMAX + x)^2 + (YMAX - y)^2) </br>
set rMotorLength = sqrt((XMAX - x)^2 + (YMAX - y)^2) </br>

If 0,0 is top left corner </br>
set lMotorLength = sqrt(x^2 + y^2) </br>
set rMotorLength = sqrt((d-x)^2 + y^2) </br>
