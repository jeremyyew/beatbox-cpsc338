import turtle 
import tkinter as tk
import time
import sys

class Ball():
    radius = 4
    # IGNORE - replicating arduino drawCircle
    def drawCircle(self, x, y, radius):
        """
        Moves the turtle to the correct position and draws a circle
        """
        self.turtle.begin_fill()
        self.turtle.penup()
        # self.turtle.setposition(x - radius/2, y - radius/2)
        self.turtle.setposition(x, y - radius)

        self.turtle.pendown()
        self.turtle.circle(radius)
        self.turtle.end_fill()

    # IGNORE - replicating delay in ms like delay() in arduino
    def delay(self, x):
        time.sleep(x * 0.001)

    # IGNORE - replicating arduino coordinate system and size
    def python_setup_ignore_for_arduino(self):
        self.WIDTH, self.HEIGHT = 128, 64
        root = tk.Tk()
        canvas = tk.Canvas(root, width=self.WIDTH, height=self.HEIGHT)
        canvas.pack()

        self.screen = turtle.TurtleScreen(canvas)
        self.screen.setworldcoordinates(0, 0, self.WIDTH - 1, self.HEIGHT - 1)

        self.turtle = turtle.RawTurtle(self.screen, visible=False)
        # self.screen = turtle.Screen()
        # self.screen.setup(128, 64)
        # assuming lower left is (0,0) and upper right is (127, 63)
        # turtle.setworldcoordinates(0, 0, 128, 128)

        self.screen.bgcolor('black')
        self.turtle.pencolor("white")
        self.turtle.fillcolor("white")
        self.turtle.hideturtle()
        self.turtle.speed(0)
        self.screen.tracer(False)
    
    # USE THIS
    # param: encoder_val = [0, 255] inclusive
    # return: velocity = [-26, 25] inclusive
    # encoder_val = 128 => velocity = 0
    # velocity is in 10px/sec since delay is 0.1 sec
    def enc_val_to_vel(self, encoder_value):
        return (encoder_value - 128) // 5  # integer division

    # USE THIS
    def at_left_edge(self, x, y):
        return x - self.radius <= 0

    # USE THIS
    def at_right_edge(self, x, y):
        return x + self.radius >= self.WIDTH

    # USE THIS
    def setup(self):
        start_x = 0
        start_y = 31

        self.x = start_x
        self.y = start_y

        # 1 => moving right, -1 => moving left
        self.direction = 1

    # USE THIS
    def loop(self): 
        self.turtle.clear()

        if (self.at_left_edge(self.x, self.y)):
            self.x = self.radius
            self.direction *= -1

        if (self.at_right_edge(self.x, self.y)):
            self.x = self.WIDTH - self.radius
            self.direction *= -1

        # x, y of center of circle
        self.drawCircle(self.x, self.y, self.radius)

        self.screen.update()
        self.delay(100)
        self.x += self.enc_val_to_vel(int(sys.argv[1])) * self.direction

# IGNORE all of this stuff
if len(sys.argv) < 2:
    print("usage: python3 ball.py encoder_value")
    print("encoder_value = [0, 255]")

    exit()

ball = Ball()

ball.python_setup_ignore_for_arduino()

ball.setup()

while True:
    ball.loop()
